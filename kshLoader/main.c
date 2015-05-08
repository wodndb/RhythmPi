#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <string.h>
#include <math.h>

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
	char bg;
	char layer[20];
	int po;
	int plength;
	int pfilteragin;
	char filtertype[20];
	int chokkakuvol;
	char ver[10];
} KshInfo;

void getKshInfo(FILE* ksh_file_stream, KshInfo* ksh_form_info) {
	char buffer[115];
	char *tokAttr;
	char *tokAttrVal;
	int mIndex;

	rewind(ksh_file_stream);
	fgets(buffer, 115, ksh_file_stream);
	printf("%s", buffer);
	while(!strcmp(buffer, "--")) {
		tokAttr = strtok(buffer, " =");
		tokAttrVal = strtok(NULL, " ;");

		printf("%s %s\n", tokAttr, tokAttrVal);

		if(strcmp(tokAttr, "title"))			{ strcpy(ksh_form_info->title, tokAttrVal); }
		else if(strcmp(tokAttr, "artist"))		{ strcpy(ksh_form_info->artist, tokAttrVal); }
		else if(strcmp(tokAttr, "effect"))		{ strcpy(ksh_form_info->effect, tokAttrVal); }
		else if(strcmp(tokAttr, "jacket"))		{ strcpy(ksh_form_info->jacket, tokAttrVal); }
		else if(strcmp(tokAttr, "illustrator"))	{ strcpy(ksh_form_info->illustrator, tokAttrVal); }
		else if(strcmp(tokAttr, "difficulty"))	{ strcpy(ksh_form_info->difficulty, tokAttrVal); }
		else if(strcmp(tokAttr, "level"))		{ ksh_form_info->level = atoi(tokAttrVal); }
		else if(strcmp(tokAttr, "t"))			{ ksh_form_info->t = atoi(tokAttrVal); }
		else if(strcmp(tokAttr, "mvol"))		{ ksh_form_info->mvol = atoi(tokAttrVal); }
		else if(strcmp(tokAttr, "o"))			{ ksh_form_info->o = atoi(tokAttrVal); }
		else if(strcmp(tokAttr, "bg"))			{ ksh_form_info->bg = atoi(tokAttrVal); }
		else if(strcmp(tokAttr, "layer"))		{ strcpy(ksh_form_info->layer, tokAttrVal); }
		else if(strcmp(tokAttr, "po"))			{ ksh_form_info->po = atoi(tokAttrVal); }
		else if(strcmp(tokAttr, "plength"))		{ ksh_form_info->plength = atoi(tokAttrVal); }
		else if(strcmp(tokAttr, "pfilteragin"))	{ ksh_form_info->pfilteragin = atoi(tokAttrVal); }
		else if(strcmp(tokAttr, "filtertype"))	{ strcpy(ksh_form_info->filtertype, tokAttrVal); }
		else if(strcmp(tokAttr, "chokkakuvol"))	{ ksh_form_info->chokkakuvol = atoi(tokAttrVal); }
		else if(strcmp(tokAttr, "ver"))			{ strcpy(ksh_form_info->ver, tokAttrVal); }
		else if(strcmp(tokAttr, "m"))			{
			mIndex = 0;
			while(tokAttrVal != NULL) {
				strcpy(*(ksh_form_info->m + mIndex), tokAttrVal);
				mIndex++;
				tokAttr = strtok(NULL, ";");
			}
		}
		else {}

		fgets(buffer, 100, ksh_file_stream);
	}
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

int main(void) {
	FILE* kshFile;
	char kshFileName[30] = {0};
	char buffer[50] = {0};
	KshInfo ki;

	scanf("%s", kshFileName);
	kshFile = fopen(kshFileName, "r");
	
	getKshInfo(kshFile, &ki);
	printKshInfo(&ki);
	
	/*
	while(!feof(kshFile)) {
		fgets(buffer, 50, kshFile);
		printf("%s", buffer);
		Sleep(50);
	}
	*/
	
	return 0;
}