/*The MIT License (MIT)

Copyright (c) 2017 Heikki Alho

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE. */

#include "statecheck.h"
#include "dataloop.h"
#include "globals.h"
#include "mqtt.h"
#include "senddata.h"
#include "wapice.h"
#include <IOT_API.h>

using namespace libconfig;

int main(int argc, char* argv[])
{

//testing mode or usemode

int opt = 0;

while ((opt = getopt(argc, argv, "t:")) != -1){
	switch (opt){
	case 't':
		testmode = atoi(optarg);
		std::cout << "Test mode enabled" << std::endl;
		break;
	default:
		break;
	}
}

//setup start

Config cfg;
try {
	cfg.readFile("config.cfg");
}

catch(const FileIOException &fioex){
	std::cerr << "I/O error while reading file." << std::endl;
	return(EXIT_FAILURE);
}

catch(const ParseException &pex){
	std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError() << std::endl;
	return(EXIT_FAILURE);
}

// Count the amount of sensors from config.cfg

const Setting& root = cfg.getRoot();

const Setting& sensors = root["sensors"];

AoS = sensors.getLength();	//AoS = Amount of sensors


//create data arrays for use from config.cfg

//timeArray -setup start
for (int i = 0; i < AoS; i++){
	std::vector <long long int> timeRow;
	for (int j = 0; j < 2; j++){
		timeRow.push_back(i * j);
	}
	timerArray.push_back(timeRow);
}
//initialize timerarray
for (int id = 0; id < AoS; id++){
	const Setting &sensor = sensors[id];
	int interval;
	sensor.lookupValue("interval", interval); 
	timerArray[id][0] = (long long) interval;
	timerArray[id][1] = (getTime() - timerArray[id][0] + (rand()%500)); // Modifying the first data read time to avoid piling up.
}
for (int i = 0; i < AoS; i++){
	for (int j = 0; j < 2; j++){
		cout << timerArray[i][j] << " ";
	}
	cout << endl; 
}
cout << endl;
//statearray setup end

//busArray setup start (mainly for modbus and i2c)
for (int i = 0; i < AoS; i++){
	std::vector <int> busRow;
	for (int j = 0; j < 4; j++){
		busRow.push_back(i * j);
	}
	busArray.push_back(busRow);
}
//initialize busArray
for (int i = 0; i < AoS; i++){
	const Setting& sensor = sensors[i];
	sensor.lookupValue("deviceNumber", busArray[i][0]);
	sensor.lookupValue("dataLocation", busArray[i][1]);
	std::string nw = "";
	sensor.lookupValue("network", nw);
	if (nw == "modbusrtu" || nw == "modbusRTU" || nw == "ModbusRTU"){
		busArray[i][2] = 0;
	}
	else if (nw == "I2C" || nw == "i2c"){
		busArray[i][2] = 1;
	}
	else if (nw == "modbustcp" || nw == "modbusTCP" || nw == "ModbusTCP"){
		busArray[i][2] = 2;
	}
	else if (nw == "mqtt" || nw == "MQTT" || nw == "Mqtt"){
		busArray[i][2] = 3;
	}

	sensor.lookupValue("messageFormat", busArray[i][3]);
}
for (int i = 0; i < AoS; i++){
	for (int j = 0; j < 4 ;j++){
		cout << busArray[i][j] << " ";
	}
	cout << endl;
}
cout << endl;
//busArray setup end

//attrArray setup start

for (int i = 0; i < AoS; i++){
	std::vector <std::string> attrRow;
	for (int j = 0; j < 7; j++){
		attrRow.push_back("i");
	}
	attrArray.push_back(attrRow);
}
//initialize attrArray

for (int id = 0; id < AoS; id++){
	const Setting &sensor = sensors[id];
	sensor.lookupValue("name", attrArray[id][0]);
	sensor.lookupValue("path", attrArray[id][1]);
	sensor.lookupValue("type", attrArray[id][2]);
	sensor.lookupValue("network", attrArray[id][3]);
	sensor.lookupValue("devicename", attrArray[id][4]);
	sensor.lookupValue("server", attrArray[id][5]);
	sensor.lookupValue("freeComment", attrArray[id][6]);
}

for (int i = 0; i < AoS; i++){
	for (int j = 0; j < 6; j++){
		cout << attrArray[i][j] << " ";
	}
	cout << endl;
}
cout << endl;

//attrArray -setup end

//modArray -setup start
//create modArray (modifier)

for (int i = 0; i < AoS; i++){
	std::vector <float> modRow;
	for (int j = 0; j < 1; j++){
		modRow.push_back(i * j);
	}
	modArray.push_back(modRow);
}

//initialize modArray

for (int id = 0; id < AoS; id++){
	const Setting &sensor = sensors[id];
	sensor.lookupValue("modifier", modArray[id][0]);
	cout << modArray[id][0] << " ";
}
cout << endl;

//modArray -setup end

cout << "Setting up Wapice-cloud " << endl;

//initialize IoT-ticket cloud service registration
wapiceDeviceSpec();

cout << "Initializing MQTT threads" << endl;

std::thread mqttsender_thread(mqttsender);

std::thread mqttlistener_thread(mqttlistener);

//Sleep for 2 second to wait for all threads.
sleep(2);

cout << "Setup done" << endl;

//main -setup end
//Clock to show program start-time
std::chrono::time_point<std::chrono::system_clock> start;
start = std::chrono::system_clock::now();
std::time_t start_time = std::chrono::system_clock::to_time_t(start);

while (true){
	stateCheck();
	if (testmode != 1){
		// Use mode. Shows number of failed data retrievals and cloud sends
		std::cout << "\t fails" << std::endl;
		std::cout << "mqtt\t" << mqttcounter << std::endl;
		std::cout << "wapice\t" << wapicecounter << std::endl;
		std::cout << "Program has been running since: " << std::ctime(&start_time) << std::endl;
		std::system("clear");
	}
}
}
