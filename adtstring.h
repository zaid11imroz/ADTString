#ifndef __ADTSTRING_H_
#define __ADTSTRING_H_


/*============================================================================|
 * Purpose : Dynamic String API.                                              |
 * Codename : adtstring.                                                      |
 * Author : Zaid Imroz.                                                       |
 * Copyrights : Reserved by the Author of the this file. But everyone can     |
                use this file or its contents as and when needed.             |
=============================================================================*/


/*
 * DEFINITION : Every character array with '\0' as its last character
                is a 'string' (Plural : 'strings') with a lowercase 's'.

 * DEFINITION : Every 'String' (Plural : 'Strings') with an uppercase 'S'
                is a data unit which points to some valid 'string' in the memory
                and also stores the length of that 'string'.

 * DEFINITION : Length of a 'string' is the total number of characters
                present in that 'string' (excluding '\0' at the end).

 * NOTE 1 : In this document "String" (with an uppercase 'S') refers to a
            user defined datatype and must not be confused with the
			'string' (with a lowercase 's') which is defined above.

 * NOTE 2 : Every 'string' created in the memory is associated
            to one and only one 'String' i.e. no two 'Strings' can point
			to the same 'string' in the memory.
            This will be referred to as the 'uniqueness' property.

 * NOTE 3 : Using assignment operator on 'Strings' will cause two "Strings"
            to get associated to the same 'string' and is in the violation of
            the 'uniqueness' property.

            If the 'uniqueness' is violated, this API can no longer
            guarantee the consistency of data it's meant to handle.

            As an analogy, consider a 'string' to be a tooth brush and two
            people ('Strings') with the same tooth brush is a bad idea.
            A disease from one could spread to the other plus it's creapy.

 * NOTE 4 : A violation of 'uniqueness' is tolerable iff
            the intent of the assignment is to associate the 'string' pointed
			by the 'String' on right hand side to the 'String' on the left hand
			side. and the "String" on the right hand side
			is guaranteed to never interfere with the 'String'
			on the left hand side (for instance a 'Strring' returned by a
			function call can be assigned to another "String" OR a 'String'
			which is going to point to some other 'string' can be assigned to
			another 'String').

 * NOTE 5 : The 'Strings' created on the stack and the 'Strings' created
            on the heap, both have 'strings' associated to them
			that are created on the heap.

            They must be freed before end of scope, using an appropriate
			function provided in the API to avoid dangling pointers.
			One exception to this is 'NaS' which is not needed to be freed anywhere
			int the program.

			'NaS' is defined to be associated with no 'string' at all.
*/


#include <stdio.h>
#include <stdlib.h>


//START: MACRO DEFINITION SECTION.
/*
 *'NaS' is a special and the only 'String'
  which is not associated to any 'string' at all.

 *Every 'String' must be set to 'NaS' initially using "DS_new_String()" function.
  "DS_create_String_array()" uses "DS_new_String()" function to set members of
  'String' array to 'NaS'.

 *Every failed operation by any function
  except 'DS_create_String_array()', is also indicated by
  returning 'NaS'.

 *Every operation with "NaS" as its operand will yield "NaS"
  as a result except 'DS_is_valid_String()' function.
*/
# define NaS ((String) {NULL, 0}) //Macro expantion for 'NaS'.
//END: MACRO DIFINITION SECTION.


//START : GLOBAL VARIABLE SECTION.
//Start : Definition of String.
typedef struct String
{
	char *data; //Points to the 'string' on the heap.
	size_t length; //length of 'string' (excluding '\0' at the end).
} String; //string is an Alias of typedef struct string OR 'String'.
//End : Definition of String.
//END : GLOBAL VARIABLE SECTION.


//START : FUNCTION DECLARATION SECTION.
//Start : Creator function signatures.
String* DS_create_String_array(size_t);
String DS_new_String();
String DS_create_empty_String();
String DS_get_String();
String DS_get_line();
String DS_init_String(char []);
//End : Creator function signatures.

//Start : Transformer function signatures.
void DS_reset_String(String*);
String DS_concat_String(String, String);
String DS_clone_String(String);
String DS_reverse_String(String);
int DS_String_to_int(String);
int DS_String_to_char(String, size_t);
double DS_String_to_float(String);
//End : Transformer function signatures.

//Start : Reporter function signatures.
int DS_compare_String(String, String);
int DS_is_NaS(String);
int DS_is_empty_String(String);
int DS_is_identical_String(String, String);
void DS_print_String(String);
void DS_print_line(String);
int DS_String_is_int(String);
int DS_String_is_char(String);
int DS_String_is_float(String);
//End : Reporter function signatures.
//END OF FUNCTION DECLARATION SECTION.


