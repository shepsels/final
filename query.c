/*
 * query.c
 *
 *  Created on: 7 ����� 2016
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

struct image_counter{
	int index;
	int cnt;
};

typedef struct image_counter* imageCounter;

void kNearestNeighbors(KDTreeNode curr , SPBPQueue bpq, SPPoint p){
	SPListElement newE;
	bool subTreeLeft;
	double distance,squerd;
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


imageCounter imageCounterCreate(int index, int cnt){
	imageCounter res;
	res=(imageCounter)malloc(sizeof(struct image_counter));
	if (res==NULL){  //memory allocation failed
		return NULL;
	}
	res->index=index;
	res->cnt=cnt;
	return res;
}

void imageCounterDestroy(imageCounter imgCnt){
	if (!imgCnt){
		return;
	}
	free(imgCnt);
	return;
}

int imgCntGetIndex(imageCounter imgCnt){
	return imgCnt->index;
}

int imgCntGetCounter(imageCounter imgCnt){
	return imgCnt->cnt;
}

void imgCntIncrementCounter(imageCounter imgCnt){
	imgCnt->cnt++;
	return;
}

void updateImageCounter(imageCounter* imagesCntArr, SPBPQueue bpq, int bpqSize){
	int i;
	SPBPQueue tmp;

	for (i=0 ; i<bpqSize ; i++){
		tmp=spBPQueuePeek(bpq);
		imgCntIncrementCounter(imagesCntArr[spListElementGetIndex(tmp)]);
		spBPQueueDequeue(bpq);
		spListElementDestroy(tmp);
	}

	return;
}

int imgCntComparator(const void* p, const void* q){
	imageCounter imgC1,imgC2;
	int l,r;
	imgC1=(*(imageCounter*)p);
	imgC2=(*(imageCounter*)q);
	l=imgCntGetCounter(imgC1);
	r=imgCntGetCounter(imgC2);
	if ((l-r)<0){
		return 1;
	}
	else if ((l-r)>0){
		return -1;
	}
	else{
		l=imgCntGetIndex(imgC1);
		r=imgCntGetIndex(imgC2);
		if ((l-r)<0){
			return -1;
		}
		else if ((l-r)>0){
			return 1;
		}
		return 0;
	}

}


////todo
//void searchSimilarImages(){
//	printf("Please enter an image path:\n");
//	scanf("%s", imagePath);
//
//    // memory allocation for the images counter array
//    imagesCnt = (imageCounter *)malloc(config->spNumOfImages* sizeof(imageCounter));
//
//    queryFeats = imageProc.getImageFeatures(imagePath, -1, &queryNumOfFeats);
//
//    queue = spBPQueueCreate(config->spKNN);
//
//    for(i=0; i<queryNumOfFeats; i++)
//    {
//        spBPQueueClear(queue);
//        kNearestNeighbors(tree, queue, queryFeats[i]);
//
//
//    }
//}
