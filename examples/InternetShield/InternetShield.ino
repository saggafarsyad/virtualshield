#include <SoftwareSerial.h>
#include <VirtualShield.h>

VirtualShield shield(true);

// @todo: 1. Create Callbacks
// Code here to send data periodically
void sendSensorData() {
	if (!shield.isConnected()) return;
	// Do stuff such reading sensor
	// This is an example of sensor data
	long sensor1 = millis();
	long sensor2 = sensor1/10;

	Log.w("Running task...");

	// addData (Parameter, Value)
	// Parameter must be string
	shield.addData("sensor1", sensor1);
	shield.addData("sensor2", sensor2);
	shield.sendData();
}

void setup() {  
	// @todo: 2. Start Shield 	
	shield.begin(115200);
	// @todo: 3. Set Task
	// setTask(Callback name, Period in millis)
	shield.setTask(sendSensorData, 10000);
}

void loop() {	
	// @todo: 4. Listen for incoming Messages from Android
	shield.listen();
	// @todo: 5. Run Task
	shield.runTask();
}


