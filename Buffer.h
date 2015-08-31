#ifndef Buffer_h
#define Buffer_h

#include "Arduino.h"

// Shield Types
typedef struct {
	float lat;
	float lng;
} Location;

typedef struct {
	float x;
	float y;
	float z;
} Accelerometer;

class Buffer {

public:
	// Default constructor
	Buffer(){};
	// setBuffer(byte * buffer);
	// setBufferPos(byte position);
	// getBuffer();
	// getBufferPos();	

	// Sensor
	Location getLocation(byte * data);
	Accelerometer getAccelerometer(byte * data);	

private: 		
	byte * payload();

	char getChar(byte * data);	
	int getInt(byte * data);
	long getLong(byte * data);
	float getFloat(byte * data);
	// Read String, return length
	byte read(byte * data, byte length, char * targetPtr);
	// Read Float to Array, return length
	byte read(byte * data, float * targetPtr);	
		
	byte write (int value);
	byte write (float value);
	byte write (long value);
	byte write (char value);
	byte write (char * value);

};

extern Buffer Decoder;

#endif