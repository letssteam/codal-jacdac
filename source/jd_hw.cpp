#include "CodalJacdac.h"

void tim_set_timer(int delta, cb_t cb)
{
    if (codal::CodalJacdac::getInstance() == nullptr) {
        return;
    }

    codal::CodalJacdac::getInstance()->executeAfterMicros(delta, cb);
}

void uart_init(void) {}

int uart_start_tx(const void* data, uint32_t numbytes)
{
    if (codal::CodalJacdac::getInstance() == nullptr) {
        return -1;
    }

    return codal::CodalJacdac::getInstance()->sendData(data, numbytes);
}

void uart_start_rx(void* data, uint32_t maxbytes)
{
    if (codal::CodalJacdac::getInstance() == nullptr) {
        return;
    }

    codal::CodalJacdac::getInstance()->receiveData(data, maxbytes);
}

void uart_disable(void)
{
    if (codal::CodalJacdac::getInstance() == nullptr) {
        return;
    }

    codal::CodalJacdac::getInstance()->disableSerial();
}

int uart_wait_high(void)
{
    // Wait the line is release (to high state) after a start condition (~15 us);
    int us    = 15;
    auto inst = codal::CodalJacdac::getInstance();

    if (inst == nullptr) {
        return -1;
    }

    while (us > 0 && !inst->isPinHigh()) {
        target_wait_us(1);
    }

    return inst->isPinHigh() ? 0 : -1;
}

// make sure data from UART is flushed to RAM (often no-op)
void uart_flush_rx(void)
{
    // No-op
}
