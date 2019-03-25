#pragma once
void createLogs(char* path, char* type, char* directory, int val) {

	strcpy(path, "Logs/");
	strcat(path, directory);
	strcat(path, "/");
	strcat(path, type);

	char* newString[10];
	sprintf(newString, "%d", val);
	if (val == 0||val==7) {
	}
	else {
		strcat(path, newString);
	}
	char format[5] = ".csv";
	strcat(path, format);
	//printf("%s\n", path);

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
	case 7:
		fprintf(log, "Tow, numSats, gnssID, svID, elev, azim, prRes, CNO\n");
		break;
	}
	fclose(log);

	return;
}
