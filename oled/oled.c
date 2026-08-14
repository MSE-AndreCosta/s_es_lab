#include "oled.h"
#include "McuI2cLib.h"
#include "McuSSD1306.h"
#include <stdio.h>
#include <string.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static bool initialized = false;

int oled_init(void)
{
    if (initialized) {
        return 0;
    }

    McuI2cLib_Init();
    McuSSD1306_Init();

    initialized = true;
    return 0;
}


static int get_local_ip(char *out, size_t out_size)
{
    struct ifaddrs *ifaddr, *ifa;
    int found = -1;

    if (getifaddrs(&ifaddr) < 0) {
        perror("oled: getifaddrs");
        return -1;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET) {
            continue;
        }
        if (strcmp(ifa->ifa_name, "lo") == 0) {
            continue; 
        }

        struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
        if (inet_ntop(AF_INET, &addr->sin_addr, out, out_size) != NULL) {
            found = 0;

            if (strncmp(ifa->ifa_name, "wlan", 4) == 0 ||
                strncmp(ifa->ifa_name, "eth", 3) == 0) {
                break;
            }
        }
    }

    freeifaddrs(ifaddr);
    return found;
}

int oled_display_ip(void)
{
    if (!initialized) {
        fprintf(stderr, "oled: not initialized\n");
        return -1;
    }

    char ip[INET_ADDRSTRLEN] = {0};
    if (get_local_ip(ip, sizeof(ip)) != 0) {
        strncpy(ip, "No IP", sizeof(ip) - 1);
    }

    McuSSD1306_Clear();
    McuSSD1306_PrintString(0, 0, (uint8_t*)"IP Address:");
    McuSSD1306_PrintString(2, 0, (uint8_t*)ip);

    return 0;
}

int oled_deinit(void)
{
    initialized = false;
    McuSSD1306_Clear();
    return 0;
}