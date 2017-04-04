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

#include "globals.h"

int AoS;		//Amount of sensors parsed from config.cfg
int globalid;		//For requesting from correct sensor with MQTT
int rflag = 0;		//requestflag
float gvalue = 0;	//global value holder
int mqttcounter = 0;	//For tracking failed MQTT request-responses
int wapicecounter = 0;	//for tracking failed Wapice send-fails
int testmode = 0;	//For handling test mode console prints

//Wapice specific strings
std::string wapiceDevID = "";
std::string wapiceUser = "";
std::string wapicePassword = "";
std::string wapiceUrl = "";

//Vector arrays for configuration
std::vector< std::vector <long long int> > timerArray;
std::vector <std::vector <int> > busArray;
std::vector <std::vector <std::string> > attrArray;
std::vector <std::vector <float> > modArray;
std::vector <std::thread> threads;
