#include <SoftwareSerial.h>
#include <VirtualShield.h>

VirtualShield shield(true);

// @todo: 1. Create Callbacks
// Code here to deal with location data
void onReceive(byte * buffer, byte length) {
	// Get location data, 
	// lat (latitude) and lng (longitude)
	Accelerometer accelerometer = shield.getAccelerometer(buffer);

	Log.w("Accelerometer", "x", accelerometer.x);
	Log.w("Accelerometer", "y", accelerometer.y);
	Log.w("Accelerometer", "z", accelerometer.z);

}

void setup() {  
	// @todo: 2. Start Shield 	
	shield.begin(115200);
	// @todo: 3. Add Sensor
	shield.addSensor(ACCELEROMETER, onReceive);
}

void loop() {	
	// @todo: 4. Listen to sensors
	shield.listen();
}


