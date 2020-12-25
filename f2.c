
/*This file contains functions that check validity of inputs such as labels, operands, etc. */

#include "h1.h"

int is_command(char instruction[]);
int is_guidance(char input[]);
int is_sourcereg(char source_reg[]);

/* Arrays of saved words */
char instructions[16][
5] = {"mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop"};
char guidances[4][7] = {"data","string","entry","extern"};
char source_registeries[8][3] = {"r0","r1","r2","r3","r4","r5","r6","r7"};

/* Checking if the intruction name is legal */
int is_instruction(char instruction[])
{
	int i;
	for (i = 0; i < 16; i++)
		if (!strcmp(instruction, instructions[i]))
			return 1;
	return 0;
}

/* Checking if the input guidance name is legal */
int is_guidance(char guidance[])
{
	int i;
	for (i = 0; i< 4; i++)
		if (!strcmp(guidance, guidances[i]))
			return 1;
	return 0;
}

/* Checking if it's an actual name of a register */
int is_sourcereg(char source_reg[])
{
	int i;
	for (i = 0; i < 8; i++)
		if (!strcmp(source_reg, source_registeries[i]))
			return 1;
	return 0;

}

/* Checking if the given string is a number (can be with '+' or '-' at the beginning)*/
int is_number(char num[])
{
	int i;
	if (!((num[0] == '-') || (num[0] == '+') || isdigit((num[0]))))
		return 0;
	for (i = 1; i< strlen(num); i++)
		if (!(isdigit(num[i])))
			return 0;
	return 1;
	
}

/* Checking if the label name is legal */
int is_label(char label[])
{
	int i;
	if (strlen(label)> 31)
		return 0;
	if (is_instruction(label))
		return 0;
	if (is_guidance(label))
		return 0;
	if (is_sourcereg(label))
		return 0;
	if (!(isalpha(label[0])))
		return 0;
	for (i = 1; i< strlen(label); i++)
		if (!(isalpha(label[i]) || isdigit(label[i])))
			return 0;
	return 1;

}


/* Checking if a string is legal (for the action of .string) */
int is_string(char str[])
{
	/*The function will find the location of the last and first quatation marks, the last note in the string (not including spaces and tabs), the first tab and will count the number of quatition marks */
	int i=0, first_qmarks, last_qmarks, first_tab, tab = 0,last_note, counter = 0;
	while (str[i] == ' ' || str[i] == '\t') i++;
	/* After white spaces, the string must start and end with '"', in the middle could be notes of ascii value of 32 to 127*/
	if (str[i] == '"')
	{
		last_note = i;
		first_qmarks = i;
		last_qmarks = i;
		counter++;
		i++;
		while (str[i] != '\0')
		{
			/* Each time the function will mark the location of the last quotation marks */
			if (str[i] == '"')
			{
				counter++;
				last_qmarks = i;
			}
			else if (str[i] == '\t')
			{
				/*The function marks the first tab to see later if it's within quatation marks boundaries (since a legal string cannot have a tab) */
				if (tab == 0)
				{
					first_tab = i;
					tab = 1;
				}
			}
			else
			{
				/* the function checks the legality of the notes, if the note is not a white space than it will mark the location of the last note to appear to check later that if it's
				within the sring boundaries*/
				if (!(((int)str[i] >31) && ((int)str[i] <= 127)))
					return 0;
				if (!(str[i] == ' '))
					last_note = i;
			}
			i++;			
		}
	}
	else
		return 0;
	/* Checks if there are less than two quotation marks, that the string between the first and the last is not empty, that the tab is not within the string and the there are no visible notes after
	the last quotation marks*/
	if (counter < 2)
		return 0;
	if ((last_qmarks - first_qmarks) == 1)
		return 0;
	if (tab == 1)
		if (first_tab < last_qmarks)
			return 0;
	if (last_note > last_qmarks)
		return 0;
	return 1;
}

/*Checking if data input is legal*/
int is_data(char data[])
{
	int i = 0;
	/* Checking for white spaces, then the first note can be either '+','-' or a digit, if it's a digit, we're checking for a string of digits,
	and then white spaces again until we get ',' in which case we repease the process or a '\0' which means we're finished. */
	while (data[i] != '\0')
	{
		while (data[i] == ' ' || data[i] == '\t')
			i++;

		if (data[i] == '+' || data[i] == '-')
			i++;

		if (isdigit(data[i]))
			while (isdigit(data[i]))
			{
				i++;
			}
		else
			return 0;

		while (data[i] == ' ' || data[i] == '\t')
			i++;

		if (data[i] == '\0')
			return 1;
		else if (data[i] == ',')
			i++;
		else
			return 0;	
	}

	return 0;
}

/*Checking if the number of operands expected is the same as in the input*/
int number_of_operands(char operands[], int num_of_operands, int line)
{

	int i = 0, counter = 0, empty = 1;
	while (operands[i] != '\0')
	{
		/* Scanning for white spaces, followd by legal notes, white spaces again and then if we reached either the end of the text or to a ',' (which will repeat the process), othewise
		it will return that the input is invalid */ 
		while (operands[i] != ',' && operands[i] != '\0')
		{
			while (operands[i] == ' ' || operands[i] == '\t')
				i++;
			while (isalpha(operands[i]) || isdigit(operands[i]) || operands[i] == '#' || operands[i] == '&' || operands[i] == '-' || operands[i] == '+')
			{
				empty = 0;
				i++;
			}
			while (operands[i] == ' ' || operands[i] == '\t')
				i++;
			if (operands[i] != ',' && operands[i] != '\0')
			{
				printf("line %d : operand list \"%s\" is an Invalid operand input\n",line, operands);
				return 0;
			}
		}
		/*The variable empty is flagged once there no notes (not including white spaces) between:
		1. two ','
		2. beginning to a ','
		3. ',' to the end
		4. beginning to the end (when there are no ',')
		If empty is flageed and we haven't reached teh end of the text, or if we reached a ',' and the next not is the end. Thne the input is not valid */
		if ((empty == 1 && operands[i] != '\0') || (operands[i] == ',' && operands[i+1] == '\0') || (empty == 1 && counter > 0))
		{
			printf("line %d : operand list \"%s\" is an Invalid operand input\n",line, operands);
			return 0;
		}
		else
		{
			if (empty == 0)
				counter++;
		}
		if (operands[i] == ',')
		{
			i++;
			empty = 1;
		}
	}
	if (counter == num_of_operands)
		return 1;
	else
		if (counter > num_of_operands)
			printf("line %d : operand list \"%s\" has too many operands\n",line, operands);
		else
		{
			if (counter == 0)
				printf("line %d : Missing operands input\n", line);
			else
				printf("line %d : operand list \"%s\" has insufficient operands\n",line, operands);
		}
	return 0;
	
}
