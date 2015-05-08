#include <stdio.h>
#include <stdlib.h>
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
	char bg[20];
	char layer[20];
	int po;
	int plength;
	int pfilteragin;
	char filtertype[20];
	int chokkakuvol;
	char ver[10];
} KshInfo;

void testKshParsing(FILE* ksh_file_stream) {
	char buffer[80];
	char tbuffer[20];
	char buffer2[20];
	char *tokStr;
	int i;

	rewind(ksh_file_stream);
	fseek(ksh_file_stream, 3, SEEK_SET);
	fgets(buffer, 80, ksh_file_stream);
	tokStr = strtok(buffer, "=");
	
	printf("%s", tokStr);
	strcpy(tbuffer, tokStr);

	tokStr = strtok(NULL, "\n");
	printf("%s", tokStr);
	
	strcpy(buffer2, tokStr);
	printf("%s\n", buffer2);

	for(i = 0; i < strlen(tokStr); i++) {
		printf("%c : %d\n", tbuffer[i], tbuffer[i]);
	}
}

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
		else if(strncmp(tokAttr, "t", strlen(tokAttr) + 1) == 0)		{ ksh_form_info->t = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "mvol", strlen(tokAttr) + 1) == 0)		{ ksh_form_info->mvol = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "o", strlen(tokAttr) + 1) == 0)		{ ksh_form_info->o = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "bg", strlen(tokAttr) + 1) == 0)		{ strcpy(ksh_form_info->bg, tokAttrVal); }
		else if(strncmp(tokAttr, "layer", strlen(tokAttr) + 1) == 0)		{ strcpy(ksh_form_info->layer, tokAttrVal); }
		else if(strncmp(tokAttr, "po", strlen(tokAttr) + 1) == 0)		{ ksh_form_info->po = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "plength", strlen(tokAttr) + 1) == 0)	 	{ ksh_form_info->plength = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "pfilteragin", strlen(tokAttr) + 1) == 0)	{ ksh_form_info->pfilteragin = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "filtertype", strlen(tokAttr) + 1) == 0)	{ strcpy(ksh_form_info->filtertype, tokAttrVal); }
		else if(strncmp(tokAttr, "chokkakuvol", strlen(tokAttr) + 1) == 0)	{ ksh_form_info->chokkakuvol = atoi(tokAttrVal); }
		else if(strncmp(tokAttr, "ver", strlen(tokAttr) + 1) == 0)		{ strcpy(ksh_form_info->ver, tokAttrVal); }
		else if(strncmp(tokAttr, "m", strlen(tokAttr) + 1) == 0)		{
			mIndex = 0;
			tokAttrVal = strtok(tokAttrVal, ";");
			while(tokAttrVal != NULL) {
				strcpy(*(ksh_form_info->m + mIndex), tokAttrVal);
				mIndex++;
				tokAttrVal = strtok(NULL, ";");
			}
		}
		else {}

		fgets(buffer, 115, ksh_file_stream);
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
	KshInfo *ki;

	ki = (KshInfo*)malloc(1 * sizeof(KshInfo));

	scanf("%s", kshFileName);
	kshFile = fopen(kshFileName, "r");
	
	getKshInfo(kshFile, ki);
	printKshInfo(ki);
	

	/*
	while(!feof(kshFile)) {
		fgets(buffer, 50, kshFile);
		printf("%s", buffer);
		Sleep(50);
	}
	*/
	
	return 0;
}
