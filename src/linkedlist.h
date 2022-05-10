#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node
{
    unsigned short address;
    unsigned short count;
    Node *prev;
    Node *next;
} Node;

typedef struct DoublyLinkedList
{
    Node *head;
    Node *tail;
} DoublyLinkedList;

Node *insert_head(DoublyLinkedList *ll, unsigned short address);
Node *insert_tail(DoublyLinkedList *ll, unsigned short address);
// void move_to_end(DoublyLinkedList *ll, unsigned short address);
void move_to_end(DoublyLinkedList *ll, Node *node);
void increase_order(DoublyLinkedList *ll, Node *node);
void remove_head(DoublyLinkedList *ll);
void list_free(DoublyLinkedList *ll);

#endif // LINKED_LIST_H