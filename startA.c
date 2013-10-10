#include <stdio.h>
#include "devApi3.h"
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
	
		0x02, 0x80, 
			0x14, 0xc1, 0xc2,0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xc1, 0xc2, 0xc3, 0xc4, 0x3c5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 
	
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

char buff[100];

		
				
typedef struct {		
	void *data;
	int size;
	int offset;
} transfert;		

transfert set[20], tmp;
int lvl = 0, moffset;
unsigned char repeat= 1, r1 = 1, r2 = 1, tkey = 0, skey = 0;

void
field_info (void) 
{
	int i, j = 30;
	printf (" ++ key %x ++ length of value %d ++\n", tkey, tmp.size);
	printf (" +++ value is +++ \n");
	if (tmp.size < j)
		j = tmp.size;
	for (i = 0; i < j; i++)
		printf ("%x ", *(unsigned char *) (tmp.data + i));
	if (j == tmp.size) 
		printf("\n");
	else 
		printf("... \n");		
}
		
		
int 
main (void) 
{
	tmp.offset = 0;
	tmp.data = a;
	tmp.size = 0;
	set[0] = tmp;
	do
	{
		printf ("\n You are on the %dnd level \n", lvl);
		printf ("Possible actions: 1 -  up lvl; 2 - down lvl; 3 - next \n");
		printf ("4 - find new; 5 - find next; 6 - loop through the opening \n");
		printf ("Q or q - to quit \n");
		fgets (buff, sizeof (buff), stdin);
		//printf("here is %d \n", buff[0]);
		switch (buff[0])
		{
			case 49:
				if (lvl <= 0) 
					printf ("You are at the top and can`t rise above \n");
				else
					lvl--;
					tmp.offset = 0;
				break;
			case 50:
				if (tmp.data == set[lvl].data)
					printf ("First you have to select the field \n");
				else
				{
					lvl++;
					set[lvl] = tmp;
					tmp.offset = 0;
				}
				break;
			case 51:
				printf("\n");
				if (tlv_list (set[lvl].data, set[lvl].size, &tmp.offset, &tkey, &tmp.data, &tmp.size))
					field_info();
				else
				{
					printf("No more fileds there. You moved to the beginning of the current lvl \n");
					tmp.offset = 0;
				}
				break;
			case 52:
				tmp.offset = 0;
				printf("Input the key ");
				fgets (buff, sizeof (buff), stdin);
				if ((buff[0] == 48) && (buff[1] == 120))
				{
					if (buff[2] < 60) 
						tkey = buff[2] - 48;
					else
						tkey = buff[2] - 87;
					tkey = (tkey << 4);
					if (buff[3] < 60) 
						tkey += buff[3] - 48;
					else
						tkey += buff[3] - 87;
				} else
					switch (sizeof (buff))
					{
						case 1:
							tkey = buff[0] - 48;
							break;
						case 2: 
							tkey = (buff[1] - 48) + (buff[0] - 48) * 10;
							break;
						default:
							tkey =  (buff[2] - 48) + (buff[1] - 48) * 10 + (buff[1] - 48) * 100;
							break;
					};
	
				if (tlv_find(set[lvl].data, set[lvl].size, &tmp.offset, tkey, &tmp.data, &tmp.size))
					field_info();
				else
				{
					printf("No more fileds by key %x there. You moved to the beginning of the current lvl \n", tkey);
					tmp.offset = 0;
				}
				break;
			case 53:
				printf("\n");
				if (tlv_find(set[lvl].data, set[lvl].size, &tmp.offset, tkey, &tmp.data, &tmp.size))
					field_info();
				else
				{
					printf("No more fileds by key %x there. You moved to the beginning of the current lvl \n", tkey);
					tmp.offset = 0;
				}
				break;
			case 54:
				tmp.offset = 0;
				printf("Input the key ");
				fgets (buff, sizeof (buff), stdin);
				if ((buff[0] == 48) && (buff[1] == 120))
				{
					if (buff[2] < 60) 
						tkey = buff[2] - 48;
					else
						tkey = buff[2] - 87;
					tkey = (tkey << 4);
					if (buff[3] < 60) 
						tkey += buff[3] - 48;
					else
						tkey += buff[3] - 87;
				} else
					switch (sizeof (buff))
					{
						case 1:
							tkey = buff[0] - 48;
							break;
						case 2: 
							tkey = (buff[1] - 48) + (buff[0] - 48) * 10;
							break;
						default:
							tkey =  (buff[2] - 48) + (buff[1] - 48) * 10 + (buff[1] - 48) * 100;
							break;
					};
				r1 = 1;
				skey = tkey;	
				do 
				{
					printf ("\n");
					printf("offset = %d \n", tmp.offset);
					if (tlv_find (set[lvl].data, set[lvl].size, &tmp.offset, tkey, &tmp.data, &tmp.size))
					{
						printf("offset = %d \n", tmp.offset);
						moffset = tmp.offset;
						field_info ();
						lvl++;
						set[lvl] = tmp;
						tmp.offset = 0;
						printf ("++++++++++++++This fild contains +++++++++++\n");
						printf ("++++++++++++++++++++++++++++++++++++\n");
						r2 = 1;
						do
						{
							if (tlv_list (set[lvl].data, set[lvl].size, &tmp.offset, &tkey, &tmp.data, &tmp.size))
								field_info();
							else
							{
								r2 = 0;
								tkey = skey;
								tmp.offset = moffset;
							}
						} while (r2);
						printf ("+++++++++++++++++++++++++++++++++++++\n");
						lvl--;
					} else
					{
						printf ("No more fileds by key %x there. You moved to the beginning of the current lvl \n", tkey);
						r1 = 0;
					}
				}while (r1);
				tmp.offset = 0;
				break;
			case 81:
				repeat = 0;
				break;
			case 113:
				repeat = 0;
				break;
		}
	} while (repeat);	
}
