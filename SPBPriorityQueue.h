#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
#include "SPList.h"
/**
 * SP Bounded Priority Queue summary
 *
 *  Implements a limited size queue with value and index.
 * The queue consists of a list, and its elements are listElements.
 * the queue is sorted by the value of the elements, and inside it
 * by their index.
 * when adding element to a queue with full capacity:
 * 	if the element has the highest value, it will not be inserted
 * 	else, the last element of the queue will be removed.
 *
 * The following functions are available:
 * spBPQueueCreate			creates an empty queue
 * spBPQueueCopy			copy the data of the queue to a new
 * 							allocated queue
 * spBPQueueDestroy			destroy an existing queue
 * spBPQueueClear			remove all elements from the queue and
 * 							free memory
 * spBPQueueSize			returns how many elements are in the queue
 * spBPQueueGetMaxSize		returns the bound size of the queue
 * spBPQueueEnqueue			insert a new element to the correct place
 * 							in the queue. remove last element if needed
 * spBPQueueDequeue			remove the first element of the queue
 * spBPQueuePeek			copy the first element of the queue
 * spBPQueuePeekLast		copy the first element of the queue
 * spBPQueueMinValue		returns the value of the first element in the queue
 * spBPQueueMaxValue		returns the value of the last element in the queue
 * spBPQueueIsEmpty			true if the queue is empty. otherwise false
 * spBPQueueIsFull			true if the queue is full. otherwise false
 * getLastElement			returns the last element in the queue
 *
 */

struct sp_bp_queue_t {

	int maxSize;
	SPList list;

};

/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t* SPBPQueue;


/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Allocates a new Bounded Priority Queue in the memory.
 *
 * @param
 * int maxSize, the maximum capacity of the queue.
 * @return
 * NULL in case allocation failure ocurred,
 * Otherwise, the new queue is returned
 */

SPBPQueue spBPQueueCreate(int maxSize);

/**
 * Creates a copy of target bpqueue.
 *
 * The new copy will contain all the elements from the source queue
 *
 * @param source The target queue to copy
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A new queue containing the same elements otherwise.
 */

SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * QueueDestroy: Deallocates an existing queue. Clears all elements by using the
 * list functions.
 *
 * @param Target quque to be deallocated. If list is NULL nothing will be
 * done since it's using listDestroy
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * Removes all elements from target queue.
 *
 * The elements are deallocated using the clear function of SPList
 * @param Target queue to remove all elements from
 * @return
 */
void spBPQueueClear(SPBPQueue source);

/**
 * Returns the number of elements in the queue.
 *
 * @param Target queue which size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the queue.
 */
int spBPQueueSize(SPBPQueue source);

/**
 * Retruns the maximum capacity allowed in the queue.
 *
 * @param the queue
 * @return
 * the maxSize of the queue
 */
int spBPQueueGetMaxSize(SPBPQueue source);

/**
 * Adds a new element to the queue, in order by value.
 *
 * @param The designated queue.
 * @param The element to insert. A copy of the element will be
 * inserted
 * @return
 *	SP_BPQUEUE_OUT_OF_MEMORY - allocation failure
 *	SP_BPQUEUE_FULL - the queue is full
 *	SP_BPQUEUE_INVALID_ARGUMENT - one of the arguments isn't correct
 *	SP_BPQUEUE_SUCCESS - element added successfully */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * Removes the element of the queue which contains the lowest value
 *
 * @param The designated queue
 * @return
 *	SP_BPQUEUE_INVALID_ARGUMENT - one of the arguments isn't correct
 *	SP_BPQUEUE_SUCCESS - element added successfully
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * creating and retrieving a new copy of the element with the lowest value
 *
 * @param The source queue
 * @return
 * NULL if the source queue or the source list is invalid
 * a copy of the first element of the queue otherwise
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * creating and retrieving a new copy of the element with the highest value
 *
 * @param The source queue
 * @return
 * NULL if the source queue or the source list is invalid
 * a copy of the last element of the queue otherwise
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * returns the minimum value in the queue
 *
 * @param The source queue
 * @return
 * -1 if the source queue or the source list is invalid
 * the minimum value in the queue otherwise
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 *  returns the maximum value in the queue
 *
 * @param The source queue
 * @return
 * -1 if the source queue or the source list is invalid
 * the maximum value in the queue otherwise
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * returns true if the queue is empty
 *
 * @param The designated queue
 * @return
 * true if the source queue and the list he contains are valid but empty
 * false otherwise
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * returns true if the queue is full
 *
 * @param The designated queue
 * @return
 * true if the source queue and the list he contains are valid and the list's size is equal to the maximum size of the queue
 * false otherwise
 */
bool spBPQueueIsFull(SPBPQueue source);

/**
 * find and returns the last element of the queue (the item with the highest value)
 *
 * @param The source queue
 * @return
 * the last element of the source queue
 */
SPListElement getLastElement(SPBPQueue source);

#endif
