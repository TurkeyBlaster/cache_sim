#include "linkedlist.h"
#include <stdlib.h>

Node * insert_tail(DoublyLinkedList *ll, unsigned short address)
{
    Node *new_node = (Node *)calloc(1, sizeof(Node));
    new_node->address = address;
    if (ll->tail)
    {
        if (ll->tail == ll->head)
        {
            ll->head->next = new_node;
        }
        ll->tail->next = new_node;
        new_node->prev = ll->tail;
    }
    else
    {
        ll->head = new_node;
    }
    ll->tail = new_node;
    return new_node;
}
// void move_to_end(DoublyLinkedList *ll, unsigned short address)
// {
//     if (ll->tail->address == address)
//     {
//         return;
//     }

//     Node *curr = ll->head;
//     while (curr->address != address)
//     {
//         curr = curr->next;
//     }
//     if (curr != ll->tail)
//     {
//         curr->prev->next = curr->next;
//         curr->next->prev = curr->prev;
//         ll->tail->next = curr;
//         curr->prev = ll->tail;
//         curr->next = ll->tail->next;
//         ll->tail = curr;
//     }
// }
void move_to_end(DoublyLinkedList *ll, Node *node)
{
    if (node == ll->tail)
    {
        return;
    }
    if (node == ll->head)
    {
        ll->head = node->next;
    }
    node->prev = ll->tail;
    node->next = ll->tail->next;
    ll->tail = node;
}
void remove_head(DoublyLinkedList *ll)
{
    if (ll->head == ll->tail)
    {
        free(ll->head);
        ll->head = ll->tail = 0;
    }
    else
    {
        ll->head = ll->head->next;
        free(ll->head->prev);
        ll->head->prev = 0;
    }
}

void list_free(DoublyLinkedList *ll)
{
    if (!ll)
    {
        return;
    }
    while (ll->head)
    {
        remove_head(ll);
    }
    free(ll);
    ll = 0;  
}
