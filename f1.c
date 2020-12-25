
/*This file includes the main and the process of the first and second pass */

# include "h1.h"

int IC, DC;
char (instruction_binary_code[SIZE]) [Label];
char (guidance_binary_code[SIZE]) [Label];

int first_pass(FILE *);
void second_pass(char []);



int main(int argc, char *argv[])
{
int i;
char temp[size];
FILE *f;

if(argc == 1)
{
	printf("You must write input file names in the command line\n");
	exit(0);
}

for(i=1;i<argc;i++)
{
	IC = initial_IC;
	DC = 0;
	if(!(f = fopen(strcat(strcpy(temp, argv[i]), ".as"), "r")))
	{
		printf("The input file %s cannot be opened\n", temp);
		break;
	}

	if(first_pass(f))
		second_pass(argv[i]);
	else 
	{
		printf("(Empty sentences and comment sentences are not included in the line numbering)\n");
		printf("We found errors in the first pass so we will not continue to the second pass\n");
		printf("End of error messages for the %s file\n\n",argv[i]);
	}
	fclose(f);
	freelist();
}

return 0;
}



/*The function is responsible for writing the file whose name has the extension ".ob"*/
/*The function receives the name of the input file without the suffix ".as"*/
void write_ob_file(char name[])
{
int i;
char address[Address];
FILE *fob;

if(!(fob = fopen(name, "w")))
{
	printf("The output file %s cannot be opened\n", name);
	return;
}
fprintf(fob, "%d ", IC - initial_IC);
fprintf(fob, "%d\n", DC);

/*Printing the instruction part*/
for(i=initial_IC;i<IC;i++)
{
	numtoseven(address, i);
	bitohex(instruction_binary_code[i]);
	fprintf(fob, "%s ", address);
	fprintf(fob, "%s\n", instruction_binary_code[i]);
}

/*Printing the guidance part*/
for(i=0;i<DC;i++)
{
	numtoseven(address, i+IC);
	bitohex(guidance_binary_code[i]);
	fprintf(fob, "%s ", address);
	fprintf(fob, "%s\n", guidance_binary_code[i]);
}

fclose(fob);
}



/*The function receives the name of the input file without the suffix ".as"*/
void second_pass(char name[])
{
int errors = 0, i;
char temp[size];

if(!updat_symbol_table(IC)) /*Adds to the value (the address in memory) the total length of the instruction part plus the loading start address of the code which is 100*/
	errors = 1;

for(i=initial_IC;i<IC;i++)
	if(isalpha(instruction_binary_code[i][0]))
		if(!find_value(instruction_binary_code[i],i)) /*Completes the encoding of all the words in the set of instructions that have not yet been encoded in the first pass*/
			errors = 1;
if(errors)
{
	printf("We found errors in the second pass so we will not continue to the writing of the output files\n");
	printf("End of error messages for the %s file\n\n",name);
	return;
}

/*writing of the output files*/
write_ob_file(strcat(strcpy(temp, name), ".ob"));

if(there_is_ent()) /*If the source file does not have any entry guidance then the assembler will not create the entries output file*/
	write_ent_file(strcat(strcpy(temp, name), ".ent"));

if(there_is_ext()) /*If the source file does not have any extern guidance then the assembler will not create the externals output file*/
	write_ext_file(strcat(strcpy(temp, name), ".ext"));
}



