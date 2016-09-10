/*
 * KDTree.c
 *
 *  Created on: 4 בספט׳ 2016
 *      Author: Noa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "SPPoint.h"
#include "KDArray.h"
#include "KDTree.h"
#include "SPConfig.h"

typedef struct kd_tree_node* KDTreeNode;

struct kd_tree_node{
	int dim;
	double val;
	KDTreeNode left;
	KDTreeNode right;
	SPPoint data;
};


KDTreeNode kdTreeInit(SPKDArray kdArr,SPLIT_METHOD spKDTreeSplitMethod,int currDim){
	KDTreeNode kdTree;                                 // todo currDim initialization should be = 0
	SPKDArray* splited;
	int dim,size,i,min,max,spread,maxIndex,tmpMaxSprd,randDim;

	kdTree=NULL;
	kdTree=(KDTreeNode)malloc(sizeof(struct kd_tree_node));
	if (kdTree==NULL){  //memory allocation failed
		return NULL;
	}

	if (getSize(kdArr)==1){
		kdTree->dim=-1;
		kdTree->val=INFINITY;
		kdTree->left=NULL;
		kdTree->right=NULL;
		kdTree->data=spPointCopy(getOriginalArr(kdArr)[0]);
		if (kdTree->data==NULL){  //memory allocation inside spPointCopy func failed
			free(kdTree);
			return NULL;
		}
		return kdTree;
	}

	dim=getDim(kdArr);
	size=getSize(kdArr);

	// "MAX_SPREAD" split method
	if (spKDTreeSplitMethod == MAX_SPREAD){
		for (i=0 ; i<dim ; i++){
			max=spPointGetAxisCoor(getOriginalArr(kdArr)[(getIndexArr(kdArr)[i][size-1])],i);
			min=spPointGetAxisCoor(getOriginalArr(kdArr)[(getIndexArr(kdArr)[i][0])],i);
			spread=max-min;
			if (i==0){
				tmpMaxSprd=spread;
				maxIndex=i;
			}
			if (tmpMaxSprd<spread){
				tmpMaxSprd=spread;
				maxIndex=i;
			}
		}
		splited=split(kdArr,maxIndex);
		kdTree->dim=maxIndex;
		kdTree->val=spPointGetAxisCoor((getOriginalArr(kdArr)[getIndexArr(kdArr)[maxIndex][getSize(splited[0])-1]]),maxIndex);
		kdTree->left=kdTreeInit(splited[0],spKDTreeSplitMethod,currDim);
		kdTree->right=kdTreeInit(splited[1],spKDTreeSplitMethod,currDim);
		kdTree->data=NULL;
	}
	// "RANDOM" split method
	else if (spKDTreeSplitMethod == RANDOM) {
		srand(time(NULL));
		randDim=(rand()%dim);   //todo is this the best way?
		splited=split(kdArr,randDim);
		kdTree->dim=randDim;
		kdTree->val=spPointGetAxisCoor((getOriginalArr(kdArr)[getIndexArr(kdArr)[randDim][getSize(splited[0])-1]]),randDim);
		kdTree->left=kdTreeInit(splited[0],spKDTreeSplitMethod,currDim);
		kdTree->right=kdTreeInit(splited[1],spKDTreeSplitMethod,currDim);
		kdTree->data=NULL;
	}
	// "INCREMENTAL" split method
	else if (spKDTreeSplitMethod == INCREMENTAL){
		splited=split(kdArr,currDim);
		kdTree->dim=currDim;
		kdTree->val=spPointGetAxisCoor((getOriginalArr(kdArr)[getIndexArr(kdArr)[currDim][getSize(splited[0])-1]]),currDim);
		currDim=((currDim+1)%dim);
		kdTree->left=kdTreeInit(splited[0],spKDTreeSplitMethod,currDim);
		kdTree->right=kdTreeInit(splited[1],spKDTreeSplitMethod,currDim);
		kdTree->data=NULL;
	}

	return kdTree;
}

void kdTreeDestroy(KDTreeNode kdTree){
	if (!getNodeData(kdTree)){
		kdTreeDestroy(getNodeLeft(kdTree));
		kdTreeDestroy(getNodeRight(kdTree));
		free(kdTree);
		return;
	}
	spPointDestroy(getNodeData(kdTree));
	free(kdTree);
	return;
//	if (!kdTree->data){   todo this or eith getters?
//		kdTreeDestroy(kdTree->left);
//		kdTreeDestroy(kdTree->right);
//		free(kdTree);
//		return;
//	}
//	spPointDestroy(kdTree->data);
//	free(kdTree);
//	return;
}

int getNodeDim(KDTreeNode kdNode){
	return kdNode->dim;
}

double getNodeVal(KDTreeNode kdNode){
	return kdNode->val;
}

KDTreeNode getNodeLeft(KDTreeNode kdNode){
	KDTreeNode res;
	res=kdNode->left;
	return res;
}

KDTreeNode getNodeRight(KDTreeNode kdNode){
	return kdNode->right;
}

SPPoint getNodeData(KDTreeNode kdNode){
	return kdNode->data;
}

//int main(){
//	printf("runningTree\n");
//	SPPoint a,b,c,d,e;
//	SPPoint* arr;
//	int dim,index,size,i;
//	SPKDArray kdArr;
//	KDTreeNode kdTree;
////	SPKDArray* splited;
//
//	dim=2;
//	index=0;
//	size=5;
//	double dataA[2] = { 1.0, 2.0 };
//	double dataB[2] = { 123.0, 70.0 };
//	double dataC[2] = { 2.0, 7.0 };
//	double dataD[2] = { 9.0, 11.0 };
//	double dataE[2] = { 3.0, 4.0 };
//	a = spPointCreate(dataA, dim, index);
//	b = spPointCreate(dataB, dim, index);
//	c = spPointCreate(dataC, dim, index);
//	d = spPointCreate(dataD, dim, index);
//	e = spPointCreate(dataE, dim, index);
//
//	arr=(SPPoint*)malloc(5*sizeof(struct sp_point_t));
//	arr[0]=a;
//	arr[1]=b;
//	arr[2]=c;
//	arr[3]=d;
//	arr[4]=e;
//
//	kdArr=init(arr,size);
////	splited=split(kdArr,0);
//	kdTree=kdTreeInit(kdArr,"MAX_SPREAD",0);
//
//
//	for (i=0 ; i<size ; i++){
//		spPointDestroy(arr[i]);
//	}
//	free(arr);
//	kdArrDestroy(kdArr);
//	kdTreeDestroy(kdTree);
//
//	printf("runningTree\n");
//
//	return 0;
//}
