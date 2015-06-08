///
//  KOREA UNIVERSITY OF TECHNOLOGY AND EDUCATION
//  SCHOOL OF COMPUTER SCIENCE AND ENGINEERING
//  EMBEDDED APPLICATION AND PRACTICE 2015 SPRING TERM PROJECT
//  RHYTHMPI : Rhythm game for raspberry pi
//  url: http://www.koreatech.ac.kr    : Official Univ. home page
//       http://cse.koreatech.ac.kr    : Official Dept. home page
//
// Author : 
//  SCHOOL OF COMPUTER SCIENCE AND ENGINEERING
//  2012136116 JEONG, JAE-U
//  wodndb@koreatech.ac.kr
//

// rpNote.c
//
//		Define notes that loaded from ksh files. 
//		This library provides queue structure and functions to save
//		a lot of notes that used in Rhythm pi
//

///
//  Includes
//
#include <rpNote.h>
#include <stdio.h>
#include <malloc.h>

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
// CreateNote()
//
//    Creates a note by allocate RpNote struct.
//
RpNote* createNote(int type, int measure, int order, int max) {
	RpNote* tempNote = (RpNote*)malloc(1 * sizeof(RpNote));
	tempNote->type = type;
	tempNote->measure = measure;
	tempNote->order = order;
	tempNote->max = max;
	tempNote->hitted = hitted;
	return tempNote;
}

///
// qError()
//
//    Error Message Function
//
void qError(char *message) {
	fprintf(stderr, "%s\n", message);
}

///
// initQueue()
//
//    Initialize Function of Queue Structure
//
void initQueue(QType *q) {
	q->front = NULL;
	q->rear = NULL;
}

///
// is_empty()
//
//    Check Queue State is empty or not empty
//
int is_empty(QType *q) {
	return (q->front == NULL);
}

///
// is_full()
//
//    Check Queue State is full or not full
//    NOTE: This function is not avilable now.
//
int is_full(QType *q) {
	return 0;
}

///
// enqueue()
//
//    Insert element to Queue
//
void enqueue(QType *q, RpNote note) {
	QNode *temp = (QNode *)malloc(1 * sizeof(QNode));

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

///
// dequeue()
//
//    Delete element from Queue
//
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
		temp = NULL;
		return note;
	}
}

///
// dequeue()
//
//    Delete element from Queue
//
RpNote dequeue_middle(QType *q, QNode *pnode, QNode *rnode) {
	QNode *temp;
	RpNote note;

	if(is_empty(q)) {
		qError("Queue is empty!");
	}
	else if(pnode != NULL) {
		printf("dequeue middle start\n");
		note = rnode->note;
		pnode->link = rnode->link;
		rnode->link = NULL;
		if(pnode->link == NULL) {
			q->rear = pnode;
		}
		free(rnode);
		rnode = NULL;
		printf("dequeue middle\n");
		return note;
	}
	else {
		printf("dequeue first start\n");
		temp = q->front;
		note = temp->note;
		q->front = q->front->link;
		temp->link = NULL;
		if(q->front == NULL) {
			q->rear = NULL;
		}
		free(temp);
		temp = NULL;
		printf("dequeue first\n");
		return note;
	}
}

///
// peek()
//
//    Peek element from Queue
//
RpNote peek(QType *q) {
	if( is_empty(q) ) {
		qError("Queue is empty");
	}
	else {
		return q->front->note;
	}
}
