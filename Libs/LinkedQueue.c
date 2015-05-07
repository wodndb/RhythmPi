#include "LinkedQueue.h"
#include <stdio.h>
#include <malloc.h>

//Error Message Function
void qError(char *message) {
	fprintf(stderr, "%s\n", message);
}

//Initialize Function of Queue Structure
void init(QType *q) {
	q->front = NULL;
	q->rear = NULL;
}

//Check Queue State is empty or not empty
int is_empty(QType *q) {
	return (q->front == NULL);
}

//Check Queue State is full or not full
//NOTE: This function is not avilable now.
int is_full(QType *q) {
	return 0;
}

//Insert element to Queue;
void enqueue(QType *q, int data) {
	QNode *temp = (QNode *)malloc(sizeof(QNode));
	if(temp == NULL) {
		qError("Can't allocation memory");
	}
	else {
		//Initialize allocated node
		temp->data = data;
		temp->link = NULL;
		if( is_empty(q) ) {
			q->front = temp;
			q->rear = temp;
		}
		else {
			q->rear->link = temp;
			q->rear = temp;
		}
	}
}

//Delete element from Queue;
int dequeue(QType *q) {
	QNode *temp = q->front;
	int data;

	if(is_empty(q)) {
		qError("Queue is empty!");
	}

	else {
		data = temp->data;
		q->front = q->front->link;
		if(q->front == NULL) {
			q->rear = NULL;
		}
		free(temp);
		return data;
	}
}

//Peek element from Queue;
int peek(QType *q) {
	if( is_empty(q) ) {
		qError("Queue is empty");
	}
	else {
		return q->front->data;
	}
}