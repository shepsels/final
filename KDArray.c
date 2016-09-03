/*
 * KDArray.c
 *
 *  Created on: 9 ����� 2016
 *      Author: Noa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

//TODO O(dXnlogn)    free memory if using this func
SPKDArray init(SPPoint* arr, int size){
	SPKDArray kdArr;
	int i,j;
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
			//destroy(kdArr);  //j=0 j<i TODO
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
//	memcpy(kdArr->originalArr,arr,size*sizeof(SPPoint));
//	printf("the first point: %f, %f\n", kdArr->originalArr[1]->data[0],kdArr->originalArr[1]->data[1]);
	for (i=0 ; i<size ; i++){
		auxP=spPointCopy(arr[i]);
		kdArr->originalArr[i]=auxP;
	}
	sortByAxis(kdArr->indexArr,arr,kdArr->size,kdArr->dim);
	printf("the first point: %f, %f\n", kdArr->originalArr[1]->data[0],kdArr->originalArr[1]->data[1]);

	//free(auxP);
	return kdArr;
}

pointIndex createPointIndex(SPPoint p, int index, int axis){
	pointIndex pI;
	pI=(pointIndex)malloc(sizeof(struct point_index));
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

void sortByAxis(int** res,SPPoint* arr, int size,int dim){  //TODO add to .h
	int i,j;
	pointIndex* tmpArr;
	pointIndex pI;


	//////temporary memory allocation////////
	tmpArr=(pointIndex*)malloc(size*sizeof(pointIndex));
	if (tmpArr==NULL){   //memory allocation failed
		return;
	}


	//initializing the tmpArr in order to sort by the points' axis
	i=0;  //rows
	for (j=0 ; j<size ; j++){ //cols
		pI=createPointIndex(arr[j],j,i);
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

	//////////////memory free/////////////
	for (j=0 ; j<size ; j++){
		destroyPI(tmpArr[j]);
	}
	free(tmpArr);

}

//void destroyArr(int** arr){
//	int rows,i;
//	if (arr==NULL){
//		return;
//	}
//	rows=(sizeof(arr))/(sizeof(arr[0]));
//	for (i=0 ; i<rows ; i++){
//		free(arr[i]);
//	}
//	free(arr);
//}

void destroy(SPKDArray kdArr){
	int i;
	if (kdArr==NULL){
			return;
		}
	//destroyArr(kdArr->indexArr);
	for (i=0 ; i<kdArr->dim ; i++){
		free(kdArr->indexArr[i]);
	}
	free(kdArr->indexArr);
	for (i=0 ; i<1 ; i++){
		printf("the first point: %f, %f\n", kdArr->originalArr[1]->data[0],kdArr->originalArr[1]->data[1]);
			spPointDestroy(kdArr->originalArr[i]);
		}
	free(kdArr->originalArr);
	free(kdArr);
}

int coorComparator(const void* p, const void* q){
//	double l = (double)spPointGetAxisCoor((*(pointIndex*)p)->point,(*(pointIndex*)p)->axis);
//	double r = (double)spPointGetAxisCoor((*(pointIndex*)q)->point,(*(pointIndex*)q)->axis);
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
//
//SPKDArray* split(SPKDArray kdArr, int coor){ //TODO don't forget to free allocation if using this func
//	int dim,size,half,i,j,k;
//	SPKDArray* res;
//	SPKDArray kdLeft,kdRight;
//	int* x;
//	SPPoint* p1;
//	SPPoint* p2;
//	pointIndex* pI1;
//	pointIndex* pI2;
//	SPPoint tmpP;
//	int* map1;
//	int* map2;
//
//	size=kdArr->size;
//	half=ceil(((double)(size))/2);
//
//	/////////// memory allocation///////
//	res=(SPKDArray*)malloc(2*sizeof(SPKDArray));
//	if (res==NULL){  //memory allocation failed
//		return NULL;
//	}
//	x=(int*)malloc(size*sizeof(int));
//	if (x==NULL){  //memory allocation failed
//		free(res);
//		return NULL;
//	}
//	p1=(SPPoint*)malloc(half*sizeof(SPPoint));
//	if (p1==NULL){  //memory allocation failed
//		free(res);
//		free(x);
//		return NULL;
//	}
//	p2=(SPPoint*)malloc((size-half)*sizeof(SPPoint));
//	if (p2==NULL){  //memory allocation failed
//		free(res);
//		free(x);
//		free(p1);
//		return NULL;
//	}
//	pI1=(pointIndex*)malloc(half*sizeof(pointIndex));
//	if (pI1==NULL){
//		free(res);
//		free(x);
//		free(p1);
//		free(p2);
//		return NULL;
//	}
//	pI2=(pointIndex*)malloc((size-half)*sizeof(pointIndex));
//		if (pI2==NULL){
//		free(res);
//		free(x);
//		free(p1);
//		free(p2);
//		free(pI1);
//		return NULL;
//		}
//	map1=(int*)malloc(size*sizeof(int));
//	if (map1==NULL){  //memory allocation failed
//		free(res);
//		free(x);
//		free(p1);
//		free(p2);
//		free(pI1);
//		free(pI2);
//		return NULL;
//	}
//	map2=(int*)malloc(size*sizeof(int));
//	if (map2==NULL){  //memory allocation failed
//		free(res);
//		free(x);
//		free(p1);
//		free(p2);
//		free(pI1);
//		free(pI2);
//		free(map1);
//		return NULL;
//	}
//
//	// creating the map array x
//	for (i=0 ; i<size ; i++){
//		if (i<half){
//			x[kdArr->indexArr[coor][i]]=0;
//		}
//		x[kdArr->indexArr[coor][i]]=1;
//	}
//	// creating the aux arrays p1,p2 and pI1 pI2
//	j=0;
//	k=0;
//	for (i=0 ; i<size ; i++){
//		if (x[i]==0){
//			tmpP=spPointCopy(kdArr->originalArr[i]);
//			p1[j]=tmpP;
//			pI1[j]=createPointIndex(tmpP,j,coor);
//			j++;
//		}
//		else if(x[i]==1){
//			tmpP=spPointCopy(kdArr->originalArr[i]);
//			p2[k]=tmpP;
//			k++;
//		}
//	}
//	// creating the map arrays map1,map2
//	j=0;
//	for (i=0 ; i<size ; i++){
//		if (kdArr->originalArr[i]==p1[j]){  // maybe by hand? TODO
//			map1[i]=j;
//		}
//		else{
//			map1[i]=-1;
//		}
//	}
//
//	k=0;
//	for (i=0 ; i<size ; i++){
//		if (kdArr->originalArr[i]==p2[k]){  // maybe by hand? TODO
//			map2[i]=k;
//		}
//		else{
//			map2[i]=-1;
//		}
//	}
//
//
//
//
//
//
//	return res;
//}

//int main() {
//	SPPoint a,b,c,d,e;
//	SPPoint* arr;
//	int dim,index,size,i;
//	SPKDArray kdArr;
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
//	printf("the first point: %f, %f\n", kdArr->originalArr[1]->data[0],kdArr->originalArr[1]->data[1]);
//
//	printf("%d",kdArr->dim);
//	dim=2; //
//	for (i=0 ; i<size ; i++){
//		spPointDestroy(arr[i]);
//	}
//	printf("the first point: %f, %f\n", kdArr->originalArr[1]->data[0],kdArr->originalArr[1]->data[1]);
//
//	free(arr);
//	destroy(kdArr);
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


