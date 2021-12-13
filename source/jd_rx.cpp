#include "CodalJacdac.h"

void jd_rx_init(void)
{
    // Nothing to do.
}

/**
 * @brief Save the received frame
 *
 * @param frame the frame received
 * @return int 1 if the oldest frame has been drop, 0 otherwise
 */
int jd_rx_frame_received(jd_frame_t* frame)
{
    if (codal::CodalJacdac::getInstance() == nullptr) {
        return 1;
    }

    return codal::CodalJacdac::getInstance()->addReceivedFrame(frame);
}

jd_frame_t* jd_rx_get_frame(void)
{
    if (codal::CodalJacdac::getInstance() == nullptr) {
        return NULL;
    }

    return codal::CodalJacdac::getInstance()->nextReceivedFrame();
}