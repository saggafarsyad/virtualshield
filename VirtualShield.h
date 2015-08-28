#ifndef VirtualShield_h
#define VirtualShield_h

#include <SoftwareSerial.h>
#include <Arduino.h>

#include "Logger.h"
#include "MQTTMessage.h"

// @todo Modify Bluetooth Pins here
#define RX_PIN 13
#define TX_PIN 12

#define BUFFER_SIZE 128

// Shield Definition
#define SHIELD_COUNT 4

#define SHIELD_OFFSET 48

#define INTERNET 0
#define LOCATION 1
#define ACCELEROMETER 2
#define PUSH_NOTIFICATION 3

// MQTT Messages
#define CONNECT       0x01  // client request to connect to server *
#define CONNACK       0x02  // connect acknowledgment *
#define PUBLISH       0x03  // publish message *
#define PUBACK        0x04  // publish acknowledgment
#define PUBREC        0x05  // publish receive
#define PUBREL        0x06  // publish release
#define PUBCOMP       0x07  // publish complete
#define SUBSCRIBE     0x08  // client subscribe request *
#define SUBACK        0x09  // subscribe acknowledgment *
#define UNSUBSCRIBE   0x0A  // client unsubscribe request *
#define UNSUBACK      0x0B  // unsubscribe acknowledgment *
#define PINGREQ       0x0C  // PING request *
#define PINGRESP      0x0D  // PING response *
#define DISCONNECT    0x0E  // client is disconnecting *

// Callbacks for receiving PUBLISH
typedef void (*SensorCallback)(byte *, byte);
// Callbacks for periodical task
typedef void (*TaskCallback)(void);

class VirtualShield {
	public:
		// Constructor
		VirtualShield(bool debug = false);
		// Initialization
		void begin(long baudRate);
		// Listen for incoming messages
		void listen();
		// Check for Connection
		bool isConnected(void);
		// Sensor Shield
		void addSensor(byte shield, void(*)(byte *, byte));
		void removeSensor(byte shield);
		// Internet Shield
		void addValue(char * param, byte value);
		void addValue(char * param, int value);
		void addValue(char * param, long value);
		void addValue(char * param, float value);
		void addValue(char * param, char * value);		
		void sendValues(byte shield);
		// Set Task
		// @todo: Add function pointer for callback
		void setTask();
		void setTaskPeriod(long millis);
		void runTask(void(*)(void));
		void unsetTask();		
	private:
		// Buffer
		byte buffer[BUFFER_SIZE];
		byte bufferPos;
		void flushBuffer();
		// Write and flush
		void write(uint8_t *bufferPtr, size_t len);
		// Internet Shield
		byte paramCount;
		// Flags
		bool connectFlag;
		bool debugFlag;
		// Periodic Task
		unsigned long taskInterval;
		unsigned long runningInterval;
		// Shield Subscription and Callbacks
		bool isSubscribe[SHIELD_COUNT];
		SensorCallback sensorCallback[SHIELD_COUNT];
		void resetSensor();
		// Internet Shield
		void addValue(char * param, byte * value, byte length, byte cmd);
		// MQTT Sender methods;		
		void sendConnectAck();
		void sendSubscribe(byte topic);
		void sendSubscribeAck();		
		void sendPublish();
		void sendUnsubscribe();
		void sendUnsubscribeAck();
		// Logging
		// void log(char * msg);
		// void log(char * key, byte value);
		// void log(char * key, byte * value, byte len);
		// void log(char * tag, char * msg);
		// void log(char * tag, char * key, byte value);
		// void log(char * tag, char * key, byte * value, byte len);
		// void logTag(char * tag);
		// void logKey(char * key);
};	

#endif