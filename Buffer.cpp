#include "Arduino.h"
#include "Buffer.h"
#include "Logger.h"

Buffer Decoder = Buffer();

int Buffer::getInt(byte * data) {
	int result = 0;

	for (byte i = 0; i < 2; i++) {
		byte shifter = i * 8;
		result = result | 
		((int) data[i]) << shifter;             
	}

	return result;      
}

long Buffer::getLong(byte * data) {
	long result = 0;

	for (byte i = 0; i < 4; i++) {
		byte shifter = i * 8;
		result = result | 
		((long) data[i]) << shifter;
	}

	return result;      
}

float Buffer::getFloat(byte * data) {	
	union floatByte { 
		byte b[4];
		float f;
	} result;

	for (byte i = 0; i < 4; i++) {
		result.b[i] = *data;
		data++;
	}

	return result.f;
}

byte Buffer::read(byte * data, float * targetPtr) {
	byte len = data[0];

	for (byte i = 0; i < len; i++) {
		byte tmp[4];
		byte k = 0;

		for (byte j = 4; j > 0; j--) {
			tmp[k] = data[j+((i*4))];

			// @debug
			// Serial.println(j+((i*4)));
			k++;
		}
 
		// @debug
		// Serial.println("***");

		targetPtr[i] = getFloat(tmp);
	}

	return len;
}