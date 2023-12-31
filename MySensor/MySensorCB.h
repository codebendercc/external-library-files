/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */


#ifndef MySensor_h
#define MySensor_h

#include "Version.h"   // Auto generated by bot
#include "MyConfig.h"
#include "MyHw.h"
#include "MyTransport.h"
#include "MyTransportNRF24.h"
#include "MyParser.h"
#ifdef MY_SIGNING_FEATURE
#include "MySigning.h"
#include "MySigningNone.h"
#endif
#include "MyMessage.h"
#ifdef MY_OTA_FIRMWARE_FEATURE
#include "utility/SPIFlash.h"
#endif
#include <stddef.h>
#include <stdarg.h>


// Set the hardware driver to use (initialized by MySensor-class)
//#if defined __AVR_ATmega328P__
#if defined(ARDUINO_ARCH_ESP8266)
#include "MyHwESP8266.h"
typedef MyHwESP8266 MyHwDriver;
#elif defined(ARDUINO_ARCH_AVR)
#include "MyHwATMega328.h"
typedef MyHwATMega328 MyHwDriver;
#endif
//#endif


#ifdef DEBUG
#define debug(x,...) hw.debugPrint(isGateway, x, ##__VA_ARGS__)
#else
#define debug(x,...)
#endif

#ifdef WITH_LEDS_BLINKING_INVERSE
#define LED_ON 0x1
#define LED_OFF 0x0
#else
#define LED_ON 0x0
#define LED_OFF 0x1
#endif


// EEPROM start address for mysensors library data
#define EEPROM_START 0
// EEPROM location of node id
#define EEPROM_NODE_ID_ADDRESS EEPROM_START
// EEPROM location of parent id
#define EEPROM_PARENT_NODE_ID_ADDRESS (EEPROM_START+1)
// EEPROM location of distance to gateway
#define EEPROM_DISTANCE_ADDRESS (EEPROM_PARENT_NODE_ID_ADDRESS+1)
#define EEPROM_ROUTES_ADDRESS (EEPROM_DISTANCE_ADDRESS+1) // Where to start storing routing information in EEPROM. Will allocate 256 bytes.
#define EEPROM_CONTROLLER_CONFIG_ADDRESS (EEPROM_ROUTES_ADDRESS+256) // Location of controller sent configuration (we allow one payload of config data from controller)
#define EEPROM_FIRMWARE_TYPE_ADDRESS (EEPROM_CONTROLLER_CONFIG_ADDRESS+24)
#define EEPROM_FIRMWARE_VERSION_ADDRESS (EEPROM_FIRMWARE_TYPE_ADDRESS+2)
#define EEPROM_FIRMWARE_BLOCKS_ADDRESS (EEPROM_FIRMWARE_VERSION_ADDRESS+2)
#define EEPROM_FIRMWARE_CRC_ADDRESS (EEPROM_FIRMWARE_BLOCKS_ADDRESS+2)
#define EEPROM_SIGNING_REQUIREMENT_TABLE_ADDRESS (EEPROM_FIRMWARE_CRC_ADDRESS+2)
#define EEPROM_LOCAL_CONFIG_ADDRESS (EEPROM_SIGNING_REQUIREMENT_TABLE_ADDRESS+32) // First free address for sketch static configuration

// Search for a new parent node after this many transmission failures
#define SEARCH_FAILURES  5


struct NodeConfig
{
	uint8_t nodeId; // Current node id
	uint8_t parentNodeId; // Where this node sends its messages
	uint8_t distance; // This nodes distance to sensor net gateway (number of hops)
};

struct ControllerConfig {
	uint8_t isMetric;
};


// Size of each firmware block
#define FIRMWARE_BLOCK_SIZE	16
// Number of times a firmware block should be requested before giving up
#define FIRMWARE_MAX_REQUESTS 5
// Number of times to request a fw block before giving up
#define MY_OTA_RETRY 5
// Number of millisecons before re-request a fw block
#define MY_OTA_RETRY_DELAY 500
// Start offset for firmware in flash (DualOptiboot wants to keeps a signature first)
#define FIRMWARE_START_OFFSET 10
// Bootloader version
#define MY_OTA_BOOTLOADER_MAJOR_VERSION 3
#define MY_OTA_BOOTLOADER_MINOR_VERSION 0
#define MY_OTA_BOOTLOADER_VERSION (MY_OTA_BOOTLOADER_MINOR_VERSION * 256 + MY_OTA_BOOTLOADER_MAJOR_VERSION)


// FW config structure, stored in eeprom
typedef struct {
	uint16_t type;
	uint16_t version;
	uint16_t blocks;
	uint16_t crc;
} __attribute__((packed)) NodeFirmwareConfig;

typedef struct {
	uint16_t type;
	uint16_t version;
	uint16_t blocks;
	uint16_t crc;
	uint16_t BLVersion;
} __attribute__((packed)) RequestFirmwareConfig;

