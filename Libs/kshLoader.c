#include <kshLoader.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <wiringPi.h>

void getKshInfo(FILE* ksh_file_stream, KshInfo* ksh_form_info) {
	char buffer[115];
	char *tokAttr;
	char *tokAttrVal;
	int mIndex;

	rewind(ksh_file_stream);
	fseek(ksh_file_stream, 3, SEEK_SET);
	fgets(buffer, 115, ksh_file_stream);
	while(strcmp(buffer, "--\n") != 0) {
		tokAttr = strtok(buffer, "=");
		tokAttrVal = strtok(NULL, "\n");
		
		if(tokAttrVal == '\0') {}
		else if(strncmp(tokAttr, "title", strlen(tokAttr) + 1) == 0) 		{ strcpy(ksh_form_info->title, tokAttrVal); }
		else if(strncmp(tokAttr, "artist", strlen(tokAttr) + 1) == 0)		{ strcpy(ksh_form_info->artist, tokAttrVal); }
		else if(strncmp(tokAttr, "effect", strlen(tokAttr) + 1) == 0)		{ strcpy(ksh_form_info->effect, tokAttrVal); }
		else if(strncmp(tokAttr, "jacket", strlen(tokAttr) + 1) == 0)		{ strcpy(ksh_form_info->jacket, tokAttrVal); }
		else if(strncmp(tokAttr, "illustrator", strlen(tokAttr) + 1) == 0)	{ strcpy(ksh_form_info->illustrator, tokAttrVal); }
		else if(strncmp(tokAttr, "difficulty", strlen(tokAttr) + 1) == 0)	{ strcpy(ksh_form_info->difficulty, tokAttrVal); }
		else if(strncmp(tokAttr, "level", strlen(tokAttr) + 1) == 0)		{ ksh_form_info->level = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "t", strlen(tokAttr) + 1) == 0)			{ ksh_form_info->t = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "mvol", strlen(tokAttr) + 1) == 0)			{ ksh_form_info->mvol = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "o", strlen(tokAttr) + 1) == 0)			{ ksh_form_info->o = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "bg", strlen(tokAttr) + 1) == 0)			{ strcpy(ksh_form_info->bg, tokAttrVal); }
		else if(strncmp(tokAttr, "layer", strlen(tokAttr) + 1) == 0)		{ strcpy(ksh_form_info->layer, tokAttrVal); }
		else if(strncmp(tokAttr, "po", strlen(tokAttr) + 1) == 0)			{ ksh_form_info->po = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "plength", strlen(tokAttr) + 1) == 0)	 	{ ksh_form_info->plength = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "pfilteragin", strlen(tokAttr) + 1) == 0)	{ ksh_form_info->pfilteragin = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "filtertype", strlen(tokAttr) + 1) == 0)	{ strcpy(ksh_form_info->filtertype, tokAttrVal); }
		else if(strncmp(tokAttr, "chokkakuvol", strlen(tokAttr) + 1) == 0)	{ ksh_form_info->chokkakuvol = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "ver", strlen(tokAttr) + 1) == 0)			{ strcpy(ksh_form_info->ver, tokAttrVal); }
		else if(strncmp(tokAttr, "m", strlen(tokAttr) + 1) == 0) {
			mIndex = 0;
			tokAttrVal = strtok(tokAttrVal, ";");
			while(tokAttrVal != NULL) {
				strcpy(*(ksh_form_info->m + mIndex), tokAttrVal);
				mIndex++;
				tokAttrVal = strtok(NULL, ";");
			}
		}
		else {
			//other attribute of ksh information
		}

		fgets(buffer, 115, ksh_file_stream);
	}
	fseek(ksh_file_stream, -3, SEEK_CUR);
}

