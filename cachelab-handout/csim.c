#include "cachelab.h"
#include "getopt.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"
#include "stdbool.h"
#include "math.h"
#define BUFF_SIZE 60


void printTrace(char op, unsigned int adr, unsigned int size, char* info) {
    printf("%c %x,%d %s\n", op, adr, size, info);
}

int main(int argc, char* argv[])
{
    unsigned int setIndBits, E, blockBits;
    char* traceFileName;
    int vFlag = 0;
    unsigned int hitCount = 0, missCount = 0, evictionCount = 0;

    int ch;
    opterr = 0;
    char* optstring = "hvs:E:b:t:";
    while ((ch = getopt (argc, argv, optstring)) != -1) {
        switch (ch) {
            case 'v':
                vFlag = 1;
                break;
            case 's':
                setIndBits = strtol(optarg, 0, 10);
                break;
            case 'E':
                E = strtol(optarg, 0, 10);
                break;
            case 'b':
                blockBits = strtol(optarg, 0, 10);
                break;
            case 't':
                traceFileName = optarg;
                break;
            default:
                abort ();
        }
    }

    unsigned int S = (int) pow(2, setIndBits);
    // unsigned int B = (int) pow(2, blockBits);

    unsigned int setIndMask = (~(~0 << setIndBits)) << blockBits;
    unsigned int blockOffsetMask = ~(~0 << blockBits);
    unsigned int linesCount = E * S;
    bool* validBits = calloc(linesCount, sizeof(bool));
    long* cachedTag = calloc(linesCount, sizeof(long));
    int* usage = calloc(linesCount, sizeof(int));

    FILE* fp = fopen(traceFileName, "r");
    char buf[BUFF_SIZE];
    char op;
    int size;
    unsigned address;
    unsigned int setInd, tag, blockOffset;
    char* info;
    int opCount = 0;
    while (fgets(buf, BUFF_SIZE, fp) != NULL) {
        if (buf[0] == 'I') {
            continue;
        }
        char* format = " %c %x,%d";
        sscanf(buf, format, &op, &address, &size);
        setInd = address & setIndMask;
        blockOffset = address & blockOffsetMask;
        tag = address ^ (setInd | blockOffset);
        setInd >>= blockBits;
        int baseInd = setInd * E;
        bool hit = false;
        int vacant = baseInd;
        opCount += 1;
        switch (op)
        {
            case 'M': {
                for (int i = baseInd; i < baseInd + E; i++) {
                    if (validBits[i]) {
                        if (cachedTag[i] == tag) {
                            hitCount += 2;
                            hit = true;
                            info = "hit hit";
                            usage[i] = opCount;
                            break;
                        }
                        vacant = (usage[i] > usage[vacant]?vacant:i);
                    }
                    else {
                        vacant = i;
                    }
                }
                if (!hit) {
                    if (usage[vacant] != 0) {
                        info = "miss eviction hit";
                        missCount += 1;
                        evictionCount += 1;
                        hitCount += 1;
                    }
                    else {
                        info = "miss hit";
                        missCount += 1;
                        hitCount += 1;
                    }
                    validBits[vacant] = true;
                    cachedTag[vacant] = tag;
                    usage[vacant] = opCount;
                }
                break;
            }
            case 'S':
            case 'L': {
                for (int i = baseInd; i < baseInd + E; i++) {
                    if (validBits[i]) {
                        if (cachedTag[i] == tag) {
                            hitCount += 1;
                            hit = true;
                            info = "hit";
                            usage[i] = opCount;
                            break;
                        }
                        vacant = (usage[i] > usage[vacant]?vacant:i);
                    }
                    else {
                        vacant = i;
                    }
                }
                if (!hit) {
                    if (usage[vacant] != 0) {
                        info = "miss eviction";
                        missCount += 1;
                        evictionCount += 1;
                    }
                    else {
                        info = "miss";
                        missCount += 1;
                    }
                    validBits[vacant] = true;
                    usage[vacant] = opCount;
                    cachedTag[vacant] = tag;
                }
                break;
            }
        default:
            break;
        }
        if (vFlag) {
            printTrace(op, address, size, info);
        }
        
    }
    printSummary(hitCount, missCount, evictionCount);
    return 0;
}