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
/// \ file :  kshLoader.h
/// \ brief : Librarys for loading songs imformation and notes from 
//            ksh file format. And defines struct for saving information
//            of ksh.
//
#ifndef RP_KSH_LOADER_H
#define RP_KSH_LOADER_H

///
// includes
//
#include <rpNote.h>
#include <stdio.h>
#include <stdlib.h>

///
// structs
//

/// ksh file information
typedef struct ksh_info {
	char title[50];         // title of this song
	char artist[50];        // who make this song
	char effect[10];        // effect to display
	char jacket[20];        // jacket of song
	char illustrator[20];   // who make jacket
	char difficulty[10];    // hardness of song
	int level;              // level about difficult to play
	int t;                  // BPM - bit per minute
	char m[4][25];          // music file names
	int mvol;               // music volumes
	int o;                  // ?
	char bg[20];            // background image
	char layer[20];	        // ?
	int po;	                // ?
	int plength;            // ?
	int pfilteragin;        // ?
	char filtertype[20];	// ?
	int chokkakuvol;        // ?
	char ver[10];           // version of ksh file format
} KshInfo;

///
//  Public Functions
//

//
/// \brief Creates a note by allocate RpNote struct.
//
/// \param ksh_file_stream      : Stream that has ksh file
/// \param ksh_form_info        : Ksh information structure
//
void getKshInfo(FILE* ksh_file_stream, KshInfo* ksh_form_info);

//
/// \brief print ksh information (for debugging)
//
/// \param ksh_form_info        : Ksh information structure
//
void printKshInfo(KshInfo* ksh_form_info);

//
/// \brief print note in ksh file format (for debugging)
//
/// \param ksh_file_stream      : Stream that has ksh file
//
int printKshNoteType(FILE* ksh_file_stream);

//
/// \brief Loading notes in ksh file format based queue structure
//
/// \param ksh_file_stream      : Stream that has ksh file
/// \param qt_ksh_note          : Queue structure
//
void loadKshNote(FILE* ksh_file_stream, QType *qt_ksh_note);

//
/// \brief print notes in ksh format (for debbuging)
//
/// \param qt_ksh_note          : Notes in queue
//
void printKshNote(QType *qt_ksh_note);

#endif //RP_KSH_LOADER_H
