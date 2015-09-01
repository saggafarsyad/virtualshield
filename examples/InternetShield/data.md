shield.addData("data_int", 1);	
[LOG] Parse Command = 1
[LOG] Buffer = [Key Header 		40,
 				Key 			100,97,116,97,95,105,110,116,
 				Value Header 	2,
 				Value			1,0]

shield.addData("data_long", 100000);
[LOG] Parse Command = 2
[LOG] Buffer = [40,100,97,116,97,95,105,110,116,2,1,0,
				Key Header 		73,
				Key				100,97,116,97,95,108,111,110,103,
				Value Header 	4,
				Value			160,134,1,0]

shield.addData("data_float", 1.111f);			
[LOG] Parse Command = 3
[LOG] Buffer = [40,100,97,116,97,95,105,110,116,2,1,0,
				73,100,97,116,97,95,108,111,110,103,4,160,134,1,0,
				Key Header 		106,
				Key				100,97,116,97,95,102,108,111,97,116,
				Value Header 	4,
				Value 			63,53,142,63]

shield.addData("data_string", "data");
[LOG] Parse Command = 4
[LOG] Buffer = [40,100,97,116,97,95,105,110,116,2,1,0,
				73,100,97,116,97,95,108,111,110,103,4,160,134,1,0,
				106,100,97,116,97,95,102,108,111,97,116,4,63,53,142,63,
				Key Header 		139,
				Key 			100,97,116,97,95,115,116,114,105,110,103,
				Value Header 	4,
				Value	 		100,97,116,97]


shield.sendData();
[LOG] MQTT PUBLISH = [Fixed Header 		48,
					  Remaining Length	73,
					  Topic MSB/LSB		0,1,
					  Topic 			48,
					  Payload			40,100,97,116,97,95,105,110,116,2,1,0,73,100,97,116,97,95,108,111,110,103,4,160,134,1,0,106,100,97,116,97,95,102,108,
					  					111,97,116,4,63,53,142,63,139,100,97,116,97,95,115,116,114,105,110,103,14,116,104,105,115,32,105,115,32,115,116,114,105,110,103]
