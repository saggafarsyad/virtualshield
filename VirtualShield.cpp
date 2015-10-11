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
	// Begin Serials
	Bluetooth.begin(baudRate);
	Log.init(baudRate);
	// @todo Reset Sensors	
	resetSensor();
	// Unset Task
	unsetTask();
	// Reset Buffer Position
	flushBuffer();
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

	bufferPos = 0;
	dataCount = 0;
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
					// Log.w("MQTT", "Send SUBSCRIBE", i);
					delay(30);
					sendSubscribe(i + SHIELD_OFFSET);	
					isSubscribe[i] = true;
				}
			}
			break;
		}

		case PUBLISH: {
			// @debug
			Log.w("MQTT", "PUBLISH Received");
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

Location VirtualShield::getLocation(byte * data) {
	float tmp[2];
	BufferProcessor.read(data, tmp);

	Location result;
	result.lat = tmp[0];
	result.lng = tmp[1];	

	return result;
}

Accelerometer VirtualShield::getAccelerometer(byte * data) {
	float tmp[3];
	BufferProcessor.read(data, tmp);

	Accelerometer result;
	result.x = tmp[0];
	result.y = tmp[1];	
	result.z = tmp[2];	
	
	return result;
}

void VirtualShield::runTask() {	
	unsigned long currentInterval = millis();

  	if (currentInterval - runningInterval >= taskInterval) {        
    	runningInterval = currentInterval;

	    // Call Task
	    if (taskCallback != 0) {
      	  	void (*task)() = taskCallback;
      		task();  
      		return;
    	}      
  	}
}

void VirtualShield::setTask(void(*callback)(), unsigned long millis) {
	VirtualShield::taskInterval = millis;
	VirtualShield::taskCallback = callback;
}	

void VirtualShield::unsetTask() {
	VirtualShield::taskCallback = 0;
}	

byte VirtualShield::addKey(char * key, byte cmd) {	
	// Log.w("Parse Command", cmd);	
	// Write Key to Buffer, return length
	byte keyLength = BufferProcessor.write(key, &buffer[VirtualShield::bufferPos + 1]);	
	// Check keyLength
	if (keyLength > 31) return -1;
	// Add Key Header, consist Command to Parse and Length
	buffer[bufferPos] = cmd << 5 | keyLength;	
	// @debug
	// Log.w(key, keyLength);
	// Return last buffer position
	return keyLength + 1;
}

void VirtualShield::addData(char * key, int value) {
	// Write key and get buffer position
	byte keyLength = addKey(key, PARSE_INT); 
	byte index = bufferPos + keyLength;
	// Write value to buffer
	byte valueLength = BufferProcessor.write(value, &buffer[index + 1]);		
	// Add value header, consist of value length
	buffer[index] = valueLength;
	// Save last buffer position	
	bufferPos += keyLength + valueLength + 1;
	// Save param count
	dataCount++;	
	// @debug
	// Log.w("Buffer", buffer, bufferPos);	
}

void VirtualShield::addData(char * key, long value) {
	// Write key and get buffer position
	byte keyLength = addKey(key, PARSE_LONG); 
	byte index = bufferPos + keyLength;
	// Write value to buffer
	byte valueLength = BufferProcessor.write(value, &buffer[index + 1]);		
	// Add value header, consist of value length
	buffer[index] = valueLength;
	// Save last buffer position	
	bufferPos += keyLength + valueLength + 1;
	// Save param count
	dataCount++;	
	// @debug
	// Log.w("Buffer", buffer, bufferPos);	
}

void VirtualShield::addData(char * key, float value) {
	// Write key and get buffer position
	byte keyLength = addKey(key, PARSE_FLOAT); 
	byte index = bufferPos + keyLength;
	// Write value to buffer
	byte valueLength = BufferProcessor.write(value, &buffer[index + 1]);		
	// Add value header, consist of value length
	buffer[index] = valueLength;
	// Save last buffer position	
	bufferPos += keyLength + valueLength + 1;
	// Save param count
	dataCount++;	
	// @debug
	// Log.w("Buffer", buffer, bufferPos);	
}

void VirtualShield::addData(char * key, char * value) {
	// Write key and get buffer position
	byte keyLength = addKey(key, PARSE_STRING); 
	byte index = bufferPos + keyLength;
	// Write value to buffer
	byte valueLength = BufferProcessor.write(value, &buffer[index + 1]);		
	// Add value header, consist of value length
	buffer[index] = valueLength;
	// Save last buffer position	
	bufferPos += keyLength + valueLength + 1;
	// Save param count
	dataCount++;	
	// @debug
	// Log.w("Buffer", buffer, bufferPos);	
}

void VirtualShield::sendData() {
	if (isConnected()) {
		// Send Publish
		sendPublish("is", 2, buffer, bufferPos);
		// Flush buffer
		flushBuffer();
	}
}

void VirtualShield::sendPublish(char * topic, byte topicLength, byte * payload, byte payloadLength) {
	// Init messages
	byte totalLength = topicLength + payloadLength + 5;
	byte msg[totalLength];
	byte index = 0;

	// Fixed Header
	msg[index] = 48; index++;
	// Remaining Length
	msg[index] = totalLength - 2; index++;
	// Topic MSB/LSB
	msg[index] = 0; index++;
	msg[index] = topicLength; index++;
	// Topic
	for (byte x = 0; x < topicLength; x++) {
		msg[index] = (byte) topic[x];
		index++;
	}	
	// Payload
	// Add Data Count
	msg[index] = dataCount; index++;

	for (byte y = 0; y < payloadLength; y++) {
		msg[index] = payload[y];
		index++;
	}

	write(msg, totalLength);

	// @debug
	// Log.w("index", index);
	// Log.w("totalLength", totalLength);
	// Log.w("MQTT PUBLISH", msg, totalLength);
}

bool VirtualShield::isConnected() {
	return connectFlag;
}