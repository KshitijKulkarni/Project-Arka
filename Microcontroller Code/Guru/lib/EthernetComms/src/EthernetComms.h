#ifndef ETHERNETCOMMS_H
#define ETHERNETCOMMS_H

#include <Arduino.h>
#include <QNEthernet.h>
#include <errno.h>
#include "DataStructs.h"

#define BATCH_SIZE 16

namespace qn = qindesign::network;

struct Packet {
    uint8_t seqCount;
    Gyan data[BATCH_SIZE];
    // Total _ bytes
};

class EthComms {

    /*
        Ethernet Communications class for sending Gyan data over UDP using QNEthernet library.
        Uses a buffer of Gyan structs to form packets of BATCH_SIZE before sending.

        The Gyan buffer is modified externally and passed as a pointer during construction.
        Methods:
            - appendPacket(): Copies data from the Gyan buffer into the current packet.
                             Returns remaining space in packet, -1 if full.
            - sendPacket(): Sends the current packet over UDP.
                            Increments sequence count and resets current index.
                            Returns 0 if success, errno if failure.
    */

    public:
        EthComms(Gyan* buffer, const u_int8_t mac[6], IPAddress ipLocal, IPAddress ipDest, uint16_t port);

        int appendPacket(); // returns remaining space in packet, -1 if full
        int sendPacket();   // returns 0 if success, errno if failure

    private:
        IPAddress _localIP;
        IPAddress _destIP;
        uint16_t _destPort;
        qn::EthernetUDP _udp;
        Gyan* _buffer;
        Packet _packet;
        uint8_t _currentIndex = 0;
};

#endif