/*
 * blufi_if.h
 *
 *  Created on: Oct 10, 2023
 *      Author: enriq
 */

#ifndef _BLUFI_IF_H_
#define _BLUFI_IF_H_

typedef struct
{   uint8_t *data; //Custom data
	uint32_t data_len;
}ts_BLEMsg;

extern void blufi_main(void);
extern bool blufi_GetBLEConStatus(void);
extern bool blufi_GetWiFiConStatus(void);
extern void example_print_all_netif_ips(const char *prefix);
extern void bluefi_ip_info(const char *prefix);

#endif /* _BLUFI_IF_H_ */
