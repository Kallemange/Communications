#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "../../../src/ISComm.h"
#include "../../../src/serialPortPlatform.h"
		


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
	//FILE * log; 
	/*
	switch (raw->dataType) {
	case 1:
		log = fopen(path1, "a");
		break;
	case 2:
		log = fopen(path2, "a");
		break;
	case 3:
		log = fopen(path3, "a");
		break;
	case 4:
		log = fopen(path4, "a");
		break;
	case 5:
		log = fopen(path5, "a");
		break;
	case 6:
		log = fopen(path6, "a");
		break;
	}
	*/
	//printf("%s", logRaw);
	if (!log){ 
		log = fopen(path1, "w");
	}
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
	fprintf(log, "%d, %d, %" PRId64 ", %lf, ", raw->dataType, raw->obsCount, raw->data.obs->time.time, raw->data.obs->time.sec);
	for (int i = 0; i < raw->obsCount; i++) {
		fprintf(log, "%f,", raw->data.obs[i].P[0]);
	};
	
	fprintf(log, "\n");
	fclose(log);

}
