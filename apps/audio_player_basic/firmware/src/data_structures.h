/* 
 * File:   data_structures.h
 * Author: C16266
 * General Data Structures
 * Created on May 31, 2017, 11:08 AM
 */

#ifndef DATA_STRUCTURES_H
#define	DATA_STRUCTURES_H

#ifdef	__cplusplus
extern "C" {
#endif

#define null NULL
/** Character Type Singly Linked List **/
typedef struct Node
{
    char val[64];
    struct Node *next;
}ListNode;


#ifdef	__cplusplus
}
#endif

#endif	/* DATA_STRUCTURES_H */

