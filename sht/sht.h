#ifndef SENSOR_H
#define SENSOR_H

#include <stdbool.h>
#include "protocol/protocol.h" 

int sht_init(void);
int sht_read(sensor_data_t *out);
int sht_deinit(void);

#endif /* SENSOR_H */