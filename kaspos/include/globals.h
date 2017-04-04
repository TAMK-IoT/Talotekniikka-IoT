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

#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <mosquitto.h>
#include <mosquittopp.h>
#include <stdio.h>
#include <cstdlib>
#include <errno.h>
#include <stdlib.h>
#include <modbus/modbus.h>
#include <sys/sysinfo.h>
#include <libconfig.h++>
#include <thread>
#include <mutex>
#include <IOT_API.h> 

#include "gettime.h"

extern int AoS;
extern int globalid;
extern int rflag;
extern int mqttcounter;
extern int wapicecounter;
extern int testmode;


extern std::string wapiceDevID;
extern std::string wapiceUser;
extern std::string wapicePassword;
extern std::string wapiceUrl;

extern std::vector< std::vector <long long int> > timerArray;
extern std::vector <std::vector <int> > busArray;
extern std::vector <std::vector <std::string> > attrArray;
extern std::vector <std::vector <float> > modArray;