/*The function receives the instruction details and assembles the word from them as required*/
void instruction_into_word(char instruction[], char source_operand[], char destination_operand[], char source_method[], char destination_method[], int num_of_operands)
{
char opcode[] = "000000", funct[] = "00000", source_register[] = "000", destination_register[] = "000", ARE[] = "100", code[word] = {'\0'};

/*Assemble the opcode and the funct as required*/
if(!(strcmp(instruction,"cmp")))
	opcode[5] = '1';
if(!(strcmp(instruction,"add")))
{
	opcode[4] = '1';
	funct[4] = '1';
}
if(!(strcmp(instruction,"sub")))
{
	opcode[4] = '1';
	funct[3] = '1';
}
if(!(strcmp(instruction,"lea")))
	opcode[3] = '1';
if(!(strcmp(instruction,"clr")))
{
	opcode[5] = '1';
	opcode[3] = '1';
	funct[4] = '1';
}
if(!(strcmp(instruction,"not")))
{
	opcode[5] = '1';
	opcode[3] = '1';
	funct[3] = '1';
}
if(!(strcmp(instruction,"inc")))
{
	opcode[5] = '1';
	opcode[3] = '1';
	funct[4] = '1';
	funct[3] = '1';
}
if(!(strcmp(instruction,"dec")))
{
	opcode[5] = '1';
	opcode[3] = '1';
	funct[2] = '1';
}
if(!(strcmp(instruction,"jmp")))
{
	opcode[5] = '1';
	opcode[2] = '1';
	funct[4] = '1';
}
if(!(strcmp(instruction,"bne")))
{
	opcode[5] = '1';
	opcode[2] = '1';
	funct[3] = '1';
}
if(!(strcmp(instruction,"jsr")))
{
	opcode[5] = '1';
	opcode[2] = '1';
	funct[4] = '1';
	funct[3] = '1';
}
if(!(strcmp(instruction,"red")))
{
	opcode[3] = '1';
	opcode[2] = '1';
}
if(!(strcmp(instruction,"prn")))
{
	opcode[5] = '1';
	opcode[3] = '1';
	opcode[2] = '1';
}
if(!(strcmp(instruction,"rts")))
{
	opcode[4] = '1';
	opcode[3] = '1';
	opcode[2] = '1';
}
if(!(strcmp(instruction,"stop")))
{
	opcode[5] = '1';
	opcode[4] = '1';
	opcode[3] = '1';
	opcode[2] = '1';
}

/*Assemble the source_register as required*/
if(!(strcmp(source_method,"11")))
{
	if(source_operand[1] == '4' || source_operand[1] == '5' || source_operand[1] == '6' || source_operand[1] == '7')
		source_register[0] = '1';
	if(source_operand[1] == '2' || source_operand[1] == '3' || source_operand[1] == '6' || source_operand[1] == '7')
		source_register[1] = '1';
	if(source_operand[1] == '1' || source_operand[1] == '3' || source_operand[1] == '5' || source_operand[1] == '7')
		source_register[2] = '1';
}

/*Assemble the destination_register as required*/
if(!(strcmp(destination_method,"11")))
{
	if(destination_operand[1] == '4' || destination_operand[1] == '5' || destination_operand[1] == '6' || destination_operand[1] == '7')
		destination_register[0] = '1';
	if(destination_operand[1] == '2' || destination_operand[1] == '3' || destination_operand[1] == '6' || destination_operand[1] == '7')
		destination_register[1] = '1';
	if(destination_operand[1] == '1' || destination_operand[1] == '3' || destination_operand[1] == '5' || destination_operand[1] == '7')
		destination_register[2] = '1';
}

/*Assemble the word as required*/
strcat(code, opcode);
strcat(code, source_method);
strcat(code, source_register);
strcat(code, destination_method);
strcat(code, destination_register);
strcat(code, funct);
strcat(code, ARE);

strcpy(instruction_binary_code[IC++], code);

/*Add a word for the source operand if necessary*/
if((num_of_operands == 2) && (strcmp(source_method,"11")))
{
	if(!(strcmp(source_method,"00")))
		inst_numtobi(source_operand);
	strcpy(instruction_binary_code[IC++], source_operand);
}

/*Add a word for the destination operand if necessary*/
if((num_of_operands != 0) && (strcmp(destination_method,"11")))
{
	if(!(strcmp(destination_method,"00")))
		inst_numtobi(destination_operand);
	if(!(strcmp(destination_method,"10")))
		strcat(destination_operand, "?");
	strcpy(instruction_binary_code[IC++], destination_operand);
}
}



