/*
MIT LICENSE

Copyright 2014-2018 Inertial Sense, Inc. - http://inertialsense.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>

// STEP 1: Add Includes
// Change these include paths to the correct paths for your project
#include "../../src/ISComm.h"
#include "../../src/serialPortPlatform.h"
#include "../../writeToFileRaw.h"
#include "../../writeToFileIns.h"
#include <time.h>


static int running = 1;

static void handleInsMessage(ins_1_t* ins, char* pathIns)
{
	w2fIns(ins, pathIns);
#if 0
	printf("INS TimeOfWeek: %.3fs, LLA: %3.7f,%3.7f,%5.2f, Euler: %5.1f,%5.1f,%5.1f\r\n", 
		ins->timeOfWeek,
		ins->lla[0], ins->lla[1], ins->lla[2], 
		ins->theta[0] * C_RAD2DEG_F, ins->theta[1] * C_RAD2DEG_F, ins->theta[2] * C_RAD2DEG_F );
#endif
}

static void handleGpsMessage(gps_pos_t* pos)
{
	printf("GPS TimeOfWeek: %dms, LLA: %3.7f,%3.7f,%5.2f\r\n", pos->timeOfWeekMs, pos->lla[0], pos->lla[1], pos->lla[2]);
	printf("\n\n handleGpsMessage \n\n");
}

static void handleImuMessage(dual_imu_t* imu)
{
	printf("IMU Time: %.3fs, PQR: %5.1f,%5.1f,%5.1f, ACC: %5.1f,%5.1f,%5.1f,\r\n", 
		imu->time, 
		imu->I[0].pqr[0], imu->I[0].pqr[1], imu->I[0].pqr[2],
		imu->I[0].acc[0], imu->I[0].acc[1], imu->I[0].acc[2]);
	printf("\n\n handleImuMessage \n\n");
}

//static void handleGPSRawMessage(gps_raw_t* raw, char* pathRaw1)
static void handleGPSRawMessage(gps_raw_t* raw, char* pathRaw1, char* pathRaw2, char* pathRaw3, char* pathRaw4, char* pathRaw5, char* pathRaw6)
{
	//printf("\n in handleGPSRawMessage\n");
	w2f(raw, pathRaw1, pathRaw2, pathRaw3, pathRaw4, pathRaw5, pathRaw6, 1); //data object, file path, print to screen (1/0)
	//w2f(raw, pathRaw1, 1); //data object, file path, print to screen (1/0)
	//printf("Obs count: %d, time: %d, dataType: %d  Pseudoranges: ", raw->obsCount, raw->data.obs->time.time, raw->dataType);
	//	printf("Eph data A: %f, sat: %d, week: %d \n", raw->data.eph.A, raw->data.eph.sat, raw->data.eph.week);
	
}

int set_configuration(serial_port_t *serialPort, is_comm_instance_t *comm)
{
	// Set INS output Euler rotation in radians to 90 degrees roll for mounting
	float rotation[3] = { 90.0f*C_DEG2RAD_F, 0.0f, 0.0f };
	int messageSize = is_comm_set_data(comm, _DID_FLASH_CONFIG, offsetof(nvm_flash_cfg_t, insRotation), sizeof(float) * 3, rotation);
	if (messageSize != serialPortWrite(serialPort, comm->buffer, messageSize))
	{
		printf("Failed to encode and write set INS rotation\r\n");
		return -3;
	}

	return 0;
}

int stop_message_broadcasting(serial_port_t *serialPort, is_comm_instance_t *comm)
{
	// Stop all broadcasts on the device
	int messageSize = is_comm_stop_broadcasts_all_ports(comm);
	if (messageSize != serialPortWrite(serialPort, comm->buffer, messageSize))
	{
		printf("Failed to encode and write stop broadcasts message\r\n");
		return -3;
	}
	return 0;
}

int save_persistent_messages(serial_port_t *serialPort, is_comm_instance_t *comm)
{
    config_t cfg;
    cfg.system = CFG_SYS_CMD_SAVE_PERSISTENT_MESSAGES;
    cfg.invSystem = ~cfg.system;

    int messageSize = is_comm_set_data(comm, DID_CONFIG, 0, sizeof(config_t), &cfg);
    if (messageSize != serialPortWrite(serialPort, comm->buffer, messageSize))
    {
        printf("Failed to write save persistent message\r\n");
        return -3;
    }
    return 0;
}

int enable_message_broadcasting_get_data(serial_port_t *serialPort, is_comm_instance_t *comm)
{
	// Ask for INS message w/ update 40ms period (4ms source period x 10).  Set data rate to zero to disable broadcast and pull a single packet.
	int messageSize;
	messageSize = is_comm_get_data(comm, _DID_INS_LLA_EULER_NED, 0, 0, 10);
	if (messageSize != serialPortWrite(serialPort, comm->buffer, messageSize))
	{
		printf("Failed to encode and write get INS message\r\n");
		return -4;
	}

#if 1
	// Ask for GPS message at period of 200ms (200ms source period x 1).  Offset and size can be left at 0 unless you want to just pull a specific field from a data set.
	messageSize = is_comm_get_data(comm, _DID_GPS1_RAW, 0, 0, 1);
	if (messageSize != serialPortWrite(serialPort, comm->buffer, messageSize))
	{
		printf("Failed to encode and write get GPS message\r\n");
		return -5;
	}
	else
	
#endif

#if 0
	// Ask for IMU message at period of 100ms (1ms source period x 100).  This could be as high as 1000 times a second (period multiple of 1)
	messageSize = is_comm_get_data(comm, _DID_IMU_DUAL, 0, 0, 100);
	if (messageSize != serialPortWrite(serialPort, comm->buffer, messageSize))
	{
		printf("Failed to encode and write get IMU message\r\n");
		return -6;
	}
#endif
	return 0;
}

void createLogs(char* path, char* type, char* timeStamp, int val) {
	char* newString[10];
	sprintf(newString, "%d", val);
	if (val == 0) {
	}
	else {
		strcat(path, newString); 
	}
	strcat(path, type);
	strcat(path, timeStamp);
	char format[5] = ".csv";
	strcat(path, format);

	FILE * log = fopen(path, "a");

	if (!log) {
		log = fopen(path, "w");
	}
	if (!log) {
		printf("File does not exist\n");
		return;
	}
	switch (val)
	{
	case 0:
		fprintf(log, "timeOfWeek, lla0,lla1,lla2,ned0,ned1,ned2,theta0,theta1,theta2\n");
		break;
	case 1:
		//Previous log format, creating a too long file forcing linebreak in log
		//fprintf(log, "obsCount, time1, time2, sat, rcv, SNR, LLI, code, qualL, qualP, reserved, L, P, D\n");
		fprintf(log, "obsCount, time1, time2, sat, SNR, LLI, code,P\n");
		break;
	case 2:
		fprintf(log, "sat, iode, iodc, sva, svh, week, code, flag,\
toe1, toe2, toc1, toc2, ttr1, ttr2, A, e, i0, OMG0, omg, \
M0, deln, OMGd, idot, crc, crs, cuc, cus, cic, cis, toes, fit, \
f0, f1, f2\n");
		break;
	case 3:
		fprintf(log, "sat, iode, frq, svh, sva, age, toe, tof\
pos0, pos1, pos2, vel0, vel1, vel2, acc0, acc1, acc2, taun, gamn, dtaun, \n");
		break;
	case 4:
		fprintf(log, "week, tow, prn, msg, reserved\n");
		break;
	case 5:
		fprintf(log, "deltype, pos0, pos1, pos2, del0, del1, del2, hgt, stationId\n");
		break;
	case 6:
		fprintf(log, "ionosphereModel");
		break;
	}
	fclose(log);

	return;
}

int main(int argc, char* argv[])
{
	//create the path for raw ans INS data
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	char timeStamp[50];
	char pathRaw1[50] = "Logs/";
	char pathRaw2[50] = "Logs/";
	char pathRaw3[50] = "Logs/";
	char pathRaw4[50] = "Logs/";
	char pathRaw5[50] = "Logs/";
	char pathRaw6[50] = "Logs/";
	char pathIns[50] = "Logs/";
	char raw[5]  = "Raw";
	char ins[5]  = "Ins";
	sprintf(timeStamp, "%d_%d_%d%d%d", (timeinfo->tm_mon + 1), timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	//create the logs for raw ans INS data
	createLogs(pathIns, ins, timeStamp, 0);
	createLogs(pathRaw1, raw, timeStamp, 1);
	createLogs(pathRaw2, raw, timeStamp, 2);
	createLogs(pathRaw3, raw, timeStamp, 3);
	createLogs(pathRaw4, raw, timeStamp, 4);
	createLogs(pathRaw5, raw, timeStamp, 5);
	createLogs(pathRaw6, raw, timeStamp, 6);
	
	if (argc < 2)
	{
		printf("Please pass the com port as the only argument\r\n");
		// In Visual Studio IDE, this can be done through "Project Properties -> Debugging -> Command Arguments: COM3" 
		return -1;
	}


	// STEP 2: Init comm instance
	is_comm_instance_t comm;
	uint8_t buffer[2048];

	// Make sure to assign a valid buffer and buffer size to the comm instance
	comm.buffer = buffer;
	comm.bufferSize = sizeof(buffer);

	// Initialize the comm instance, sets up state tracking, packet parsing, etc.
	is_comm_init(&comm);


	// STEP 3: Initialize and open serial port
	serial_port_t serialPort;

	// Initialize the serial port (Windows, MAC or Linux) - if using an embedded system like Arduino,
	//  you will need to handle the serial port creation, open and reads yourself. In this
	//  case, you do not need to include serialPort.h/.c and serialPortPlatform.h/.c in your project.
	serialPortPlatformInit(&serialPort);

	// Open serial, last parameter is a 1 which means a blocking read, you can set as 0 for non-blocking
	// you can change the baudrate to a supported baud rate (IS_BAUDRATE_*), make sure to reboot the uINS
	//  if you are changing baud rates, you only need to do this when you are changing baud rates.
	if (!serialPortOpen(&serialPort, argv[1], IS_BAUDRATE_921600, 1))
	{
		printf("Failed to open serial port on com port %s\r\n", argv[1]);
		return -2;
	}
	//printf("opened port");

	int error;

	// STEP 4: Stop any message broadcasting
	if (error = stop_message_broadcasting(&serialPort, &comm))
	{
		return error;
	}


#if 0	// STEP 5: Set configuration
	if (error = set_configuration(&serialPort, &comm))
	{
		return error; 
	}
#endif

	
    // STEP 6: Enable message broadcasting
	// Ask for INS message w/ update 40ms period (4ms source period x 10).  Set data rate to zero to disable broadcast and pull a single packet.
	if (error = enable_message_broadcasting_get_data(&serialPort, &comm))
	{
		return error;
	}
	//printf("Requesting messages");
	// Ask for INS message w/ update 40ms period (4ms source period x 10).  Set data rate to zero to disable broadcast and pull a single packet.
	int messageSize = is_comm_get_data(&comm, _DID_INS_LLA_EULER_NED, 0, 0, 10);
	//if (messageSize != serialPortWrite(&serialPort, &comm.buffer, messageSize))
	//{
	//	printf("Failed to encode and write get INS message\r\n");
	//}

#if 1
	// Ask for GPS message at period of 200ms (200ms source period x 1).  Offset and size can be left at 0 unless you want to just pull a specific field from a data set
	messageSize = is_comm_get_data(&comm, _DID_GPS1_RAW, 0, 0, 1);
	if (messageSize != serialPortWrite(&serialPort, &comm.buffer, messageSize))
	{
		printf("Failed to encode and write get GPS message\r\n");
	}
	
#endif
	
#if 0   // STEP 7: (Optional) Save currently enabled streams as persistent messages enabled after reboot
    save_persistent_messages(&serialPort, &comm);
#endif

	//printf("step 8");
	// STEP 8: Handle received data
	int count;
	uint8_t inByte;
	//printf("starting while(running)");
	// You can set running to false with some other piece of code to break out of the loop and end the program
	while (running)
	{
		//printf("in while(running)");
		// Read one byte with a 20 millisecond timeout
		while ((count = serialPortReadCharTimeout(&serialPort, &inByte, 20)) > 0)
		{
			switch (is_comm_parse(&comm, inByte))
			{
			case _DID_INS_LLA_EULER_NED:
				handleInsMessage((ins_1_t*)buffer, pathIns);
				break;

			case _DID_GPS1_POS:
				handleGpsMessage((gps_pos_t*)buffer);
				break;

			case _DID_IMU_DUAL:
				handleImuMessage((dual_imu_t*)buffer);
				break;

			case _DID_GPS1_RAW:
				handleGPSRawMessage((gps_raw_t*)buffer, pathRaw1, pathRaw2, pathRaw3, pathRaw4, pathRaw5, pathRaw6);
				break;				
				
				// TODO: add other cases for other data ids that you care about
			}
		}
	}
}