void printKshInfo(KshInfo* ksh_form_info) {
	int mIndex;
	printf("title : %s\n", ksh_form_info->title);
	printf("artist : %s\n", ksh_form_info->artist);
	printf("effect : %s\n", ksh_form_info->effect);
	printf("jacket : %s\n", ksh_form_info->jacket);
	printf("illustrator : %s\n", ksh_form_info->illustrator);
	printf("difficulty : %s\n", ksh_form_info->difficulty);
	printf("level : %d\n", ksh_form_info->level);
	printf("t : %d\n", ksh_form_info->t);
	
	for(mIndex = 0; mIndex < 4; mIndex++) {
		printf("m%d : %s\n", mIndex, *(ksh_form_info->m + mIndex));
	}
	
	printf("mvol : %d\n", ksh_form_info->mvol);
	printf("o : %d\n", ksh_form_info->o);
	printf("bg : %s\n", ksh_form_info->bg);
	printf("layer : %s\n", ksh_form_info->layer);
	printf("po : %d\n", ksh_form_info->po);
	printf("plength : %d\n", ksh_form_info->plength);
	printf("pfilteragin : %d\n", ksh_form_info->pfilteragin);
	printf("filtertype : %s\n", ksh_form_info->filtertype);
	printf("chokkakuvol : %d\n", ksh_form_info->chokkakuvol);
	printf("ver : %s\n", ksh_form_info->ver);
}

int printKshNoteType(FILE* ksh_file_stream) {
	int sw = 0;
	int measure = 0;
	char buffer[80];
	
	do {
		fgets(buffer, 80, ksh_file_stream);
		if(strcmp(buffer, "--\n") != 0) {
			sw = 0;
			if(buffer[0] == '1')	{ printf("    BT-1"); sw++;}
			if(buffer[1] == '1')	{ printf("        BT-2"); sw++;}
			if(buffer[2] == '1')	{ printf("            BT-3"); sw++;}
			if(buffer[3] == '1')	{ printf("                BT-4"); sw++;}
			if(buffer[5] != '0')	{ printf("    [[FX-L]]"); sw++;}
			if(buffer[6] != '0')	{ printf("            [[FX-R]]"); sw++;}
			if(buffer[8] != '-')	{ printf("NB-L"); sw++;}
			if(buffer[9] != '-')	{ printf("                    NB-R"); sw++;}
			printf("\n");
		}
		else {
			printf("<<-- Measure : %d -->>\n", ++measure);
		}
		delay(20);	
	} while(!feof(ksh_file_stream));
}

void loadKshNote(FILE* ksh_file_stream, QType *qt_ksh_note) {
	int noteType;
	int order = 0;
	int measure = 0;
	char buffer[80];
	RpNote tempNote;
	
	printf("entering laodKshNote function!\n");

	initQueue(qt_ksh_note);

	do {
		fgets(buffer, 80, ksh_file_stream);
		if(strcmp(buffer, "--\n") != 0) {
			if(buffer[0] == '1') {
				tempNote.order = ++order;
				tempNote.type = RP_NOTE_TYPE_BT_FIRST;
				tempNote.measure = measure;
				enqueue(qt_ksh_note, tempNote);
			}
			if(buffer[1] == '1') {
				tempNote.order = ++order;
				tempNote.type = RP_NOTE_TYPE_BT_SECOND;
				tempNote.measure = measure;
				enqueue(qt_ksh_note, tempNote);
			}
			if(buffer[2] == '1') {
				tempNote.order = ++order;
				tempNote.type = RP_NOTE_TYPE_BT_THIRD;
				tempNote.measure = measure;
				enqueue(qt_ksh_note, tempNote);
			}
			if(buffer[3] == '1') {
				tempNote.order = ++order;
				tempNote.type = RP_NOTE_TYPE_BT_FOURTH;
				tempNote.measure = measure;
				enqueue(qt_ksh_note, tempNote);
			}
			if(buffer[5] != '0') {
				tempNote.order = ++order;
				tempNote.type = RP_NOTE_TYPE_FX_LEFT;
				tempNote.measure = measure;
				enqueue(qt_ksh_note, tempNote);
			}
			if(buffer[6] != '0') {
				tempNote.order = ++order;
				tempNote.type = RP_NOTE_TYPE_FX_RIGHT;
				tempNote.measure = measure;
				enqueue(qt_ksh_note, tempNote);
			}
			//
			// I will parsing knov!
			//
		}
		else {
			measure++;
			order = 0;
		}
	} while(!feof(ksh_file_stream));
}
void printKshNote(QType *qt_ksh_note) {
	printf("Entering printkshNote function!\n");
	QNode* tempQNode = qt_ksh_note->front;
	while(tempQNode->link != NULL) {
		printf("MEASURE : %d, ORDER : %d, TYPE : %d\n", 
			tempQNode->note.measure, tempQNode->note.order, tempQNode->note.type);
		tempQNode = tempQNode->link;
	}
}
