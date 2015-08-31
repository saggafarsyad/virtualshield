#include <SoftwareSerial.h>
#include <VirtualShield.h>

VirtualShield shield(true);

void setup() {  
	shield.begin(115200);
	shield.addSensor(LOCATION, onReceiveLocation);
	shield.addSensor(ACCELEROMETER, onReceiveAccelerometer);
}

void loop() {	
	shield.listen();
}

void onReceiveLocation(byte * buffer, byte length) {
	Location location = shield.getLocation(buffer);

	Log.w("Location", "Latitude", location.lat);
	Log.w("Location", "Longitude", location.lng);
}

void onReceiveAccelerometer(byte * buffer, byte length) {
	Accelerometer value = shield.getAccelerometer(buffer);

	Log.w("Accelerometer", "x", value.x);
	Log.w("Accelerometer", "y", value.y);
	Log.w("Accelerometer", "z", value.z);
}