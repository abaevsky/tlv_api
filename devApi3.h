/*
Internal function. She does not give out
*/

inline static int 
tlv_next (void *data, int *shift, char *key, void **value, int *count)
{
	char key_in = *key;
	void *k;
	int i, j;
	
	j = *shift;
	*key = *(char *) (data + *shift);
	k = data + *shift + 1;
	
	if (( *(char *) k ) & 0x80) 
	{
		i = (( *(char *) k) & 0x7f);
		*count = (i << 8) | ( *(char *) (k + 1));
		*value = k + 2;
		*shift += *count + 3;
	}else
	{
		*count = *(char*) k;
		*value = k + 1;
		*shift += *count + 2;
	}	
	
	if ( *((char *) (data + j)) == key_in) 
		return 1;
	else 
		return 0;	
};

/*
This function looks for the filed of type  "key", beginning at offset "offset"
If such a field is, the function returns 1. If not - 0
In addition, the function returns the value of the field and its size
The arguments:

data - a pointer to the TLV structure.
	Can be both external and internal

size - the size of the internal TLv structure. 
	For external structure must be set to 0

offset - the absolute offset of the search. 
	The first time is set to 0, the value returned by the subsequent use

key - Type the required field

value - Returns a pointer to the value of the field

length - The return value of the field size. 
*/

inline static int 														
tlv_find (void *data, int size, int *offset, char key, void **value, int *length)                  
{                                                                                                                                    
	int ln;  
	char tmp;
	
	if (size == 0)
	{                                                                                                        
		ln = *(unsigned short *) (data + 1);                                                   
		if (*offset == 0) 
			*offset = 3;                                                                                
	} else                                                                                                                     
		ln = size;    
	
	while (1)                                                                                                               
	{
		tmp = key;
		if (ln < *offset  + 2) 
			return 0; 
		if ( tlv_next(data, offset, &tmp, &*value, length)) 
			return 1; 
	};
};

/*

This function returns the number of fields of type "key",
or zero if no such field.

data - a pointer to the TLV structure.
	Can be both external and internal.

size - the size of the internal TLv structure. 
	For external structure must be set to 0

key - Type the required field

*/


inline static int 
tlv_count (void *data, int size, char key) 
{
	int offset = 0, j = 0;
	int i, c, length;
	char temp;
	void *value;
	
	if (size == 0) 
	{
		length = * (unsigned short *) (data + 1);
		offset = 3;
	} else 
		length = size;
	
	while (length > offset + 2) 
	{
		temp = key;
		i = tlv_next (data, &offset, &temp, &value, &c);
		if (i) j++;
	};
	
	return j;
};

/*

This function prints field TLV records one by one, 
without any conditions. Returns 1 if the field is available, 
or 0 if the limit is reached.

data - a pointer to the TLV structure.
	Can be both external and internal.

size - the size of the internal TLv structure. 
	For external structure must be set to 0

offset - the absolute offset of the search. 
	The first time is set to 0, the value returned by the subsequent use

key - Returns the type of the field. The input is does not matter

value - Returns a pointer to the value of the field

length - The return value of the field size. 

*/



inline static int
tlv_list (void *data, int size, int *offset, char *key, void **value, int *length)     
{
	int ln;      
	
	if (size == 0) {                                                                                                        
		ln = *(unsigned short *) (data + 1);                                                   
		if (*offset == 0) 
			*offset = 3;       
	} else                                                                                                                     
		ln = size; 
	
	if (ln < *offset +2)
		return 0;
	tlv_next (data, offset, key, &*value, length);
	return 1;	
}



	