/*The function receives an operand and updates the method accordingly*/
void get_method(char operand[], char method[])
{
int j;

if(operand[0] == '#') /*method 0*/
{
	for(j = 0; j <= strlen(operand); j++)
		operand[j] = operand[j+1];
	if(is_number(operand))
	{
		method[0] = '0';
		method[1] = '0';
	}
}
else if(is_label(operand)) /*method 1*/
{
	method[0] = '0';
	method[1] = '1';
}
else if(operand[0] == '&') /*method 2*/
{
	for(j = 0; j <= strlen(operand); j++)
		operand[j] = operand[j+1];
	if(is_label(operand))
	{
		method[0] = '1';
		method[1] = '0';
	}
}
else if((operand[0] == 'r')&&(operand[1] == '0' || operand[1] == '1' || operand[1] == '2' || operand[1] == '3' || operand[1] == '4' || operand[1] == '5' || operand[1] == '6' || operand[1] == '7')&&(operand[2] == '\0')) /*method 3*/
{
	method[0] = '1';
	method[1] = '1';
}
else
	method[0] = '\0';
method[2] = '\0';
}



/*The function receives an instruction as s and the rest of the line as buffer and returns 0 if errors are found and 1 if no errors are found*/
int instruction_handling(char s[], char buffer[], int line)
{
int i, j;
char source_operand[size], destination_operand[size], source_method[3], destination_method[3];

if(!(strcmp(s,"mov"))||!(strcmp(s,"cmp"))||!(strcmp(s,"add"))||!(strcmp(s,"sub"))||!(strcmp(s,"lea"))) /*Instructions requiring two operands*/
{
	if(!number_of_operands(buffer, 2, line))
		return 0;

	/*Checking the source operand and the source method*/
	i = 0;
	for(j=0; j<strlen(buffer); j++)
	{
		if(buffer[j] == ',')
			break;
		if(buffer[j] != ' ' && buffer[j] != '\t')
			source_operand[i++] = buffer[j];
	}
	source_operand[i]='\0';
	get_method(source_operand, source_method);
	if (source_method[0] == '\0')
	{
		printf("line %d : %s is an invalid source operand\n",line,source_operand);
		return 0;
	}
	if (!(!strcmp(source_method,"01") || (strcmp(s,"lea") && (!strcmp(source_method,"00") || !strcmp(source_method,"11")))))	
	{
		printf("line %d : %d is an invalid source method for %s\n",line,method_bitodec(atoi(source_method)),s);
		return 0;
	}

	/*Checking the destination operand and the destination method*/
	i = 0;
	for(j++; j<strlen(buffer); j++)
	{
		if(buffer[j] != ' ' && buffer[j] != '\t')
			destination_operand[i++] = buffer[j];
	}
	destination_operand[i]='\0';
	get_method(destination_operand, destination_method);
	if (destination_method[0] == '\0')
	{
		printf("line %d : %s is an invalid destination operand\n",line,destination_operand);
		return 0;
	}
	if (!(!strcmp(destination_method,"01") || !strcmp(destination_method,"11") || (!strcmp(s,"cmp") && !strcmp(destination_method,"00"))))
	{
		printf("line %d : %d is an invalid destination method for %s\n",line,method_bitodec(atoi(destination_method)),s);
		return 0;
	}

	instruction_into_word(s, source_operand, destination_operand, source_method, destination_method, 2);
}

else if(!(strcmp(s,"clr"))||!(strcmp(s,"not"))||!(strcmp(s,"inc"))||!(strcmp(s,"dec"))||!(strcmp(s,"jmp"))||!(strcmp(s,"bne"))||!(strcmp(s,"jsr"))||!(strcmp(s,"red"))||!(strcmp(s,"prn"))) /*Instructions requiring one operand*/
{
	if(!number_of_operands(buffer, 1, line))
		return 0;

	/*Checking the destination operand and the destination method*/
	i = 0;
	for(j=0; j<strlen(buffer); j++)
	{
		if(buffer[j] != ' ' && buffer[j] != '\t')
			destination_operand[i++] = buffer[j];
	}
	destination_operand[i]='\0';
	get_method(destination_operand, destination_method);
	if (destination_method[0] == '\0')
	{
		printf("line %d : %s is an invalid destination operand\n",line,destination_operand);
		return 0;
	}
	if (!(!strcmp(destination_method,"01") || (!strcmp(destination_method,"00") && !strcmp(s,"prn")) || (!strcmp(destination_method,"10") && (!strcmp(s,"jmp") || !strcmp(s,"bne") || !strcmp(s,"jsr"))) || (!strcmp(destination_method,"11") && (strcmp(s,"jmp") || strcmp(s,"bne") || strcmp(s,"jsr")))))
	{
		printf("line %d : %d is an invalid destination method for %s\n",line,method_bitodec(atoi(destination_method)),s);
		return 0;
	}

	instruction_into_word(s, NULL, destination_operand, "00", destination_method, 1);
}

else if(!(strcmp(s,"rts"))||!(strcmp(s,"stop"))) /*Instructions without operands*/
{
	if(!number_of_operands(buffer, 0, line))
		return 0;

	instruction_into_word(s, NULL, NULL, "00", "00", 0);
}

else
{
	printf("line %d : %s is an invalid instruction\n",line,s);
	return 0;
}

return 1;
}



