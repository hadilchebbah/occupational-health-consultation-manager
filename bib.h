#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define MAX_HISTORY 5
#define NAME_LEN 30
#define DATE_LEN 11
#define HISTORY_LEN 150
 // Structure for employee data
typedef struct Employee {
    int Emp_Id;                 // Employee ID number
    char name[NAME_LEN];        // Employee full name
    int cons_num;               // Total consultations count
    char last_date[DATE_LEN];   // Last visit date
    char ret_work[DATE_LEN];    // Return to work  date
    char history[MAX_HISTORY][NAME_LEN]; // Last 5 visit reasons
} Emplyee;
typedef struct emp_record{
        Emplyee info ;
    struct emp_record *adr;
} emp_record;
//**************************STRUCTURE DEFINITION****************************/////////////////
typedef struct emp_consult{ // employee consultation record definition
    int Emp_Id;// the employee Id
    char name[NAME_LEN];// the employee name
    char cons_time[5];// the consultation time
    char cons_reason[NAME_LEN];// the consultation reason
} emp_consult;
typedef struct cons_node{//consultation queue node definition
    emp_consult cons_rec;// the value is the employee information
    struct cons_node *ad;//pointer to the next node
}cons_node;
typedef struct que{// record contains head and tail of the queue
    cons_node *h,*t;
}que;
que priority_tab[4];//table of proirities such that each element is a linked list of emplyoees that have the same priority
/////////***************************************************************************//////////////////////////////////////


void Allocate (emp_record **p );//create new node of emp record
/////////////////////////////////////
void Ass_adr(emp_record *p, emp_record *q);   //access to the node
//////////////////////////////
emp_record* Next(emp_record *p); //give the next
// Function to create a new node
emp_record* create_node(int id, char *name, int cons_num, char *last_date, char *ret_work, char *history_line);
/////////////////////////////////////
char* valid_reason();// function to get a valid reason from the user
////////////////////////////////////////////////
int id_unicity(emp_record *head, int id); // to check if the id alredy exist or not
// Function to insert a node at the end of the linked list
void add_node(emp_record **head, emp_record *new_node);//function to add new one to the list
///////////////////////////////////////////
void add(emp_record **head);//function to add an employee at the end of the list
////////////////////////////////////////////////////////
void full_record(Emplyee *emp,emp_record*head); //function to full the record of employee

// Function to read from file and fill the linked list
void fill_ll(const char *fname, emp_record **head) ;//function to fill the li
// Function to print employee records
void print_records(emp_record *head);//function to print the list
//////////////////////////////////////////
void free_list(emp_record *head);// Function to free memory
/////////////////////////////////////////
void access_Id (emp_record*h,int Id ,emp_record**p,emp_record**q);//function to return the address of employee defind by it's id and the previous in the list of emp_record
/////////////////////////////////////////////////
void Delete_Id(emp_record ** head,int Id);//function to delete employee from the list by using the ID
/////////////////////////////////////////////////////////
void update_history(emp_record * head,int Id,char reason[NAME_LEN]);//function to update the history
/////////////////////////////////////////////////////////
void update_date(emp_record * head,int Id,char date[10],int choix);//function to update the date of last date or the date of return to work
/////////////////////////////////////////////////////////////////////////////
//FUNCTION TO create a node that contains emp_consult record and the address to the next
cons_node *create_node2(int id, char *name, char *cons_time, char *cons_reason);
/////////////////////////////////////////////////////////////////////
void full_tab_prio(cons_node *n);// Function to full the table of priority
////////////////////////////////////////////////////////////////////
void fill_ll2(const char *fname);//FUNCTION TO READ FROM FILE
////////////////////////////////////////////////////////////////////////
void print_records2();//Function to print the record
/////////////////////////////////////////////////////////////
void full_record2(emp_consult *emp);//function to remplir record of consultation
//////////////////////////////////////////////////
void add_mul_ones();//function to add multiple ones to the queue
/////////////////////////////////////////////////////
void full_tab_cons(emp_consult T[10], que priority_tab[4]) ;//function to full the table of consultation
///////////////////////////////////////////////////////////////
void print_cons_tab(emp_consult T[10]);//function to print the cons_table
//////////////////////////////////////////////
void sort_time(emp_consult  T[10],int i);//function to sort by time(we call it if the reason is equal)
/////////////////////////////////////////
void sort_tab(emp_consult T[10]);//sort table by using reason and time if reason is equal
///////////////////////////////////////////
void get_current_date(char*temp );// Gets current date in DD/MM/YYYY format
////////////////////////////////////////////
void get_tomorrow_date(char* temp); // Gets tomorrow's date in DD/MM/YYYY format
///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int Tab_num_con(emp_consult T[10]);//function to calculate the number of consultation
/////////////////////////////////////////////////////////
int possible_reshulde(emp_consult T[10]);//function to verify we can reshulde(if we have not ergence reason)
//////////////////////////////////////////////////////////
int first_not_erg(emp_consult T[10]);//function to found the first not ergence
///////////////////////////////////////////////////////////////////////////
void print_priority_queues();//
///////////////////////////////////////////////////////////////////////////
void add_urgency(emp_consult T[10], emp_record *employee_head);//function to add one ergence (reshulde)
///////////////////////////////////////////////////////////////////////////
void update_emp_record(int id, emp_record *head);//UPDATE THE EMP RECORD
////////////////////////////////////////////////////////
void close_consultation(int emp_id, emp_record *head);// Enhanced close_consultation function
//////////////////////////////////////////////
void free_consultation_queues();// Function to free all consultation queues
////////////////////////////////////////////////////////
void update_emp_records_file(const char *filename, emp_record *head) ;
///////////////////////////////////////////////////////
void schedule_next_day_appointments(emp_record *head, que priority_tab[]);//function to schedule next day(periodic and return to work)
///////////////////////////////////////////
void update_cons_file(const char *filename, que priority_tab[]);
///////////////////////////////////////////////
