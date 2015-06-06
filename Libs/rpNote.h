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

//
/// \ file :  rpNote.h
/// \ brief : Define notes that loaded from ksh files. 
///           This library provides queue structure and functions to save
///           a lot of notes that used in Rhythm pi
//

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
#define RP_NOTE_TYPE_LONG_END   0x4000
#define RP_NOTE_TYPE_LONG_MID	0x2000
#define RP_NOTE_TYPE_LONG_STT   0x1000
#define RP_NOTE_TYPE_BT_FIRST   0x0800   // |#| | | | // 0b 1000 0000
#define RP_NOTE_TYPE_BT_SECOND  0x0400   // | |#| | | // 0b 0100 0000
#define RP_NOTE_TYPE_BT_THIRD   0x0200   // | | |#| | // 0b 0010 0000
#define RP_NOTE_TYPE_BT_FOURTH  0x0100   // | | | |#| // 0b 0001 0000
// Division Line between bt and fx, So 0x080 is jumped
#define RP_NOTE_TYPE_FX_LEFT    0x0040   // |###| | | // 0b 0000 0100
#define RP_NOTE_TYPE_FX_RIGHT   0x0020   // | | |###| // 0b 0000 0010
// Division Line between fx and knov, So 0x010 is jumped
                                         // +-+-+-+-+ // Line of judgement
                                         // |1|2|3|4| // Arrangement of BT-button
                                         // |FXL|FXR| // Arrangement of FX-button

///
// structs
//

/// RpNote has information of a note.
typedef struct _note {
   int type;      // type of note
   int measure;   // music term.
   int order;     // order of note in a measure 
   int max;       // number of notes in a measure
} RpNote;

/// QNode is node of queue, that has note and next nodes address
typedef struct queueNode {
	RpNote note;
	struct queueNode *link;
} QNode;

/// QType is Queue structure.
typedef struct queueType {
	QNode *front, *rear;   //front of queue and rear of queue 
} QType;

///
//  Public Functions
//

//
/// \brief Creates a note by allocate RpNote struct.
//
/// \param type      Type of note
/// \param measure   music term.
/// \param order     order of note in a measure 
/// \param max       number of notes in a measure
//
/// \return          *RpNote that has allocated RpNote data
RpNote* createNote(int type, int measure, int order, int max);

//
/// \brief Print error message
//
/// \param Message   string to print
//
void qError(char *Message);

//
/// \brief Initialize Function of Queue Structure
//
/// \param q         Queue structure
//
void initQueue(QType *q);

//
/// \brief Check Queue State is empty or not empty
//
/// \param q         Queue structure
//
/// \return          1  if queue is empty
///                  0  if queue is not empty
int is_empty(QType *q);

//
/// \brief Check Queue State is full or not full
//
/// \param q         Queue structure
//
/// \return          1  if queue is full
///                  0  if queue is not full
int is_full(QType *q);

//
/// \brief Insert element to Queue;
//
/// \param q         Queue structure
/// \param note      note that will be enqueued to Queue;
//
void enqueue(QType *q, RpNote note);

//
/// \brief Delete element from Queue (front)
//
/// \param q         Queue structure
//
/// \return          RpNote that deleted from queue
RpNote dequeue(QType *q);

//
/// \brief Delete element from Queue (particular node)
//
/// \param q         Queue structure
//
/// \return          RpNote that deleted from queue
RpNote dequeue(QType *q, QNode *node);

//
/// \brief Peek element from Queue
//
/// \param q         Queue structure
//
/// \return          RpNote that peeked in queue
RpNote peek(QType *q);\

#endif /* RP_NOTE_H */
