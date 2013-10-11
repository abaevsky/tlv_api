#include <stdio.h>
#include "tlv.h"

/*
**************************************************
This example of the second space EEPROM
**************************************************
*/
char a[] = { 0x00, 0x89, 0x00,
	0x03, 0x05, 
		0xc0, 0xc1, 0xc2, 0xc3, 0xc4,
	
	0x01, 0x04, 
		0xd1, 0xb2, 0xa3, 0xc4,
	
	0x04, 0x08,
		0x01, 0x02, 
			0xab, 0xbc,
		0x03, 0x02,
			0x01, 0x02,
	
	0x02, 0x80,
		0x14, 0xc1, 0xc2,0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca,
	
	0x13, 0x24 , 
	
		0x03, 0x05, 
			0xc0, 0xc1, 0xc2, 0xc3, 0xc4,
	
		0x01, 0x04, 
			0xc1, 0xc2, 0xc3, 0xc4, 
	
		0x02, 0x80, 0x14, 
			0xbb, 0xc2,0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xc1, 0xc2, 0xc3, 0xc4, 0x3c5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 
	
	0x03, 0x02, 
		0xc1, 0xc2, 
		
	0x04, 0x08,
		0xba, 0x02, 
			0x2c, 0x2b,
		0x13, 0x02,
			0x01, 0x02,	
	
	0x07, 0x02, 
		0xc1, 0xc2,
		
	0x03, 0x05, 
		0xc0, 0xc1, 0xc2, 0xc3, 0xc4,
		
	0x03, 0x02, 
		0xc1, 0xc2, 
		
	0x04, 0x08,
		0x05, 0x02, 
			0xcb, 0xcc,
		0x07, 0x02,
			0x21, 0x22,
	
	0x07, 0x02, 
		0xd1, 0xd2,
		
	0x03, 0x05, 
		0xc0, 0xc1, 0xc2, 0xc3, 0xc4};
/*	
this variable will be used to store the key of the field		
*/
unsigned char key;

/*
This variable is the absolute offset of the search. 
The first time is set to 0, the value returned by the subsequent use		
*/		
int offset;		

/*
A pointer to the TLV structure.
Can be both external or internal
*/
void *pointer_area;		
		
/*
The size of the internal TLv structure. 
For external structure must be set to 0	
*/		
int size_area;

/*
In this variable returns a pointer to the value of the field
*/
void *pointer_value;

/*
In this variable returns the size of value of the field
*/
int size_value;


int i;


/*
***********************************************************
This is suport function. It displays the data field
***********************************************************
*/


void
field_info (void) 
{
	int i, j = 30;
	printf (" ++ key %x ++ length of value %d ++\n", key, size_value);
	printf (" +++ value is +++ \n");
	if (size_value < j)
		j = size_value;
	for (i = 0; i < j; i++)
		printf ("%x ", *(unsigned char *) (pointer_value + i));
	if (j == size_value) 
		printf("\n");
	else 
		printf("... \n");		
}
		
		





int 
main (void) 
{
	
/*
***************************************************************
Let's find first fild by key 0x01
Since we are looking for in the external structures, the size of the field is affixed 0
	
We can find the field or no
If the field is found, the function returns 1 and will change output arguments
If the field will not be found, the function returns zero and nothing else will do
	
If that's so, call the display of the result when the 1 returns

**************************************************************
*/	
	pointer_area = a;
	offset = 0;
	size_area = 0;
	key = 0x01;
	
	printf ("Let's find first fild by key 0x01\n");
	if (tlv_find (pointer_area, size_area, &offset, key, &pointer_value, &size_value))
		field_info ();
	printf("\n");
	
/*
****************************************************************************************
Ok. Now let's find first fild by key 0x02
This field is remarkable in that it has a long record format size of value.
If we want to search from the beginning of the recording, we have to put down the offset to zero.
the other parameters remain the same as they are already established and have not changed	
	
****************************************************************************************
*/

	offset = 0;
	key = 0x02;
	
	printf ("Ok. Now let's find first fild by key 0x02\n");
	if (tlv_find (pointer_area, size_area, &offset, key, &pointer_value, &size_value))
		field_info ();
	printf("\n");
	
	
/*
*********************************************************************************************************
Next we will try to look for a field in a loop.
There are many fields with a key 0x03, let's find them.
It does not require any additional steps. Just need to take care to use 
the result of each iteration. In our case, we simply display found on screen
*********************************************************************************************************
*/
	offset = 0;
	key = 0x03;
	
	i=1;
	
	printf ("Next we will try to look for a field in a loop. key 0x03 \n");
	do {
		if (tlv_find (pointer_area, size_area, &offset, key, &pointer_value, &size_value))
			field_info ();
		else
			i = 0;
	} while (i);
	printf("\n");
	
/*
**********************************************************************************************************	
It's good. 	But what if we do not need the values of fields,
and we want to know just how many of these fields?
Let's try another function. This function returns the number 
of fields found on this level. It does not address the embedded structure	
**********************************************************************************************************
*/
	
	i = tlv_count (pointer_area, size_area, key);
	printf ("count of key 0x03 = %d \n\n", i);
			
	
/*
*************************************************************************************************	
There is one useful function - the unconditional listing.
The variable "key" is used to return a value. The input is not important.	
The function will cycle through a field one by one to return the unit. 
If the offset is closer to the end of the structure, the function returns zero, 
and will not make any other changes.	
Otherwise, this function is similar to the search function
Let's scroll through the field of external structures

*******************************************************************************
*/	
	offset = 0;
	
	i=1;
	
	printf ("Let's scroll through the field of external structures\n");
	do {
		if (tlv_list (pointer_area, size_area, &offset, &key, &pointer_value, &size_value))
			field_info ();
		else
			i = 0;
	} while (i);
	printf ("\n");
	
/*
******************************************************************************************
Now we have enough experience with external entities. Let's take a look inside
Working with the internal structures of the dispensation as well as with external.
The only difference - you need to specify the size of the area.
Let's find a field with a key 0x13 and list its contents
******************************************************************************************
*/
	key = 0x13;
	offset = 0;
	
	printf ("Let's find a field with a key 0x13 and list its contents \n");
	if (tlv_find (pointer_area, size_area, &offset, key, &pointer_value, &size_value)) {
		field_info ();
		pointer_area = pointer_value;        /* just pass pointer at the beginning of the found area*/
		size_area = size_value;                   /*  and its size. */
		i=1;
		
		offset = 0;	  /* It's important */
		do {
			if (tlv_list (pointer_area, size_area, &offset, &key, &pointer_value, &size_value))
				field_info ();
			else
				i = 0;
		}while (i);
	}
	printf("\n");
/*
******************************************************************************************
Search in the internal structures arranged in exactly the same.
We have already established a pointer to the value of the field 13 and indicated its size.		
Let's find inside the field 0x02		
******************************************************************************************
*/		
		
	key = 0x02;
	offset = 0;
	
	printf ("Let's find inside the field 0x13 key 0x02	\n");
	if (tlv_find (pointer_area, size_area, &offset, key, &pointer_value, &size_value))
		field_info ();	
	printf ("\n");
/*
*************************************************************************************************	
Now we use all the features of our library.
Caution: these functions are not able to control the accuracy of TVL structures
Attempt to divide the indivisible value of the field can lead to an error of memory. 
Responsibility for proper use is on the side of the call
************************************************************************************************
*/
}
