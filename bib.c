#include <stdio.h>
#include <stdlib.h>
#include "bib.h"
void Allocate (emp_record **p )//create new node of emp record
{
    *p = malloc(sizeof(emp_record));
}

void Ass_adr(emp_record *p, emp_record *q) {  //access to the node
    p->adr = q;
}
emp_record* Next(emp_record *p) { //give the next
    return p->adr;
}

// Function to create a new node
emp_record* create_node(int id, char *name, int cons_num, char *last_date, char *ret_work, char *history_line) {
    emp_record *new_node = (emp_record*)malloc(sizeof(emp_record));//ALLOCATE (new_node)
    new_node->info.Emp_Id = id;
    strncpy(new_node->info.name, name, NAME_LEN - 1);//copy name
    new_node->info.name[NAME_LEN - 1] = '\0';

    new_node->info.cons_num = cons_num;
    strncpy(new_node->info.last_date, last_date, DATE_LEN - 1);
    new_node->info.last_date[DATE_LEN - 1] = '\0';

    if (strlen(ret_work) > 0) {//return to work field exist
        strncpy(new_node->info.ret_work, ret_work, DATE_LEN - 1);
        new_node->info.ret_work[DATE_LEN - 1] = '\0';
    } else {
        strcpy(new_node->info.ret_work, "/");// return to work date doesn't exist
    }

    for (int i = 0; i < MAX_HISTORY; i++) {
        new_node->info.history[i][0] = '\0';
    }

    // Parse history reasons
    char *reason = strtok(history_line, ",");
    int i = 0;
    while (reason != NULL && i < MAX_HISTORY) {
        strncpy(new_node->info.history[i], reason, NAME_LEN - 1);//copy reason number i
        new_node->info.history[i][NAME_LEN - 1] = '\0';
        reason = strtok(NULL, ",");
        i++;
    }

    new_node->adr = NULL;
    return new_node;
}
// function to get a valid reason from the user
char* valid_reason() {
    char* reason = (char*)malloc(NAME_LEN* sizeof(char));
    printf("Enter consultation reason (Work-accident/Occupational-disease/Return-to-Work/Pre-employement/Periodic): ");
    scanf(" %[^\n]", reason);
    while (strcmp(reason, "Work-accident") != 0 && // this loop will be repeated until get a valid reason
           strcmp(reason, "Occupational-disease") != 0 &&
           strcmp(reason, "Return-to-Work") != 0 &&
           strcmp(reason, "Pre-employement") != 0 &&
           strcmp(reason, "Periodic") != 0) {
        printf("Invalid reason. Please enter one of: Work-accident, Occupational-disease, Return-to-Work, Pre-employement, Periodic\n");
        scanf(" %[^\n]", reason);
    }
    return reason;
}
////////////////////////////////////////////////
int id_unicity(emp_record *head, int id) {// to check if the id already exist or not
    emp_record *save=head;
    while (save != NULL) {
        if (save->info.Emp_Id == id) return 0;/// the id exist(not unique)
        save= save->adr;
    }
    return 1;// if the id doesn't exist(unique)
}

// Function to insert a node at the end of the linked list
void add_node(emp_record **head, emp_record *new_node) {
    if (*head == NULL) {
        *head = new_node;
        return;
    }

    emp_record *temp = *head;
    while (temp->adr != NULL) {
        temp = temp->adr;
    }
    temp->adr = new_node;
}
//function to add new one to the list
void add(emp_record **head) {
    Emplyee emp; //emp is record of all the information
    full_record(&emp,*head); //remplir emp by the information add id from utilisateur

    emp_record *new_node = create_node(emp.Emp_Id, emp.name, emp.cons_num, emp.last_date, emp.ret_work, emp.history); //crete new node to the list with the information add it previosly
    if (new_node != NULL) { //cas if the employee is added
        add_node(head, new_node);
        printf("Employee added successfully.\n");
    } else {
        printf("Error adding employee.\n"); //it not added
    }
}


////////////////////////////////////////////////////////

