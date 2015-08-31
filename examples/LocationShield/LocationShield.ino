#include <SoftwareSerial.h>
#include <VirtualShield.h>

VirtualShield shield(true);

// @todo: 1. Create Callbacks
// Code here to deal with location data
void onReceiveLocation(byte * buffer, byte length) {
	// Get location data, 
	// lat (latitude) and lng (longitude)
	Location location = shield.getLocation(buffer);

	Log.w("Location", "Latitude", location.lat);
	Log.w("Location", "Longitude", location.lng);
}

void setup() {  
	// @todo: 2. Start Shield 	
	shield.begin(115200);
	// @todo: 3. Add Sensor
	shield.addSensor(LOCATION, onReceiveLocation);
	shield.addSensor(ACCELEROMETER, onReceiveAccelerometer);
}

void loop() {	
	// @todo: 4. Listen to sensors
	shield.listen();
}


