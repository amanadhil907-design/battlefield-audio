#include "JitterBuffer.hpp"
#include <algorithm>
#include <iostream>

JitterBuffer::JitterBuffer(size_t maxCapacity) 
    : maxSize(maxCapacity), targetBufferSize(3) {}

void JitterBuffer::pushPacket(const AudioPacket& packet) {
    std::lock_guard<std::mutex> lock(bufferMutex);
    if (buffer.size() >= maxSize) buffer.pop_front();
    
    auto it = std::lower_bound(buffer.begin(), buffer.end(), packet,
        [](const AudioPacket& a, const AudioPacket& b) {
            return a.sequenceNumber < b.sequenceNumber;
        });
    buffer.insert(it, packet);
}

bool JitterBuffer::popPacket(AudioPacket& outPacket) {
    std::lock_guard<std::mutex> lock(bufferMutex);
    if (buffer.empty() || buffer.size() < targetBufferSize) return false;
    outPacket = buffer.front();
    buffer.pop_front();
    return true;
}

void JitterBuffer::updateNetworkStats(double jitterMs) {
    std::lock_guard<std::mutex> lock(bufferMutex);
    size_t newTarget = 3 + static_cast<size_t>((jitterMs / 20.0) * 2);
    targetBufferSize = std::clamp(newTarget, size_t(3), size_t(15));
    std::cout << "[JB] Jitter: " << jitterMs << "ms. Target Buffer: " 
              << targetBufferSize << " packets" << std::endl;
}

size_t JitterBuffer::getCurrentSize() {
    std::lock_guard<std::mutex> lock(bufferMutex);
    return buffer.size();
}