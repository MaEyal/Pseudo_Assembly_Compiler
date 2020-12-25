
/* This file contains the methods of the linked list which is used for the table of symbols */

# include "h1.h"

ptr h = NULL;
ptr *hptr = &h;



/*The function inserts the label in s into the table of symbols*/
/*The function returns 0 if errors are found and 1 if no errors are found*/
int add2list(char s[], int v, int is_dc)
{
ptr p;
ptr t;

/*Create a new node*/
t = (ptr)malloc(sizeof(item));
if(!t)
{
	printf("Cannot build list\n");
	exit(0);
}
strcpy(t -> symbol, s);
t -> value = v;
t -> dc = is_dc;

p = *hptr;
while((p) && (strcmp(p -> symbol, t -> symbol)) && (p -> next))
	p = p -> next;
if(!p) /*Add the node at the beginning of the list*/
	*hptr = t;
else if(!strcmp(p -> symbol, t -> symbol)) /*The node already exists in the list*/
{
	if((t -> value != -1) && (p -> value == -1)) /*The label has already appeared as an operand of the entry guidance and now it appears in another way*/
	{
		p -> value = t -> value;
		p -> dc = t -> dc;
		return 1;
	}

	if((t -> value == -1) && (p -> value != -1)) /*The label appears as an operand of the entry guidance after it already appeared in another way*/
	{
		p -> entry = 1;
		return 1;
	}

	if(((t -> value == -1) && (p -> value == -1)) || ((t -> value == 0) && (p -> value == 0))) /*It is not considered an error if the source file contains more than one entry guidance with the same label as the operand or more than one extern guidance with the same label as the operand*/
		return 1;

	if(((t -> value) && !(p -> value) && !(p -> dc)) || (!(t -> value) && !(t -> dc) && (p -> value))) /*The purpose of the extern guidance is to inform the assembler that the label is defined in another source file and that the assembly code in the current file uses the label*/
	{
		printf("The label %s is defined in the current source file and appears as an operand of the extern guidance\n",t -> symbol);
		return 0;
	}

	printf("The label %s is defined more than once\n",t -> symbol);
	return 0;
}
else /*Add the node at the end of the list*/
	p -> next = t;
t -> next = NULL;
if(t -> value == -1)
	t -> entry = 1;
return 1;
}



/*The function frees the memory we have allocated for the list*/
void freelist()
{
ptr p;
while(*hptr)
{
	p = *hptr;
	*hptr = (*hptr) -> next;
	free(p);
}
}



/*The update adds to the value (the address in memory) the total length of the instruction part plus the loading start address of the code which is 100*/
/*The function returns 0 if errors are found and 1 if no errors are found*/
int updat_symbol_table(int IC)
{
int errors = 0;
ptr p;
p = *hptr;
while(p)
{
	if(p -> value == -1)
	{
		printf("The label %s is not defined\n",p -> symbol);
		errors = 1;
	}
	if(p -> dc)
		p -> value += IC;
	p = p -> next;
}

if(errors)
	return 0;

return 1;
}



/*Using the symbol table the function completes the encoding of all the words in the set of instructions that have not yet been encoded in the first pass*/
/*The function returns 0 if errors are found and 1 if no errors are found*/
int find_value(char s[], int IC)
{
char temp[Label], address[Address];
ptr p;
p = *hptr;
while(p)
{
	if(!strcmp(p -> symbol, s))
	{
		if(p -> value == 0)
		{
			numtobi(s, p -> value, 'E'); /*External*/
			numtoseven(address, IC);
			strcat(p -> external, address);
		}
		else
			numtobi(s, p -> value, 'R'); /*Relocatable*/
		return 1;
	}
	if(!strcmp(strcat(strcpy(temp, p -> symbol), "?"), s))
	{
		if(p -> value == 0)
		{
			printf("Addressing Method 2 cannot be used on a label defined in another source file (external address)\n");
			return 0;
		}
		else
		{
			numtobi(s, p -> value - IC + 1, 'A'); /*Absolute*/
			return 1;
		}
	}
	p = p -> next;
}

if(s[strlen(s)-1]=='?')
	s[strlen(s)-1]='\0';
printf("The label %s does not appear in the table of symbols\n",s);
return 0;
}



/*The function returns 1 if there is ent and 0 otherwise*/
int there_is_ent()
{
ptr p;
p = *hptr;
while(p)
{
	if(p -> entry == 1)
		return 1;
	p = p -> next;
}
return 0;
}



/*The function returns 1 if there is ext and 0 otherwise*/
int there_is_ext()
{
ptr p;
p = *hptr;
while(p)
{
	if(p -> value == 0)
		return 1;
	p = p -> next;
}
return 0;
}



/*The function is responsible for writing the file whose name has the extension ".ent"*/
/*The function receives the name of the input file without the suffix ".as"*/
void write_ent_file(char name[])
{
char address[Address];
FILE *fent;
ptr p;
p = *hptr;
if(!(fent = fopen(name, "w")))
{
	printf("The output file %s cannot be opened\n", name);
	return;
}
while(p)
{
	if(p -> entry == 1)
	{
		numtoseven(address, p -> value);
		fprintf(fent, "%s ", p -> symbol);
		fprintf(fent, "%s\n", address);
	}
	p = p -> next;
}
fclose(fent);
}



/*The function is responsible for writing the file whose name has the extension ".ext"*/
/*The function receives the name of the input file without the suffix ".as"*/
void write_ext_file(char name[])
{
int i, j;
char address[Address];
FILE *fext;
ptr p;
p = *hptr;
if(!(fext = fopen(name, "w")))
{
	printf("The output file %s cannot be opened\n", name);
	return;
}
while(p)
{
	if(p -> value == 0)
	{
		for(i=0;i<strlen(p -> external)/7;i++)
		{
			for(j=0;j<7;j++)
				address[j] = (p -> external)[i*7+j];
			address[Address] = '\0';
			fprintf(fext, "%s ", p -> symbol);
			fprintf(fext, "%s\n", address);
		}
	}
	p = p -> next;
}
fclose(fext);
}

