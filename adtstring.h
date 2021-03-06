#ifndef __ADTSTRING_H_
#define __ADTSTRING_H_


/*============================================================================|
 * Purpose : C API for Dynamic Strings.                                       |
 * Codename : adtstring.                                                      |
 * Author : Zaid Imroz.                                                       |
 * Copyrights : Reserved by the Author. But anyone can use this file          |
  				or its contents as and when needed.	      |
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
			'string' (with a lowercase 's') which is as defined above.

 * NOTE 2 : Every 'string' created in the memory is associated
            to one and only one 'String' i.e. no two 'Strings' must point
			to the same 'string' in the memory.
            This will be referred to as the 'uniqueness' property.

 * NOTE 3 : Using assignment operator on 'Strings' will cause two "Strings"
            to get associated to the same 'string' and is a violation of
            the 'uniqueness' property.

            If the 'uniqueness' is violated, this API can no longer
            guarantee the consistency of data it's meant to handle and in the
			worst case this can lead to memory leakage.

            As an analogy, consider a 'string' to be a tooth brush and two
            people ('Strings') with the same tooth brush is a bad idea.
            A disease from one could spread to the other plus it's creapy.

 * NOTE 4 : The 'Strings' created at compile time and the 'Strings' created
            at rumtime, both have 'strings' associated to them
			that are created at runtime.

            They must be freed before end of the scope in which they are
			defined, using an appropriat function provided in this API
			to avoid dangling pointers and memory leakage.

			One exception to this rule is 'NaS' which is not needed to be
			freed anywhere in the program. 'NaS' stands for 'Not a String'
			and is a used as a 'sentinel String' to indicate return failures
			and to maintain consistency.

			'NaS' is designed to be associated with no 'string' at all.
*/


#include <stdio.h>
#include <stdlib.h>


//START: MACRO DEFINITION SECTION.
/*
 *'NaS' is a special and the only 'String'
  which is not associated to any 'string' at all.

 *Every 'String' must be set to 'NaS' initially using "DS_new_String()"
  function. This is mandatory and must be done at the time of declaration of
  a 'String'.
	e.g.: String str = DS_new_String();

 *Every operation with "NaS" as its operand will yield "NaS"
  as a result except 'DS_is_valid_String()' function.
*/
# define NaS ((String) {NULL, 0}) //Macro expantion for 'NaS'.
//END: MACRO DIFINITION SECTION.


//START : GLOBAL VARIABLE SECTION.
//Start : Definition of String.
typedef struct String
{
	char *data; //Points to the 'string' at runtime.
	size_t length; //hold length of 'string' (excluding '\0' at the end).
} String; //String is an Alias of 'typedef struct String' a.k.a. 'String'.
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
String DS_clone_String(const String);
//End : Creator function signatures.

//Start : Transformer function signatures.
void DS_reset_String(String*);
String DS_concat_String(const String, const String);
String DS_reverse_String(const String);
int DS_String_to_int(const String);
int DS_String_to_char(const String, size_t);
double DS_String_to_float(const String);
//End : Transformer function signatures.

//Start : Reporter function signatures.
int DS_compare_String(const String, const String);
int DS_is_NaS(const String);
int DS_is_empty_String(const String);
int DS_is_identical_String(const String, const String);
void DS_print_String(const String);
void DS_print_line(const String);
int DS_String_is_int(const String);
int DS_String_is_char(const String);
int DS_String_is_float(const String);
//End : Reporter function signatures.
//END OF FUNCTION DECLARATION SECTION.


//START : FUNCTION DEFINITION SECTION.
//Start : Definiton of creator functions.
String* DS_create_String_array(size_t size)
/*
 *Creates a new 'String' array at runtime.
 *Array is of size 'size'.
 *'size' must be an integer greater than '0'.
 *Every 'String' in the array is set to 'NaS'.
 *Returns the starting address of the 'String' array.
 *If not enough memory, or 'size' <= '0', will return 'NULL' instead.
*/
{
	//Arrauy with size = 0 does not exit.
	if(0 == size)
		return NULL;

	//Allocating memory for 'String' array of given size.
	String *s = malloc(size * sizeof(String));

	//If out of memory then return 'NULL'.
	if(NULL == s)
		return NULL;

	//If memory allocated, then initialize all 'Strings' to 'NaS'.
	for(int i = 0; i < size; ++i)
		s[i] = DS_new_String();

	//All set, at this point, return 'String' array 's'.
	return s;
}

