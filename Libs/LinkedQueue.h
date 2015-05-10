#ifndef LINKED_QUEUE
#define LINKED_QUEUE

#include <rpNote.h>

typedef struct queueNode {
	RpNote note;
	struct queueNode *link;
} QNode;


typedef struct qeueuType {
	QNode *front, *rear;
} QType;


void qError(char *Message);				//Error Message Function

void init(QType *q);					//Initialize Function of Queue Structure

int is_empty(QType *q);					//Check Queue State is empty or not empty

int is_full(QType *q);					//Check Queue State is full or not full

void enqueue(QType *q, RpNote note);			//Insert element to Queue;

RpNote dequeue(QType *q);			//Delete element from Queue;

RpNote peek(QType *q);				//Peek element from Queue;


#endif // LINKED_QUEUE
