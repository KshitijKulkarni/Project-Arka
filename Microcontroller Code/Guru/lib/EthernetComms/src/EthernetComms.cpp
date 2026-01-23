#include <Arduino.h>
#include <QNEthernet.h>
#include <errno.h>

#include "DataStructs.h"
#include "EthernetComms.h"

EthComms::EthComms(Gyan* buffer, const u_int8_t mac[6], IPAddress ipLocal, IPAddress ipDest, uint16_t port)
    : _buffer(buffer), _localIP(ipLocal), _destIP(ipDest), _destPort(port) {
        qn::Ethernet.begin(mac, _localIP, IPAddress(255, 255, 255, 0));
        _packet.seqCount = 0;
}

int EthComms::appendPacket() {
    
    /*
        Coppies over all the data in the buffer and adds it to the packet, and increments the current index.
        Returns remaining space in packet, -1 if full.
    */
    
    if (_currentIndex >= BATCH_SIZE) {
        return -1;
    }
    memcpy(&_packet.data[_currentIndex], _buffer, sizeof(Gyan));
    _currentIndex++;
    return BATCH_SIZE - _currentIndex;
}

int EthComms::sendPacket() {

    /*
        Sends the current packet over UDP.
        increments the sequence count and resets the current index.
        Returns 0 if success, errno if failure.
    */

    bool result = _udp.send(_destIP, _destPort, (uint8_t*)&_packet, sizeof(uint8_t) + sizeof(Gyan) * _currentIndex);
    if (!result) {
        return errno;
    }
    else {
        _packet.seqCount++;
        _currentIndex = 0;
        return 0;
    }
}