#pragma once

#include <queue>
#include <vector>

#include "types.h"

class IdManager {
public:
    // Constructor initializes the next available ID
    IdManager() : nextId_(1) {}

    // Function to create or recycle an ID
    u32 createId() {
        if (!availableIds_.empty()) {
            u32 id = availableIds_.top();
            availableIds_.pop();
            return id;
        }
        return nextId_++;
    }

    // Function to release an ID, making it available for reuse
    void releaseId(u32 id) {
        availableIds_.push(id);
    }

private:
    u32 nextId_;  // Next ID to be used if no recycled ID is available
    std::priority_queue<u32, std::vector<u32>, std::greater<u32>> availableIds_; // Min-heap for recycled IDs
};