/*The function receives a guidance as s and the rest of the line as buffer and returns 0 if errors are found and 1 if no errors are found*/
int guidance_handling(char s[], char buffer[], int line)
{
int i, j = 0;
char number[word], letter[word], label[size];

if(s[0] == '\0')
{
	printf("line %d : there is no guidance after the .\n",line);
	return 0;
}

if(!(strcmp(s,"data"))) /*data*/
{
	while(buffer[j] == ' ' || buffer[j] == '\t') j++;
	if (buffer[j] == '\0')
	{
		printf("line %d : Missing data input\n",line);
		return 0;
	}
	j = 0;
	if(!is_data(buffer))
	{
		printf("line %d : %s is an invalid data\n",line,buffer);
		return 0;
	}
	while(j<strlen(buffer))
	{
		i = 0;
		for(; j<strlen(buffer); j++)
		{
			if(buffer[j] == ',')
			{
				j++;
				break;
			}
			if(buffer[j] != ' ' && buffer[j] != '\t')
				number[i++] = buffer[j];
		}
		number[i]='\0';
		guide_numtobi(number);
		strcpy(guidance_binary_code[DC++], number);
	}
}
else if(!(strcmp(s,"string"))) /*string*/
{
	while(buffer[j] == ' ' || buffer[j] == '\t') j++;
	if (buffer[j] == '\0')
	{
		printf("line %d : Missing string input\n",line);
		return 0;
	}
	j = 0;
	if(!is_string(buffer))
	{
		printf("line %d : %s is an invalid string\n",line,buffer);
		return 0;
	}
	strforguidance(buffer);
	while(buffer[j] != '\0')
	{
		letter[0] = buffer[j];
		chartobinary(letter);
		strcpy(guidance_binary_code[DC++], letter);
		j++;
	}
	letter[0] = '\0';
	chartobinary(letter);
	strcpy(guidance_binary_code[DC++], letter);
}
else if(!(strcmp(s,"entry")) || !(strcmp(s,"extern"))) /*entry and extern*/
{
	if(!number_of_operands(buffer, 1, line))
		return 0;

	i = 0;
	while(buffer[j] == ' ' || buffer[j] == '\t')
		j++;
	while(buffer[j] != ' ' && buffer[j] != '\t' && buffer[j] != '\0')
	{
		label[i++] = buffer[j];
		j++;
	}
	label[i]='\0';
	if(!is_label(label))
	{
		printf("line %d : %s is an invalid label\n",line,label);
		return 0;
	}
	if(!(strcmp(s,"entry")))
		add2list(label, -1, 0);
	else
		add2list(label, 0, 0);
}
else
{
	printf("line %d : %s is an invalid guidance\n",line,s);
	return 0;
}
return 1;
}



