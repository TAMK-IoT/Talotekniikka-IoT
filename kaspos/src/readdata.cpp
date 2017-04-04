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

#include "readdata.h"


float readData(int id){

	float value = 0;

	switch (busArray[id][2]) {
	//Modbus RTU read data. Not used, checksum problems present
	case 0:{
		int slaveAddr = busArray[id][0];
		int holdingReg = busArray[id][1];
		std::string deviceName = attrArray[id][4];
		uint16_t tab_reg[64];

		//Create RTU context with parameters,
		// device name, baud rate, parity bit, data bits, stop bits)
		const char *mycharp = deviceName.c_str();

		modbus_t *ctx = modbus_new_rtu(mycharp, 9600, 'N', 8, 1);
		if (!ctx) {
    			fprintf(stderr, "Failed to create the context: %s\n", modbus_strerror(errno));
    			break;
		}
		if (modbus_connect(ctx) == -1) {
 			fprintf(stderr, "Unable to connect: %s\n", modbus_strerror(errno));
			modbus_free(ctx);
    			break;
		}
		modbus_flush(ctx);
		modbus_set_debug(ctx, TRUE);
		modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);

		modbus_set_slave(ctx, slaveAddr);
		int num = modbus_read_registers(ctx, holdingReg, 2, tab_reg);

		if (num == -1){
			fprintf(stderr, "Failed to read: %s\n", modbus_strerror(errno));
		}

		for (int i=0; i < num; i++) {
   			printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
		}

		modbus_close(ctx);
		modbus_free(ctx);
		}
		break;
	//Placefolder case for readdata methods
	case 1:	{

		}

		break;
	//Modbus TCP for readdata. Working, not used.
	case 2:	{
		int slaveAddr = busArray[id][0];
                int holdingReg = busArray[id][1];
                std::string deviceName = attrArray[id][4];
                uint16_t tab_reg[64];

		modbus_t *ctx = modbus_new_tcp("192.168.112.56", 502);
		modbus_set_debug(ctx, TRUE);
		if (ctx == NULL) {
			fprintf(stderr, "Unable to allocate libmodbus context\n");
			return -1;
		}

		if (modbus_connect(ctx) == -1) {
			fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
			modbus_free(ctx);
			return -1;
		}

		modbus_set_slave(ctx, slaveAddr);
                int num = modbus_read_registers(ctx, holdingReg, 1, tab_reg);

                if (num == -1){
                        fprintf(stderr, "Failed to read: %s\n", modbus_strerror(errno));
                }

                for (int i=0; i < num; i++) {
                        printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
                }
		modbus_free(ctx);
		}
		break;
	//MQTT readdata, working and used. Handled with request flag rflag in threads (see mqtt.cpp)
	case 3:{
		rflag = 1;
		usleep(10); // To let mqtt send thread handle everything before next potential mqtt request
		}
		break;

	default:{
		//Do nothing
		}
		break;
	}
	//MQTT Does not return any value to be used, senddata handled in on_message callback
	return value;
}
