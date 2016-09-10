/*
 * KDTree.h
 *
 *  Created on: 4 בספט׳ 2016
 *      Author: Noa
 */

#ifndef KDTREE_H_
#define KDTREE_H_

# include "KDArray.h"
# include "SPPoint.h"
# include "SPConfig.h"

struct kd_tree_node;
typedef struct kd_tree_node* KDTreeNode;

KDTreeNode kdTreeInit(SPKDArray kdArr,SPLIT_METHOD spKDTreeSplitMethod,int currDim);

int getNodeDim(KDTreeNode kdNode);

double getNodeVal(KDTreeNode kdNode);

KDTreeNode getNodeLeft(KDTreeNode kdNode);

KDTreeNode getNodeRight(KDTreeNode kdNode);

SPPoint getNodeData(KDTreeNode kdNode);

#endif /* KDTREE_H_ */
