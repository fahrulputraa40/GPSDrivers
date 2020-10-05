/*
 * GPSL89.c
 *
 *  Created on: Oct 1, 2020
 *      Author: fahrul
 */
#include "GPSDrivers.h"

char Driverbuff[90];
char DriverSerBuffer[100];

#define MAX_PRECISION_GPSDRIVER	(4)

static const float rounders_GPSDRIVER[MAX_PRECISION_GPSDRIVER + 1] =
{
	0.5,				// 0
	0.05,				// 1
	0.005,				// 2
	0.0005,				// 3
	0.00005,			// 4
};


uint8_t GPSDrivers_EXOR_checksum(char *pData, uint8_t len);

void GPSDrivers__construct(GPSDrivers *GPSDriver_def,
		uint8_t (*GPSDevice_Command)(GPSDrivers *GPSDriver,GPSDrivers_CMD cmd, void *args),
		uint8_t (*availableFunction)(),
		uint8_t (*readFunction)(),
		uint8_t (*writeFunction)(uint8_t *pData, uint8_t len),
		unsigned long (*mil)(),
		void (*fDelay)(unsigned long Delay)) {
	GPSDriver_def->write = writeFunction;
	GPSDriver_def->GPSDevice_Command = GPSDevice_Command;
	GPSDriver_def->available = availableFunction;
	GPSDriver_def->sendCommand = GPSDrivers_sendCommand;
	GPSDriver_def->read = readFunction;GPSDriver_def->millis = mil;
	GPSDriver_def->delay = fDelay;
}

void GPSDrivers_enbaleRMCOnly(GPSDrivers *GPSDriver){
	GPSDriver->GPSDevice_Command(GPSDriver, enambeRMCOnly, NULL);
}

uint8_t GPSDrivers_EXOR_checksum(char *pData, uint8_t len){
	uint8_t crc = 0;
	for(uint8_t i=0;i<len;i++){
		crc ^= (uint8_t)pData[i];
	}
	return crc;
}

uint8_t GPSDrivers_sendCommand(GPSDrivers *GPSDriver, const char *data, ...){
	char buf[60];
	memset(Driverbuff, '\0', sizeof(Driverbuff));
	va_list ap;
	va_start (ap, data);
	vsnprintf(buf, sizeof(buf), data, ap);
	va_end(ap);
	sprintf(Driverbuff, "$%s*%2X\r\n", buf, GPSDrivers_EXOR_checksum(buf, strlen(buf)));
	return GPSDriver->write((uint8_t *)Driverbuff, strlen(Driverbuff));
//	return 1;
}

double GPSDrivers_getLongitudeAsDMS(GPSDrivers *GPSDriver){
	return GPSDriver->longitude;
}

double GPSDrivers_getLatitudeAsDMS(GPSDrivers *GPSDriver){
	return GPSDriver->latitude;
}

double GPSDrivers_getLongitudeAsDecimal(GPSDrivers *GPSDriver){
	double dec = 0;
	double temp;
	dec = (int)(GPSDriver->longitude / 100);
	temp = ((int)(GPSDriver->longitude)%100);
	temp /= 60;
	dec += temp;
	temp = GPSDriver->longitude - ((int)GPSDriver->longitude);
	temp /= 60;
	dec += temp;
	return GPSDriver->longitudeRef == 'W' ? dec*-1 : dec;
}

double GPSDrivers_getLatitudeAsDecimal(GPSDrivers *GPSDriver){
	double dec = 0;
	double temp;
	dec = (int)(GPSDriver->latitude / 100);
	temp = ((int)(GPSDriver->latitude)%100);
	temp /= 60;
	dec += temp;
	temp = GPSDriver->latitude - ((int)GPSDriver->latitude);
	temp /= 60;
	dec += temp;
	return GPSDriver->latitudeRef == 'S' ? dec*-1 : dec;
}

uint8_t GPSDrivers_getDate(GPSDrivers *GPSDriver){
	return GPSDriver->GPSL89_TimeStamp.date;
}

uint8_t GPSDrivers_getMonth(GPSDrivers *GPSDriver){
	return GPSDriver->GPSL89_TimeStamp.month;
}

uint8_t GPSDrivers_dataIsValid(GPSDrivers *GPSDriver){
	return GPSDriver->isValid;
}