typedef struct {
	uint16_t type;
	uint16_t version;
	uint16_t block;
} __attribute__((packed)) RequestFWBlock;

typedef struct {
	uint16_t type;
	uint16_t version;
	uint16_t block;
	uint8_t data[FIRMWARE_BLOCK_SIZE];
} __attribute__((packed)) ReplyFWBlock;


#ifdef __cplusplus
class MySensor
{
  public:
	/**
	* Constructor
	*
	* Creates a new instance of Sensor class.
	*
	*/
	MySensor(MyTransport &radio =*new MyTransportNRF24(), MyHw &hw=*new MyHwDriver()
#ifdef MY_SIGNING_FEATURE
		, MySigning &signer=*new MySigningNone()
#endif
#ifdef WITH_LEDS_BLINKING
		, uint8_t _rx=DEFAULT_RX_LED_PIN,
		uint8_t _tx=DEFAULT_TX_LED_PIN,
		uint8_t _er=DEFAULT_ERR_LED_PIN,
		unsigned long _blink_period=DEFAULT_LED_BLINK_PERIOD
#endif
		);

	/**
	* Begin operation of the MySensors library
	*
	* Call this in setup(), before calling any other sensor net library methods.
	* @param incomingMessageCallback Callback function for incoming messages from other nodes or controller and request responses. Default is NULL.
	* @param nodeId The unique id (1-254) for this sensor. Default is AUTO(255) which means sensor tries to fetch an id from controller.
	* @param repeaterMode Activate repeater mode. This node will forward messages to other nodes in the radio network. Make sure to call process() regularly. Default in false
	* @param parentNodeId Use this to force node to always communicate with a certain parent node. Default is AUTO which means node automatically tries to find a parent.
	*/

	void begin(void (* msgCallback)(const MyMessage &)=NULL, uint8_t nodeId=AUTO, boolean repeaterMode=false, uint8_t parentNodeId=AUTO);

	/**
	 * Return the nodes nodeId.
	 */
	uint8_t getNodeId();

	/**
	* Each node must present all attached sensors before any values can be handled correctly by the controller.
    * It is usually good to present all attached sensors after power-up in setup().
	*
	* @param sensorId Select a unique sensor id for this sensor. Choose a number between 0-254.
	* @param sensorType The sensor type. See sensor typedef in MyMessage.h.
	* @param description A textual description of the sensor.
	* @param ack Set this to true if you want destination node to send ack back to this node. Default is not to request any ack.
	* @param description A textual description of the sensor.
	*/
	void present(uint8_t sensorId, uint8_t sensorType, const char *description="", bool ack=false);

	/**
	 * Sends sketch meta information to the gateway. Not mandatory but a nice thing to do.
	 * @param name String containing a short Sketch name or NULL  if not applicable
	 * @param version String containing a short Sketch version or NULL if not applicable
	 * @param ack Set this to true if you want destination node to send ack back to this node. Default is not to request any ack.
	 *
	 */
	void sendSketchInfo(const char *name, const char *version, bool ack=false);

	/**
	* Sends a message to gateway or one of the other nodes in the radio network
	*
	* @param msg Message to send
	* @param ack Set this to true if you want destination node to send ack back to this node. Default is not to request any ack.
	* @return true Returns true if message reached the first stop on its way to destination.
	*/
	bool send(MyMessage &msg, bool ack=false);

	boolean sendRoute(MyMessage &message);

	/**
	 * Send this nodes battery level to gateway.
	 * @param level Level between 0-100(%)
	 * @param ack Set this to true if you want destination node to send ack back to this node. Default is not to request any ack.
	 *
	 */
	void sendBatteryLevel(uint8_t level, bool ack=false);

	/**
	* Requests a value from gateway or some other sensor in the radio network.
	* Make sure to add callback-method in begin-method to handle request responses.
	*
	* @param childSensorId  The unique child id for the different sensors connected to this Arduino. 0-254.
	* @param variableType The variableType to fetch
	* @param destination The nodeId of other node in radio network. Default is gateway
	*/
	void request(uint8_t childSensorId, uint8_t variableType, uint8_t destination=GATEWAY_ADDRESS);

	/**
	 * Requests time from controller. Answer will be delivered to callback.
	 *
	 * @param callback for time request. Incoming argument is seconds since 1970.
	 */
	void requestTime(void (* timeCallback)(unsigned long));


	/**
	 * Processes incoming messages to this node. If this is a relaying node it will
	* Returns true if there is a message addressed for this node just was received.
	* Use callback to handle incoming messages.
	*/
	boolean process();

	/**
	 * Returns the most recent node configuration received from controller
	 */
	ControllerConfig getConfig();

	/**
	 * Save a state (in local EEPROM). Good for actuators to "remember" state between
	 * power cycles.
	 *
	 * You have 256 bytes to play with. Note that there is a limitation on the number
	 * of writes the EEPROM can handle (~100 000 cycles on ATMega328).
	 *
	 * @param pos The position to store value in (0-255)
	 * @param Value to store in position
	 */
	void saveState(uint8_t pos, uint8_t value);

