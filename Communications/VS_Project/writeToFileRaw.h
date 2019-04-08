#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "../../../src/ISComm.h"
#include "../../../src/serialPortPlatform.h"
		
/*  Logger for the Raw data produced by the IS unit.
	Depending on the type of data (as per the value in raw.dataType),
	different messages will be stored to the log. 
	All six paths are sent as a temporary solution. 
*/

static void w2fSat(gps_sat_t *sat, char* path) {
	FILE * log = fopen(path, "a");
	if (!log) {
		log = fopen(path, "w");
	}
	if (!log) {
		printf("File does not exist\n");
		return;
	}
	fprintf(log,"%"PRIu32",%d,", sat->timeOfWeekMs, sat->numSats);
	for (int i = 0; i < sat->numSats; i++) {
		fprintf(log, "%"PRIu8",%"PRIu8",%d,%d,%d,%d,",
			sat->sat[i].gnssId, sat->sat[i].svId, sat->sat[i].elev,
			sat->sat[i].azim, sat->sat[i].prRes, sat->sat[i].cno);
	}
	fprintf(log, "\n");
	fclose(log);
}


/*Args: raw, path for log, print to terminal 1/0*/
static void w2f(gps_raw_t *raw, char* path1, char* path2, char* path3, char* path4, char* path5, char* path6, int print) {
//static void w2f(gps_raw_t *raw, char* path1, int print) {

	FILE * log=NULL;
	if (raw->dataType == 1) { log = fopen(path1, "a"); }
	
	else if (raw->dataType == 2) { log = fopen(path2, "a"); }
	
	else if (raw->dataType == 3) { log = fopen(path3, "a");	}

	else if (raw->dataType == 4) { log = fopen(path4, "a"); }

	else if (raw->dataType == 5) { log = fopen(path5, "a"); }

	else if (raw->dataType == 6) { log = fopen(path6, "a"); }
	
	if (!log){
		printf("File does not exist\n");
		return;
	}
	
	if (print) {
		switch(raw->dataType){
		case 1:
			printf("dataType: %d, obsCount: %d, time: %" PRId64 ", %lf \n", raw->dataType, raw->obsCount, raw->data.obs->time.time, raw->data.obs->time.sec);
			printf("Pseudoranges: ");
			for (int i = 0; i < raw->obsCount; i++) {
				printf("%f,", raw->data.obs[i].P[0]);
			};
			printf("\n");
			break;
		case 2:
			printf("Ephmeris data\n");
			break;
		case 3:
			printf(log, "Glonass Ephemeris data\n");
			break;
		case 4:
			printf(log, "SBAS Data\n");
			break;
		case 5:
			printf(log, "baseAntenna Data\n");
			break;
		case 6:
			printf(log, "ionosphereModel Data\n");
			break;
		}
		
		
	}
	int i = 0;
	switch (raw->dataType) {
	case 1:
		//time1, time2, sat, rcv, SNR, LLI, code, qualL, qualP, reserved, L, P, D\n");
		/*Issue identified that forced new line is introduced, removing some params temporarily
		fprintf(log, "%d,%"PRId64",%f,", raw->obsCount, raw->data.obs[i].time.time, raw->data.obs[i].time.sec);
		for (int i = 0; i < raw->obsCount; i++) {
			fprintf(log, "%"PRIu8",%"PRIu8",%d,%d,%d,%d,%d,%d,%f,%f,%f,",
				raw->data.obs[i].sat,
				raw->data.obs[i].rcv, raw->data.obs[i].SNR[0], raw->data.obs[i].LLI[0],
				raw->data.obs[i].code[0], raw->data.obs[i].qualL[0], raw->data.obs[i].qualP[0],
				raw->data.obs[i].reserved, raw->data.obs[i].L[0], raw->data.obs[i].P[0], raw->data.obs[i].D[0]);
		}
		fprintf(log, "\n");
		*/
		fprintf(log, "%d,%"PRId64",%f,", raw->obsCount, raw->data.obs[i].time.time, raw->data.obs[i].time.sec);
		for (int i = 0; i < raw->obsCount; i++) {
			fprintf(log, "%"PRIu8",%d,%d,%d,%f,",
				raw->data.obs[i].sat, raw->data.obs[i].SNR[0], raw->data.obs[i].LLI[0],	
				raw->data.obs[i].code[0], raw->data.obs[i].P[0]);
		}
		fprintf(log, "\n");
		break;
		break;
	case 2:
		//"sat, iode, iodc, sva, svh, week, code, flag,\
toe1, toe2, toc1, toc2, ttr1, ttr2, A, e, i0, OMG0, omg, \
M0, deln, OMGd, idot, crc, crs, cuc, cus, cic, cis, toes, fit, \
f0, f1, f2\n
		/*
		fprintf(log,"%"PRId32",%d,%d,%d,%d,%d,%d,%d,%d,%f,%d,%f,%d,%f,%f,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
			raw->data.eph.sat, raw->data.eph.iode, raw->data.eph.iodc,
			raw->data.eph.sva, raw->data.eph.svh, raw->data.eph.week,
			raw->data.eph.code, raw->data.eph.flag, raw->data.eph.toe,
			raw->data.eph.toc, raw->data.eph.ttr, raw->data.eph.A, 
			raw->data.eph.e, raw->data.eph.i0, raw->data.eph.OMG0,
			raw->data.eph.omg, raw->data.eph.M0, raw->data.eph.deln, 
			raw->data.eph.OMGd, raw->data.eph.idot, raw->data.eph.crc,
			raw->data.eph.crs, raw->data.eph.cuc, raw->data.eph.cus, 
			raw->data.eph.cic, raw->data.eph.cis, raw->data.eph.toes,
			raw->data.eph.fit, raw->data.eph.f0, raw->data.eph.f1, raw->data.eph.f2);
		*/
		fprintf(log, "%"PRId32",%"PRId32",%"PRId32",",  raw->data.eph.sat, raw->data.eph.iode, raw->data.eph.iodc );
		fprintf(log, "%"PRId32",%"PRId32",%"PRId32",",  raw->data.eph.sva, raw->data.eph.svh, raw->data.eph.week);
		fprintf(log, "%"PRId32",%"PRId32",%d,",		    raw->data.eph.code, raw->data.eph.flag, raw->data.eph.toe.time);
		fprintf(log, "%f,%d,%f,",	 				    raw->data.eph.toe.sec, raw->data.eph.toc.time, raw->data.eph.toc.sec);
		fprintf(log, "%d,%f,%f,",					    raw->data.eph.ttr.time, raw->data.eph.ttr.sec, raw->data.eph.A);
		fprintf(log, "%f,%f,%f,",					    raw->data.eph.e, raw->data.eph.i0, raw->data.eph.OMG0);
		fprintf(log, "%f,%f,%f,",					    raw->data.eph.omg, raw->data.eph.M0, raw->data.eph.deln);
		fprintf(log, "%f,%f,%f,",						raw->data.eph.OMGd, raw->data.eph.idot, raw->data.eph.crc);
		fprintf(log, "%f,%f,%f,",						raw->data.eph.crs, raw->data.eph.cuc, raw->data.eph.cus);
		fprintf(log, "%f,%f,%f,",						raw->data.eph.cic, raw->data.eph.cis, raw->data.eph.toes);
		fprintf(log, "%f,%f,%f,%f",						raw->data.eph.fit, raw->data.eph.f0, raw->data.eph.f1, raw->data.eph.f2);
		fprintf(log, "\n");
			
			
			
			
			
			
			
		break;
	case 3:
		printf(log, "Glonass Ephemeris data\n");
		break;
	case 4:
		printf(log, "SBAS Data\n");
		break;
	case 5:
		printf(log, "baseAntenna Data\n");
		break;
	case 6:
		printf(log, "ionosphereModel Data\n");
		break;
	}


	fclose(log);

}
