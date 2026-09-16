//
//  playerInventory.cpp
//  GoGABOT
//
//  Created by Carmudi on 02/09/21.
//  Copyright © 2021 9GATE. All rights reserved.
//

#include "playerInventory.h"

int PlayerInventory::getTotalCurrentBlock() {
    lock_guard<mutex> lock(this->mtx);
    return this->totalBlocksInInventory;
}

void PlayerInventory::updateInventoryTotalBlock(int amount) {
    lock_guard<mutex> lock(this->mtx);
    this->totalBlocksInInventory += amount;
}

void PlayerInventory::setTotalInventoryBlock(int amount) {
    lock_guard<mutex> lock(this->mtx);
    this->totalBlocksInInventory = amount;
}

int PlayerInventory::getTotalDroppedItem() {
    lock_guard<mutex> lock(this->mtx);
    return this->droppedItemCounter;
}

void PlayerInventory::setTotalDroppedItem(int amount) {
    lock_guard<mutex> lock(this->mtx);
    this->droppedItemCounter = amount;
}

void PlayerInventory::updateTotalDroppedItem(int amount) {
    lock_guard<mutex> lock(this->mtx);
    this->droppedItemCounter += amount;
}

int PlayerInventory::getDroppedItemUID() {
    lock_guard<mutex> lock(this->mtx);
    return this->droppedItemUID;
}

void PlayerInventory::setDroppedItemUID(int UID) {
    lock_guard<mutex> lock(this->mtx);
    this->droppedItemUID = UID;
}

void PlayerInventory::increaseDroppedItemUID() {
    lock_guard<mutex> lock(this->mtx);
    this->droppedItemUID++;
}