	/**
	 * Load a state (from local EEPROM).
	 *
	 * @param pos The position to fetch value from  (0-255)
	 * @return Value to store in position
	 */
	uint8_t loadState(uint8_t pos);

	/**
	* Returns the last received message
	*/
	MyMessage& getLastMessage(void);



	/**
	 * Wait for a specified amount of time to pass.  Keeps process()ing.
	 * This does not power-down the radio nor the Arduino.
	 * Because this calls process() in a loop, it is a good way to wait
	 * in your loop() on a repeater node or sensor that listens to messages.
	 * @param ms Number of milliseconds to sleep.
	 */
	void wait(unsigned long ms);

	/**
	 * Sleep (PowerDownMode) the MCU and radio. Wake up on timer.
	 * @param ms Number of milliseconds to sleep.
	 */
	void sleep(unsigned long ms);

	/**
	 * Sleep (PowerDownMode) the MCU and radio. Wake up on timer or pin change.
	 * See: http://arduino.cc/en/Reference/attachInterrupt for details on modes and which pin
	 * is assigned to what interrupt. On Nano/Pro Mini: 0=Pin2, 1=Pin3
	 * @param interrupt Interrupt that should trigger the wakeup
	 * @param mode RISING, FALLING, CHANGE
	 * @param ms Number of milliseconds to sleep or 0 to sleep forever
	 * @return true if wake up was triggered by pin change and false means timer woke it up.
	 */
	bool sleep(uint8_t interrupt, uint8_t mode, unsigned long ms=0);

	/**
	 * Sleep (PowerDownMode) the MCU and radio. Wake up on timer or pin change for two separate interrupts.
	 * See: http://arduino.cc/en/Reference/attachInterrupt for details on modes and which pin
	 * is assigned to what interrupt. On Nano/Pro Mini: 0=Pin2, 1=Pin3
	 * @param interrupt1 First interrupt that should trigger the wakeup
	 * @param mode1 Mode for first interrupt (RISING, FALLING, CHANGE)
	 * @param interrupt2 Second interrupt that should trigger the wakeup
	 * @param mode2 Mode for second interrupt (RISING, FALLING, CHANGE)
	 * @param ms Number of milliseconds to sleep or 0 to sleep forever
	 * @return Interrupt number wake up was triggered by pin change and negative if timer woke it up.
	 */
	int8_t sleep(uint8_t interrupt1, uint8_t mode1, uint8_t interrupt2, uint8_t mode2, unsigned long ms=0);

#ifdef WITH_LEDS_BLINKING
	/**
	 * Blink with LEDs
	 * @param cnt how many blink cycles to keep the LED on. Default cycle is 300ms
	 */
	void rxBlink(uint8_t cnt);
	void txBlink(uint8_t cnt);
	void errBlink(uint8_t cnt);
#endif

  protected:
	NodeConfig nc; // Essential settings for node to work
	ControllerConfig cc; // Configuration coming from controller
#ifdef MY_OTA_FIRMWARE_FEATURE
	NodeFirmwareConfig fc;
	bool fwUpdateOngoing;
	unsigned long fwLastRequestTime;
	uint16_t fwBlock;
	uint8_t fwRetry;
	SPIFlash flash;
#endif

	bool repeaterMode;
	bool autoFindParent;
	bool isGateway;

	MyMessage msg;  // Buffer for incoming messages.
	MyMessage tmpMsg ;  // Buffer for temporary messages (acks and nonces among others).

#ifdef WITH_LEDS_BLINKING
	uint8_t pinRx; // Rx led pin
	uint8_t pinTx; // Tx led pin
	uint8_t pinEr; // Err led pin

	// these variables don't need to be volatile, since we are not using interrupts
	uint8_t countRx;
	uint8_t countTx;
	uint8_t countErr;

	unsigned long ledBlinkPeriod;
	void handleLedsBlinking(); // do the actual blinking
	unsigned long blink_next_time;
#endif

	MyTransport& radio;
#ifdef MY_SIGNING_FEATURE
	uint8_t doSign[32]; // Bitfield indicating which sensors require signed communication
	MyMessage msgSign;  // Buffer for message to sign.
	MySigning& signer;
#endif
	MyHw& hw;

	boolean sendWrite(uint8_t dest, MyMessage &message);

  private:
#ifdef DEBUG
	char convBuf[MAX_PAYLOAD*2+1];
#endif
	uint8_t failedTransmissions;
    void (*timeCallback)(unsigned long); // Callback for requested time messages
    void (*msgCallback)(const MyMessage &); // Callback for incoming messages from other nodes and gateway.

#ifdef MY_OTA_FIRMWARE_FEATURE
// do a crc16 on the whole received firmware
    bool isValidFirmware();
#endif


    void requestNodeId();
	void setupNode();
	void findParentNode();
	uint8_t crc8Message(MyMessage &message);
};
#endif

#endif
