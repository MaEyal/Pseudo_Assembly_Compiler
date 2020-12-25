# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>
# define Address 8
# define word 25
# define Label 33
# define size 81
# define initial_IC 100
# define SIZE 5000

/* A pointer for a node in the linked link */
typedef struct node * ptr;

/* Defining a linked list node for the table of symbols*/
typedef struct node {
char symbol[Label], external[size];
int value, dc, entry;
ptr next;
} item;


int add2list(char [], int, int);
void freelist();
int updat_symbol_table(int);
int find_value(char [], int);
int there_is_ent();
int there_is_ext();
void write_ent_file(char []);
void write_ext_file(char []);


int is_label(char []);
int is_number(char []);
int is_string(char []);
int is_data(char []);
int number_of_operands(char [], int, int);

void inst_numtobi(char []);
void guide_numtobi(char []);
void chartobinary(char []);
void bitohex(char []);
void numtobi(char [], int, char);
void numtoseven(char [], int);
void strforguidance(char []);
int method_bitodec(int);
