#include "pic.hpp"

static inline int is_master(pic_controller* ctrl)
{
    return ctrl == &pic.ctrl[0];
}

static void pic_reset(void)
{
    for (int i = 0; i < 2; i++)
    {
        pic_controller* ctrl = &pic.ctrl[i];
        ctrl->vector_offset = 0;
        ctrl->imr = 0xFF;
        ctrl->irr = 0;
        ctrl->isr = 0;
        ctrl->in_initialization = 0;
        ctrl->read_isr = 0;
        ctrl->elcr = 0;
    }
}

static inline uint8_t rol(uint8_t value, uint8_t priority_base)
{
    // 0 1 2 3 4 5 6 7
    //               ^-- If 7 is priority base, then...
    // 0 1 2 3 4 5 6 7
    // ^-- 0 is highest priority interrupt
    // Rotate count: 7 ^ 7 = 0

    // 0 1 2 3 4 5 6 7
    //             ^-- If 6 is priority base, then...
    // 0 1 2 3 4 5 6 7
    //               ^-- 7 is highest priority interrupt
    // Normalized:
    // 7 0 1 2 3 4 5 6
    // Rotate count: 6 ^ 7 = 1
    uint8_t count = priority_base ^ 7;
    return (value << count) | (value >> (8 - count));
}

static void pic_internal_update(pic_controller* ctrl)
{
    int unmasked, isr;

    if (!(unmasked = ctrl->irr & ~ctrl->imr))
    {
        return;
    }

    unmasked = rol(unmasked, ctrl->priority_base);

    isr = rol(ctrl->isr, ctrl->priority_base);

    if ((ctrl->ocw[3] & 0x60) == 0x60)
    {
        unmasked &= ~isr;

        for (int i = 0; i < 8; i++)
        {
            if (unmasked & (1 << i))
            {
                ctrl->highest_priority_irq_to_send = (ctrl->priority_base + 1 + i) & 7;

                if (is_master(ctrl))
                    ;
                else
                    ;
                return;
            }
        }
    } 
    else
    {

    }
}

static void pic_clear_specific(pic_controller* ctrl, int irq)
{
    ctrl->isr &= ~(1 << irq);
}

static void pic_write_icw(struct pic_controller* ctrl, int id, uint8_t value)
{
    //PIC_LOG("write to icw%d: %02x\n", id, value);
    switch (id) {
    case 1: // ICW1
        ctrl->icw_index = 2;
        ctrl->icw[1] = value;

        ctrl->imr = 0;
        ctrl->isr = 0;
        ctrl->irr = 0;
        ctrl->priority_base = 7; // Make IRQ0 have highest priority
        break;
    case 2:
        ctrl->vector_offset = value & ~7;
        ctrl->icw[2] = value;
        if (ctrl->icw[1] & 2) {
            // single pic
            if (ctrl->icw[1] & 1)
                ctrl->icw_index = 4;
            else
                ctrl->icw_index = 5;
        } else
            ctrl->icw_index = 3;
        break;
    case 3:
        ctrl->icw[3] = value;
        ctrl->icw_index = 5 ^ (ctrl->icw[1] & 1);
        break;
    case 4:
        ctrl->icw[4] = value;
        ctrl->autoeoi = value & 2;
        ctrl->icw_index = 5;
    }
    ctrl->in_initialization = ctrl->icw_index != 5;
}


static void pic_write_ocw(pic_controller* ctrl, int index, int data)
{
    ctrl->ocw[index] = data;
    switch (index)
    {
    case 1:
        ctrl->imr = data;
        pic_internal_update(ctrl);
    case 2:
        int rotate = data & 0x80, specific = data & 0x40, eoi = data & 0x20, l = data & 7;
        if (eoi)
        {
            if (specific)
            {
                pic_clear_specific(ctrl, l);
                if (rotate)
                    ;
            }
        }
    }
}

static void pic_writeb(uint16_t port, uint8_t data)
{
    pic_controller* ctrl = &pic.ctrl[port >> 7 & 1];
    if ((port & 1) == 0)
    {
        switch(data >> 3 & 3)
        {
        case 0:
            pic_write_ocw(ctrl, 2, data);
            break;
        case 1:
            pic_write_ocw(ctrl, 3, data);
        default:
            ctrl->in_initialization = 1;
            ctrl->imr = ctrl->isr = ctrl->irr = 0;
            ctrl->priority_base = 7;
            ctrl->autoeoi = ctrl->rotate_on_autoeoi = 0;
            pic_write_icw(ctrl, 1, data);
            break;
        }
    }
    else
    {
        if (ctrl->in_initialization)
            pic_write_icw(ctrl, ctrl->icw_index, data);
        else
            pic_write_ocw(ctrl, 1, data);
    }
}

void pic_init(IOBus* iobus)
{
    iobus->register_io_out8(0x21, pic_writeb);
    iobus->register_io_out8(0xA1, pic_writeb);
    pic_reset();
    pic.irq_bus_value = -1;
}