void full_record(Emplyee *emp,emp_record*head){ //function to full the record of employee
    int id_valid = 0;//boolean variable
    while (!id_valid) { //boocle
        printf("Enter Employee ID: ");
        scanf("%d", &emp->Emp_Id);
         getchar();//
        if (!id_unicity(head, emp->Emp_Id)) {  // Check unicity of the id
            printf("Error: Employee ID %d already exists!\n", emp->Emp_Id);
        } else {
            id_valid = 1; // ID is unique
        }
    }
    printf("Enter Name: ");
    fgets(emp->name, NAME_LEN, stdin); //we use fget to inter the espace between full name and firstname
    emp->name[strcspn(emp->name, "\n")] = '\0';  // Remove trailing newline

    printf("Enter Number of Consultations: ");
    scanf("%d", &emp->cons_num);
    getchar();  // Consume leftover newline and delete /n

    get_current_date(emp->last_date);
    printf("enter the Last Consultation Date : %s\n", emp->last_date);
    printf("Enter Return to Work Date (DD/MM/YYYY) or '/' if unknown: ");
    scanf(" %[^\n]", emp->ret_work);
    int cpt;
    if (emp->cons_num<=5)  {cpt=emp->cons_num;}//the number of reason to inter it the number of consultation
    else {cpt=5;}  // if the number of consultation more than 5 we inter 5 reason
    for (int i = 0; i <cpt ; i++) { //boocle to inter the reason
        printf("*************reason %d ****************\n",i+1);
        char *reason = valid_reason();
        strcpy(emp->history[i], reason); // Copy the reason to the table


    }
}
// Function to read from file and fill the linked list
void fill_ll(const char *fname, emp_record **head) {
    FILE *file = fopen(fname, "r");
    if (file == NULL) {
        printf("The file doesn't exist.\n");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        int emp_id, cons_num;
        char name[NAME_LEN] = "", last_date[DATE_LEN] = "", ret_work[DATE_LEN] = "", history_line[HISTORY_LEN] = "";

        int fields = sscanf(line, "%d;%29[^;];%d;%10[^;];%10[^;];%149[^\n]",
                            &emp_id, name, &cons_num, last_date, ret_work, history_line);

        if (fields < 6 || strlen(ret_work) == 0) {
            strcpy(ret_work, "/");
            sscanf(line, "%d;%29[^;];%d;%10[^;];;%149[^\n]", &emp_id, name, &cons_num, last_date, history_line);
        }
        if ( strlen(name) == 0 || strlen(last_date) == 0) {
            printf("Skipping invalid record: %s\n", line);
            continue;
        }

        emp_record *new_node = create_node(emp_id, name, cons_num, last_date, ret_work, history_line);
        add_node(head, new_node);
    }

    fclose(file);
}

////////////*************** Function to print employee records******************/////////////////////
void print_records(emp_record *head) {
    while (head != NULL) {
        printf("\n The name : %s\n", head->info.name);
        printf(" The ID : %d\n", head->info.Emp_Id);
        printf(" The number of consultations  : %d\n", head->info.cons_num);
        printf(" The date of the last consultation  : %s\n", head->info.last_date);
        printf(" The date to return to work  : %s\n", head->info.ret_work);
        printf(" Printing the last 5 reasons:\n");
        for (int i = 0; i < MAX_HISTORY; i++) {// print the history reason
            if (strlen(head->info.history[i]) > 0) {
                printf(" The %d reason is: %s\n", i + 1, head->info.history[i]);
            } else {
                printf(" The %d reason is: \n", i + 1);
            }
        }
        head = head->adr;
        printf("_____________________________________\n");
    }
}

////////****************** Function to free memory***************////////////////
void free_list(emp_record *head) {
    emp_record *temp;
    while (head != NULL) {
        temp = head;
        head = head->adr;
        free(temp);
    }
}
////////***********************access by id function **********************//////////////////
void access_Id (emp_record*h,int Id ,emp_record**p,emp_record**q){//h=head
      *p=h;//to save the head
      *q=NULL;//to pointe the previous of p
      int found=0;
      while(*p!=NULL && found ==0)
      {
            if((*p)->info.Emp_Id==Id){
            found=1;
            }
        else {  *q=*p;
                *p=(*p)->adr;
           }
      }
}


/////////////////////////////////////////////////
//function to delete employee from the list by using the ID
void Delete_Id(emp_record ** head,int Id)
{
    emp_record * p;//pointer vers emp_record to use it the call of access_ID
    emp_record *q;
    access_Id(*head,Id,&p,&q);//p point vers the employee we need to delete it and q pointe vers the previous
    if (p!=NULL) //the emloyee is in the begining or the middle
    {
     if (p==*head) //the employee is in the begining
     {
      *head=(*head)->adr;//we change the head
     }
     else  //in the middle
        Ass_adr(q,Next(p));
      free(p);
    }
    else printf("the employer doesn't exist");
}
//////////////////////////////////////////////////////////

