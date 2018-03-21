

#include <stdint.h>
#include <pic32mx.h>



void preset_time(void){
	int tensec = 1;
	char* minutestring;
	while(1){
		if(getbtns() == 0x1) break;
		else if(getbtns() == 0x2){
			if(tensec > 0) tensec--;
		}
		else if(getbtns() == 0x4){
			if(tensec < 10)tensec++;
		}
		if(tensec != 10) minutestring = tensec + '0';
		else if(tensec == 10) minutestring = "10";
		display_string(3, minutestring);
		
		display_update();
		delay(1000);
	}
	set_time(tensec, minutestring);
}
