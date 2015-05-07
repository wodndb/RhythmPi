#ifndef LINKED_QUEUE
#define LINKED_QUEUE

typedef struct queueNode {
	int data;
	struct queueNode *link;
} QNode;

typedef struct qeueuType {
	QNode *front, *rear;
} QType;

void qError(char *Message);	//Error Message Function
void init(QType *q);		//Initialize Function of Queue Structure
int is_empty(QType *q);		//Check Queue State is empty or not empty
int is_full(QType *q);		//Check Queue State is full or not full
void enqueue(QType *q, int data);		//Insert element to Queue;
int dequeue(QType *q);		//Delete element from Queue;
int peek(QType *q);			//Peek element from Queue;

#endif /* LINKED_QUEUE */