uint8_t GPSDrivers_getYear(GPSDrivers *GPSDriver){
	return GPSDriver->GPSL89_TimeStamp.year;
}

uint8_t GPSDrivers_getSecond(GPSDrivers *GPSDriver){
	return GPSDriver->GPSL89_TimeStamp.second;
}

uint8_t GPSDrivers_getMinute(GPSDrivers *GPSDriver){
	return GPSDriver->GPSL89_TimeStamp.minute;
}

uint8_t GPSDrivers_getHour(GPSDrivers *GPSDriver){
	return GPSDriver->GPSL89_TimeStamp.hour;
}

void GPSDrivers_setPerform2Warm(GPSDrivers *GPSDriver){
	GPSDriver->GPSDevice_Command(GPSDriver, PerformWarm, NULL);
}

void GPSDrivers_setPerform2Hot(GPSDrivers *GPSDriver){
	GPSDriver->GPSDevice_Command(GPSDriver, PerformHot, NULL);
}

void GPSDrivers_setPerform2cold(GPSDrivers *GPSDriver){
	GPSDriver->GPSDevice_Command(GPSDriver, PerformCold, NULL);
}

void GPSDrivers_initTime(GPSDrivers *GPSDriver,  uint8_t day, uint8_t month, uint8_t year, uint8_t hour, uint8_t minute, uint8_t second){
	uint8_t buf[6];
	buf[0] = day;
	buf[1] = month;
	buf[2] = year;
	buf[3] = hour;
	buf[4] = minute;
	buf[5] = second;
	GPSDriver->GPSDevice_Command(GPSDriver, Init_Time,(char *)buf);
}

void GPSDrivers_initLocation_and_Time(GPSDrivers *GPSDriver, double latitude, char latRef, double longitude, char longRef, uint8_t day, uint8_t month, uint8_t year, uint8_t hour, uint8_t minute, uint8_t second){
	GPSDrivers sCommand;
	sCommand.latitude = latitude;
	sCommand.longitude = longitude;
	sCommand.GPSL89_TimeStamp.date = day;
	sCommand.GPSL89_TimeStamp.hour = hour;
	sCommand.GPSL89_TimeStamp.minute = minute;
	sCommand.GPSL89_TimeStamp.month = month;
	sCommand.GPSL89_TimeStamp.year = year;
	sCommand.GPSL89_TimeStamp.second = second;
	sCommand.latitudeRef = latRef;
	sCommand.longitudeRef = longRef;
	GPSDriver->GPSDevice_Command(GPSDriver, Init_Loc_and_Time, &sCommand);
}

void GPSDriver_setAGPS(GPSDrivers *GPSDriver, uint8_t format){
	GPSDriver->GPSDevice_Command(GPSDriver, AGPS, &format);
}

void GPSDriver_setGNSSconstellation(GPSDrivers *GPSDriver, uint8_t format)
{
	GPSDriver->GPSDevice_Command(GPSDriver, SetConstellation, &format);
}
void GPSDrivers_saveParam(GPSDrivers *GPSDriver){
	GPSDriver->GPSDevice_Command(GPSDriver, SaveParam, NULL);
}

void GPSDrivers_reboot(GPSDrivers *GPSDriver){
	GPSDriver->GPSDevice_Command(GPSDriver, System_Reset, NULL);
}

char *GPSDriver_d2s(double f, char *buf, uint8_t precision){
	char * ptr = buf;
	char * p = ptr;
	char * p1;
	char c;
	long intPart;
	if (precision > MAX_PRECISION_GPSDRIVER)		// check precision bounds
		precision = MAX_PRECISION_GPSDRIVER;
	if (f < 0) {		// sign stuff
		f = -f;
		*ptr++ = '-';
	}
	if (precision < 0) {  // negative precision == automatic precision guess
		if (f < 1.0) precision = 4;
		else if (f < 10.0) precision = 3;
		else if (f < 100.0) precision = 2;
		else if (f < 1000.0) precision = 1;
		else precision = 0;
	}
	if (precision)  	// round value according the precision
		f += rounders_GPSDRIVER[precision];
	// integer part...
	intPart = f;
	f -= intPart;
	if (!intPart)
		*ptr++ = '0';
	else
	{
		p = ptr;	// save start pointer
		while (intPart) { // convert (reverse order)
			*p++ = '0' + intPart % 10;
			intPart /= 10;
		}
		p1 = p;  // save end pos
		while (p > ptr)	{ // reverse result
			c = *--p;
			*p = *ptr;
			*ptr++ = c;
		}
		ptr = p1;	// restore end pos
	}
	if (precision) {	// decimal part
		*ptr++ = '.';	// place decimal point
		while (precision--)	 { // convert
			f *= 10.0;
			c = f;
			*ptr++ = '0' + c;
			f -= c;
		}
	}
	*ptr = 0;	// terminating zero
	return buf;
}

