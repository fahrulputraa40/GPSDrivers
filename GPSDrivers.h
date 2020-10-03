/*
 * GPSL89.h
 *
 *  Created on: Oct 1, 2020
 *      Author: fahrul
 */

#ifndef GPSDRIVERS_GPSDRIVERS_H_
#define GPSDRIVERS_GPSDRIVERS_H_

#include "stdint.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"

typedef enum GPSDrivers_CMD_enum{
	enambeRMCOnly,
	PerformWarm,
	PerformCold,
	PerformHot,
	System_Reset,
	SaveParam,
	Init_Time,
	Init_Loc_and_Time,
	AGPS,
	SetConstellation
}GPSDrivers_CMD;

typedef struct GPSDrivers_Def{
	double longitude; //as degree
	double latitude; //as degree
	char latitudeRef;
	char longitudeRef;
	uint8_t isValid;
	struct GPSL89_TimeStamp{
		uint8_t second;
		uint8_t minute;
		uint8_t hour;
		uint8_t date;
		uint8_t month;
		uint8_t year;
	}GPSL89_TimeStamp;
	uint8_t (*available)();
	uint8_t (*read)();
	uint8_t (*write)(uint8_t *pData, uint8_t len);
	uint8_t (*sendCommand)(struct GPSDrivers_Def *GPSDriver, const char *cmd, ...);
	uint8_t (*GPSDevice_Command)(struct GPSDrivers_Def *GPSDriver,GPSDrivers_CMD cmd, void *args);
}GPSDrivers;

void GPSDrivers__construct(GPSDrivers *GPSDriver_def,
		uint8_t (*GPSDevice_Command)(GPSDrivers *GPSDriver,GPSDrivers_CMD cmd, void *args),
		uint8_t (*availableFunction)(),
		uint8_t (*readFunction)(),
		uint8_t (*writeFunction)(uint8_t *pData, uint8_t len));

void GPSDrivers_enbaleRMCOnly(GPSDrivers *GPSDriver);

void GPSDrivers_setPerform2Warm(GPSDrivers *GPSDriver);

void GPSDrivers_setPerform2Hot(GPSDrivers *GPSDriver);

void GPSDrivers_setPerform2cold(GPSDrivers *GPSDriver);

void GPSDrivers_initTime(GPSDrivers *GPSDriver, uint8_t day, uint8_t month, uint8_t year, uint8_t hour, uint8_t minute, uint8_t second);

void GPSDrivers_initLocation_and_Time(GPSDrivers *GPSDriver, double latitude, char latRef, double longitude, char longRef, uint8_t day, uint8_t month, uint8_t year, uint8_t hour, uint8_t minute, uint8_t second);

void GPSDriver_setGNSSconstellation(GPSDrivers *GPSDriver, uint8_t format);

void GPSDriver_setAGPS(GPSDrivers *GPSDriver, uint8_t format);

double GPSDrivers_getLongitude(GPSDrivers *GPSDriver);

double GPSDrivers_getLatitude(GPSDrivers *GPSDriver);

uint8_t GPSDrivers_getDate(GPSDrivers *GPSDriver);

uint8_t GPSDrivers_getMonth(GPSDrivers *GPSDriver);

uint8_t GPSDrivers_getYear(GPSDrivers *GPSDriver);

uint8_t GPSDrivers_getSecond(GPSDrivers *GPSDriver);

uint8_t GPSDrivers_getMinute(GPSDrivers *GPSDriver);

uint8_t GPSDrivers_getHour(GPSDrivers *GPSDriver);

uint8_t GPSDrivers_sendCommand(GPSDrivers *GPSDriver, const char *data, ...);

void GPSDrivers_saveParam(GPSDrivers *GPSDriver);

void GPSDrivers_reboot(GPSDrivers *GPSDriver);

char *GPSDriver_d2s(double f, char *buf, uint8_t precision);

void GPSDriver_decodRMC(GPSDrivers *GPSDriver,char *buf);

extern char Driverbuff[90];
#endif /* GPSDRIVERS_GPSDRIVERS_H_ */
