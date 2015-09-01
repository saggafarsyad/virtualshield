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

	char getChar(byte * data);	
	int getInt(byte * data);
	long getLong(byte * data);
	float getFloat(byte * data);
	// Read String, return length
	byte read(byte * data, byte length, char * targetPtr);
	// Read Float to Array, return length
	byte read(byte * data, float * targetPtr);	
		
	byte write (int value, byte *bufferPtr);
	byte write (float value, byte *bufferPtr);
	byte write (long value, byte *bufferPtr);
	byte write (char value, byte *bufferPtr);
	byte write (char * value, byte *bufferPtr);	
};

extern Buffer BufferProcessor;

#endif