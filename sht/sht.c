#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "sht.h"

#define I2C_BUS_PATH   "/dev/i2c-1"
#define SHT31_ADDR     0x44

static int i2c_fd = -1;

int sht_init(void)
{
    if (i2c_fd >= 0) {
        return 0;
    }

    i2c_fd = open(I2C_BUS_PATH, O_RDWR);
    if (i2c_fd < 0) {
        perror("sensor: open i2c bus");
        return -1;
    }

    if (ioctl(i2c_fd, I2C_SLAVE, SHT31_ADDR) < 0) {
        perror("sensor: select i2c device");
        close(i2c_fd);
        i2c_fd = -1;
        return -1;
    }

    return 0;
}

int sht_read(sensor_data_t *out)
{
    if (i2c_fd < 0) {
        fprintf(stderr, "sensor: not initialized\n");
        return -1;
    }
    if (!out) {
        fprintf(stderr, "sensor: invalid output pointer\n");
        return -1;
    }

    char config[2] = {0x2C, 0x06}; 
    if (write(i2c_fd, config, sizeof(config)) != (ssize_t)sizeof(config)) {
        perror("sensor: write measurement command");
        return -1;
    }

    usleep(15000);

    char data[6] = {0};
    if (read(i2c_fd, data, sizeof(data)) != (ssize_t)sizeof(data)) {
        perror("sensor: read failed");
        return -1;
    }

    out->temp_c   = (((data[0] * 256) + data[1]) * 175.0) / 65535.0 - 45.0;
    out->temp_f   = (((data[0] * 256) + data[1]) * 315.0) / 65535.0 - 49.0;
    out->humidity = (((data[3] * 256) + data[4])) * 100.0 / 65535.0;

    return 0;
}

int sht_deinit(void)
{
    if (i2c_fd >= 0) {
        close(i2c_fd);
        i2c_fd = -1;
    }
    return 0;
}