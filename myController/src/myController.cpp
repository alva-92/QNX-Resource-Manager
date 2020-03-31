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

int status_check(){

	FILE *fp;
	char first[255];
	char second[255];
	memset( &first, 0, sizeof(255));
	memset( &second, 0, sizeof(255));
	fp = fopen("/dev/local/mydevice", "r");
	if (fp == NULL)
	{
		printf("Failed to check device status");
		return 1;
	}
	else
	{
		fscanf(fp, "%s %s",first, second);

		if (strcmp("",second)!=0){
			printf("Status: %s\n", second);
		}

		if (strcmp("closed", second) == 0)
		{
			return 1;
		}
		fclose(fp);
		return 0;
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


	if (status_check() != 0){
		/* Remove the name from the space */
		name_detach(attach, 0);
	}


	while(1)
	{
	   rcvid = MsgReceivePulse(attach->chid, &msg, sizeof(msg), NULL);

	   if (rcvid == -1) /* Error condition, exit */
	   {
		   printf("Failed to receive message");
		   break;
	   }


	   if (rcvid == 0) /* Pulse received */
	   {
		   switch (msg.pulse.code)
		   {
			   case MY_PULSE_CODE:
				   printf("Small Integer: %d\n", msg.pulse.value.sival_int);
					if (status_check() != 0)
					{
						/* Remove the name from the space */
						name_detach(attach, 0);
						exit(0);
					}
				   break;
			   default:
				   printf("Not the expected code");
				   break;
			   }
		   continue;
	   }

	}

	return EXIT_SUCCESS;
}
