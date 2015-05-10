#include <LinkedQueue.h>
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
void enqueue(QType *q, RpNote note) {
	QNode *temp = (QNode *)malloc(sizeof(QNode));
	if(temp == NULL) {
		qError("Can't allocation memory");
	}
	else {
		//Initialize allocated node
		temp->note = note;
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
RpNote dequeue(QType *q) {
	QNode *temp = q->front;
	RpNote note;

	if(is_empty(q)) {
		qError("Queue is empty!");
	}

	else {
		note = temp->note;
		q->front = q->front->link;
		if(q->front == NULL) {
			q->rear = NULL;
		}
		free(temp);
		return note;
	}
}

//Peek element from Queue;
RpNote peek(QType *q) {
	if( is_empty(q) ) {
		qError("Queue is empty");
	}
	else {
		return q->front->note;
	}
}
