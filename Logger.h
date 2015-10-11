#ifndef Log_h
#define Log_h

#include "Arduino.h"

#define DEFAULT_TAG "LOG"
// @todo: Documentation

class Logger {

public:
	Logger(){};
	// Init
	void init(long baudRate);
	// Messages
	void w(char * msg);
	void w(char * tag, char * msg);
	// Bytes	
	void w(char * key, byte value);
	void w(char * key, byte * value, byte len);	
	void w(char * tag, char * key, byte value);
	void w(char * tag, char * key, byte * value, byte len);	
	// Float
	void w(char * key, float value);
	void w(char * key, float * value, byte len);	
	void w(char * tag, char * key, float value);
	void w(char * tag, char * key, float * value, byte len);	
	
private:
	void logTag(char * tag);
	void logKey(char * key);
};	

extern Logger Log;

#endif