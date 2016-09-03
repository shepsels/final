/*
 * SPPoint.c
 *
 *  Created on: 27 ���� 2016
 *      Author: Noa
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPPoint.h"


SPPoint spPointCreate(double* data, int dim, int index){
	SPPoint newPoint;
	double* newData;

	if(data == NULL || dim <=0 || index <0){
		return NULL;
	}
	newPoint=(SPPoint)malloc(sizeof(struct sp_point_t));
	if(newPoint == NULL)    //memory allocation failed
		{
			return NULL;
		}

	newPoint->index=index;
	newPoint->dim=dim;
	newData=(double*)malloc(dim*sizeof(double));
	if(newData == NULL)    //memory allocation failed
		{
			free(newPoint);
			return NULL;
		}
	memcpy(newData,data,dim*sizeof(double));
	newPoint->data=newData;

	return newPoint;
}

SPPoint spPointCopy(SPPoint source){
	SPPoint pointCopy;
	double* newData;

	pointCopy=(SPPoint)malloc(sizeof(struct sp_point_t));
		if(pointCopy == NULL)    //memory allocation failed
			{
				free(pointCopy);
				return NULL;
			}
	newData=(double*)malloc(source->dim*sizeof(double));
	if(newData == NULL)    //memory allocation failed
			{
				free(pointCopy);
				return NULL;
			}
	memcpy(newData,source->data,source->dim*sizeof(double));
	pointCopy->data=newData;
	pointCopy->index=source->index;
	pointCopy->dim=source->dim;
	return pointCopy;
}

void spPointDestroy(SPPoint point){
	if (point == NULL) {
			return;
		}
	free(point->data);
	free(point);

}

int spPointGetDimension(SPPoint point){
	int res=point->dim;
	return res;
}

int spPointGetIndex(SPPoint point){
	return point->index;
}

double spPointGetAxisCoor(SPPoint point, int axis){
	double res;
	if (!point){
		printf("point in axis func is null"); //TODO
		return -1.0;
	}
	res=(double)(point->data[axis]);
	return res;
}

double spPointL2SquaredDistance(SPPoint p, SPPoint q){
	double pI,qI,res=0;
	int i;

	for (i=0 ; i<p->dim ; i++){
		pI=p->data[i];
		qI=q->data[i];
		res+=(pI-qI)*(pI-qI);
	}
	return res;
}
