#include <SoftwareSerial.h>
#include <VirtualShield.h>

VirtualShield shield(true);

// @todo: 1. Create Callbacks
// Code here to send data periodically
void send() {
	if (!shield.isConnected()) return;
	// Do stuff such reading sensor
	long testData = millis();

	Log.w("Running task...");

	shield.addData("getValue_mq135", testData);
	shield.addData("getValue_mq2", testData/10);
	shield.sendData();
}

void setup() {  
	// @todo: 2. Start Shield 	
	shield.begin(115200);
	// @todo: 3. Set Task
	shield.setTask(send, 10000);
}

void loop() {	
	// @todo: 4. Listen to sensors
	shield.listen();
	// @todo: 5. Run Task
		shield.runTask();
}