//function to update the history
void update_history(emp_record * head,int Id,char reason[NAME_LEN])
{
if (head!=NULL)  //the list exist
{   emp_record *p, *q;
    access_Id(head, Id, &p, &q);
   if (p!=NULL) //the id exist
   {
   int i=p->info.cons_num; //we effect to i the number of consultation
   if(i<5) //the history reason less then 5
   {
   strcpy(p->info.history[i], reason);
   }
   else{ //if the 5 reason full
    for (int j=0;j<4;j++)
    {
    strcpy(p->info.history[j],p->info.history[j+1] ); //shift the reason
    }
    strcpy(p->info.history[4],reason);//the reason be in the lest one(5)
   }
   p->info.cons_num++;//increment number of consultation
}
else {//the ID doesn't exist
    printf("the ID doesn't exist");
}

} //the file is empty if head is null
else printf("the file is empty");
}


//function to update the date of last date or the date of return to work
void update_date(emp_record * head,int Id,char date[10],int choix)
{
if (head!=NULL)
{  emp_record *p, *q;
    access_Id(head, Id, &p, &q);
   if (p!=NULL)
   {if (choix==1)//choix 1 means we want to update the last date
{
   strcpy(p->info.last_date, date);
}
else //update return to work
{
   strcpy(p->info.ret_work, date);
}
}
else printf("the Id doesn't exist");//if p is null
}
else printf("your file is empty");//if head is null
}
//**********************second part**********************************
//FUNCTION TO create a node that contains emp_consult record and the address to the next
cons_node *create_node2(int id, char *name, char *cons_time, char *cons_reason) {
    cons_node *new_node = (cons_node*)malloc(sizeof(cons_node));//allocate(new_node)
    new_node->cons_rec.Emp_Id = id;//affect employee id to 'cons_rec.Emp_Id'
    strncpy(new_node->cons_rec.name, name, NAME_LEN-1);// Copy the name
    new_node->cons_rec.name[NAME_LEN-1] = '\0';//add the nil termination at the end of the name
    int i;
    for (i = 0; cons_time[i] && i < 5; i++) {
        new_node->cons_rec.cons_time[i] = cons_time[i];
    }
    new_node->cons_rec.cons_time[i] = '\0';
    strncpy(new_node->cons_rec.cons_reason, cons_reason, NAME_LEN-1); // Copy consultation reason
    new_node->cons_rec.cons_reason[NAME_LEN-1] = '\0';

    new_node->ad = NULL;
    return new_node;
}
///************** Function to full the table of priority***************************/////////////////////////////////////////////
void full_tab_prio(cons_node *n) {
    // Priority assignment based on consultation reason
    if (strcmp(n->cons_rec.cons_reason, "Work-accident") == 0) {// Highest priority (0)
        if (priority_tab[0].h == NULL) {// the list doesn't exist
            priority_tab[0].t = n;
            priority_tab[0].h = n;//create the list
        } else {// insert node to the list at the end
            priority_tab[0].t->ad = n;
            priority_tab[0].t = n;
        }
    }
    else if (strcmp(n->cons_rec.cons_reason, "Occupational-disease") == 0) {// Priority 1
        if (priority_tab[1].h == NULL) {// the list doesn't exist
            priority_tab[1].t = n;
            priority_tab[1].h = n;//create the list
        } else {// insert node to the list at the end
            priority_tab[1].t->ad = n;
            priority_tab[1].t = n;
        }
    }
    else if (strcmp(n->cons_rec.cons_reason, "Return-to-Work") == 0 ||strcmp(n->cons_rec.cons_reason, "Pre-employement") == 0) { // Priority 2 (same for both reasons)
        if (priority_tab[2].h == NULL) {
            priority_tab[2].t = n;
            priority_tab[2].h = n;
        } else {
            priority_tab[2].t->ad = n;
            priority_tab[2].t = n;
        }
    }
    else if (strcmp(n->cons_rec.cons_reason, "Periodic") == 0) { // Lowest priority (3)
        if (priority_tab[3].h == NULL) {
            priority_tab[3].t = n;
            priority_tab[3].h = n;
        } else {
            priority_tab[3].t->ad = n;
            priority_tab[3].t = n;
        }
    }

}
//////////*****************FUNCTION TO READ FROM FILE OF CONSULTATIONS***********************///////////////////////////
 void fill_ll2(const char *fname) {
    FILE *file = fopen(fname, "r");//open file in read mode
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline and trailing whitespace
        line[strcspn(line, "\n")] = '\0';
        while (strlen(line) > 0 && isspace(line[strlen(line) - 1])) {
            line[strlen(line) - 1] = '\0';
        }
        if (strlen(line) == 0) continue;
        int emp_id = 0;
        char name[NAME_LEN] = "";
        char cons_time[6] = "00:00";
        char cons_reason[NAME_LEN] = "";
        // Parse ID
        char *token = strtok(line, ";");
        if (!token || sscanf(token, "%d", &emp_id) != 1) continue;
        // Parse name
        token = strtok(NULL, ";");
        if (!token) continue;
        strncpy(name, token, NAME_LEN - 1);
        name[NAME_LEN - 1] = '\0';
        // Parse time
        token = strtok(NULL, ";");
        if (token) {
            char *h_pos = strchr(token, 'h');
            if (h_pos) *h_pos = ':';

            if (strlen(token) == 3 && token[2] == ':') {
                snprintf(cons_time, sizeof(cons_time), "%s00", token);
            } else {
                strncpy(cons_time, token, 5);
                cons_time[5] = '\0';
            }
        }
        // Parse reason
        token = strtok(NULL, "\0");
        if (token) {
            while (strlen(token) > 0 && isspace(token[strlen(token) - 1])) {
                token[strlen(token) - 1] = '\0';
            }
            strncpy(cons_reason, token, NAME_LEN - 1);
            cons_reason[NAME_LEN - 1] = '\0';
        }
        // Create node and insert
        cons_node *node = create_node2(emp_id, name, cons_time, cons_reason);
        full_tab_prio(node);
    }
    fclose(file);
}


