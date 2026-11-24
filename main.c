#include <stdio.h>
#include <stdlib.h>
#include "bib.h"

int main() {
    emp_record *employee_head = NULL;      // Head of linked list for employees
    emp_consult daily_consult[10];         // Array to hold up to 10 consultations per day
    int choice, id;

    char emp_file[] = "EmpRecords.txt";
    char cons_file[] = "Consultations.txt";

    // Initialize priority queue for 4 priority levels
    for (int i = 0; i < 4; i++) {
        priority_tab[i].h = NULL;
        priority_tab[i].t = NULL;
    }

    do {
        printf("***************TP 1 ALSDD has  been implemented by Hadil Chabbah and Sellam Samia*************\n");
        printf("\n=================== MENU ==================\n");
        printf("1. Load employee records\n");
        printf("2. Show employee records\n");
        printf("3. add employee (on demande)\n");
        printf("4. delete employee \n");
        printf("5. Load consultations (today)\n");
        printf("6. Add consultations (on demand)\n");
        printf("7. Show consultation queue\n");
        printf("8. full daily consultation schedule(from queue)\n");
        printf("9. print daily consultation scedule\n");
        printf("10. add urgent consultation \n");
        printf("11. Close a consultation\n");
        printf("12. Schedule next-day appointments\n");
        printf("13. Save updated records to files\n");
        printf("0. Exit\n");
        printf("========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fill_ll(emp_file, &employee_head);
                break;

            case 2:
                print_records(employee_head);
                break;
            case 3:
                 add(&employee_head);
                 break;
            case 4:
                  printf("enter the Id of employee to delete \n");
                  scanf("%d",&id);
                  Delete_Id(&employee_head,id);
                  break;
            case 5:
                fill_ll2(cons_file);  // Build the priority queues from today's consultations
                break;

            case 6:
                add_mul_ones();       // Add multiple on-demand consultations
                break;

            case 7:
                print_records2();     // Print all consultations in the queues
                break;

            case 8:
                full_tab_cons(daily_consult, priority_tab);  // Fill daily schedule (max 10)
                break;
            case 9:
                 print_cons_tab(daily_consult);
                 break;
            case 10:
                add_urgency(daily_consult,employee_head);
                break;
            case 11:
                printf("Enter Employee ID to close consultation: ");
                scanf("%d", &id);
                close_consultation(id, employee_head);
                full_tab_cons(daily_consult, priority_tab);  // Fill daily schedule (max 10)
                break;

            case 12:
              schedule_next_day_appointments(employee_head, priority_tab);
                break;

            case 13:
                update_emp_records_file(emp_file, employee_head);  // Save employee list
                update_cons_file(cons_file, priority_tab);         // Save consultation queue
                break;
            case 0:
                printf("Exit !!!...\n");
                free_list(employee_head);
                free_consultation_queues();
                break;

            default:
                printf("Invalid choice\n");
        }

    } while (choice != 0);

    return 0;
}
