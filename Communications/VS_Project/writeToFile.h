#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "../../../src/ISComm.h"
#include "../../../src/serialPortPlatform.h"
		



static void w2f(gps_raw_t *raw, char* path, char* pathIns) {
	
	//sprintf
	//char path2[] = "./Logs/test5.txt";
	FILE * logRaw=fopen(path, "a");
	if (!logRaw){ 
		logRaw = fopen(path, "w");
	}
	if (!logRaw){
		printf("File does not exist\n");
		return;
	}
	printf("dataType: %d, obsCount: %d, time: %" PRId64 ", %lf \n", raw->dataType, raw->obsCount, raw->data.obs->time.time, raw->data.obs->time.sec);
	
	printf("Pseudoranges: ");
	for (int i = 0; i < raw->obsCount; i++) {
		printf("%f,", raw->data.obs[i].P[0]);
	};
	printf("\n");
	
	fprintf(logRaw, "%d, %d, %" PRId64 ", %lf, ", raw->dataType, raw->obsCount, raw->data.obs->time.time, raw->data.obs->time.sec);
	for (int i = 0; i < raw->obsCount; i++) {
		fprintf(logRaw, "%f,", raw->data.obs[i].P[0]);
	};
	fprintf(logRaw, "\n");
	fclose(logRaw);

}