//Function to print the record
void print_records2() {
    for (int i = 0; i < 4; i++) {
        cons_node *current = priority_tab[i].h;
        while (current != NULL) {
            printf("\nName: %s\n", current->cons_rec.name);
            printf("ID: %d\n", current->cons_rec.Emp_Id);
            printf("Time: %.5s\n", current->cons_rec.cons_time); // Ensure only 5 chars for time
            printf("Reason: %s\n", current->cons_rec.cons_reason);
            printf("_____________________________________\n");
            current = current->ad;//access to the next
        }
    }
}
//function to remplir record of consultation (reason is Occupational-disease or Work-accident or Pre-employement)
void full_record2(emp_consult *emp) {
    printf("Enter Employee ID: ");
    scanf("%d", &emp->Emp_Id);
    while(getchar() != '\n'); //

    printf("Enter Name: ");
    fgets(emp->name, NAME_LEN, stdin);//we use fgets to can inter the espace between fullname and firstname
    emp->name[strcspn(emp->name, "\n")] = '\0';//we use it with the fgets to delite /n

    printf("Enter the consultation time: ");
    fgets(emp->cons_time, 6, stdin); // Read exactly 5 characters (hh:mm)
    emp->cons_time[strcspn(emp->cons_time, "\n")] = '\0';//we use it with the fgets to delite /n
    char *reason = valid_reason();
    while(strcmp(reason, "Return-to-Work") == 0 ||//only accept the valid reason
          strcmp(reason, "Periodic") == 0) {
    printf("Urgent reason must be Work-accident or Occupational-disease or Pre-employement \n");//print to inter the true reason
    free(reason);
    reason = valid_reason();//call the function to inter the reason
    }
    strncpy(emp->cons_reason, reason, NAME_LEN-1);//inialize the reason to the cons-reason
    free(reason);//free the reason because it is poiter vers char

}
//function to add multiple ones to the queue
void add_mul_ones() {
    int num; //variable to know the number of employee you want to add it
    printf("what is the number of employee you want to add? ");
    scanf("%d", &num);
    while(getchar() != '\n'); // Clear the newline after reading number

    for (int i = 0; i < num; i++) {
        printf("\n--- add tha employee %d/%d ---\n", i+1, num);
        emp_consult infor;
        full_record2(&infor);//ask user to inter the information of the new employee
        cons_node* newn = create_node2(infor.Emp_Id, infor.name, infor.cons_time, infor.cons_reason);
        if(newn != NULL) {//if the employee add it succefully
            full_tab_prio(newn);//add the new employee to queue in the good place
            printf("Employee added successfully.\n");
        } else  //the employee doesn't add
            printf("Error adding employee.\n");
        }
    }

