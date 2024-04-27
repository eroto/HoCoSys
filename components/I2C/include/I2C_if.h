/*
 * I2C_if.h
 *
 *  Created on: Aug 9, 2023
 *      Author: Enrique Rodriguez Toscano
 */

#ifndef __I2C_IF_H__
#define __I2C_IF_H__


void I2C_init(void);

void i2c_test_task(void *arg);
esp_err_t i2c_master_init(void);

#if SOC_I2C_NUM > 1
esp_err_t i2c_slave_init(void);
#endif

#endif /*__I2C_H_*/
