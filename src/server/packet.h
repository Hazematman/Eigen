#ifndef PACKET_H
#define PACKET_H

typedef enum {
    PACKET_LOAD_MAP,
    PACKET_LOAD_PLAYER,
} PacketType;

struct PacketLoadMap {
    PacketType type;
    char filename[1024];
};

#endif