//function to full the table of consultation
void full_tab_cons(emp_consult T[10], que priority_tab[4]) {
    // Initialize table with empty values
    for (int i = 0; i < 10; i++) {
        T[i].Emp_Id = 0;
        T[i].name[0] = '\0';
        T[i].cons_time[0] = '\0';
        T[i].cons_reason[0] = '\0';
    }

    int index = 0;//use this variable like an index to the table

    // Process queues in priority order (0 to 3)
    for (int prio = 0; prio < 4; prio++) {
        cons_node *current = priority_tab[prio].h;//the current take the value of head of priority tab with index prio

        while (current != NULL && index < 10) {
            // Copy employee data
            T[index].Emp_Id = current->cons_rec.Emp_Id;
            strcpy(T[index].name, current->cons_rec.name);

            // Convert time format from 8h30 to 8:30
            int j;
            for (j = 0; current->cons_rec.cons_time[j] && j < 5; j++) {
                T[index].cons_time[j] = (current->cons_rec.cons_time[j] == 'h') ? ':' : current->cons_rec.cons_time[j];
            }
            T[index].cons_time[j] = '\0';

            // Copy consultation reason
            strcpy(T[index].cons_reason, current->cons_rec.cons_reason);
            current = current->ad;//current poite vers the next
            index++;//increment the index
        }
    }
}
//function to print the cons_table
void print_cons_tab(emp_consult T[10]) {
    printf("\nDaily Consultation Schedule:\n");
    printf("===========================\n");

    for (int i = 0; i < 10; i++) {
        if (T[i].Emp_Id == 0) continue;

        printf("\nconsultation %d:\n", i+1);//print the number of consultation
        printf("  ID:     %d\n", T[i].Emp_Id);
        printf("  Name:   %s\n", T[i].name);

        // Format time for display
        char display_time[6];
        strncpy(display_time, T[i].cons_time, 5);
        display_time[5] = '\0';
        printf("  Time:   %s\n", display_time);

        printf("  Reason: %s\n", T[i].cons_reason);
    }
    printf("\n===========================\n");
}
//function to sort by time(we call it if the reason is equal)
void sort_time(emp_consult  T[10],int i) {
    emp_consult temp; // Temporary variable for swapping
    if (strcmp(T[i].cons_time, T[i + 1].cons_time) > 0) { //it means T[i].cons_time more than T[i+1].cons_time
        //swap T[i] and T[i+1]
        temp = T[i];
        T[i] = T[i + 1];
        T[i + 1] = temp;
        }
        }
//sort table by using reason and time if reason is equal
void sort_tab(emp_consult T[10])
    {
      for (int i=0;i<9;i++)
      {
         if (strcmp(T[i].cons_reason , T[i + 1].cons_reason) == 0)//if reason are equal we sort by time
         {
              sort_time(T,i);
         }
      }
    }

////////////////////////////////////////////////////////////////

// Gets current date in DD/MM/YYYY format
void get_current_date(char*temp ) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(temp, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
}

