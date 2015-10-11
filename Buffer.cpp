#include "Arduino.h"
#include "Buffer.h"
#include "Logger.h"

Buffer BufferProcessor = Buffer();

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

byte Buffer::write(char * value, byte *buffer) {
	byte i = 0;

	while (value[i] != '\0' && i < 128) {
		*buffer = (byte) value[i];
		i++; buffer++;
	}

	return i;
}

byte Buffer::write(float value, byte *buffer) {
	union floatByte { 
    	byte b[4];
    	float f;
  	} res;

	res.f = value; 
	byte i = 0;
	while (i < 4) {
		*buffer = res.b[i];
		i++; buffer++;
	}

	return i;
}

byte Buffer::write(int value, byte *buffer) {
	*buffer = value & 0xFF; buffer++;
	*buffer = (value >> 8) & 0xFF;

	return 2;
}

byte Buffer::write(long value, byte *buffer) {
  *buffer = value & 0xFF; buffer++;
  *buffer = (value >> 8) & 0xFF; buffer++;
  *buffer = (value >> 16) & 0xFF; buffer++;
  *buffer = (value >> 24) & 0xFF; buffer++;

  return 4;
}