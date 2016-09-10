/*
 * KDArray.c
 *
 *  Created on: 9 баев„ 2016
 *      Author: Noa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "SPPoint.h"
#include "KDArray.h"

struct point_index{
	SPPoint point;
	int index;
	int axis;
};

struct kd_array{
	SPPoint* originalArr;
	int** indexArr;
	int size,dim;
};

typedef struct point_index* pointIndex;
typedef struct kd_array* SPKDArray;

//todo MA faiure-free in all places +spPointCreate+createPointIndex   //+bool+documantation
//TODO O(dXnlogn)    free memory if using this func
SPKDArray init(SPPoint* arr, int size){
	SPKDArray kdArr;
	int i,j,success;
//	bool success;  todo
	SPPoint auxP;
	if (arr==NULL){
		return NULL;
	}
	/////////// memory allocation///////
	kdArr=(SPKDArray)malloc(sizeof(struct kd_array));
	if(kdArr == NULL)    //memory allocation failed
		{
			return NULL;
		}

	kdArr->dim=arr[0]->dim;//initializing the int fields
	kdArr->size=size;

	kdArr->originalArr=(SPPoint*)malloc((kdArr->size)*sizeof(SPPoint));
	if (kdArr->originalArr==NULL){   //memory allocation failed
		free(kdArr);
		return NULL;
	}
	kdArr->indexArr=(int**)malloc((kdArr->dim)*sizeof(int*));
	if(kdArr->indexArr == NULL)    //memory allocation failed
	{
		free(kdArr->originalArr);
		free(kdArr);
		return NULL;
	}
	for (i=0 ; i<kdArr->dim ; i++){
		kdArr->indexArr[i]=(int*)malloc(size*sizeof(int));
		if (kdArr->indexArr[i]==NULL){ //memory allocation failed
			for (j=0 ; j<i ; j++){
				free(kdArr->indexArr[j]);
			}
			free(kdArr->indexArr);
			free(kdArr->originalArr);
			free(kdArr);
			return NULL;
		}
	}

	/////////// initializing the remain fields/////////////
	for (i=0 ; i<size ; i++){
		auxP=spPointCopy(arr[i]);
		kdArr->originalArr[i]=auxP;
	}
	success=sortByAxis(kdArr->indexArr,arr,kdArr->size,kdArr->dim);
	if (success==false){   //memory allocation inside  sortByAxis func failed
		kdArrDestroy(kdArr);
		return NULL;
	}
	return kdArr;
}

pointIndex createPointIndex(SPPoint p, int index, int axis){  //todo MA faiure-free in all places +spPointCreate
	pointIndex pI;
	pI=(pointIndex)malloc(sizeof(struct point_index));
	if (pI==NULL){  //memory allocation failed
		return NULL;
	}
	pI->axis=axis;
	pI->index=index;
	pI->point=spPointCopy(p);
	return pI;
}

void destroyPI(pointIndex pI){
	if (pI==NULL){
		return;
	}
	spPointDestroy(pIGetPoint(pI));
	free(pI);
}

bool sortByAxis(int** res,SPPoint* arr, int size,int dim){  //TODO add to .h  change to bool
	int i,j;
	pointIndex* tmpArr;
	pointIndex pI;


	//////temporary memory allocation////////
	tmpArr=(pointIndex*)malloc(size*sizeof(pointIndex));
	if (tmpArr==NULL){   //memory allocation failed
		return false;
	}


	//initializing the tmpArr in order to sort by the points' axis
	i=0;  //rows
	for (j=0 ; j<size ; j++){ //cols
		pI=createPointIndex(arr[j],j,i);
		if (pI==NULL){     //memory allocation failed todo free all memory
			free(tmpArr);
			return false;
		}
		tmpArr[j]=pI;
	}
	while (i<dim){
		if (i!=0){
			for (j=0 ; j<size ; j++){ //changing the axis field for the qsort func
				tmpArr[j]->axis=i;
			}
		}
		qsort(tmpArr,size,sizeof(pointIndex),coorComparator);
		for (j=0 ; j<size ; j++){ //copying the indexes of the sorted array
			res[i][j]=pIGetIndex(tmpArr[j]);
		}
		i++;
	}

	////////////// memory free /////////////
	for (j=0 ; j<size ; j++){
		destroyPI(tmpArr[j]);
	}
	free(tmpArr);

	return true;

}

void kdArrDestroy(SPKDArray kdArr){
	int i;
	if (kdArr==NULL){
			return;
		}
	for (i=0 ; i<kdArr->dim ; i++){
		free(kdArr->indexArr[i]);
	}
	free(kdArr->indexArr);
	for (i=0 ; i<kdArr->size ; i++){
			spPointDestroy(kdArr->originalArr[i]);
		}
	free(kdArr->originalArr);
	free(kdArr);
}

int coorComparator(const void* p, const void* q){
	pointIndex p2,q2;
	SPPoint pP,qP;
	int pA,qA;
	p2=(*(pointIndex*)p);
	q2=(*(pointIndex*)q);
	pP=pIGetPoint(p2);
	qP=pIGetPoint(q2);
	pA=pIGetAxis(p2);
	qA=pIGetAxis(q2);
	double l = (double)spPointGetAxisCoor(pP,pA);
	double r = (double)spPointGetAxisCoor(qP,qA);
	if ((l-r)<0){
		return -1;
	}
	else if ((l-r)>0){
		return 1;
	}
	return 0;
}

SPKDArray* split(SPKDArray kdArr, int coor){ //TODO don't forget to free allocation if using this func
	int dim,size,half,i,j,k;
	SPKDArray* res;
	SPKDArray kdLeft,kdRight;
	int* x;
	SPPoint* p1;
	SPPoint* p2;
	pointIndex* pI1;
	pointIndex* pI2;
	SPPoint tmpP;
	int* map1;
	int* map2;

	dim=kdArr->dim;
	size=kdArr->size;
	half=ceil(((double)(size))/2);

	/////////// memory allocation///////
	res=(SPKDArray*)malloc(2*sizeof(SPKDArray));
	if (res==NULL){  //memory allocation failed
		return NULL;
	}
	x=(int*)malloc(size*sizeof(int));
	if (x==NULL){  //memory allocation failed
		free(res);
		return NULL;
	}
	p1=(SPPoint*)malloc(half*sizeof(SPPoint));
	if (p1==NULL){  //memory allocation failed
		free(res);
		free(x);
		return NULL;
	}
	p2=(SPPoint*)malloc((size-half)*sizeof(SPPoint));
	if (p2==NULL){  //memory allocation failed
		free(res);
		free(x);
		free(p1);
		return NULL;
	}
	pI1=(pointIndex*)malloc(half*sizeof(pointIndex));
	if (pI1==NULL){
		free(res);
		free(x);
		free(p1);
		free(p2);
		return NULL;
	}
	pI2=(pointIndex*)malloc((size-half)*sizeof(pointIndex));
		if (pI2==NULL){
		free(res);
		free(x);
		free(p1);
		free(p2);
		free(pI1);
		return NULL;
		}
	map1=(int*)malloc(size*sizeof(int));
	if (map1==NULL){  //memory allocation failed
		free(res);
		free(x);
		free(p1);
		free(p2);
		free(pI1);
		free(pI2);
		return NULL;
	}
	map2=(int*)malloc(size*sizeof(int));
	if (map2==NULL){  //memory allocation failed
		free(res);
		free(x);
		free(p1);
		free(p2);
		free(pI1);
		free(pI2);
		free(map1);
		return NULL;
	}

	// creating the map array x
	for (i=0 ; i<size ; i++){
		if (i<half){
			x[kdArr->indexArr[coor][i]]=0;
		}
		else{
			x[kdArr->indexArr[coor][i]]=1;
		}
	}
	// creating the aux arrays p1,p2 and pI1 pI2
	j=0;
	k=0;
	for (i=0 ; i<size ; i++){
		if (x[i]==0){
			tmpP=spPointCopy(kdArr->originalArr[i]);
			p1[j]=tmpP;
			pI1[j]=createPointIndex(tmpP,i,coor);
			j++;
		}
		else if(x[i]==1){
			tmpP=spPointCopy(kdArr->originalArr[i]);
			p2[k]=tmpP;
			pI2[k]=createPointIndex(tmpP,i,coor);
			k++;
		}
	}
	// creating the map arrays map1,map2
	for (i=0 ; i<size ; i++){ //initializing map1 map2
		map1[i]=-1;
		map2[i]=-1;
	}
	for (j=0 ; j<half ; j++){
		map1[pI1[j]->index]=j;
	}
	for (k=0 ; k<(size-half) ; k++){
		map2[pI2[k]->index]=k;
	}

	kdLeft=splitSort(kdArr->indexArr,map1,x,0,half,size,dim,coor);
	kdRight=splitSort(kdArr->indexArr,map2,x,1,(size-half),size,dim,coor);
	if (kdLeft==NULL||kdRight==NULL){  //memory allocation inside splitSort func failed
		free(res);
		free(x);
		for (i=0 ; i<half ; i++){
			spPointDestroy(p1[i]);
			destroyPI(pI1[i]);
		}
		for (i=0 ; i<(size-half) ; i++){
			spPointDestroy(p2[i]);
			destroyPI(pI2[i]);
		}
		free(p1);
		free(p2);
		free(pI1);
		free(pI2);
		free(map1);
		free(map2);
		kdArrDestroy(kdLeft);
		kdArrDestroy(kdRight);
		return NULL;
	}

	kdLeft->originalArr=p1;
	kdRight->originalArr=p2;
	res[0]=kdLeft;
	res[1]=kdRight;

	///////// memory free /////////
	free(x);
	for (i=0 ; i<half ; i++){
		destroyPI(pI1[i]);
	}
	for (i=0 ; i<(size-half) ; i++){
		destroyPI(pI2[i]);
	}
	free(pI1);
	free(pI2);
	free(map1);
	free(map2);

	return res;
}

SPKDArray splitSort(int** indexArr,int* map,int* x,int lOrR,int half,int size,int dim,int coor){
	SPKDArray splitedKdArr;
	int i,j,k,newI;


	/////////// memory allocation///////

	splitedKdArr=(SPKDArray)malloc(sizeof(struct kd_array));
	if (splitedKdArr==NULL){  //memory allocation failed
		return NULL;
	}
	splitedKdArr->indexArr=(int**)malloc(dim*sizeof(int*));
	if (splitedKdArr->indexArr==NULL){   //memory allocation failed
		free(splitedKdArr);
		return NULL;
	}
	for (i=0 ; i<dim ; i++){
		splitedKdArr->indexArr[i]=(int*)malloc(half*sizeof(int));
		if (splitedKdArr->indexArr[i]==NULL){   //memory allocation failed
			for(j=0 ; j<i ; j++){
				free(splitedKdArr->indexArr[j]);
			}
			free(splitedKdArr->indexArr);
			free(splitedKdArr);
			return NULL;
		}
	}

	// initializing fields
	splitedKdArr->dim=dim;
	splitedKdArr->size=half;

	// creating the new kd-array
	for (i=0 ; i<dim ; i++){
		k=0;
		for (j=0 ; j<size ; j++){
			if (x[indexArr[i][j]]==lOrR){
				newI=map[indexArr[i][j]];
				if (newI!=-1){
					splitedKdArr->indexArr[i][k]=newI;
					k++;
				}
			}
		}
	}

	return splitedKdArr;
}

//int main() {
//	printf("running2\n");
//	SPPoint a,b,c,d,e;
//	SPPoint* arr;
//	int dim,index,size,i;
//	SPKDArray kdArr;
//	SPKDArray* splited;
//
//	dim=2;
//	index=0;
////	size=5;
//	size=3;
//	double dataA[2] = { 1.0, 2.0 };
//	double dataB[2] = { 123.0, 70.0 };
//	double dataC[2] = { 2.0, 7.0 };
//	double dataD[2] = { 9.0, 11.0 };
//	double dataE[2] = { 3.0, 4.0 };
//	a = spPointCreate(dataA, dim, index);
////	b = spPointCreate(dataB, dim, index);
//	c = spPointCreate(dataC, dim, index);
////	d = spPointCreate(dataD, dim, index);
//	e = spPointCreate(dataE, dim, index);
//
////	arr=(SPPoint*)malloc(5*sizeof(struct sp_point_t));
//	arr=(SPPoint*)malloc(3*sizeof(struct sp_point_t));
////	arr[0]=a;
////	arr[1]=b;
////	arr[2]=c;
////	arr[3]=d;
////	arr[4]=e;
//	arr[0]=a;
//	arr[1]=c;
//	arr[2]=e;
//
//	kdArr=init(arr,size);
//	splited=split(kdArr,0);
//
//	kdArrDestroy(splited[0]);
//	kdArrDestroy(splited[1]);
//	free(splited);
//
//	for (i=0 ; i<size ; i++){
//		spPointDestroy(arr[i]);
//	}
//	free(arr);
//	kdArrDestroy(kdArr);
//	printf("running2\n");
//
//	return 0;
//}

SPPoint pIGetPoint(pointIndex pI){
	SPPoint res;
	res=pI->point;
	return res;
}

int pIGetIndex(pointIndex pI){
	int res;
	res=pI->index;
	return res;
}

int pIGetAxis(pointIndex pI){
	int res;
	res=pI->axis;
	return res;
}

SPPoint* getOriginalArr(SPKDArray kdArr){
	return kdArr->originalArr;
}

int** getIndexArr(SPKDArray kdArr){
	return kdArr->indexArr;
}

int getSize(SPKDArray kdArr){
	return kdArr->size;
}

int getDim(SPKDArray kdArr){
	return kdArr->dim;
}

