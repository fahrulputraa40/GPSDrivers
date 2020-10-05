/*
 * GPSL89_commandList.c
 *
 *  Created on: Oct 1, 2020
 *      Author: fahrul
 */
#include "GPSL89_commandList.h"


uint8_t GPSL89_CommandList(GPSDrivers *GPSDriver, GPSDrivers_CMD cmd,
		void *args) {
	switch (cmd) {
	case enambeRMCOnly: {
		GPSDriver->sendCommand(GPSDriver, "PSTMCFGMSGL,%d,%d,0x%08X,0x%08X", 1,	50, 0x40,0x10); // Enable RMC only and timesampling 5s
	}
		break;
	case PerformWarm:
		GPSDriver->sendCommand(GPSDriver, "PSTMWARM");
		break;
	case PerformCold:
		GPSDriver->sendCommand(GPSDriver, "PSTMCOLD");
		break;
	case PerformHot:
		GPSDriver->sendCommand(GPSDriver, "PSTMHOT");
		break;
	case System_Reset:
		GPSDriver->sendCommand(GPSDriver, "PSTMSRR");
		break;
	case SaveParam:
		GPSDriver->sendCommand(GPSDriver, "PSTMSAVEPAR");
		break;
	case Init_Time: {
		/*
		 * ARGS data
		 *
		 * index 0: day
		 * index 1: month
		 * index 2: year
		 * index 3: day
		 * index 4: minute
		 * index 5: second
		 */
		char buf[20];
		uint8_t *arg = (uint8_t*) args;
		sprintf(buf, "%02d,%02d,20%02d,%02d,%02d,%02d", arg[0], arg[1], arg[2],
				arg[3], arg[4], arg[5]);
		GPSDriver->sendCommand(GPSDriver, "PSTMINITTIME,%s", buf);
	}
		break;
	case Init_Loc_and_Time: {
		char buff[10];
		char buf[40];
		GPSDrivers *arg_ = (GPSDrivers*) args;
		sprintf(buf,
				"%s,%c,%s,%c,0500,%02d,%02d,%02d,%02d,%02d,%02d", //change format to double
				GPSDriver_d2s(arg_->latitude, buff, 3),
				arg_->latitudeRef,
				GPSDriver_d2s(arg_->longitude, buff, 3),
				arg_->longitudeRef,
				arg_->GPSL89_TimeStamp.date,
				arg_->GPSL89_TimeStamp.month, arg_->GPSL89_TimeStamp.year,
				arg_->GPSL89_TimeStamp.hour, arg_->GPSL89_TimeStamp.minute,
				arg_->GPSL89_TimeStamp.second);
		GPSDriver->sendCommand(GPSDriver, "PSTMINITGPS,%s", buf);
	}
		break;
	case AGPS: //Asisted GPS
		GPSDriver->sendCommand(GPSDriver,
				"PSTMCFGAGPS,%c",
				'0' + *((uint8_t*) args));
		break;
	case SetConstellation: //Asisted GPS
			GPSDriver->sendCommand(GPSDriver,
					"PSTMSETCONSTMASK,%c",
					'0' + *((uint8_t*) args));
			break;
	case Config_Date_Time:
		GPSDriver->sendCommand(GPSDriver, "PSTMCFGTDATA,1964,3443,50,00000012");
		break;

	default:
		break;
	}
	return 1;
}
