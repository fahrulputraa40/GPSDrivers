#if 0
#include "GPSDrivers.h"
uint8_t GPS_write(uint8_t *pData, uint8_t len){

}

uint8_t GPS_read(){

}

uint8_t GPS_available(){

}

GPSDrivers GPSDriver;

uint8_t test()
{
	//construct
	GPSDrivers__construct(&GPSDriver, GPSL89_CommandList, GPS_available, GPS_read, GPS_write, HAL_GetTick, NULL);

while(1){
	//Command
	while(HALSerial_available(&debug)){
			  char w[33];
			  sprintf(w, "%c", HALSerial_read(&debug));
			  HALSerial_write(&debug, w, strlen(w));
			  if(w[0] == 'g')
				  GPSDrivers_enbaleRMCOnly(&GPSDriver);
			  else if(w[0] == 'a')
				  GPSDrivers_initTime(&GPSDriver, 5, 10, 20, 7, 25, 20);
			  else if(w[0] == 'b')
			  			  GPSDrivers_saveParam(&GPSDriver);
			  else if(w[0] == 'c')
			  	GPSDrivers_reboot(&GPSDriver);
			  else if(w[0] == 'd')
				  GPSDrivers_setPerform2Hot(&GPSDriver);
			  else if(w[0]=='e')
				  GPSDrivers_initLocation_and_Time(&GPSDriver,
						  745.304,
						  'N',
						  1100.404,
						  'S',
						  9,
						  10,
						  20,
						  9,
						  30,
						  20);
			  else if(w[0] == 'f')
				  GPSDriver_setAGPS(&GPSDriver, 1);
			  else if(w[0] == 'h')
				  GPSDrivers_configDateAndTime(&GPSDriver);


		  }

		 //Run
		  GPSDrivers_loop(&GPSDriver);

		  //Print Data
		  char buff[150];
		  		char lat[20], Long[20];
		  		memset(buff, '\0', sizeof(buff));
		  		sprintf(buff, "GPS: \n"
		  				"valid: %d\n"
		  				"Lat: %s\n"
		  				"Long: %s\n"
		  				"Time: %d:%d:%d\n"
		  				"Date: %d-%d-%d\n\n",
		  				GPSDriver.isValid,
		  				GPSDriver_d2s(GPSDrivers_getLatitudeAsDecimal(&GPSDriver), lat,
		  						4),
		  				GPSDriver_d2s(GPSDrivers_getLongitudeAsDecimal(&GPSDriver),
		  						Long, 4), GPSDriver.GPSL89_TimeStamp.hour + 7,
		  				GPSDriver.GPSL89_TimeStamp.minute,
		  				GPSDriver.GPSL89_TimeStamp.second,
		  				GPSDriver.GPSL89_TimeStamp.date,GPSDriver.GPSL89_TimeStamp.month, GPSDriver.GPSL89_TimeStamp.year);
		  		HALSerial_write(&debug, (uint8_t* )buff, strlen(buff));
}
}
#endif
