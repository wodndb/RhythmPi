// KOREA UNIVERSITY OF TECHNOLOGY AND EDUCATION
// EMBBEDDED APPLICATION AND PRACTICE
// SPRING TERM PROJECT BASED ON RASPBERRY PI
// COMPUTER SCIENCE AND ENGINEERING 2012136116
// JAEU JEONG

#include <rpNote.h>
#include <stdio.h>
#include <malloc.h>

float v_rp_note_bt_1[] = { -0.1f, -0.1f, 0.0f, 
                           -0.1f,  0.1f, 0.0f,
						   -0.3f, -0.1f, 0.0f,
						   -0.3f,  0.1f, 0.0f };
								  
float v_rp_note_bt_2[] = { -0.1f, -0.1f, 0.0f, 
                           -0.1f,  0.1f, 0.0f,
					       -0.3f, -0.1f, 0.0f,
				           -0.3f,  0.1f, 0.0f };

float v_rp_note_bt_3[] = { -0.1f, -0.1f, 0.0f, 
                           -0.1f,  0.1f, 0.0f,
					       -0.3f, -0.1f, 0.0f,
						   -0.3f,  0.1f, 0.0f };

float v_rp_note_bt_4[] = { -0.1f, -0.1f, 0.0f, 
                           -0.1f,  0.1f, 0.0f,
					       -0.3f, -0.1f, 0.0f,
						   -0.3f,  0.1f, 0.0f };

float v_rp_note_fx_l[] = { -0.1f, -0.1f, 0.0f, 
                           -0.1f,  0.1f, 0.0f,
					   	   -0.3f, -0.1f, 0.0f,
						   -0.3f,  0.1f, 0.0f };

float v_rp_note_fx_r[] = { -0.1f, -0.1f, 0.0f, 
                           -0.1f,  0.1f, 0.0f,
					       -0.3f, -0.1f, 0.0f,
						   -0.3f,  0.1f, 0.0f };


RpNote* createNote(int type, int measure, int order) {
	RpNote* tempNote = (RpNote*)malloc(1 * sizeof(RpNote));
	tempNote->type = type;
	tempNote->measure = measure;
	tempNote->order = order;
	
	return tempNote;
}

//Error Message Function
void qError(char *message) {
	fprintf(stderr, "%s\n", message);
}

//Initialize Function of Queue Structure
void initQueue(QType *q) {
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