//START : FUNCTION DEFINITION SECTION.
//Start : Definiton of creator functions.
String* DS_create_String_array(size_t size)
/*
 *Creates a new 'String' array on heap.
 *Array is of size 'size'.
 *'size' must be an integer greater than '0' and smaller than 'INT_MAX'.
 *Every string in the array is set to 'NaS'.
 *Returns the starting address of the array.
 *If not enough memory, or 'size' <= '0', will return 'NULL' instead.
*/
{
	//Arrauy with size = 0 does not exit.
	if(0 == size)
		return NULL;

	//Allocating memory on heap for 'String' array of given size.
	String *s = malloc(size * sizeof(String));

	//if out of memory then return 'NULL'.
	if(NULL == s)
		return NULL;

	//if allocated memory then initialize all 'Strings' to 'NaS'.
	for(int i = 0; i < size; ++i)
		s[i] = DS_new_String();

	//All set, at this point, return 'String' pointer 's'.
	return s;
}

String DS_new_String()
/*
 *Can set any string to 'NaS' by returning 'NaS'.
*/
{
	return NaS;
}

String DS_create_empty_String()
/*
 *Creates a valid but empty string on heap and returns
  the string
 *An empty string contains only '\0' as a character.
 *If failsed due to lack of memory, returns 'NaS' instead.
*/
{
	String s = DS_new_String();

	//Allocate '1' byte and set it to '\0'.
	s.data = malloc(1);

	//If alloction failed, return 'NaS'.
	if(NULL == s.data)
		return NaS;

	//If sucessful, set 's.data' to an empty string.
	s.data[0] = '\0';

	return s;
}

String DS_get_String()
/*
 *Reads an input from 'stdin' untill 'EOF' is encountered.
 *Stores the input in a dynamically expanding string.
 *Returns the string.
 *If not enough memory for the string returns 'NaS' instead.
*/
{
	String s = DS_new_String();

	//Setting initial limit on the number of characters in the array 's.data'.
	size_t limit = 256;

	//Allocating memory for the array 's.data' on heap.
	s.data = malloc(limit);

	//If allocation failed then return NaS.
	if(NULL == s.data)
		return NaS;

	//taking input using getchar untill 'EOF' is encountered.
	int c;
	while((c = getchar()) != EOF)
	{
		//Placing at indices starting from '0' in 's.data'.
		s.data[s.length] = (char)c;
		++s.length;

		//Checking if 's.length' has reached last index i.e. "limit - 1"
		//of the array 's.data'.
		if((limit - 1) == s.length)
		{
			//Doubling the limit for next input(s).
			limit = limit<<1;

			//Reallocating the array 's.data' with new size 'limit * 2'
			//to some new location on heap.
			s.data = realloc(s.data, limit);

			//If failed then return 'NaS'.
			if(NULL == s.data)
				return NaS;
		}
	}

	//At this point it is guaranteed that 's.length < (limit - 1)'.
	//Appending '\0' at the end of the array 's.data'.
	s.data[s.length] = '\0';

	//Reallocating the array 's.data' to a minimum required size
	//which is 's.length + 1'.
	s.data = realloc(s.data, (s.length + 1));

	//Return the newly created string 's' which
	//has a char pointer 's.data' that points a string on heap and
	//a size_t variable containing its length.
	return s;
}

String DS_get_line()
/*
 *Reads an input from 'stdin' untill 'EOF' or newline is encountered.
 *Stores the input in a dynamically expanding string.
 *Returns the string.
 *If not enough memory for the string returns 'NaS' instead.
*/
{
	String s = DS_new_String();

	//Setting initial limit on the number of characters in the array 's.data'.
	size_t limit = 256;

	//Allocating memory for the array s.data on heap.
	s.data = malloc(limit);

	//If allocation failed then return NaS.
	if(NULL == s.data)
		return NaS;

	//taking input using getchar untill 'EOF' or newline is encountered.
	int c;
	while(((c = getchar()) != EOF) && (c != '\n'))
	{
		s.data[s.length] = (char)c;
		++s.length;

		//Checking if s.length has reached last index i.e. "limit - 1"
		//of the array 's.data'.
		if((limit - 1) == s.length)
		{
			//Doubling the limit for next input(s).
			limit = limit<<1;

			//Reallocating the array 's.data' with new size 'limit * 2'
			//to some new location on heap.
			s.data = realloc(s.data, limit);

			//If failed then return 'NaS'.
			if(NULL == s.data)
				return NaS;
		}
	}

	//At this point it is guaranteed that 's.length < (limit - 1)'.
	//Appending '\0' at the end of the array 's.data'.
	s.data[s.length] = '\0';

	//Reallocating the array 's.data' to a minimum required size
	//which is 's.length + 1'.
	s.data = realloc(s.data, (s.length + 1));

	//Return the newly created string 's' which
	//has a char pointer s.data that points a string on heap and
	//a size_t variable containing its length.
	return s;
}

