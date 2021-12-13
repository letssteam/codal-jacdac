#include "CodalJacdac.h"

void jd_tx_init(void)
{
    // Nothing to do
}

void jd_tx_flush(void)
{
    // if (target_in_irq()) jd_panic();
    // if (isSending) return;
    // if (sendFrame[bufferPtr].size == 0) return;
    // sendFrame[bufferPtr].device_identifier = jd_device_id();
    // jd_compute_crc(&sendFrame[bufferPtr]);
    // bufferPtr ^= 1;
    // jd_packet_ready();
    // jd_services_packet_queued();

    // jd_reset_frame(&sendFrame[bufferPtr]);

    auto inst = codal::CodalJacdac::getInstance();

    if (inst == nullptr) return;

    if (inst->isIdle() || !inst->isSendPending()) {
        return;
    }

    // TO DO
}

int jd_tx_is_idle(void)
{
    auto inst = codal::CodalJacdac::getInstance();
    if (inst->isIdle() || !inst->isSendPending()) {
        return 1;
    }

    return 0;
}

// bridge between phys and queue imp, phys calls this to get the next frame.
jd_frame_t* jd_tx_get_frame(void)
{
    if (codal::CodalJacdac::getInstance() == nullptr) {
        return NULL;
    }

    return codal::CodalJacdac::getInstance()->nextSendFrame();
}

// bridge between phys and queue imp, marks as sent.
void jd_tx_frame_sent(jd_frame_t* frame)
{
    delete frame;
}

int jd_send(unsigned service_num, unsigned service_cmd, const void* data, unsigned service_size)
{
    // if (target_in_irq()) jd_panic();

    if (codal::CodalJacdac::getInstance() == nullptr) {
        return -1;
    }

    jd_frame_t* frame = new jd_frame_t();

    jd_reset_frame(frame);
    void* trg = jd_push_in_frame(frame, service_num, service_cmd, service_size);

    if (!trg) {
        return -1;
    }

    if (data) {
        memcpy(trg, data, service_size);
    }

    frame->device_identifier = jd_device_id();

    codal::CodalJacdac::getInstance()->addSendFrame(frame);

    return 0;
}