/*The function receives a label and the rest of the line as res and returns 0 if errors are found and 1 if no errors are found*/
int label_handling(char label[], char res[], int line)
{
int i = 0, j = 0, k = 0;
char s[size], buffer[size];
/*s will contain the first word in the res without white characters and buffer will contain the rest of the res*/

if(res[0] == '\0')
{
	printf("line %d : there is no instruction and no guidance after the label %s\n",line,label);
	return 0;
}

if(!is_label(label))
{
	printf("line %d : %s is an invalid label\n",line,label);
	return 0;
}

while(res[j] == ' ' || res[j] == '\t')
	j++;
while(res[j] != ' ' && res[j] != '\t' && res[j] != '\0')
{
	s[i++] = res[j];
	j++;
}
s[i]='\0';
while(j<strlen(res))
{
	j++;
	buffer[k++] = res[j];
}
buffer[k]='\0';
if(s[0] == '.')
{
	for(j = 0; j < i; j++)
		s[j] = s[j+1];
	if(!strcmp(s,"data") || !strcmp(s,"string")) /*A label defined at the beginning of an entry line or at the beginning of an extern line is meaningless and the assembler ignores this label*/
		if(!add2list(label, DC, 1))
			return 0;
	if(!(guidance_handling(s, buffer, line))) /*guidance_handling*/
		return 0;
}
else 
{
	if(!add2list(label, IC, 0))
		return 0;
	if(!(instruction_handling(s, buffer, line))) /*instruction_handling*/
		return 0;
}

return 1;
}



/*The function receives a pointer to the input file and returns 0 if errors are found and 1 if no errors are found*/
int first_pass(FILE *f)
{
int errors = 0, line = 0, i, j;
char c, s[size], buffer[size];
/*s will contain the first word in the line without white characters and buffer will contain the rest of the line without the character '\n'*/

while(fscanf(f, "%c", &c) != EOF)
{
	while((c==' '||c=='\t'||c=='\n')&&(fscanf(f, "%c", &c) != EOF));
	if(c==' '||c=='\t'||c=='\n')
		break;

	/*comment sentences*/
	while(c == ';')
	{
		while(c!='\n')
			fscanf(f, "%c", &c);
		while((c==' '||c=='\t'||c=='\n')&&(fscanf(f, "%c", &c) != EOF));
		if(c==' '||c=='\t'||c=='\n')
			return errors ? 0 : 1;
	}

	/*Empty sentences and comment sentences are not included in the line numbering*/
	line++;
	i = 0;
	while(c!=' '&&c!='\t'&&c!='\n')
	{
		s[i++] = c;
		fscanf(f, "%c", &c);
	}
	s[i]='\0';

	if(c=='\n')
		buffer[0]='\0';
	else
	{
		j = 0;
		while(c!='\n')
		{
			fscanf(f, "%c", &c);
			buffer[j++] = c;
		}
		buffer[j-1]='\0';
	}
	if(s[i-1] == ':')
	{
		s[i-1]='\0';
		if(!(label_handling(s, buffer, line))) /*label_handling*/
			errors = 1;
	}
	else if(s[0] == '.')
	{
		for(j = 0; j < i; j++)
			s[j] = s[j+1];
		if(!(guidance_handling(s, buffer, line))) /*guidance_handling*/
			errors = 1;
	}
	else 
		if(!(instruction_handling(s, buffer, line))) /*instruction_handling*/
			errors = 1;
}

if(errors)
	return 0;

return 1;

}
