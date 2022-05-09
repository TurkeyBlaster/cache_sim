#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node
{
    unsigned short address;
    Node *prev;
    Node *next;
} Node;

typedef struct DoublyLinkedList
{
    Node *head;
    Node *tail;
} DoublyLinkedList;

void insert_tail(DoublyLinkedList *ll, unsigned short address);
void move_to_end(DoublyLinkedList *ll, unsigned short address);
void remove_head(DoublyLinkedList *ll);

#endif // LINKED_LIST_H