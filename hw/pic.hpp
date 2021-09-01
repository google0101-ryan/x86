#include "cpu.hpp"
#include "IOBus.hpp"

struct pic_controller
{
    uint8_t vector_offset;
    uint8_t imr, irr, isr;

    uint8_t pin_state;

    uint8_t icw[5], icw_index;
    uint8_t ocw[4];

    uint8_t read_isr;

    uint8_t autoeoi, rotate_on_autoeoi;

    uint8_t priority_base; // lowest priority base

    uint8_t in_initialization;

    uint8_t highest_priority_irq_to_send;

    // Has the INTR line been raised?
    uint8_t raised_intr_line;

    // PCI stuff
    uint8_t elcr;
};

struct
{
    int irq_bus_value;
    struct pic_controller ctrl[2];
} pic;

void pic_init(IOBus* iobus);