String DS_init_String(char c[])
/*
 *Takes a string literal as an input.
 *creates a new string on heap.
 *Initialize the string to the given input.
 *Returns the newly created string.
 *If out of memory, returns 'NaS' instead.
*/
{
	String s = DS_new_String();

	//Setting initial limit on the number of characters in the array 's.data'.
	size_t limit = 256;

	//Allocating memory for the array s.data on heap.
	s.data = malloc(limit);

	//If allocation failed then return NaS.
	if(NULL == s.data)
		return NaS;

	//tracersing array 'c' untill '\0' is encountered.
	int i = 0;
	while('\0' != c[i])
	{
		s.data[s.length] = c[i];
		++s.length;
		++i;

		//Checking if s.length has reached last index i.e. "limit - 1"
		//of the array 's.data'.
		if((limit - 1) == s.length)
		{
			//Doubling the limit for next input(s).
			limit = limit<<1;

			//Reallocating the array 's.data' with new size 'limit * 2'
			//to some new location on heap.
			s.data = realloc(s.data, limit);

			//If failed then return 'NaS'.
			if(NULL == s.data)
				return NaS;
		}
	}

	//At this point it is guaranteed that 's.length < (limit - 1)'.
	//Appending '\0' at the end of the array 's.data'.
	s.data[s.length] = '\0';

	//Reallocating the array 's.data' to a minimum required size
	//which is 's.length + 1'.
	s.data = realloc(s.data, (s.length + 1));

	//Return the newly created string 's' which
	//has a char pointer s.data that points a string on heap and
	//a size_t variable containing its length.
	return s;
}
//End : Definition of creator functions.

//Start : Definition of transformer functions.
void DS_reset_String(String *s)
/*
 *Takes address of a string as an argument.
 *Frees the memory occupied by the string.
 *Sets the string to 'NaS'.
*/
{
	//If pointer 's' points to nothng at all.
	if(NULL == s)
		return;

	//If '*s' is NaS.
	if(DS_is_NaS(*s))
		return;

	//If pointer 's' points to some valid string.
	free(s->data);
	(*s) = DS_new_String();
}

String DS_concat_String(String one, String two)
/*
*Takes two strings as arguments.
*If any of the two strings is 'NaS', returns 'NaS'.
*If both strings are valid but empty strings, returns an empty string.
*Else creates a new string,
 copies the data of both the strings in sequence from left to right,
 returns the string.
 *If out of memory, returns 'NaS' instead.
*/
{
	//Checking if any string is 'NaS'.
	if((DS_is_NaS(one)) || (DS_is_NaS(two)))
		return NaS;

	String s = DS_new_String();

	//Checking if any string is an empty string.
	if(0 != (one.length + two.length))
	{
		//If atleast one string is non empty then.
		//Allocating memory for character array 's.data'.
		s.data = malloc((one.length + two.length + 1));

		//If allocation failed then return 'NaS'.
		if(NULL == s.data)
		 	return NaS;

		//Copy both character arrays 'one.data' & 'two.data'
		//in 's.data' one by one.
		int i;

		//Copy all characters from 'one.data' to 's.data'.
		for(i = 0; i < one.length; ++i)
			s.data[i] = one.data[i];

		//Copy all characters from 'two.data' to 's.data'
		//starting from index 'one.length' as
		//'one.length - 1' was the last index occupied by
		//last character (before '\0') of 'one.data'.
		for(i = 0; i < two.length; ++i)
			s.data[i + one.length] = two.data[i];

		//Setting length of 's.data'.
		s.length = one.length + two.length;

		//Append '\0' to 's.data' on the last index
		//i.e. 'one.length + two.length'.
		s.data[s.length] = '\0';

		return s;
	}

	//If both strings are empty.
	return DS_create_empty_String();
}

String DS_clone_String(String from)
{
	//If the string 'fomr' is 'NaS', return 'NaS'.
	if(DS_is_NaS(from))
		return NaS;

	//If the string 'from' is an empty string,
	// return empty string.
	if(DS_is_empty_String(from))
		return DS_create_empty_String();

	String s = DS_new_String();

	s.data = malloc(from.length + 1);

	if(NULL == s.data)
		return NaS;

	int i;
	for(i = 0; i < from.length; ++i)
		s.data[i] = from.data[i];

	s.data[from.length] = '\0';
	s.length = from.length;

	return s;
}

String DS_reverse_String(String s)
{
	if(DS_is_NaS(s))
		return NaS;

	if(DS_is_empty_String(s))
		return DS_create_empty_String();

	String str = DS_new_String();

	str.data = malloc(s.length + 1);

	if(NULL == str.data)
		return NaS;

	int i;
	for(i = 0; i < s.length; ++i)
		str.data[i] = s.data[s.length - 1 - i];

	return str;
}

