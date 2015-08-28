#include "Arduino.h"
#include "MQTTMessage.h"
#include "VirtualShield.h"

MQTTMessage::MQTTMessage(byte fixedHeader, byte remainingLength, byte * bufferPtr) {
	MQTTMessage::fixedHeader = fixedHeader;
	MQTTMessage::remainingLength = remainingLength;
	MQTTMessage::bufferPtr = bufferPtr;
}

byte MQTTMessage::getType() {
	return MQTTMessage::fixedHeader >> 4 & 0x0F;	
}

byte MQTTMessage::readTopic(byte **targetTopic) {	
	byte length;
	byte index;
	// Get length
	switch (MQTTMessage::getType()) {
		case PUBLISH: {
			index = 0;
			length = (256 * bufferPtr[index] + bufferPtr[index++]);
			index++;
			break;
		}

		case SUBACK: {			
			index =  2;
			// Get length
			length = (256 * bufferPtr[index]) + bufferPtr[index++];
			index++;
			break;
		}

		default: return SHIELD_COUNT;
	}
	// Assign Topic Pointer		
	byte *tmpPtr = bufferPtr;
	tmpPtr+=index;		

	*targetTopic = tmpPtr;

	// @debug
	// log("Index", index);
	// log("TopicPtr", topicPtr, 1);

	// Return Length
	return length;
}

byte MQTTMessage::getLength(byte msb, byte lsb) {
	return (256 * msb) + lsb;
}

byte MQTTMessage::readPayload(byte **targetPayload) {		
	byte length;
	byte index;
	// Get length
	switch (MQTTMessage::getType()) {
		case PUBLISH: {			
			index = 0;
			byte topicLength = getLength(bufferPtr[index], bufferPtr[index++]);
			length = MQTTMessage::remainingLength - (topicLength + 2);
			index+=topicLength + 2;
			break;
		}

		default: return SHIELD_COUNT;
	}
	// Assign Topic Pointer		
	byte *tmpPtr = bufferPtr;
	tmpPtr+=index;		

	*targetPayload = tmpPtr;

	// @debug
	// log("Index", index);
	// log("TopicPtr", topicPtr, 1);

	// Return Length
	return length;
}

	