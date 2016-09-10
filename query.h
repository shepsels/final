/*
 * query.h
 *
 *  Created on: 7 בספט׳ 2016
 *      Author: Noa
 */

#ifndef QUERY_H_
#define QUERY_H_

void kNearestNeighbors(KDTreeNode curr , SPBPQueue bpq, SPPoint p);
bool isLeaf(KDTreeNode curr);

#endif /* QUERY_H_ */
