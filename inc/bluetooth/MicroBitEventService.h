#ifndef MICROBIT_EVENT_SERVICE_H
#define MICROBIT_EVENT_SERVICE_H

#include "MicroBitConfig.h"
#include "ble/BLE.h"
#include "MicroBitEvent.h"
#include "EventModel.h"

// UUIDs for our service and characteristics
extern const uint8_t  MicroBitEventServiceUUID[];
extern const uint8_t  MicroBitEventServiceMicroBitEventCharacteristicUUID[];
extern const uint8_t  MicroBitEventServiceClientEventCharacteristicUUID[];
extern const uint8_t  MicroBitEventServiceMicroBitRequirementsCharacteristicUUID[];
extern const uint8_t  MicroBitEventServiceClientRequirementsCharacteristicUUID[];

struct EventServiceEvent
{
    uint16_t    type;
    uint16_t    reason;
};


/**
  * Class definition for a MicroBit BLE Event Service.
  * Provides a BLE gateway onto an Event Model.
  */
class MicroBitEventService : public MicroBitComponent
{
    public:

    /**
      * Constructor.
      * Create a representation of the EventService
      * @param _ble The instance of a BLE device that we're running on.
      * @param _messageBus An instance of an EventModel which events will be mirrored from.
      */
    MicroBitEventService(BLEDevice &_ble, EventModel &_messageBus);

    /**
     * Periodic callback from MicroBit scheduler.
     * If we're no longer connected, remove any registered Message Bus listeners.
     */
    virtual void idleTick();

    /**
      * Callback. Invoked when any of our attributes are written via BLE.
      */
    void onDataWritten(const GattWriteCallbackParams *params);

    /**
      * Callback. Invoked when any events are sent on the microBit message bus.
      */
    void onMicroBitEvent(MicroBitEvent evt);

    /**
      * Read callback on microBitRequirements characteristic.
      *
      * Used to iterate through the events that the code on this micro:bit is interested in.
      */
    void onRequirementsRead(GattReadAuthCallbackParams *params);

    private:

    // Bluetooth stack we're running on.
    BLEDevice           &ble;
	EventModel	        &messageBus;

    // memory for our event characteristics.
    EventServiceEvent   clientEventBuffer;
    EventServiceEvent   microBitEventBuffer;
    EventServiceEvent   microBitRequirementsBuffer;
    EventServiceEvent   clientRequirementsBuffer;

    // handles on this service's characterisitics.
    GattAttribute::Handle_t microBitEventCharacteristicHandle;
    GattAttribute::Handle_t clientRequirementsCharacteristicHandle;
    GattAttribute::Handle_t clientEventCharacteristicHandle;
    GattCharacteristic *microBitRequirementsCharacteristic;

    // Message bus offset last sent to the client...
    uint16_t messageBusListenerOffset;

};


#endif