int DS_String_to_int(String s)
{
	if(DS_is_NaS(s))
		return 0;

	return atoi(s.data);
}

int DS_String_to_char(String s, size_t index)
{
	if(DS_is_NaS(s))
		return 0;

	if(index >= s.length)
		return -1;

	return s.data[index];
}

double DS_String_to_float(String s)
{
	if(DS_is_NaS(s))
		return 0.0;

	return atof(s.data);
}
//End : Definition of transformer functions.

//Start : Definition of reporter functions.
int DS_compare_String(String one, String two)
{
	int i, n;
	(one.length < two.length) ? (n = one.length) : (n = two.length);
	for(i = 0; i < n; ++i)
	{
		if(one.data[i] == two.data[i])
			continue;

		if(one.data[i] < two.data[i])
			return 1;

		return 2;
	}
	if(one.length < two.length)
		return 1;

	if(one.length == two.length)
		return 0;

	return 2;
}

int DS_is_NaS(String s)
{
	//If string is 'NaS', return false.
	if(NULL == s.data && 0 == s.length)
		return 1;

	//If string 's' is not 'NaS', return true .
	return 0;
}

int DS_is_empty_String(String s)
{
	if((0 == s.length) && (NULL != s.data))
		return 1;

	return 0;
}

int DS_is_identical_String(String one, String two)
{
	if(one.length != two.length)
		return 0;

	int i;
	for(i = 0; i < one.length; ++i)
		if(one.data[i] != two.data[i])
			return 0;

	return 1;
}

void DS_print_String(String s)
{
	if(DS_is_NaS(s))
		return;

	for(int i = 0; i < s.length; ++i)
		printf("%c", s.data[i]);

	if(0 == s.length)
		printf("NULL");
}

void DS_print_line(String s)
{
	DS_print_String(s);
	printf("\n");
}

int DS_String_is_int(String s)
{
	int state = 1, i, is_digit;
	for(i = 0; i < s.length; ++i)
	{
		if(state == 0)
			return 0;

		is_digit = (s.data[i] >= '0' && s.data[i] <= '9');
		switch(state)
		{
			case 1 :
				switch(s.data[i])
				{
					case '+' :
					case '-' :
						state = 2;
						continue;

					default :
						if(is_digit)
						{
							state = 3;
							continue;
						}

						state = 0;
						continue;
				}

			case 2 :
				if(is_digit)
				{
					state = 3;
					continue;
				}

				state = 0;
				continue;

			case 3 :
				if(is_digit)
					continue;

				state = 0;
		}
	}

	if(state == 3)
		return 1;

	return 0;
}

int DS_String_is_char(String s)
{
	if((s.length == 0) || (s.length == 1))
	{
		return 1;
	}
	return 0;
}

int DS_String_is_float(String s)
{
	int state = 1, i, is_digit;
	for(i = 0; i < s.length; ++i)
	{
		if(state == 0)
		{
			return 0;
		}
		is_digit = ((s.data[i] >= '0') && (s.data[i] <= '9'));
		switch(state)
		{
			case 1 :
				switch(s.data[i])
				{
					case '+' :
					case '-' :
						state = 2;
						continue;
					case '.' :
						state = 4;
						continue;
					default :
						if(is_digit)
						{
							state = 3;
							continue;
						}
						state = 0;
						continue;
				}
			case 2 :
				if(is_digit)
				{
					state = 3;
					continue;
				}
				if(s.data[i] == '.')
				{
					state = 4;
					continue;
				}
				state = 0;
				continue;
			case 3 :
				switch(s.data[i])
				{
					case '.' :
						state = 4;
						continue;
					case 'e' :
					case 'E' :
						state = 6;
						continue;
					default :
						if(is_digit)
						{
							continue;
						}
						state = 0;
						continue;
				}
			case 4 :
				if(is_digit)
				{
					state = 5;
					continue;
				}
				state = 0;
				continue;
			case 5 :
				switch(s.data[i])
				{
					case 'e' :
					case 'E' :
						state = 6;
						continue;
					default :
						if(is_digit)
						{
							continue;
						}
						state = 0;
						continue;
				}
			case 6 :
				switch(s.data[i])
				{
					case '+' :
					case '-' :
						state = 7;
						continue;
					default :
						if(is_digit)
						{
							state = 8;
							continue;
						}
						state = 0;
						continue;
				}
			case 7 :
				if(is_digit)
				{
					state = 8;
					continue;
				}
				state = 0;
				continue;
			case 8 :
				if(is_digit)
				{
					continue;
				}
				state = 0;
		}
	}
	if((state == 4) || (state == 5) || (state == 8))
	{
		return 1;
	}
	return 0;
}
//End : Definition of reporter functons.
//END : FUNCTION DEFINITION SECTION.

#endif