String DS_new_String()
/*
 *Can set any string to 'NaS' by returning 'NaS'.
 *Must be used with assignment operator in code.
*/
{
	return NaS;
}

String DS_create_empty_String()
/*
 *Creates a valid but empty "String" at runtime and returns
  the 'String'.
 *An empty 'String' contains only '\0' as a character in the 'string'
  associated to it.
 *If failed due to lack of memory, returns 'NaS' instead.
*/
{
	String s = DS_new_String();

	//Allocate '1' byte for 'string'.
	s.data = malloc(1);

	//If alloction failed, return 'NaS'.
	if(NULL == s.data)
		return NaS;

	//If sucessful, set '\0' as the only character in the 'string'.
	s.data[0] = '\0';

	return s;
}

String DS_get_String()
/*
 *Reads an input from 'stdin' untill 'EOF' is encountered.
 *Makes a 'String' from the input.
 *Returns the 'String'.
 *If not enough memory returns 'NaS' instead.
*/
{
	String s = DS_new_String();

	//Setting initial limit on the number of characters in the 'string'.
	size_t limit = 256;

	//Allocating memory for the 'string' with size equal to 'limit'.
	s.data = malloc(limit);

	//If allocation failed then return NaS.
	if(NULL == s.data)
		return NaS;

	//Taking input from 'stdin' untill 'EOF' is encountered.
	int c;
	while((c = getchar()) != EOF)
	{
		//Placing at indices starting from '0' in 'string'.
		//s.length was set to '0' by 'DS_new_String()' function initially.
		s.data[s.length] = (char)c; //We can use 's.length++'
		++s.length;

		//Checking if 'length' has reached last index i.e. "limit - 1"
		if((limit - 1) == s.length)
		{
			//Doubling the limit to accomodate remaining input characters.
			limit = limit<<1; //Left Shift operator.

			//Reallocating the 'string' with new size.
			s.data = realloc(s.data, limit);

			//If failed then return 'NaS'.
			if(NULL == s.data)
				return NaS;
		}
	}

	//At this point it is guaranteed that 'length' < '(limit - 1)'.
	//Appending '\0' at the end.
	s.data[s.length] = '\0';

	//Reallocating the 'string' to a minimum required size
	//which is 'Length + 1'.
	s.data = realloc(s.data, (s.length + 1));

	//Return the newly created 'String' that contains the 'string' and 'length'.
	return s;
}

String DS_get_line()
/*
 *Reads an input from 'stdin' untill 'EOF' or newline is encountered.
 *Stores the input in a "String".
 *Returns the 'String'.
 *If not enough memory for the 'String' returns 'NaS' instead.
*/
{
	String s = DS_new_String();

	//Setting initial limit on the number of characters in the 'string'.
	size_t limit = 256;

	//Allocating memory for the 'string' with size equal to 'limit'.
	s.data = malloc(limit);

	//If allocation failed then return NaS.
	if(NULL == s.data)
		return NaS;

	//taking input using getchar untill 'EOF' or 'newline' is encountered.
	int c;
	while(((c = getchar()) != EOF) && (c != '\n'))
	{
		s.data[s.length] = (char)c;
		++s.length;

		//Checking if 'length' has reached last index i.e. "limit - 1"
		if((limit - 1) == s.length)
		{
			//Doubling the limit to accomodate remaining input characters.
			limit = limit<<1; //Left Shift operator.

			//Reallocating the 'string' with new size.
			s.data = realloc(s.data, limit);

			//If failed then return 'NaS'.
			if(NULL == s.data)
				return NaS;
		}
	}

	//At this point it is guaranteed that 'length' < '(limit - 1)'.
	//Appending '\0' at the end.
	s.data[s.length] = '\0';

	//Reallocating the 'string' to a minimum required size
	//which is 'Length + 1'.
	s.data = realloc(s.data, (s.length + 1));

	//Return the newly created 'String' that contains the 'string' and 'length'.
	return s;
}

