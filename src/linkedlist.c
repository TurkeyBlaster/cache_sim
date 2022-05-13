#include "linkedlist.h"
#include <stdlib.h>

DoublyLinkedList *list_malloc()
{
    DoublyLinkedList *ll = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));
    ll->head = ll->tail = 0;
    return ll;
}
Node *insert_head(DoublyLinkedList *ll, unsigned short address)
{
    Node *new_node = (Node *)calloc(1, sizeof(Node));
    new_node->address = address;
    if (ll->head)
    {
        new_node->next = ll->head;
        ll->head->prev = new_node;
    }
    else
    {
        ll->tail = new_node;
    }
    ll->head = new_node;
    return new_node;
}
Node *insert_tail(DoublyLinkedList *ll, unsigned short address)
{
    Node *new_node = (Node *)calloc(1, sizeof(Node));
    new_node->address = address;
    if (ll->tail)
    {
        new_node->prev = ll->tail;
        ll->tail->next = new_node;
    }
    else
    {
        ll->head = new_node;
    }
    ll->tail = new_node;
    return new_node;
}
void move_to_end(DoublyLinkedList *ll, Node *node)
{
    if (node == ll->tail)
    {
        return;
    }
    if (node == ll->head)
    {
        ll->head = ll->head->next;
        ll->head->prev = 0;
    }
    node->prev = ll->tail;
    node->next = ll->tail->next;
    ll->tail->next = node;
    ll->tail = node;
}
void increase_order(DoublyLinkedList *ll, Node *node)
{
    if (ll->head == ll->tail)
    {
        return;
    }
    if (node->count >= ll->tail->count)
    {
        move_to_end(ll, node);
    }
    else
    {
        Node *curr = node->next;
        while (node->count >= curr->count)
        {
            curr = curr->next;
        }
        if (node == ll->head && curr != ll->head->next)
        {
            ll->head = ll->head->next;
            ll->head->prev = 0;
        }
        node->prev = curr->prev->prev;
        node->next = curr;
    }
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
