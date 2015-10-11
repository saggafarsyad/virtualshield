#ifndef MQTTMessage_h
#define MQTTMessage_h

#include "Arduino.h"

class MQTTMessage {

public:		
	MQTTMessage(byte fixedHeader, byte remainingLength, byte * bufferPtr);	
	byte getType();
	byte getRemainingLength();	
	byte readPayload(byte **targetPayload);
	byte readTopic(byte **targetTopic);
	// byte getBuffer(byte position);	
	// byte readBuffer(byte * targetBuffer);	
	// byte readBuffer(byte startPos, byte length, byte * targetBuffer);
	// byte readPayload(byte payloadLength, byte * targetPayload);
	// // byte readTopic(byte topicLength, char * targetTopic);	
	// bool isTopicEqual(char * topic, byte topicLength);
	// byte getTopicLength();
	// byte * getBuffer();

private:
	byte fixedHeader;
	byte remainingLength;
	byte *bufferPtr;

	byte getTopicLength();
	byte getPayloadLength();

	byte getLength(byte msb, byte lsb);
	// byte publishTopicLength();	
	// byte publishTopic(byte topicLength, char * targetTopic);
	
	// byte subscribeTopicLength();
	// byte subscribeTopic(byte topicLength, char * targetTopic);		
};

#endif