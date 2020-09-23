/* Fill in your Name and GNumber in the following two comment fields
 * Name: Deep Kumar
 * GNumber: G01215145
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
int schedule_count(Process *list) {
	
	// check is list is null
    if(list == NULL) {
		    return 0;
	  }
  	// recursive solution for counter
  	return 1 + schedule_count(list->next);
}

/* Schedule Insert
 * - Insert the Process into the List 
 * - insertion in sorted order of pid.
 */
void schedule_insert(Process **list, Process *node) { 
  
  	Process *current; 
    // Special case for the head node
    if (*list == NULL || (*list)->pid >= node->pid) { 
		node->next = *list; 
        *list = node; 
    } else { 
        current = *list; 
		// Locate the position to insert new node 
		while (current->next!=NULL && current->next->pid < node->pid) { 
			current = current->next; 
        } 
        node->next = current->next; 
        current->next = node; 
    } 
	  return;
}  


/* Schedule Terminate
 * - Unallocate the Memory for the given Node
 */
void schedule_terminate(Process *node) {
	// free the node and assign null to it.
    free(node);
    node = NULL;
	return;
}

/* Schedule Generate
 * - Allocate the Memory and Create the Process from the given Variables
 */
Process *schedule_generate(const char *name, int pid, int time_remaining, int time_last_run) {
	// allocate the Memory for a new Process
	Process *node = malloc(sizeof(Process));
	if(node == NULL) {
		return NULL;
	}
	// copy data into newly generated Process node
	strncpy(node->name, name, strlen(name) + 1);
	node -> pid = pid;
	node -> time_remaining = time_remaining;
	node -> time_last_run = time_last_run;
	node -> next = NULL;
	return node;
}

/* remove Process 
 * - Removes the selected process from the list of Processes.
*/
void removeProcess(Process **head, Process *node) {
   // check for null head node
	if(head == NULL) {
		return;
	}
   // check if head node needs to be removed
	if((*head)->pid == node->pid) {
		*head = (*head)->next;
		return;
	}
   
	Process *walker = *head;
	// find the Process node location and remove it
	while(walker->next !=NULL){
		if(walker->next->pid == node->pid) {
		walker->next = walker->next->next;
		} else {
		walker = walker->next;
		}
	}
}

/* Schedule Select
 * - Select the next Process to be run using the given conditions:
 *   1) The Process with the lowest time_remaining should run next.
 *   - If there are any ties, select the Process with the lowest PID
 *   2) If there is a Process that has not run in >= TIME_STARVATION, then
 *      select that one instead.
 *   - If there are any ties, select the Process with the lowest PID
 *   3) If the list is NULL, return NULL
 * - Remove the selected Process from the list
 * - Return the selected Process
 */
Process *schedule_select(Process **list) {
	// check if list is null
	if(*list == NULL) {
		return NULL;
	}
    Process *minTime = *list;
    Process *walker = *list;
    
	// check if head is starved
	if((clock_get_time() - walker->time_last_run) >= TIME_STARVATION) {
		removeProcess(list, walker);
		return walker;
	}
    
	//to find out the node to be selected
	while(walker->next != NULL) {
		walker = walker->next;
      
		// if node is starved
		if((clock_get_time() - walker->time_last_run) >= TIME_STARVATION) {
			removeProcess(list, walker);
			return walker;
		}
		// if node has minimum time remaining
		if(minTime->time_remaining > walker->time_remaining) {
			minTime = walker; 
		}
    }
  
	// remove selected node from list
	removeProcess(list, minTime); 
	return minTime;
}