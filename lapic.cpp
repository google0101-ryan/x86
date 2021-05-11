#include <lapic.h>
#include <cstdlib>
#include <string.h>
#include <unistd.h>

#define IRR_L 32

#define T_IRQ0 32 // IRQ 0 corresponds to int T_IRQ

#define IRQ_TIMER 0
#define IRQ_KBD 1
#define IRQ_COM1 4
#define IRQ_IDE 14
#define IRQ_ERROR 19
#define IRQ_SPURIOUS 31


LAPIC::LAPIC(CPU* cpu)
{
    this->cpu = cpu;
    memset(registers, 0, sizeof(registers));
    pthread_t timer_thread;
    this->timer_thread = &timer_thread;
}

void LAPIC::lapic_send_intr()
{
    if (!int_enabled || !unit_enabled || !cpu->eflags->IF || registers[EOI >> 4] != 0)
    {
        return;
    }
    pthread_mutex_lock(&lock);
    uint8_t i;
    for (i = 0; i < IRR_L; i++)
    {
        if (irr[i] > 0)
        {
            isr[i] = irr[i];
            isr_index = i;
            cpu->int_r = irr[i];
            registers[EOI >> 4] = 1;
            irr[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&lock);
}

void LAPIC::lapic_eoi()
{
    pthread_mutex_lock(&lock);
    isr[isr_index] = 0;
    registers[EOI >> 4] = 0;
    pthread_mutex_unlock(&lock);
    lapic_send_intr();
}

void LAPIC::lapic_write_to_irr(uint8_t irq)
{
    if (!unit_enabled || !int_enabled)
    {
        return;
    }
    pthread_mutex_lock(&lock);
    uint8_t i;
    for (i = 0; i < IRR_L; i++)
    {
        if (irr[i] == 0)
        {
            irr[i] = irq;
            pthread_mutex_lock(&lock);
            lapic_send_intr();
        }
    }
    pthread_mutex_unlock(&lock);
}

void *timer_loop(void *ptr)
{
    LAPIC* lapic = (LAPIC*)ptr;
    while (1)
    {
        lapic->lapic_write_to_irr(T_IRQ0 + IRQ_TIMER);
        usleep(10 *1000);
    }
    return NULL;
}

void LAPIC::check_int_enabled()
{
    if (registers[LINT0 >> 4] == MASKED && registers[LINT1 >> 4] == MASKED)
    {
        int_enabled = 1;
    }
    else
    {
        int_enabled = 0;
    }
}

void LAPIC::lapic_write_reg(uint32_t addr, uint32_t val)
{
    uint32_t offset = (addr - LAPIC_DEFAULT_BASE);
    uint8_t index = offset >> 4;
    registers[index] = val;
    if (offset == SVR)
    {
        if (val & 0x100)
        {
            unit_enabled = 1;
        }
        return;
    }
    else if (offset == TICR)
    {
        pthread_create(timer_thread, NULL, timer_loop, (void *)this);
    }
    else if (offset == TPR)
    {
        check_int_enabled();
    }
    else if (offset == EOI && val == 0)
    {
        check_int_enabled();
        lapic_eoi();
    }
}

uint32_t LAPIC::lapic_read_reg(uint32_t addr)
{
    uint8_t index = (addr - LAPIC_DEFAULT_BASE) >> 4;
    return registers[index];
}