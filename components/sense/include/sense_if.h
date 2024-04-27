/*
 * sense_if.h
 *
 *  Created on: Jun 17, 2023
 *      Author: Enrique Rodriguez-Toscano
 */

#ifndef COMPONENTS_SENSE_INCLUDE_SENSE_IF_H_
#define COMPONENTS_SENSE_INCLUDE_SENSE_IF_H_


#if CONFIG_OutputsConfig == 1

#define DO_00	CONFIG_GPIO_OUT_TrillerDirection
#define DO_01	CONFIG_GPIO_OUT_TrillerSpeed
#define DO_02	CONFIG_GPIO_OUT_FanDirection
#define DO_03	CONFIG_GPIO_OUT_FanSpeed
#define DO_04	CONFIG_GPIO_OUT_GREEN_LED
#define DO_05	CONFIG_GPIO_OUT_RED_LED
#define DO_06	CONFIG_GPIO_OUT_BLUE_LED
#define DO_07	CONFIG_GPIO_OUT_BUZZER
#define DO_08	CONFIG_GPIO_OUTPUT_Resistor
#define DO_09	CONFIG_GPIO_OUTPUT_Sprinkler

#endif

typedef enum
{
	CHAM_TEMP = 0,
	CHAM_HUM,
	SYS_VOLT,
	MILL_VOLT,
	VENT_VOLT,
	RESISTOR_VOL,
	MAX_ADC1_SENSORS
}sensorADC1_t;

typedef enum
{
	DUMMY_SENSOR = 0,
	MAX_ADC2_SENSORS
}sensorADC2_t;

//sense_read_t  Read_Type =

float sense_getIntTempCelcius(void);

uint8_t sense_read_AI(sensorADC1_t sensor, int *out);

void sense_read_DI(void);

esp_err_t sense_init(void);
void sense_init_DIs(void);
void sense_init_DOs(void);



#endif /* COMPONENTS_SENSE_INCLUDE_SENSE_IF_H_ */
