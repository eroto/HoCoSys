/*
 * BLE_SecServ.h
 *
 *  Created on: Oct 27, 2024
 *      Author: enrique
 */

#ifndef BLE_SEC_SERV_H
#define BLE_SEC_SERV_H

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"

/* Attributes State Machine */
enum
{
    IDX_SVC,
    IDX_CHAR_A,
    IDX_CHAR_VAL_A,
    IDX_CHAR_CFG_A,

    IDX_CHAR_B,
    IDX_CHAR_VAL_B,

    IDX_CHAR_C,
    IDX_CHAR_VAL_C,

    HRS_IDX_NB,
};


extern esp_err_t esp_bluedroid_init(void);
extern esp_err_t esp_bluedroid_enable(void);
extern esp_err_t esp_ble_gatts_register_callback(esp_gatts_cb_t callback);

#endif /* BLE_SEC_SERV_H */