char *GPSDriver_parseTimeStamp(char *src, char *dest, uint8_t index){
	src+=index;

	if(strlen(src) < 2){
		dest[0] = '0';
		dest[1]  ='0';
		return dest;
	}
	for(uint8_t c = 0;c< 2;c++){
		dest[c] = src[c];
	}
	return dest;
}

void GPSDriver_decodRMC(GPSDrivers *GPSDriver,char *buf){
	const char *GPRMC = "RMC,";
	char *pointer;
	char *str;
	if(strstr(buf, GPRMC) == NULL){
		return;
	}
	pointer = strstr(buf, GPRMC)+strlen(GPRMC);
	str = strtok_r(pointer, ",", &pointer);
	for(uint8_t i = 0; i<10;i++){
		if(i == 0 && str != NULL && strlen(str) > 0){//timestamp
			char temp[2];
			GPSDriver->GPSL89_TimeStamp.hour = atoi(GPSDriver_parseTimeStamp(str, temp, 0));
			GPSDriver->GPSL89_TimeStamp.minute = atoi(GPSDriver_parseTimeStamp(str, temp, 2));
			GPSDriver->GPSL89_TimeStamp.second = atoi(GPSDriver_parseTimeStamp(str, temp, 4));
		}
		else if(i==1 && str != NULL && strlen(str) > 0){
			GPSDriver->isValid = str[0] == 'A' ? 1 : 0;
		}
		else if(i==2 && str != NULL && strlen(str) > 0){
			GPSDriver->latitude = atof(str);
		}
		else if(i==3 && str != NULL && strlen(str) > 0){
			GPSDriver->latitudeRef = str[0] == 'S' ? 'S' : 'N';
		}
		else if(i==4 && str != NULL && strlen(str) > 0){
			GPSDriver->longitude = atof(str);
		}
		else if(i==5 && str != NULL && strlen(str) > 0){
			GPSDriver->longitudeRef = str[0] == 'E' ? 'E' : 'W';
		}
		else if(i==8 && str != NULL && strlen(str) > 0){
			char temp[2];
			GPSDriver->GPSL89_TimeStamp.date = atoi(GPSDriver_parseTimeStamp(str, temp, 0));
			GPSDriver->GPSL89_TimeStamp.month = atoi(GPSDriver_parseTimeStamp(str, temp, 2));
			GPSDriver->GPSL89_TimeStamp.year = atoi(GPSDriver_parseTimeStamp(str, temp, 4));
		}
		str = strtok_r(pointer, ",", &pointer);
	}
}

void GPSDrivers_configDateAndTime(GPSDrivers *GPSDriver){
	GPSDriver->GPSDevice_Command(GPSDriver, Config_Date_Time, NULL);
}

void GPSDrivers_loop(GPSDrivers *GPSDriver){
	if(GPSDriver->available == NULL || GPSDriver->read == NULL)
		return;
	memset(DriverSerBuffer, '\0', sizeof(DriverSerBuffer)-1);
	uint8_t pos = 0;
	unsigned long Millis = GPSDriver->millis();
	while(GPSDriver->millis() - Millis < 1000){
		if(GPSDriver->available() > 0)
			{
				DriverSerBuffer[pos] = (char)GPSDriver->read();
				if(DriverSerBuffer[pos-1] == '\r' && DriverSerBuffer[pos] == '\n')
					break;
				pos++;
			}
			if(GPSDriver->delay != NULL)
				GPSDriver->delay(100);
	}
	GPSDriver_decodRMC(GPSDriver, DriverSerBuffer);
}

//if(GPSDriver->available() > 0)
//		{
//			DriverSerBuffer[pos] = (char)GPSDriver->read();
//			pos++;
//			if(DriverSerBuffer[pos] == '\r' || DriverSerBuffer[pos] == '\n')
//				break;
//		}
//		if(GPSDriver->delay != NULL)
//			GPSDriver->delay(100);
