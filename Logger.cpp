#include "Arduino.h"
#include "Logger.h"

void Logger::init(long baudRate) {
	Serial.begin(baudRate);
}

// Logger
void Logger::w(char * msg) {
	w(DEFAULT_TAG, msg);
}
	
void Logger::w(char * key, byte * value, byte len) {
	w(DEFAULT_TAG, key, value, len);
}

void Logger::w(char * key, byte value) {
	w(DEFAULT_TAG, key, value);
}

void Logger::w(char * tag, char * msg) {
	logTag(tag);
	Serial.println(msg);
}

void Logger::w(char * tag, char * key, byte value) {
	logTag(tag);
	logKey(key);
	Serial.println(value);
}

void Logger::w(char * tag, char * key, byte * value, byte len) {
	logTag(tag);
	logKey(key);

	Serial.print("[");

	for (byte i = 0; i < len - 1; i++) {
		Serial.print(*value);
		value++;
		Serial.print(',');
	}

	Serial.print(*value);
	Serial.println("]");
}

void Logger::logTag(char * tag) {
	Serial.print('[');
	Serial.print(tag);
	Serial.print("] ");	
}

void Logger::logKey(char * key) {
	Serial.print(key);
	Serial.print(" = ");
}

Logger Log = Logger();