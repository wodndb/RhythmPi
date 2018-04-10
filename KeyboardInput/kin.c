#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <../Libs/rpUtil.h>

int main(void) {
	char c;

	while(1) {
		if(rp_kbhit()) {
			printf("(%c)\n", rp_getch());
			rp_fpurge_stdin();
		}
		else {
			printf("XXX\n");
		}
		usleep(10000);
	}

	return 0;
}
