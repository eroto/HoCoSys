
#ifndef __SYSINIT_H_
#define __SYSINIT_H_

uint8_t sysinit1(void);
uint8_t sysinit2(void);

uint8_t appinit1(void);
uint8_t appinit2(void);

char * ResetResons[11] = {
		    "UNKNOWN",    //!< Reset reason can not be determined
		    "POWERON",    //!< Reset due to power-on event
		    "EXT",        //!< Reset by external pin (not applicable for ESP32)
		    "SW",         //!< Software reset via esp_restart
		    "PANIC",      //!< Software reset due to exception/panic
		    "INT_WDT",    //!< Reset (software or hardware) due to interrupt watchdog
		    "RST_TASK_WDT",   //!< Reset due to task watchdog
		    "RST_WDT",        //!< Reset due to other watchdogs
		    "RST_DEEPSLEEP",  //!< Reset after exiting deep sleep mode
		    "RST_BROWNOUT",   //!< Brownout reset (software or hardware)
		    "RST_SDIO"		//!< Reset over SDIO
 };

#endif /* __SYSINIT_H_ */
