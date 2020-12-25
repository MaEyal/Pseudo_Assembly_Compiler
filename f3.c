
/* This file contain functions that convert numbers between bases, chars to their binary ascii representation, etc.*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

void guide_numtobi(char num[]);
void bitohex(char bi[]);
int bitodec(char bi[]);
void chartobinary(char c[]);
void inst_numtobi(char num[]);
void numtoseven(char [], int);
void numtoseven(char [], int);
void strforguidance(char []);
int method_bitodec(int);

/* Converting a number (received in a string with with '+','-' or neither) into a 24 bit binary representations for a guidance command */
void guide_numtobi(char num[])
{
	int poz = 1, numint, i = 0,j = 0;
	char numnew[8];
	if (num[0] == '+')
		j = 1;
	if (num[0] == '-')
	{
		j = 1;
		poz = 0;
	}
	/* In order to avoid '+' or '-', coping the number into a new char array */
	for(j=j; j < strlen(num); j++)
	{
		numnew[i] = num[j];
		i++;
	}
	numnew[i] = '\0';
	numint = atoi(numnew);
	/* Converting the number into its binary representation */
	for (j= 23; j >= 0; j--)
	{
		num[j] = '0' + numint%2;
		numint /= 2;
	}
	num[24] = '\0';
	/* Changing the number into its negative binary representation if needed */
	if (poz == 0)
		{
			int found = 0;
			i = 23;
			while (found == 0)
			{
				if (num[i] == '1')
					found = 1;
				i--;
			}		

			for (i=i; i >= 0; i--)
					if (num[i] == '0')
						num[i] = '1';
					else
						num[i] = '0';
		}		
}

/* Converting a number (received in a string with with '+','-' or neither) into a 21 bit binary representations for an instruction  */
void inst_numtobi(char num[])
{
	int poz = 1, numint, i = 0,j = 0;
	char numnew[8];
	if (num[0] == '+')
		j = 1;
	if (num[0] == '-')
	{
		j = 1;
		poz = 0;
	}
	/* In order to avoid '+' or '-', coping the number into a new char array */
	for(j=j; j < strlen(num); j++)
	{
		numnew[i] = num[j];
		i++;
	}
	numnew[i] = '\0';
	numint = atoi(numnew);
	/* Converting the number into its binary representation */
	j = 20;

	i = 0;
	for (j=j; j >= 0; j--)
	{
		if (pow(2,(double)j) > numint)
			num[i] = '0';
		else
		{
			num[i] = '1';
			numint = numint - pow(2,(double)j);
		}
		i++;
	}
	num[24] = '\0';
	num[23] = '0';
	num[22] = '0';
	num[21] = '1';
	/* Changing the number into its negative binary representation if needed */
	if (poz == 0)
		{
			int found = 0;
			i = 20;
			while (found == 0)
			{
				if (num[i] == '1')
					found = 1;
				i--;
			}		

			for (i=i; i >= 0; i--)
					if (num[i] == '0')
						num[i] = '1';
					else
						num[i] = '0';
		}
			
}

/* Converting a character into a 24bit representation to its ascii vlaue */
void chartobinary(char c[])
{
	int ascii_value = (int)c[0], i = 0, j = 23;
	for (j=j; j >= 0; j--)
	{
		if (pow(2,(double)j) > ascii_value)
			c[i] = '0';
		else
		{
			c[i] = '1';
			ascii_value = ascii_value - pow(2,(double)j);
		}
		i++;
	}
	c[24] = '\0';
}

/* Converting the 24bit word from binary to hex base in 4 digit parts */
void bitohex(char bi[])
{
	char hex[7], part[5];
	int i = 23, p, hexcounter = 5;
	hex[6] = '\0';
	for (i = i; i >= 0; i = i-4)
	{	
		for (p = 0; p < 4; p++)
			part[3-p] = bi[i-p];
		part[4] = '\0';
		if (bitodec(part) <= 9)
			hex[hexcounter--] = bitodec(part)+48;
		else
			hex[hexcounter--] = bitodec(part)+87;	
	}
	for (i = 0; i<6; i++)
		bi[i] = hex[i];
	bi[6] = '\0';
		
}

/* converting a 4bit binary number to decimal */
int bitodec(char bi[])
{
	int num = 0, i;
	for (i=0; i<4; i++)
	{
		if (bi[i] == '1')
			num = num + pow(2, (double)(3-i));
	}
	return num;
}

/* converting a number to binary with a choice of ARE*/
void numtobi(char bi[], int num, char ARE)
{
	int i=20, poz = 1;
	int found = 0;
	bi[24] = '\0';
	bi[23] = '0'; bi[22] = '0'; bi[21] = '0';
	/* Placing the value 1 either in A, R or E*/
	switch (ARE)
	{
		case 'E':
		{
			bi[23] = '1';
			break;
		}
		case 'R':
		{
			bi[22] = '1';
			break;
		}
		case 'A':
		{
			bi[21] = '1';
			break;
		}
		default:
		{
			printf("invalid field: when inputting a number (%d) Must choose A,R or E\n", num);
			return;
		}
	}
	if (num < 0)
	{
		poz = 0;
		num *= (-1);
	}
	/* convering the number to binary*/
	for (;i>=0;i--)
	{
		bi[i] = '0' + num%2;
		num /= 2;
	}
	/* If the number was negative, changes it's representation to a negative one*/
	if (poz == 0)
		{
			i = 20;
			while (found == 0)
			{
				if (bi[i] == '1')
					found = 1;
				i--;
			}		

			for (i=i; i >= 0; i--)
					if (bi[i] == '0')
						bi[i] = '1';
					else
						bi[i] = '0';
		}
		

}


/*Converting an int into a 7 digit string */
void numtoseven(char num_in_string[], int num)
{
	int i, dev_factor = 10;
	num_in_string[7] = '\0';
	for (i = 6; i>=0; i--)
	{
		num_in_string[i] = '0' + num%dev_factor;
		num /= dev_factor;
	}
	
	
}

/* receives the full line that is after the .string guidance and returns only the part that needs to be translated into binary */
void strforguidance(char str[])
{
	int i = 0, j = 0, first_qmark, last_qmark;
	while (str[i] != '"')
		i++;
	first_qmark = i;
	while (str[i] != '\0')
	{
		if (str[i] == '"')
			last_qmark = i;
		i++;
	}
	i = first_qmark + 1;
	for (; i < last_qmark; i++)
		str[j++] = str[i];
	str[j] = '\0';
}

/* This function receives a source method number in binary and returns its value in decimal base */
int method_bitodec(int source_method)
{
	if (source_method == 11)
		return 3;
	else if (source_method == 10)
		return 2;
	else if (source_method == 1)
		return 1;
	else
		return 0;
}




