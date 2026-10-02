#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <cstdlib>
#include "JitterBuffer.hpp"

JitterBuffer jitterBuffer(50);
std::atomic<bool> keepRunning(true);

void rtpReceiverThread() {
    uint32_t seq = 0;
    while (keepRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        AudioPacket pkt;
        pkt.sequenceNumber = seq++;
        pkt.timestamp = seq * 960;
        pkt.payload = {0x01, 0x02, 0x03};
        pkt.arrivalTime = std::chrono::steady_clock::now();
        
        static double fakeJitter = 0.0;
        fakeJitter += (rand() % 10 - 5);
        if (fakeJitter < 0) fakeJitter = 0;
        
        jitterBuffer.updateNetworkStats(fakeJitter);
        jitterBuffer.pushPacket(pkt);
    }
}

void audioPlaybackThread() {
    while (keepRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        AudioPacket pkt;
        if (jitterBuffer.popPacket(pkt)) {
            std::cout << "[AUDIO] Playing packet seq: " << pkt.sequenceNumber << std::endl;
        }
    }
}

int main() {
    std::cout << "--- Battlefield Audio System Initializing ---" << std::endl;
    std::thread receiver(rtpReceiverThread);
    std::thread player(audioPlaybackThread);
    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    keepRunning = false;
    receiver.join();
    player.join();
    std::cout << "--- System Stopped ---" << std::endl;
    return 0;
}