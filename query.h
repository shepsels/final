/*
 * query.h
 *
 *  Created on: 7 ����� 2016
 *      Author: Noa
 */

#ifndef QUERY_H_
#define QUERY_H_

void kNearestNeighbors(KDTreeNode curr , SPBPQueue bpq, SPPoint p);
bool isLeaf(KDTreeNode curr);

struct image_counter;
typedef struct image_counter * imageCounter;


#endif /* QUERY_H_ */

