#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <wiringPi.h>
#include "../Libs/rpNote.h"
#include "../Libs/kshLoader.h"

int main(void) {
	FILE* kshFile;
	char kshFileName[30] = {0};
	char buffer[50] = {0};
	KshInfo *ki;
	QType qt;

	ki = (KshInfo*)malloc(1 * sizeof(KshInfo));

	scanf("%s", kshFileName);
	kshFile = fopen(kshFileName, "r");
	
	getKshInfo(kshFile, ki);
	printKshInfo(ki);
	printKshNoteType(kshFile);
	
	loadKshNote(kshFile, &qt);
	printKshNote(kshFile, &qt);

	return 0;
}
