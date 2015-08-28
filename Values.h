#ifndef Values_h
#define Values_h

#include "Arduino.h"

typedef struct {
	float lat;
	float lng;
} Location;

typedef struct {
	float x;
	float y;
	float z;
} Accelerometer;

class Values {

public:
	// Default constructor
	Values(){};
	// Sensor
	Location getLocation(byte * data);
	Accelerometer getAccelerometer(byte * data);	

private: 		
	char getChar(byte * data);	
	int getInt(byte * data);
	long getLong(byte * data);
	float getFloat(byte * data);
	// Read String, return length
	byte read(byte * data, byte length, char * targetPtr);
	// Read Float to Array, return length
	byte read(byte * data, float * targetPtr);		
};

extern Values Decoder;

#endif