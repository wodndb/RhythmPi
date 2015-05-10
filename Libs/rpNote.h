///
// INFORMATION OF THIS SOURCE : 
//  KOREA UNIVERSITY OF TECHNOLOGY AND EDUCATION
//  SCHOOL OF COMPUTER SCIENCE AND ENGINEERING
//  (url: www.koreatech.ac.kr, cse.koreatech.ac.kr)
//  EMBEDDED APPLICATION AND PRACTICE 2015 SPRING TERM PROJECT
//  RHYTHMPI : Rhythm game for raspberry pi
//
// Author : 
//  SCHOOL OF COMPUTER SCIENCE AND ENGINEERING
//  2012136116 JEONG, JAE-U
//  wodndb@koreatech.ac.kr
//
// LISENCE:
//  FREE FOR ALL IF YOU CAN UNDERSTAND THIS SOURCE CODE :)
//
// \ NAME : rpNote.h
// \ this header file defines note that from files
///

////
// TO DO
// 1. Add long note
// 2. Add nobe line
// 3. Parsing ksh file
// 4. roading note from ksh file
////

#ifndef RP_NOTE_H
#define RP_NOTE_H

// Define note type
#define RP_NOTE_TYPE_BT_FIRST   0x80   // |#| | | | // 0b 1000 0000
#define RP_NOTE_TYPE_BT_SECOND  0x40   // | |#| | | // 0b 0100 0000
#define RP_NOTE_TYPE_BT_THIRD   0x20   // | | |#| | // 0b 0010 0000
#define RP_NOTE_TYPE_BT_FOURTH  0x10   // | | | |#| // 0b 0001 0000
#define RP_NOTE_TYPE_FX_LEFT    0x08   // |###| | | // 0b 0000 1000
#define RP_NOTE_TYPE_FX_RIGHT   0x04   // | | |###| // 0b 0000 0100
                                       // +-+-+-+-+ // Line of judgement
                                       // |1|2|3|4| // Arrangement of BT-button
                                       // |FXL|FXR| // Arrangement of FX-button

typedef struct _note {
   int type;
   int measure;
   int order;
} RpNote;

typedef struct queueNode {
	RpNote note;
	struct queueNode *link;
} QNode;

typedef struct qeueuType {
	QNode *front, *rear;
} QType;

RpNote* createNote(int type, int measure, int order);
void qError(char *Message);				//Error Message Function
void init(QType *q);					//Initialize Function of Queue Structure
int is_empty(QType *q);					//Check Queue State is empty or not empty
int is_full(QType *q);					//Check Queue State is full or not full
void enqueue(QType *q, RpNote note);			//Insert element to Queue;
RpNote dequeue(QType *q);			//Delete element from Queue;
RpNote peek(QType *q);				//Peek element from Queue;

#endif /* RP_NOTE_H */
