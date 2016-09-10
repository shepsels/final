/*
 * query.h
 *
 *  Created on: 7 ����� 2016
 *      Author: Noa
 */

#ifndef QUERY_H_
#define QUERY_H_

struct image_counter;
typedef struct image_counter* imageCounter;

void kNearestNeighbors(KDTreeNode curr , SPBPQueue bpq, SPPoint p);
bool isLeaf(KDTreeNode curr);

imageCounter imageCounterCreate(int index, int cnt);
void imageCounterDestroy(imageCounter imgCnt);
int imgCntGetIndex(imageCounter imgCnt);
int imgCntGetCounter(imageCounter imgCnt);
void imgCntIncrementCounter(imageCounter imgCnt);
void updateImageCounter(imageCounter* imagesCntArr, SPBPQueue bpq, int bpqSize);
int imgCntComparator(const void* p, const void* q);

#endif /* QUERY_H_ */
