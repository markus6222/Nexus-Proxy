//
//  playerInventory.h
//  GoGABOT
//
//  Created by Carmudi on 02/09/21.
//  Copyright © 2021 9GATE. All rights reserved.
//

#ifndef playerInventory_h
#define playerInventory_h

#include <atomic>
#include <mutex>

#endif /* playerInventory_h */

using namespace std;

class PlayerInventory {
private:
    mutex mtx;

public:
    struct Item
    {
        uint16_t id;
        uint8_t count;
        uint8_t type;
    };

    atomic<int> totalBlocksInInventory{ 0 };
    atomic<int> droppedItemCounter{ 0 };
    atomic<int> droppedItemUID{ -1 };

    // totalBlockInInventory
    int getTotalCurrentBlock();
    void updateInventoryTotalBlock(int amount);
    void setTotalInventoryBlock(int amount);

    // droppedItemCounter
    int getTotalDroppedItem();
    void setTotalDroppedItem(int amount);
    void updateTotalDroppedItem(int amount);

    // droppedItemUID
    int getDroppedItemUID();
    void increaseDroppedItemUID();
    void setDroppedItemUID(int UID);
};
