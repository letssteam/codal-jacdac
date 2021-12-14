#pragma once

#include <queue>

#include "CodalComponent.h"
#include "Event.h"
#include "SingleWireSerial.h"
#include "jd_protocol.h"

#ifndef JD_RECEIVE_FIFO_SIZE
#define JD_RECEIVE_FIFO_SIZE 16
#endif

#ifndef JD_SEND_FIFO_SIZE
#define JD_SEND_FIFO_SIZE 16
#endif

#ifndef JD_EVENT_LIST_SIZE
#define JD_EVENT_LIST_SIZE 16
#endif

namespace codal {

enum JacdacState { IDLE, RECEIVING, SENDING };

class CodalJacdac : public CodalComponent {
  public:
    static CodalJacdac* getInstance(codal::SingleWireSerial* serial = nullptr, uint16_t id = DEVICE_ID_JACDAC);

    /**
     * @brief Add the frame to the receive queue
     *
     * @param frame the frame received
     * @return int 1 if the oldest frame has been dropped, 0 otherwise
     */
    int addReceivedFrame(jd_frame_t* frame);

    /**
     * @brief Get the oldest frame received
     *
     * @return the oldest frame or NULL if there is no frame in the queue
     */
    jd_frame_t* nextReceivedFrame();

    /**
     * @brief Add the frame to sending queue
     *
     * @param frame the frame to queue
     * @return int 1 if the oldest frame has been dropped, 0 otherwise
     */
    int addSendFrame(jd_frame_t* frame);

    /**
     * @brief Get the oldest frame to send
     *
     * @return the oldest frame or NULL if there is no frame in the queue
     */
    jd_frame_t* nextSendFrame();

    /**
     * @brief Get if there is frame to send
     *
     * @return true if there are pending frame false otherwise
     */
    bool isSendPending() { return sendQueue.size() > 0; }

    /**
     * @brief Get if the system is on idle state
     *
     * @return true The system is idle false otherwise
     */
    bool isIdle() { return state == JacdacState::IDLE; }

    /**
     * @brief Execute a callback after a defined amount of time
     *
     * @param us The time to wait (in µs) before executing the calback
     * @param cb The callback to execute (should be greter than 0)
     */
    void executeAfterMicros(int us, cb_t cb);

    /**
     * @brief Send data over the Serial
     *
     * @param data data to send
     * @param numbytes number of bytes to send
     * @return int 0 if success, -1 otherwise
     */
    int sendData(const void* data, uint32_t numbytes);

    /**
     * @brief Receive data over the Serial
     *
     * @param data pointer to store data
     * @param maxbytes maximum amount of bytes to receive
     * @return int
     */
    int receiveData(void* data, uint32_t maxbytes);

    /**
     * @brief Disable the serail, and go back to IDLE
     *
     */
    void disableSerial();

    /**
     * @brief Retrieve if the pin digitalValue is HIGH
     *
     * @return true if the digitalValue is HIGH, false otherwise
     */
    bool isPinHigh();

    /**
     * Implement this function to receive a function call after the devices'
     * device model has been instantiated.
     */
    virtual int init() final override;

    /**
     * Implement this function to receive a callback every SCHEDULER_TICK_PERIOD_MS.
     */
    virtual void periodicCallback() final override;

    /**
     * Implement this function to receive a callback when the device is idling.
     */
    virtual void idleCallback() final override;

  private:
    static CodalJacdac* instance;
    codal::SingleWireSerial* serial;
    JacdacState state;
    std::queue<jd_frame_t*> receiveQueue;
    std::queue<jd_frame_t*> sendQueue;
    cb_t eventList[JD_EVENT_LIST_SIZE];

    CodalJacdac(codal::SingleWireSerial* serial, uint16_t id = DEVICE_ID_JACDAC);
    ~CodalJacdac();

    void idleState();
    void receivingState();
    void sendingState();

    /**
     * Event handler, called when a requested time has elapsed (used for brightness control).
     */
    void onTimeoutEvent(Event evt);

    static void pinIRQHandler(int event);
    static void serialIRQHandler(uint16_t errCode);
};

}  // namespace codal