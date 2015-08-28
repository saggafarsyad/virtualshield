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
	Log.w("Ready");
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

	if (Bluetooth.available() > 0) 
		fixedHeader = Bluetooth.read();

	if (Bluetooth.available() > 0)
		remainingLength = Bluetooth.read();

	byte i = 0;
	while (Bluetooth.available() > 0 && i < remainingLength) {
		buffer[i] = Bluetooth.read();
		i++;
	}

	// Create MQTTMessage
	MQTTMessage mqtt(fixedHeader, remainingLength, buffer);
	// Parse Messages
	switch (mqtt.getType()) {
		case CONNECT: {
			Log.w("Connected");
			// Set Connect Flag
			connectFlag = true;
			// Send Connect Acknowledgement
			sendConnectAck();
			// @todo Start sending SUBSCRIBE
			for(byte i = 1; i < SHIELD_COUNT; i++) {
				if (sensorCallback[i] != 0 && isSubscribe[i] == false) {			
					// @debug
					Log.w("MQTT", "Send SUBSCRIBE", i);
					delay(30);
					sendSubscribe(i + SHIELD_OFFSET);	
					isSubscribe[i] = true;
				}
			}
			break;
		}

		case PUBLISH: {
			// @debug
			// Log.w("MQTT", "PUBLISH Received");
			// Log.w("MQTT", "Buffer", buffer, remainingLength);
			
			// Get Topic
			byte *topicPtr;
			byte topicLen = mqtt.readTopic(&topicPtr);
			// Get Shield
			byte shield = *topicPtr - SHIELD_OFFSET;
			Log.w("Shield", shield);
			// Call Callbacks
			if (shield < SHIELD_COUNT && 
				sensorCallback[shield] != 0 && 
				isSubscribe[shield] == true) {
				// Get Payload Pointer
				byte *payloadPtr;
				byte payloadLength = mqtt.readPayload(&payloadPtr);

				// @debug
				// Log.w("payload", payloadPtr, payloadLength);

				// // Get Callback
				void (*receiver)(byte * payload, byte length) = sensorCallback[shield];
				// // Run Callback
				receiver(payloadPtr, payloadLength);								
			}

			break;
		}

		case SUBSCRIBE: {
			break;
		}

		case SUBACK: {			
			// @debug			
			// Log.w("MQTT", "SUBACK Received");
			// Log.w("MQTT", "Buffer", buffer, remainingLength);

			// // Get Topic
			// byte *topicPtr;
			// mqtt.readTopic(&topicPtr);

			// byte shield = *topicPtr - SHIELD_OFFSET;

			// if (sensorCallback[shield] != 0 && isSubscribe[shield] == false) {
			// 	isSubscribe[shield] = true;

			// 	// @debug
			// 	Log.w("MQTT", "Topic Subscribed", shield);
			// }
			
			// break;
		}

		case UNSUBSCRIBE: {
			break;
		}

		case UNSUBACK: {
			break;
		}

		case DISCONNECT: {
			Log.w("Disconnected");			
			// Set connect flag
			connectFlag = false;
			// Clear subscription
			resetSensor();
			break;
		}
	}
	
	// flushBuffer();	
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
	// Log.w("Send SUBSCRIBE", topic);

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
	// Log.w("msg", msg, totalLen);

	write(msg, totalLen);
}