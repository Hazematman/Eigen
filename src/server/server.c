#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <enet/enet.h>
#include "util/array.h"
#include "util/util.h"
#include "util/timer.h"
#include "server.h"
#include "packet.h"

#define MAX_PACKET_SIZE (16*1024)

struct ServerState {
    ENetAddress address;
    ENetHost *server;
    struct Array *peerList;
    struct Array *packetList;
    bool running;
    uint64_t curPeerId;
};

struct Peer {
    uint64_t id;
    ENetPeer *peer;
};

struct Packet {
    struct Peer *peer;
    uint8_t data[MAX_PACKET_SIZE];
    size_t size;
};

static int serverInit(struct ServerState *state) {
    state->address.host = ENET_HOST_ANY;
    state->address.port = SERVER_DEFAULT_PORT;

    state->server = enet_host_create(&state->address, 4, 2, 0, 0);

    if(state->server == NULL) {
        printf("[SERVER] Failed to create server host\n");
        return -1;
    }

    state->running = true;

    state->peerList = arrayCreate(sizeof(struct Peer));
    state->curPeerId = 0;

    state->packetList = arrayCreate(sizeof(struct Packet));
    
    return 0;
}

static void serverDestroy(struct ServerState *state) {
    enet_host_destroy(state->server);
}

static void sendMapLoad(struct ServerState *state, struct Peer *peer, const char *filename) {
    struct Packet packet;
    struct PacketLoadMap mapPack;
    mapPack.type = PACKET_LOAD_MAP;
    strcpy(mapPack.filename, filename);

    packet.peer = peer;
    memcpy(packet.data, &mapPack, sizeof(struct PacketLoadMap));
    packet.size = sizeof(struct PacketLoadMap);

    arrayPush(state->packetList, &packet);
}

static void sendInit(struct ServerState *state, struct Peer *peer) {
    sendMapLoad(state, peer, "./data/maps/test.map");
}

static void handlePacket(struct ServerState *state, uint8_t *data, size_t length) {
    UNUSED(state);
    UNUSED(data);
    UNUSED(length);
}

static void doFrame(float dt) {
    UNUSED(dt);
}

int serverRun(void *ptr) {
    struct ServerState state;
    UNUSED(ptr);
    if(serverInit(&state) != 0) {
        return -1;
    }

    printf("[SERVER] Initalized and running\n");

    ENetEvent event;
    struct Timer dtTimer;
    timerReset(&dtTimer);
    while(state.running) {
        while(enet_host_service(state.server, &event, 10) > 0) {
            switch(event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("[SERVER] Peer connecting\n");
                    enet_peer_timeout(event.peer, ENET_PEER_TIMEOUT_LIMIT,
                                                    ENET_PEER_TIMEOUT_MINIMUM,
                                                    ENET_PEER_TIMEOUT_MAXIMUM);

                    struct Peer peer;
                    peer.id = state.curPeerId++; 
                    peer.peer = event.peer;
                    struct Peer *p = arrayPush(state.peerList, &peer); 

                    sendInit(&state, p);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("[SERVER] peer disconnecting\n");
                    for(size_t i=0; i < arrayLength(state.peerList); i++) {
                        struct Peer *peer = (struct Peer*)arrayGet(state.peerList, i);
                        if(event.peer == peer->peer) {
                            arrayDelete(state.peerList, i);
                            break;
                        } 
                    }
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    handlePacket(&state, event.packet->data, event.packet->dataLength);
                    enet_packet_destroy(event.packet);
                    break;
                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        }

        while(arrayLength(state.packetList) > 0) {
            size_t i = arrayLength(state.packetList) - 1;
            struct Packet *p = (struct Packet*)arrayGet(state.packetList, i);
            if(p->peer == NULL) {

            } else {
                ENetPacket *packet = enet_packet_create(p->data, p->size, ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(p->peer->peer, 0, packet);
                enet_host_flush(state.server);
            }

            arrayPop(state.packetList, NULL);
        }

        doFrame(timerGetElapsedTime(&dtTimer));

        timerReset(&dtTimer);
    }

    serverDestroy(&state);
    return 0;
}
