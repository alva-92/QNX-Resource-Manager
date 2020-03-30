/**
 * Description This program is the server to which the
 *             myDevice program sends requests to
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#define MY_PULSE_CODE   _PULSE_CODE_MINAVAIL
#define ATTACH_POINT    "mydevice"

typedef union
{
	struct _pulse   pulse;
	char msg[255];
} my_message_t;

void status_check(){

	FILE *fp;
	int c;
	fp = fopen("/dev/local/mydevice", "r");
	if (fp == NULL)
	{
		printf("Failed to check device status");
		return;
	}
	else
	{
		   while(1) {
			  c = fgetc(fp);
			  if( feof(fp) ) {
				 break;
			  }
			  printf("%c", c);
			  // IF VALUE == STATUS
			  	  // PRINT VALUE
			  // IF VALUE == CLOSED
			  	  // name_detach
			  	  // EXIT
			  // Close device by its fds
		   }
		   fclose(fp);
	}

}

int main() {
	name_attach_t *attach;
	my_message_t msg;
	int rcvid;

	/* Create a local name to register the device */
	if ((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL)
	{
		printf("Failed to attach to device");
		return EXIT_FAILURE;
	}


	status_check();


	while(1)
	{
	       rcvid = MsgReceivePulse(attach->chid, &msg, sizeof(msg), NULL);

	       if (rcvid == -1) {/* Error condition, exit */
	    	   printf("Failed to receive message");
	           break;
	       }


	       if (rcvid == 0) {/* Pulse received */
	           switch (msg.pulse.code) {
	           case MY_PULSE_CODE:
	        	   printf("Small Integer: %d\n", msg.pulse.value.sival_int);
	        	   status_check();
	               break;
	           default:
	               /*
	                * A pulse sent by one of your processes or a
	                * _PULSE_CODE_COIDDEATH or _PULSE_CODE_THREADDEATH
	                * from the kernel?
	                */
	        	   printf("Not the expected code");
	               break;
	           }
	           continue;
	       }

	}

	return EXIT_SUCCESS;
}
