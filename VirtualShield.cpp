#include "VirtualShield.h"

// Initialize Serial
SoftwareSerial Bluetooth(RX_PIN, TX_PIN);

// constructor
VirtualShield::VirtualShield(bool debug)
{
  VirtualShield::debugFlag = debug;  

  // Init Sensors
  for (byte i = 0; i < SHIELD_COUNT; i++) {
  	VirtualShield::sensorCallback[i] = 0;
  }
}

// begin
void VirtualShield::begin(long baudRate) {
	// Set default task inteval
	VirtualShield::taskInterval = 1000;
	// Begin Bluetooth Serial
	Bluetooth.begin(baudRate);
	// @todo Reset Sensors	
	resetSensor();
	// Reset Buffer Position
	VirtualShield::bufferPos = 0;
	// @debug
	log("Ready");
}

void VirtualShield::write(uint8_t *bufferPtr, size_t len) {	
	Bluetooth.write(bufferPtr, len);
	Bluetooth.flush();
}

void VirtualShield::flushBuffer() {	
	for (byte i = 0; i < BUFFER_SIZE; i++) {
		buffer[i] = 0;	
	}
}

void VirtualShield::listen() {
	byte fixedHeader = 0;
	byte remainingLength = 0;
	byte type = 0;	

	if (Bluetooth.available() > 0) 
		fixedHeader = Bluetooth.read();

	if (Bluetooth.available() > 0)
		remainingLength = Bluetooth.read();

	byte i = 0;
	while (Bluetooth.available() > 0 && i < remainingLength) {
		buffer[i] = Bluetooth.read();
		i++;
	}

	// Get Type
	type = fixedHeader >> 4 & 0x0F;	
	// Parse Messages
	switch (type) {
		case CONNECT: {
			log("Connected");
			// Set Connect Flag
			connectFlag = true;
			// Send Connect Acknowledgement
			sendConnectAck();
			// @todo Start sending SUBSCRIBE
			for(byte i = 1; i < SHIELD_COUNT; i++) {
				if (sensorCallback[i] != 0 && isSubscribe[i] == false) {			
					delay(30);
					sendSubscribe(i + SHIELD_OFFSET);											
				}
			}
			break;
		}

		case PUBLISH: {
			log("MQTT", "Topic", buffer[2]);
			log("MQTT", "Buffer", buffer, remainingLength);
		
			break;
		}

		case SUBSCRIBE: {
			break;
		}

		case SUBACK: {			
			// @debug			
			log("MQTT", "SUBACK RECEIVED");
			// Get Topic Length		
			break;
		}

		case UNSUBSCRIBE: {
			break;
		}

		case UNSUBACK: {
			break;
		}

		case DISCONNECT: {
			log("Disconnected");			
			// Set connect flag
			connectFlag = false;
			// Clear subscription
			resetSensor();
			break;
		}
	}
	
	flushBuffer();	
}

// Logger
void VirtualShield::log(char * msg) {
	log("LOG", msg);
}
	
void VirtualShield::log(char * key, byte * value, byte len) {
	log("LOG", key, value, len);
}

void VirtualShield::log(char * key, byte value) {
	log("LOG", key, value);
}

void VirtualShield::log(char * tag, char * msg) {
	logTag(tag);
	Serial.println(msg);
}

void VirtualShield::log(char * tag, char * key, byte value) {
	logTag(tag);
	logKey(key);
	Serial.println(value);
}

void VirtualShield::log(char * tag, char * key, byte * value, byte len) {
	logTag(tag);
	logKey(key);

	Serial.print("[");

	for (byte i = 0; i < len - 1; i++) {
		Serial.print(*value);
		value++;
		Serial.print(',');
	}

	Serial.print(*value);
	Serial.println("]");
}

void VirtualShield::logTag(char * tag) {
	Serial.print('[');
	Serial.print(tag);
	Serial.print("] ");	
}

void VirtualShield::logKey(char * key) {
	Serial.print(key);
	Serial.print(" = ");
}

// Shield 
void VirtualShield::addSensor(byte shield, void(*callback)(byte*, byte)) {
	sensorCallback[shield] = callback;
}

void VirtualShield::resetSensor() {
	for (byte i = 1; i < SHIELD_COUNT; i++) {
		VirtualShield::isSubscribe[i] = false;
	}
}

// MQTT Sender
void VirtualShield::sendConnectAck() {
	byte msg[4] = {32,2,0,0};
  	write(msg, 4);
}

void VirtualShield::sendSubscribe(byte topic) {	
	// @debug
	log("Send SUBSCRIBE", topic);

	// Get Total Length
	byte topicLen = 1;
	byte totalLen = 6 + topicLen;

	// Init Message
	byte msg[totalLen];
	byte i = 0;
	// Add Fixed Header
	msg[i] = SUBSCRIBE << 4; i++;
	// Add Remaining Length
	msg[i] = totalLen - 2; i++;
	// Add Message MSB/LSB
	msg[i] = 0; i++;
	msg[i] = 1; i++;
	// Add Topic MSB/LSB
	msg[i] = 0; i++;
	msg[i] = topicLen; i++;
	// Add Topic
	msg[i] = topic;

	//@debug
	// log("msg", msg, totalLen);

	write(msg, totalLen);
}

// MQTT Message Utilities

