/*
 * BLE_SecServ.h
 *
 *  Created on: Oct 27, 2024
 *      Author: enriq
 */

#ifndef COMPONENTS_BLUFI_INCLUDE_BLE_SECSERV_H_
#define COMPONENTS_BLUFI_INCLUDE_BLE_SECSERV_H_

#define GATTS_TABLE_TAG "SEC_GATTS_DEMO"

#define HEART_PROFILE_NUM                         1
#define HEART_PROFILE_APP_IDX                     0
#define ESP_HEART_RATE_APP_ID                     0x55
#define HEART_RATE_SVC_INST_ID                    0
#define EXT_ADV_HANDLE                            0
#define NUM_EXT_ADV_SET                           1
#define EXT_ADV_DURATION                          0
#define EXT_ADV_MAX_EVENTS                        0

#define GATTS_DEMO_CHAR_VAL_LEN_MAX               0x40

static uint16_t profile_handle_table[HRS_IDX_NB];

static uint8_t ext_adv_raw_data[] = {
        0x02, 0x01, 0x06,
        0x02, 0x0a, 0xeb, 0x03, 0x03, 0xab, 0xcd,
        0x11, 0X09, 'E', 'S', 'P', '_', 'B', 'L', 'E', '5', '0', '_', 'S', 'E', 'R', 'V', 'E', 'R',
};

static esp_ble_gap_ext_adv_t ext_adv[1] = {
    [0] = {EXT_ADV_HANDLE, EXT_ADV_DURATION, EXT_ADV_MAX_EVENTS},
};

esp_ble_gap_ext_adv_params_t ext_adv_params_2M = {
    .type = ESP_BLE_GAP_SET_EXT_ADV_PROP_CONNECTABLE,
    .interval_min = 0x20,
    .interval_max = 0x20,
    .channel_map = ADV_CHNL_ALL,
    .filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
    .primary_phy = ESP_BLE_GAP_PHY_1M,
    .max_skip = 0,
    .secondary_phy = ESP_BLE_GAP_PHY_2M,
    .sid = 0,
    .scan_req_notif = false,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .tx_power = EXT_ADV_TX_PWR_NO_PREFERENCE,
};

struct gatts_profile_inst {
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
};

/* Service */
static const uint16_t GATTS_SERVICE_UUID_TEST      = 0x00FF;
static const uint16_t GATTS_CHAR_UUID_TEST_A       = 0xFF01;
static const uint16_t GATTS_CHAR_UUID_TEST_B       = 0xFF02;
static const uint16_t GATTS_CHAR_UUID_TEST_C       = 0xFF03;

static const uint16_t primary_service_uuid         = ESP_GATT_UUID_PRI_SERVICE;
static const uint16_t character_declaration_uuid   = ESP_GATT_UUID_CHAR_DECLARE;
static const uint16_t character_client_config_uuid = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
static const uint8_t char_prop_read                =  ESP_GATT_CHAR_PROP_BIT_READ;
static const uint8_t char_prop_write               = ESP_GATT_CHAR_PROP_BIT_WRITE;
static const uint8_t char_prop_read_write_notify   = ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_NOTIFY;
static const uint8_t heart_measurement_ccc[2]      = {0x00, 0x00};
static const uint8_t char_value[4]                 = {0x11, 0x22, 0x33, 0x44};
#define CHAR_DECLARATION_SIZE       (sizeof(uint8_t))
#define SVC_INST_ID                 0

#endif /* COMPONENTS_BLUFI_INCLUDE_BLE_SECSERV_H_ */
