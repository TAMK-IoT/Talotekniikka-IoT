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

#include "senddata.h"


void senddatathread(int _id, float _value){

	//usleep(rand() %5000);
	if (attrArray[_id][5] == "Wapice" || attrArray[_id][5] == "wapice"){

		if (testmode == 1){
			std::cout << "Wapice send  " << _id << "value : " << _value << ", fails: " << wapicecounter << std::endl;
		}
		wapicecounter = wapicecounter + 1;
		wapiceSend(attrArray[_id][0], attrArray[_id][1], attrArray[_id][2], _value);
	}

	else if (attrArray[_id][5] == "Elisa" || attrArray[_id][5] == "elisa"){
	//Not implemented, placeholder
		if (testmode == 1){
			std::cout << "Elisa send" << std::endl;
		}
	}
	else if (attrArray[_id][5] == "Amazon" || attrArray[_id][5] == "amazon"){
	//Not implemented, placeholder
		if (testmode == 1){
			std::cout << "Amazon send" << std::endl;
		}
	}
	else{
		std::cout <<  "Server name isn't recognized, skipping senddata." << std::endl;
	}
}
