#ifndef VirtualShield_h
#define VirtualShield_h

#include <SoftwareSerial.h>
#include <Arduino.h>

#include "Logger.h"
#include "Buffer.h"
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

// PARSER COMMANDS
#define PARSE_INT 1
#define PARSE_LONG 2
#define PARSE_FLOAT 3
#define PARSE_STRING 4

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
		bool isConnected();
		// Sensor Shield
		void addSensor(byte shield, void(*)(byte *, byte));
		void removeSensor(byte shield);
		// Internet Shield
		void addData(char * key, int value);
		void addData(char * key, long value);
		void addData(char * key, float value);
		void addData(char * key, char * value);		
		void sendData();
		// Set Task
		// @todo: Add function pointer for callback
		void runTask();		
		void setTask(void(*)(), unsigned long millis);		
		void unsetTask();		
		// Get Sensor data
		// Sensor
		Location getLocation(byte * buffer);
		Accelerometer getAccelerometer(byte * buffer);
	private:
		// Buffer
		byte buffer[BUFFER_SIZE];
		byte bufferPos;
		// Size of Data saved in buffer
		byte dataCount;
		void flushBuffer();
		// Write and flush
		void write(uint8_t *bufferPtr, size_t len);
		// Internet Shield
		
		// Flags
		bool connectFlag;
		bool debugFlag;
		// Periodic Task
		TaskCallback taskCallback;
		unsigned long taskInterval;
		unsigned long runningInterval;
		// Shield Subscription and Callbacks
		bool isSubscribe[SHIELD_COUNT];
		SensorCallback sensorCallback[SHIELD_COUNT];
		void resetSensor();
		// Internet Shield
		// Add Key to buffer, return buffer position
		byte addKey(char * key, byte cmd);
		// MQTT Sender methods;		
		void sendConnectAck();
		void sendSubscribe(byte topic);
		void sendSubscribeAck();		
		void sendPublish(char * topic, byte topicLength, byte * payload, byte payloadLength);
		void sendUnsubscribe();
		void sendUnsubscribeAck();
		// Logging
};	

#endif	