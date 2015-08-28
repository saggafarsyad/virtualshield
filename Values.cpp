#include "Arduino.h"
#include "Values.h"
#include "Logger.h"

Values Decoder = Values();

Location Values::getLocation(byte * data) {
	float tmp[2];
	read(data, tmp);

	Location result;
	result.lat = tmp[0];
	result.lng = tmp[1];	

	return result;
}

Accelerometer Values::getAccelerometer(byte * data) {
	float tmp[3];
	read(data, tmp);

	Accelerometer result;
	result.x = tmp[0];
	result.y = tmp[1];	
	result.z = tmp[2];	
	
	return result;
}

int Values::getInt(byte * data) {
	int result = 0;

	for (byte i = 0; i < 2; i++) {
		byte shifter = i * 8;
		result = result | 
		((int) data[i]) << shifter;             
	}

	return result;      
}

long Values::getLong(byte * data) {
	long result = 0;

	for (byte i = 0; i < 4; i++) {
		byte shifter = i * 8;
		result = result | 
		((long) data[i]) << shifter;
	}

	return result;      
}

float Values::getFloat(byte * data) {	
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

byte Values::read(byte * data, float * targetPtr) {
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