/*
 * sense_if.h
 *
 *  Created on: Jun 17, 2023
 *      Author: Enrique Rodriguez-Toscano
 */

#ifndef __SENSE_H__
#define __SENSE_H__


/*---------------------------------------------------------------
        IN and OUT configurations
---------------------------------------------------------------*/
#if CONFIG_OutputsConfig == 1
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<DO_00) | (1ULL<<DO_01) | (1ULL<<DO_02) | (1ULL<<DO_03) |\
							  (1ULL<<DO_04 | (1ULL<<DO_05) | (1ULL<<DO_06) | (1ULL<<DO_07) |\
							  (1ULL<<DO_08) | (1ULL<<DO_09)))
#endif



#if CONFIG_InputsConfig == 1

#define DI_00     CONFIG_GPIO_IN_Door
#define DI_11     CONFIG_GPIO_IN_MillerActButton

#define GPIO_INPUT_PIN_SEL  ((1ULL<<DI_00) | (1ULL<<DI_11))
#endif


void sense_init_IntTempSensor(temperature_sensor_handle_t* temp_sensor);
void sense_init_AIs(void);
void sense_init_DIs(void);
static bool example_adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle);

#endif /*__SENSE_H__*/