String DS_init_String(char c[])
/*
 *Takes a 'string' literal as an input.
 *Creates a new 'String'.
 *Stores given input in the 'String'.
 *Returns the newly created 'String'.
 *If out of memory, returns 'NaS' instead.
*/
{
	String s = DS_new_String();

	//Setting initial limit on the number of characters in the 'string'.
	size_t limit = 256;

	//Allocating memory for the 'string' with size equal to 'limit'.
	s.data = malloc(limit);

	//If allocation failed then return NaS.
	if(NULL == s.data)
		return NaS;

	//tracersing array 'c' untill '\0' is encountered.
	int i = 0; //'s.length' is also equal to 'i' and could be used.
	while('\0' != c[i])
	{
		//Copying contents to the 'string'.
		s.data[s.length] = c[i];
		++s.length;
		++i;

		//Checking if 'length' has reached last index i.e. "limit - 1"
		if((limit - 1) == s.length)
		{
			//Doubling the limit to accomodate remaining input characters.
			limit = limit<<1; //Left Shift operator.

			//Reallocating the 'string' with new size.
			s.data = realloc(s.data, limit);

			//If failed then return 'NaS'.
			if(NULL == s.data)
				return NaS;
		}
	}

	//At this point it is guaranteed that 'length' < '(limit - 1)'.
	//Appending '\0' at the end.
	s.data[s.length] = '\0';

	//Reallocating the 'string' to a minimum required size
	//which is 'Length + 1'.
	s.data = realloc(s.data, (s.length + 1));

	//Return the newly created 'String' that contains the 'string' and 'length'.
	return s;
}

String DS_clone_String(const String from)
{
	//If the input 'String' is 'NaS', return 'NaS'.
	if(DS_is_NaS(from))
		return NaS;

	String s = DS_new_String();

	//Allocating memory to clone input 'String'.
	//Size equals length of input 'String' plus 1 for '\0'.
	s.data = malloc(from.length + 1);

	//If no memory, return 'NaS'.
	if(NULL == s.data)
		return NaS;

	//Copy everything till '\0' at the index 'from.length'.
	int i;
	for(i = 0; i <= from.length; ++i)
		s.data[i] = from.data[i];

	s.length = from.length;

	return s;
}
//End : Definition of creator functions.

//Start : Definition of transformer functions.
void DS_reset_String(String *s)
/*
 *Takes address of a 'String' as an argument.
 *Frees the memory occupied by the 'String'.
 *Sets the 'String' to 'NaS'.
 *Returns 'NULL' if a 'NULL' pointer is passed.
*/
{
	//If pointer is already 'NULL'.
	if(NULL == s)
		return;

	//If '*s' is 'NaS'.
	if(DS_is_NaS(*s))
		return;

	//If pointer 's' points to some valid 'String'.
	free(s->data);
	(*s) = DS_new_String();
}

String DS_concat_String(const String one,const String two)
/*
*Takes two 'Strings' as arguments.
*If any of the two 'Strings' is 'NaS', returns 'NaS'.
*If both 'Strings' are valid but empty 'Strings, returns an empty 'String'.
*Else creates a new 'String',
 copies the data of both the 'Strings' (first 'one' then 'two')in it.
 *If out of memory, returns 'NaS' instead.
*/
{
	//Checking if any 'String' is 'NaS'.
	if((DS_is_NaS(one)) || (DS_is_NaS(two)))
		return NaS;

	String s = DS_new_String();

	//If any of the two has some data.
	if(0 != (one.length + two.length))
	{
		//If atleast one 'String' is non empty then,
		//Allocating memory for new 'String'.
		s.data = malloc(one.length + two.length + 1);

		//If allocation failed then return 'NaS'.
		if(NULL == s.data)
		 	return NaS;

		//Copy contents of both 'Strings' to the new 'String'.
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

		//Setting length of 's'.
		s.length = one.length + two.length;

		//Append '\0' to 's.data' on the last index
		//i.e. 'one.length + two.length'.
		s.data[s.length] = '\0';

		return s;
	}

	//If both strings are empty.
	return DS_create_empty_String();
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
		return '\0';

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
		return 1;
	
	return 0;
}

int DS_String_is_float(String s)
{
	int state = 1, i, is_digit;
	for(i = 0; i < s.length; ++i)
	{
		if(state == 0)
			return 0;
		
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
							continue;
							
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
							continue;
						
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
					continue;
				
				state = 0;
		}
	}
	if((state == 4) || (state == 5) || (state == 8))
		return 1;
	
	return 0;
}
//End : Definition of reporter functons.
//END : FUNCTION DEFINITION SECTION.

#endif
