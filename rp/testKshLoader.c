#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <wiringPi.h>
#include <rpNote.h>
#include <kshLoader.h>

int main(void) {
	FILE* kshFile;
	char buffer[80];
	KshInfo *ki;
	QType qt;

	ki = (KshInfo*)malloc(1 * sizeof(KshInfo));

	//printf("Please input ksh file : ");
	//scanf("%s", buffer);
	kshFile = fopen("../kshLoader/test.ksh", "r");

	getKshInfo(kshFile, ki);
	printKshInfo(ki);

	loadKshNote(kshFile, &qt);
	printKshNote(&qt);
}
