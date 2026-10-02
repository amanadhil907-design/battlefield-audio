#pragma once
#include <deque>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdint>

struct AudioPacket {
    uint32_t sequenceNumber;
    uint64_t timestamp;
    std::vector<uint8_t> payload;
    std::chrono::steady_clock::time_point arrivalTime;
};

class JitterBuffer {
public:
    JitterBuffer(size_t maxCapacity = 50);
    void pushPacket(const AudioPacket& packet);
    bool popPacket(AudioPacket& outPacket);
    void updateNetworkStats(double jitterMs);
    size_t getCurrentSize();

private:
    std::deque<AudioPacket> buffer;
    std::mutex bufferMutex;
    size_t maxSize;
    size_t targetBufferSize;
};