//
// Created by Haichen Dong on 2019-05-10.
//

#ifndef DEMO1_IOMANAGER_HPP
#define DEMO1_IOMANAGER_HPP

#include <stdio.h>
#include <string.h>
#include "map.hpp"
#include "utility.hpp"

// 8K per page (There should be NO elements with sizes exceeding 8K)
const size_t pageSize = 8192;
// Bufferpool size: 8K * 128 = 1M
const size_t bufferSize = 128;
const size_t hotListSize = 100;
const size_t coldListSize = bufferSize - hotListSize;

namespace sjtu {
    class IOManager {
    private:
        FILE *file;
        char pool[bufferSize][pageSize];
        size_t poolAge;
        size_t fileSize;
        size_t totalQuery, hitQuery;

        struct Node {
            size_t index; // Page id (1,2,3...) in hard drive file
            size_t value; // Pool of this page
            size_t age;
            short isEdited;
        };

        map<size_t, Node*> ageMap, idxMap;

    public:
        IOManager(FILE *_f) {
            file = _f;
            initBuff();
        }
        IOManager(char *filename) {
            FILE *tmp = fopen(filename, "a");
            fclose(tmp);
            file = fopen(filename, "r+");
            initBuff();
        }
        void initBuff() {
            totalQuery = hitQuery = 0;
            fseek(file, 0, SEEK_END);
            fileSize = ftell(file);
            printf("INIT FILESIZE %d\n", fileSize);
            memset(pool, 0, sizeof(pool));
            for (int i = 0; i < bufferSize; i++) {
                Node *node = new Node();
                node->index = - i - 1;
                node->value = i;
                node->age = ++poolAge;
                node->isEdited = -1;
                ageMap.insert({node->age, node});
                idxMap.insert({node->index, node});
            }
        }

        ~IOManager() {
            for (auto it = idxMap.begin(); it != idxMap.end(); it++) {
                if (it->second->isEdited == 1) {
                    flushToDisk(it->second);
                }
                delete it->second;
            }
            printf("TOTAL %u HIT %u RATIO %lf\n", totalQuery, hitQuery, 1.0 * hitQuery / totalQuery);
        }

        void flushToDisk(Node* node) {
//            printf("         FLUSH %d    %d\n", node->index, node->index * pageSize);
//            for (int i = 0; i < pageSize / 4; i++) printf("%d ", ((int*)pool[node->value])[i]);
//            printf("\n");
            fseek(file, node->index * pageSize, SEEK_SET);
            fwrite(pool[node->value], pageSize, 1, file);
        }
        void readFromDisk(Node* node) {
//            printf("         READ  %d    %d\n", node->index, node->index * pageSize);
            fseek(file, node->index * pageSize, SEEK_SET);
            fread(pool[node->value], pageSize, 1, file);
        }

        Node* getLastPage() {
            auto it = ageMap.begin();
            if (it->second->isEdited == 1) {
                flushToDisk(it->second);
            }
            return it->second;
        }

        Node* getAvailableMemory(int forIndex) {
            Node* node = getLastPage();
            ageMap.erase(ageMap.find(node->age));
            idxMap.erase(idxMap.find(node->index));
            node->age = ++poolAge;
            node->index = forIndex;
            ageMap.insert({node->age, node});
            idxMap.insert({node->index, node});
            readFromDisk(node);
            return node;
        }

        Node* getBufferId(size_t index) {
            totalQuery++;
            auto it = idxMap.find(index);
            if (it != idxMap.end()) {
                hitQuery++;
                ageMap.erase(ageMap.find(it->second->age));
                it->second->age = ++poolAge;
                ageMap.insert({it->second->age, it->second});
                return it->second;
            } else {
                Node* node = getAvailableMemory(index);
                return node;
            }
        }

        void getElement(char *t, size_t offset, size_t elementSize) {
            size_t beginIndex = offset / pageSize;
            size_t pagePosition = offset % pageSize;
            size_t pageLeft = pageSize - pagePosition;
//            printf("GET %d %d %d\n", beginIndex, pagePosition, pageLeft);
            if (pageLeft >= elementSize) {
                Node *poolid = getBufferId(beginIndex);
                memcpy(t, pool[poolid->value] + pagePosition, elementSize);
            } else {
                Node *poolid1 = getBufferId(beginIndex);
                Node *poolid2 = getBufferId(beginIndex + 1);
                memcpy(t, pool[poolid1->value] + pagePosition, pageLeft);
                memcpy(t + pageLeft, pool[poolid2->value], elementSize - pageLeft);
            }
        }

        size_t createElement(size_t elementSize) {
            fileSize += elementSize;
            return fileSize - elementSize;
        }

        void setElement(char *t, size_t offset, size_t elementSize) {
            size_t beginIndex = offset / pageSize;
            size_t pagePosition = offset % pageSize;
            size_t pageLeft = pageSize - pagePosition;
//            printf("SET %d %d %d\n", beginIndex, pagePosition, pageLeft);
            if (pageLeft >= elementSize) {
                Node *poolid = getBufferId(beginIndex);
                memcpy(pool[poolid->value] + pagePosition, t, elementSize);
                poolid->isEdited = true;
            } else {
                Node *poolid1 = getBufferId(beginIndex);
                Node *poolid2 = getBufferId(beginIndex + 1);
                memcpy(pool[poolid1->value] + pagePosition, t, pageLeft);
                memcpy(pool[poolid2->value], t + pageLeft, elementSize - pageLeft);
                poolid1->isEdited = true;
                poolid2->isEdited = true;
            }
        }

    };
}

#endif //DEMO1_IOMANAGER_HPP