// Gets tomorrow's date in DD/MM/YYYY format
void get_tomorrow_date(char* temp) {
    time_t now = time(NULL);
    now += 24 * 60 * 60; // Add one day in seconds
    struct tm *t = localtime(&now);
    sprintf(temp, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
}


//////////////////////////////////////////////////////////////////
//function to calculate the number of consultation
int Tab_num_con(emp_consult T[10])
{
    int count = 0;//counter to calculate the number of consultation in the table

    for(int i = 0; i < 10; i++)
    {

        if((T[i].Emp_Id != 0) && (strlen(T[i].name) > 0 )) //if it is not empty
        {
            count++;
        }
    }

    return count;//the output
}
/////////////////////////////////////////////////////////
//function to verify we can reshulde(if we have not ergence reason)
int possible_reshulde(emp_consult T[10])
{
    int i=0;//integer to use it like an index in table of consultation
    int poss; //boolean to verify if reason not ergence is found so we can do reashould
    poss=0;
  while (poss==0 && i<10)
   {
     if (strcmp(T[i].cons_reason,"Return-to-Work")==0 || strcmp(T[i].cons_reason,"Periodic")==0 || strcmp(T[i].cons_reason,"Pre-employment") )
     {
          poss=1;//we found the reason not ergence so we can do schouloud
     }

        i++;//increment i
   }
   return poss;
}
//////////////////////////////////////////////////////////
//function to found the first not ergence
int first_not_erg(emp_consult T[10])
{
    for (int i = 0; i < 10; i++) {
        if (T[i].Emp_Id != 0 &&  // Check if consultations is exist
            strcmp(T[i].cons_reason, "Return-to-Work") == 0 ||  //if reason is not ergence
             strcmp(T[i].cons_reason, "Periodic") == 0 ||
             strcmp(T[i].cons_reason, "Pre-employement") == 0) {
            return i;//return the position of the first not ergence
        }
    }
    return -1;  // No suitable cons found
}
///////////////////////////////////////////////////////////////////////////
///////////////************function to print priority queue***********///////////////////////

///////////////////////////////////////////////////////////////////////////
void add_urgency(emp_consult T[10], emp_record *employee_head) {
    // Check if rescheduling is possible
    if (possible_reshulde(T) != 1) { //we can't do reshulde
        printf("Cannot add urgent appointment.\n");
        return;
    }
    int num = Tab_num_con(T);
    int pos = first_not_erg(T);
    // Validate position
    if (pos < 0 || pos >= num) {  //pos is negative or more than number of consultation means doesn't exist
        printf("No suitable appointment slot found for urgent case.\n");
        return;
    }
    // Save original time
    char original_time[6] = {0};
    strncpy(original_time, T[pos].cons_time, sizeof(original_time)-1);
    // Shift appointments
    if (num < 10) { //the table doesn't full
        for (int i = num; i > pos; i--) {
            T[i] = T[i-1];
        }
        num++;
    } else { //the table of consultation is full
        // Reschedule last appointment
        int reschedule_emp_id = T[9].Emp_Id;//save it's id
        char employee_name[NAME_LEN];//variable to save the name
        strncpy(employee_name, T[9].name, NAME_LEN-1);//save it's name
        employee_name[NAME_LEN-1] = '\0';

        for (int i = 8; i >= pos; i--) { //shift the consultation
            T[i+1] = T[i];
        }
        char tomorrow_date[DATE_LEN]; //update the date of the last one if it is full
        get_tomorrow_date(tomorrow_date);//it take the date of tomorrow
        update_date(employee_head, reschedule_emp_id, tomorrow_date, 1);//update it's date in the file
        printf("Employee %s's appointment rescheduled to tomorrow.\n", employee_name);
    }
    // Get urgent appointment details
    printf("Enter urgent employee ID: ");
    scanf("%d", &T[pos].Emp_Id);
    while(getchar() != '\n');
    printf("Enter urgent employee name: ");
    fgets(T[pos].name, NAME_LEN, stdin);
    T[pos].name[strcspn(T[pos].name, "\n")] = '\0';//to delete "/n"
   // Only allow urgent reasons
   char *reason = valid_reason();
   while(strcmp(reason, "Work-accident") != 0 &&//only accept the valid reason
      strcmp(reason, "Occupational-disease") != 0) {
    printf("Urgent reason must be Work-accident or Occupational-disease\n");
    free(reason);
    reason = valid_reason();//call the function to inter the reason
    }
    strncpy(T[pos].cons_reason, reason, NAME_LEN-1);
    T[pos].cons_reason[NAME_LEN-1] = '\0';
    free(reason);//free the reason because it is poiters
    strncpy(T[pos].cons_time, original_time, sizeof(T[pos].cons_time)-1);//the new consultation takes the time of the place witch is placed

    printf("Urgent appointment added successfully .\n");
}
//////////////////////////////////////////////////////////////////////////////////////////////
//////////************************UPDATE THE EMP RECORD**************************//////////////
void update_emp_record(int id, emp_record *head) {
   emp_record *p, *q;
   access_Id(head, id, &p, &q);// call the function to find the employee by ID

    if (p == NULL) { // special case 1 if the employee doesn't exist
        printf("the employee doesn't exist in this list.\n");
        return;
    }
    char current_date[DATE_LEN];// Get current date for last consultation
    get_current_date(current_date);
    // Update last consultation date
   update_date(head, id,current_date,1);
    // Get consultation reason from user
    char *reason=valid_reason();
     while(strcmp(reason, "Pre-employement") == 0 ) { // loop until the user enter a valid reason in case of update
    printf("warning:the reason to do updates can't be 'Pre-employement'\n");
    free(reason); // free invalid reason memory
    reason = valid_reason();// get new reason
    }
    // Update consultation history
    update_history(head, id, reason);
    // Handle special cases
    if(strcmp(reason,"Return-to-Work")==0){// IN this case the user enter the return to work date
        char ret_work_date[DATE_LEN];
            printf("Enter return to work date (DD/MM/YYYY): ");
            scanf("%10s", ret_work_date);
            update_date(head, id,ret_work_date,0);
            }
            else{
                 update_date(head, id,"/",0);// the field of return to work date doesn't exist
            }
      free(reason);
    printf("Employee record updated successfully.\n");
}
//////***************** close_consultation function***************////////////////////////////////
void close_consultation(int emp_id, emp_record *head) {
    if (head == NULL) {// if the queue of consultations is empty
        printf("the  employee queue doesn't exist.\n");
        return;
    }
    int found = 0;// TO CHECK IF THE EMPLOYEE EXIST OR NOT IN CONSULTATION QUEUE
    for (int i = 0; i < 4; i++) {// LOOP TO ACCESS AL THE ELEMENT IN THE PREIORITY TAB
        cons_node *current = priority_tab[i].h;//initialize cuurent to h(head) to save the head
        cons_node *prev = NULL;
        while (current != NULL) {// LOOP TO ACCESS AL THE ELEMENT IN THE LIST INSIDE THE ELEMENTS OF PRIORITY TAB
            if (current->cons_rec.Emp_Id == emp_id) {// the employee is found in the queue
                found = 1;
               char *reason=valid_reason();/// call this function to get a valid reason from user
               if(strcmp(reason,"Pre-employement")==0){ // special case . in this case the employee must add to the list of employees
                printf("\n*******************add the employee to the list of employees************************\n\n");
                add(&head);////function to add new one to the list
               }else{ // normal case , update existing employee record
                      printf("\n*******************update employee record***************** \n\n");
                      update_emp_record(emp_id, head);}
                // Then remove from queue
               if (prev == NULL) {//means the employee is found in the first node
                   priority_tab[i].h = current->ad;// remove the head
                   // If queue becomes empty after removal, update tail pointer
                if (priority_tab[i].h == NULL) {
                        priority_tab[i].t = NULL;
                    }
                }
                else {
                    prev->ad = current->ad;
                    if (current == priority_tab[i].t) {// if removing last node then  update tail pointer
                        priority_tab[i].t = prev;
                    }
                }
                free(current);
                printf("Consultation closed for Employee ID: %d\n", emp_id);
                return;
            }
            prev = current;
            current = current->ad;
        }
        }
}
///////////////////////////////////////////////////////////////////////////////////////
/////**************** Function to free all consultation queues*************////////////
void free_consultation_queues() {
    for (int i = 0; i < 4; i++) {// LOOP TO ACCESS AL THE ELEMENT IN THE PREIORITY TAB
        cons_node *save = priority_tab[i].h;
        while (save != NULL) {// LOOP TO ACCESS AL THE ELEMENT IN THE LIST INSIDE THE ELEMENTS OF PRIORITY TAB
            cons_node *temp = save;//affect save to the pioter temp to preserve save and use temp to free memoery
            save = save->ad;
            free(temp);
        }
        priority_tab[i].h = NULL;
        priority_tab[i].t = NULL;
    }
}
/////////////////////////////////////////////////////////////////////////
void schedule_next_day_appointments(emp_record *head, que priority_tab[]) {

    char tomorrow_date[DATE_LEN];//variable to tomorrow day
    get_tomorrow_date(tomorrow_date);//call function to have tomorrow

    int count = 0;//counter of number schedule_next_day_appointments
    emp_record *current = head;//pointer to  parcour the record of employement

    // Schedule Return-to-Work appointments
    while (current != NULL) {
        if (strcmp(current->info.ret_work, tomorrow_date) == 0) {//if the date of return to work of the employee is tomorrow
                cons_node *new_node = create_node2(current->info.Emp_Id, //create new node with 8in the time
                                                 current->info.name,
                                                 "08:00",
                                                 "Return-to-Work");
                if (new_node) {//if the new node created
                    if (priority_tab[2].h == NULL) { //insert in the head
                        priority_tab[2].h = priority_tab[2].t = new_node;
                    } else { //insert in the end
                        priority_tab[2].t->ad = new_node;
                        priority_tab[2].t = new_node;
                    }
                    count++;//increment counter
                }

        }
        current = current->adr;//current pointer vers the next
    }

    // Schedule Periodic appointments
    current = head;
    while (current != NULL) {
            int last_day, last_month, last_year;
            int tomorrow_day, tomorrow_month, tomorrow_year;

            if (sscanf(current->info.last_date, "%d/%d/%d", &last_day, &last_month, &last_year) == 3 &&//copy the day;month;and year of last day
                sscanf(tomorrow_date, "%d/%d/%d", &tomorrow_day, &tomorrow_month, &tomorrow_year) == 3) {//copy the day;month and year of tomorrow date

                if (tomorrow_day == last_day &&//the same day
                    tomorrow_month == last_month &&//the same month
                    tomorrow_year == last_year + 1) {//if it pass one year

                    cons_node *new_node = create_node2(current->info.Emp_Id,
                                                     current->info.name,
                                                     "09:00",
                                                     "Periodic");//create new node
                    if (new_node) {//insert in the head
                        if (priority_tab[3].h == NULL) {
                            priority_tab[3].h = priority_tab[3].t = new_node;
                        } else {//insert in the end
                            priority_tab[3].t->ad = new_node;
                            priority_tab[3].t = new_node;
                        }
                        count++;//increment
                    }
                }
            }

        current = current->adr;//current pointer vers the next
    }
    printf("the number of employee reshuld to tommorrow %d\n",count);
}
///////////**********function to update EmpRecords file*******///////////
void update_emp_records_file(const char *filename, emp_record *head) {
    if (head == NULL) {
        printf("the list of employee is empty.\n");
        FILE *file = fopen(filename, "w");  // Create an empty file
        fclose(file);
        return;
    }
    FILE *file = fopen(filename, "w");// create and open file in write mode
    emp_record *save = head;///initialize the pionter save to the head to save the head
    while (save != NULL) {// loop to access all elements of priority queue
            //// write to file
        fprintf(file, "%d;%s;%d;%s;", save->info.Emp_Id , save->info.name , save->info.cons_num , save->info.last_date);
        if (strlen(save->info.ret_work) > 0 && strcmp(save->info.ret_work, "/") != 0) {
            fprintf(file, "%s;", save->info.ret_work);
        } else {
            fprintf(file, ";");// if the field of retun to work date doesn't exist
        }
        for (int i = 0; i < MAX_HISTORY; i++) { // loop to access all element of the table of history to write history
            if (strlen(save->info.history[i]) > 0) {
                fprintf(file, "%s", save->info.history[i]);
                if (i < MAX_HISTORY - 1 && strlen(save->info.history[i + 1]) > 0) {
                    fprintf(file, ",");//','the delemeter between reasons
                }
            }
        }
        fprintf(file, "\n");
        save = save->adr;
    }
    fclose(file);//close the file
    printf("Employee records saved to %s.\n", filename);
}
////////////////////////////////////////////////////////////////////////////////////////
////////////**********FUNCTION TO UPDATE CONSULTATIONS FILE*************////////////////
void update_cons_file(const char *filename, que priority_tab[]) {
    FILE *file = fopen(filename, "w");//open the file in write mode
    for (int prio = 0; prio < 4; prio++) {
        cons_node *save = priority_tab[prio].h;///initialize the pionter save to the head to save the head
 while (save != NULL) {// loop to access all elements of priority queue
     char time_str[6];// Convert time format from "8:30"  to "8h30"
    strncpy(time_str, save->cons_rec.cons_time, 5);
      time_str[5] = '\0';
       for (int i = 0; i < 5; i++) {// Replace' :' with 'h'
                if (time_str[i] == ':') {
                    time_str[i] = 'h';
                    break;
                }
            }
   fprintf(file, "%d;%s;%s;%s\n", save->cons_rec.Emp_Id, save->cons_rec.name, time_str, save->cons_rec.cons_reason);

    save = save->ad;//pass the pointer to the next node while its<>nil
        }
    }

    fclose(file);
    printf("Consultations saved to %s successfully.\n", filename);
}
//////////////////////////////////////////////////////////////////
