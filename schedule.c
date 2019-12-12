/* Fill in your Name and GNumber in the following two comment fields
 * Name : Mithilaesh Jayakumar
 * GNumber : G01206238
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "structs.h"
#include "constants.h"

/* Schedule Count
 * - Return the number of Processes in the list.
 */

int schedule_count(Process *list) 
	{
		int process_count = 0;		//Variable to store the number of processes in the list                                    
		Process *current_process = list;  //Pointer to iterate through each process in the list
		
		while(current_process != NULL)		//Iterate through the list until it reaches the end
			{
				process_count++;
				current_process = current_process->next; 
			}
		
		return process_count;
	}

/* Schedule Insert
 * - Insert the Process into the List with the Following Constraints
 *   list is a pointer to the head pointer for a singly linked list.
 *   It may begin as NULL
 *   1) Insert in the order of Ascending PIDs
 *   (eg. 1, 13, 14, 20, 32, 55)
 * Remember to set list to the new head if it changes!
 */

void schedule_insert(Process **list, Process *node)
{
	if(node == NULL)
		{
			return NULL;
		}		

	else
		{
			if((*list) == NULL)	//Assign the node to list if the list is NULL
				{		
					(*list) = node;
				}		
	
			else if((*list)->pid >= node->pid)	//Insert before head node if it's pid greater than the incoming node
				{
					node->next = (*list);
					(*list) = node;
				}
	
			else	//Insert in-between the list
				{
					Process *current_process = (*list);
			
					while(current_process->next != NULL && current_process->next->pid < node->pid)	//Iterate through if current node pid is less
						{
							current_process = current_process->next;
						}
			
					node->next = current_process->next;	//Insert before the current node whose pid is greater	
					current_process->next = node;
				}
		}
}

/* Schedule Terminate
 * - Unallocate the Memory for the given Node
 */

void schedule_terminate(Process *node)
{
		free(node);	//Clear the memory of the passed node
}

/* Schedule Generate
 * - Allocate the Memory and Create the Process from the given Variables
 *   Remember to initialize all values, including next!
 */

Process *schedule_generate(const char *name, int pid, int time_remaining, int time_last_run) 
{
	Process *node = NULL;
	node = (Process *)malloc(sizeof(Process));	//Dynamic memory allocation for the node to be created
	
	if(node == NULL)
		{
			return NULL;
		}
	
	else //Assigning the values to the created node
		{
			strncpy(node->name,name,sizeof(node->name));
			node->pid = pid;
			node->time_remaining = time_remaining;
			node->time_last_run = time_last_run;
			node->next = NULL;
			
			return node;
		}
}

/* Schedule Select
 * - Select the next Process to be run using the following conditions:
 *   1) The Process with the lowest time_remaining should run next.
 *   - If there are any ties, select the Process with the lowest PID
 *   2) If there is a Process that has not run in >= TIME_STARVATION, then
 *      select that one instead.
 *      (Use the function clock_get_time() to get the current time)
 *   - If there are any ties, select the Process with the lowest PID
 *   3) If the list is NULL, return NULL
 * - Remove the selected Process from the list
 * - Return the selected Process
 */

Process *schedule_select(Process **list) 
{
	if((*list) != NULL) 
		{
			Process *selected_process = NULL;
			Process *current_process = (*list);

			while(current_process != NULL)	//To check for the process with starvation
				{
					if((clock_get_time() - current_process->time_last_run) >= TIME_STARVATION)
						{
							selected_process = current_process;
							break;
						}
					current_process = current_process->next;
				}
	
			if(selected_process == NULL)	//If there's no starvation then select based on time_remaining
				{
					selected_process = (*list);
					current_process = (*list);
	
					while(current_process != NULL)
						{
							if(current_process->time_remaining < selected_process->time_remaining)
								{
									selected_process = current_process;
								}
							current_process = current_process->next;
						}
				}		

			current_process = (*list);	//Reset the current to start of the list
			Process *previous_process = NULL;

			while(current_process != NULL) //Logic to remove the selected process
				{
					if(current_process == selected_process)
						{
							if(previous_process != NULL)	//Adjust the list if the selected process is not the head
								{
									previous_process->next = current_process->next;
									current_process->next = NULL;
									return current_process;	
								}
							else	//Adjust the list if the selected process is the head
								{
									(*list) = current_process->next;
									current_process->next = NULL;
									return current_process;
								}
						}
					previous_process = current_process;
					current_process = current_process->next;
				}
		}
	return NULL;
}
