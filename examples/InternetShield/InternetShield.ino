#include <SoftwareSerial.h>
#include <VirtualShield.h>

VirtualShield shield(true);

// @todo: 1. Create Callbacks
// Code here to send data periodically
void send() {
	// Do stuff such reading sensor
	Serial.println("Sending data...");
}

void setup() {  
	// @todo: 2. Start Shield 	
	shield.begin(115200);
	// @todo: 3. Set Task
	shield.setTask(send, 1000);
}

void loop() {	
	// @todo: 4. Listen to sensors
	shield.listen();
	// @todo: 5. Run Task
	shield.runTask();
}


