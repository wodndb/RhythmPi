#include <stdio.h>
#include <wiringPi.h>

#define RE_A 15
#define RE_B 16
#define MAX_MOV 10 
int main(void) {
	int ra, rb;
	int mov_num[MAX_MOV] = {0};
	int mov_index = 0;
	int i;
	int ROT_DATA[4] = {0, 1, 3, 2};
	int past_input_code = 0;
	int code = 0;

	if(wiringPiSetup() == -1) {
		return 0;
	}
	
	pinMode(RE_A, INPUT);
	pinMode(RE_B, INPUT);

	while(1) {
		//Input purse from A pin pulse and sampling it(moving average filter)
		ra = digitalRead(RE_A);
		mov_num[mov_index] = ra;
		mov_index++;
		mov_index %= MAX_MOV;
		for(i = 0; i < MAX_MOV; i++) {
			if(mov_num[i] == 1) {
				ra = 1;
				break;
			}
		}
		if(i == MAX_MOV) {
			ra = 0;
		}
		
		//input pulse from B pin
		rb = digitalRead(RE_B);

		if(rb == 1) {
			printf("    *\t");
		}
		else {
			printf("*    \t");
		}

		if(ra == 1) {
			printf("    *\t");	
		}
		else {
			printf("*    \t");
		}

		code = (ra & 0x01) | ((rb & 0x01) << 1);

		printf("%d%d : %d\t", rb, ra, (ra & 0x01) | ((rb & 0x01) << 1));
		if(past_input_code == code) {printf("");}
		else if(code == ROT_DATA[(ROT_DATA[past_input_code] + 1) % 4]) {printf("    >>>> : %d", (ROT_DATA[past_input_code] + 1) % 4);}
		else if(code == ROT_DATA[(ROT_DATA[past_input_code] + 3) % 4]) {printf("<<<<     : %d", (ROT_DATA[past_input_code] + 3) % 4);}
		else {printf("          ????");}
		printf("\n");

		past_input_code = code;

		//delay(10);
	}
}
