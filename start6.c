#include <stdio.h>
#include "devApi3.h"
char a[] = { 0x00, 0x24, 0x00, 0x01, 0x03, 0xc1, 0xc2, 0xc3, 
	0x0b, 0x08,
	     0x0a, 0x02, 0xb1, 0xb2,
		0x0b, 0x02, 0xb1, 0xb2,
	0x00, 0x02, 0xc1, 0xc2,
	0x04, 0x02, 0xc1, 0xc2,
	0x01, 0x08,
	     0x0a, 0x02, 0xb1, 0xb2,
		0x0b, 0x02, 0xb1, 0xb2,};
int shift, count, i, j;
void *val;
int 
main (void) 
{
	printf ("%x \n", sizeof (a));
	
	printf("It`s work!\n");
	shift = 0;
	shift = 0;
	i=tlv_find( a, 0x00, &shift, 0x0b, &val, &count);
	printf (" shift = %d, i = %d, count = %d \n", shift, i, count);
	printf (" Value of key 0x13 is : ");
	for (j = 0; j < count; j++) printf (" %x,", *(char*)(val+j)); 
	printf (" \n");
	printf ("count of key 0x03 = %d \n", tlv_count ( a, 0, 0x01));
	printf ("count of key 0x03 = %d \n", tlv_count ( val, count, 0x0a));
}
	 