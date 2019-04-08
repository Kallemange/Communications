#pragma once
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "../../../src/ISComm.h"
#include "../../../src/serialPortPlatform.h"




static void w2fIns(ins_1_t *ins, char* path) {
	
	FILE * log = fopen(path, "a");
	if (!log) {
		log = fopen(path, "w");
	}
	if (!log) {
		printf("File does not exist\n");
		return;
	}
	fprintf(log, "%.3f, %3.7f, %3.7f, %5.2f, %f, %f, %f, %5.1f, %5.1f, %5.1f\n",
		ins->timeOfWeek,
		ins->lla[0], ins->lla[1], ins->lla[2],
		ins->ned[0], ins->ned[1], ins->ned[2],
		ins->theta[0] * C_RAD2DEG_F, ins->theta[1] * C_RAD2DEG_F, ins->theta[2] * C_RAD2DEG_F);
	fclose(log);

}

