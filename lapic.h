#include <CPU.h>
#include <pthread.h>

#define APIC_REGISTERS_SIZE 64

#define LAPIC_DEFAULT_BASE 0xFEE00000

#define TPR 0x0080        // Task Priority
#define EOI 0x00B0        // EOI
#define SVR 0x00F0        // Spurious Interrupt Vector
#define TIMER 0x0320      // Local Vector Table 0 (TIMER)
#define TICR 0x0380       // Timer Initial Count
#define LINT0 0x0350      // Local Vector Table 1 (LINT0)
#define LINT1 0x0360      // Local Vector Table 2 (LINT1)
#define ERROR 0x0370      // Local Vector Table 3 (ERROR)
#define MASKED 0x00010000 // Interrupt masked


class LAPIC
{
    uint32_t registers[APIC_REGISTERS_SIZE];
    uint8_t unit_enabled;
    uint8_t int_enabled;
    uint8_t irr[32];
    uint8_t isr[32];
    pthread_mutex_t lock;
    pthread_t *timer_thread;
    uint8_t isr_index;
    CPU* cpu;
    void lapic_eoi();
    void check_int_enabled();
public:
    LAPIC(CPU* cpu);
    void lapic_send_intr();
    void lapic_write_to_irr(uint8_t irq);

    void lapic_write_reg(uint32_t addr, uint32_t val);
    uint32_t lapic_read_reg(uint32_t addr);
};