/*
 * query.c
 *
 *  Created on: 7 בספט׳ 2016
 *      Author: Noa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "SPPoint.h"
#include "KDArray.h"
#include "KDTree.h"
#include "SPBPriorityQueue.h"
#include "query.h"

// todo init:Maintain a BPQ of the candidate nearest neighbors, called 'bpq'
// todo Set the maximum size of 'bpq' to spKNN    (in main?)

void kNearestNeighbors(KDTreeNode curr , SPBPQueue bpq, SPPoint p){
	SPListElement newE;
	bool subTreeLeft;
	double distance,squerd; //todo change name
	if (curr==NULL){
		return;
	}

	if (isLeaf(curr)==1){  //curr is a leaf
		newE=spListElementCreate(spPointGetIndex(getNodeData(curr)),spPointL2SquaredDistance(getNodeData(curr),p));
		spBPQueueEnqueue(bpq,newE);
		return;
	}

	if ((spPointGetAxisCoor(p,getNodeDim(curr))<=(getNodeVal(curr)))){
		kNearestNeighbors(getNodeLeft(curr),bpq,p);
		subTreeLeft=true;
	}
	else{
		kNearestNeighbors(getNodeRight(curr),bpq,p);
		subTreeLeft=false;
	}

	distance=(getNodeVal(curr)-(spPointGetAxisCoor(p,getNodeDim(curr))));
	squerd=distance*distance;
	if ((!spBPQueueIsFull(bpq))||(squerd<spBPQueueMinValue(bpq))){
		//recursively search the other subtree on the next axis todo delete
		if (subTreeLeft){
			kNearestNeighbors(getNodeRight(curr),bpq,p);
		}
		else{
			kNearestNeighbors(getNodeLeft(curr),bpq,p);
		}
	}

}

bool isLeaf(KDTreeNode curr){
	if ((getNodeDim(curr)==-1) && (getNodeVal(curr)==INFINITY) && (getNodeData(curr)!=NULL)
			 && (getNodeLeft(curr)==NULL) && (getNodeRight(curr)==NULL)){
		return true;
	}
	return false;
}
