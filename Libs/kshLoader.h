#ifndef RP_KSH_LOADER_H
#define RP_KSH_LOADER_H

#include <rpNote.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ksh_info {
	char title[50];
	char artist[50];
	char effect[10];
	char jacket[20];
	char illustrator[20];
	char difficulty[10];
	int level;
	int t;
	char m[4][25];
	int mvol;
	int o;
	char bg[20];
	char layer[20];
	int po;
	int plength;
	int pfilteragin;
	char filtertype[20];
	int chokkakuvol;
	char ver[10];
} KshInfo;

void getKshInfo(FILE* ksh_file_stream, KshInfo* ksh_form_info);
void printKshInfo(KshInfo* ksh_form_info);
int printKshNoteType(FILE* ksh_file_stream);
void loadKshNote(FILE* ksh_file_stream, QType *qt_ksh_note);
void printKshNote(QType *qt_ksh_note);

#endif
