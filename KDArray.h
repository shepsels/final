/*
 * KDArray.h
 *
 *  Created on: 9 баев„ 2016
 *      Author: Noa
 */

#ifndef KDARRAY_H_
#define KDARRAY_H_

# include "SPPoint.h"
# include <stdbool.h>

struct point_index;
typedef struct point_index* pointIndex;
struct kd_array;
typedef struct kd_array* SPKDArray;

/**
 * Initializes the kd-array with the data given by arr
 *
 * @return
 * NULL in case of allocation failure
 * Otherwise, the new kd-array is returned
 */
SPKDArray init(SPPoint* arr, int size);

/**
 * Free all memory allocation associated with the array of kd-array,
 * if the array is NULL nothing happens.
 */

/**TODO
 *
 */
pointIndex createPointIndex(SPPoint p, int index, int axis);

/**TODO
 *
 */
void destroyPI(pointIndex pI);

/**TODO
 *
 */
bool sortByAxis(int** res,SPPoint* arr, int size,int dim);
//void destroyArr(int** arr);

/**
 * Free all memory allocation associated with kd-array,
 * if kd-array is NULL nothing happens.
 */
void kdArrDestroy(SPKDArray kdArr);


/**
 *comparator for the qsort function.
 *comparing according to the current axis
 */
int coorComparator(const void* p,const void* q);

/**
 *splits the given kd-array into two kd-arrays such that the first n/2(rounded up) points
 *with the respect to the coordinate coor are in kdLeft and the rest of the points are in kdRight
 *
 *@return   TODO
 *
 */
SPKDArray* split(SPKDArray kdArr, int coor);

SPKDArray splitSort(int** indexArr,int* map,int* x,int lOrR,int half,int size,int dim,int coor);

//TODO
SPPoint pIGetPoint(pointIndex pI);

int pIGetIndex(pointIndex pI);

int pIGetAxis(pointIndex pI);

SPPoint* getOriginalArr(SPKDArray kdArr);

int** getIndexArr(SPKDArray kdArr);

int getSize(SPKDArray kdArr);

int getDim(SPKDArray kdArr);

#endif /* KDARRAY_H_ */
