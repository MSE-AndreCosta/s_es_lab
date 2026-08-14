#ifndef BROADCAST_H
#define BROADCAST_H

#define BROADCAST_PAYLOAD_PERIOD_S 5
#define BROADCAST_PORT             19048

void *broadcast_sender_task(void *arg);

#endif /*BROADCAST_H*/
