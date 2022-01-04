#include "CodalJacdac.h"

#include "EventModel.h"
#include "Timer.h"
#include "jd_protocol.h"

using namespace codal;

CodalJacdac* CodalJacdac::instance = nullptr;

CodalJacdac* CodalJacdac::getInstance(SingleWireSerial* serial, uint16_t id)
{
    if (instance == nullptr && serial != nullptr) {
        instance = new CodalJacdac(serial, id);
    }

    return instance;
}

int CodalJacdac::addReceivedFrame(jd_frame_t* frame)
{
    int ret = 0;
    if (receiveQueue.size() >= JD_RECEIVE_FIFO_SIZE) {
        receiveQueue.pop();
        ret = 1;
    }

    receiveQueue.push(frame);
    return ret;
}

jd_frame_t* CodalJacdac::nextReceivedFrame()
{
    jd_frame_t* ret = NULL;

    if (!receiveQueue.empty()) {
        ret = receiveQueue.front();
        receiveQueue.pop();
    }

    return ret;
}

int CodalJacdac::addSendFrame(jd_frame_t* frame)
{
    int ret = 0;
    if (sendQueue.size() >= JD_SEND_FIFO_SIZE) {
        sendQueue.pop();
        ret = 1;
    }

    sendQueue.push(frame);
    return ret;
}

jd_frame_t* CodalJacdac::nextSendFrame()
{
    jd_frame_t* ret = NULL;

    if (!sendQueue.empty()) {
        ret = sendQueue.front();
        sendQueue.pop();
    }

    return ret;
}

void CodalJacdac::executeAfterMicros(int us, cb_t cb)
{
    for (unsigned i = 0; i < JD_EVENT_LIST_SIZE; ++i) {
        if (eventList[i] == NULL) {
            eventList[i] = cb;
            system_timer_event_after_us((uint32_t)us, DEVICE_ID_JACDAC, i);
            return;
        }
    }
}

int CodalJacdac::sendData(const void* data, uint32_t numbytes)
{
    instance->sendingState();
    return instance->serial->send((uint8_t*)data, numbytes);
}

int CodalJacdac::receiveData(void* data, uint32_t maxbytes)
{
    instance->receivingState();
    return instance->serial->receive((uint8_t*)data, maxbytes);
}

void CodalJacdac::disableSerial()
{
    instance->idleState();
}

bool CodalJacdac::isPinHigh()
{
    return instance->serial->p.getDigitalValue() == 1;
}

int CodalJacdac::init()
{
    // Nothing to do
    return DEVICE_OK;
}

void CodalJacdac::periodicCallback()
{
    jd_process_everything();
}

void CodalJacdac::idleCallback() {}

CodalJacdac::CodalJacdac(SingleWireSerial* serial, uint16_t id)
    : CodalComponent(id, 0), serial(serial), state(JacdacState::IDLE)
{
    tim_init();
    uart_init();
    jd_init();

    serial->setMode(SingleWireMode::SingleWireDisconnected);
    serial->setBaud(1000000);

    serial->p.setIRQ(pinIRQHandler);
    idleState();

    for (unsigned i = 0; i < JD_EVENT_LIST_SIZE; ++i) {
        eventList[i] = NULL;
    }

    if (EventModel::defaultEventBus)
        EventModel::defaultEventBus->listen(id, DEVICE_ID_JACDAC, this, &CodalJacdac::onTimeoutEvent,
                                            MESSAGE_BUS_LISTENER_IMMEDIATE);
}

CodalJacdac::~CodalJacdac() {}

void CodalJacdac::idleState()
{
    instance->serial->setMode(SingleWireMode::SingleWireDisconnected);
    instance->serial->p.eventOn(DEVICE_PIN_EVENT_ON_EDGE);
    instance->serial->setIRQ(NULL);
    instance->state = JacdacState::IDLE;
}

void CodalJacdac::receivingState()
{
    instance->serial->setMode(SingleWireMode::SingleWireRx);
    instance->serial->p.eventOn(DEVICE_PIN_EVENT_NONE);
    instance->serial->setIRQ(serialIRQHandler);
    instance->state = JacdacState::RECEIVING;
}

void CodalJacdac::sendingState()
{
    instance->serial->setMode(SingleWireMode::SingleWireTx);
    instance->serial->p.eventOn(DEVICE_PIN_EVENT_NONE);
    instance->serial->setIRQ(serialIRQHandler);
    instance->state = JacdacState::SENDING;
}

void CodalJacdac::onTimeoutEvent(Event evt)
{
    if (evt.value < JD_EVENT_LIST_SIZE && eventList[evt.value] != NULL) {
        eventList[evt.value]();
        eventList[evt.value] = NULL;
    }
}

void CodalJacdac::pinIRQHandler(int event)
{
    if (instance == nullptr) return;

    switch (event) {
        case DEVICE_PIN_EVT_FALL:
            if (instance->state == IDLE) {
                instance->receivingState();
            }
            break;

        case DEVICE_PIN_EVT_RISE:
            break;

        default:
            break;
    }
}

void CodalJacdac::serialIRQHandler(uint16_t errCode)
{
    if (instance == nullptr) return;

    switch (errCode) {
        case SWS_EVT_DATA_RECEIVED:
            instance->idleState();
            // Read data
            break;

        case SWS_EVT_DATA_SENT:
            instance->idleState();
            break;

        case SWS_EVT_ERROR:
        case SWS_EVT_DATA_DROPPED:
            if (instance->state == JacdacState::RECEIVING) {
                // Flush the receive buffer ?
            }
            else if (instance->state == JacdacState::SENDING) {
                // Flush the sending buffer ?
            }

            instance->idleState();
            break;

        default:
            break;
    }
}