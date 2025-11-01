#pragma once
#include "esp_netif.h"
#include <stdbool.h>

void portal_init(esp_netif_t *ap_netif);
bool portal_is_logged_in(const uint8_t mac[6]);
void portal_set_logged_in(const uint8_t mac[6], bool ok);
void nat_enable_temporarily(int seconds);
