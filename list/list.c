/** list.c
 *
 * implementation of an XOR linked list with head and tail sentinels.
 *
 * Copyright (c) 2019, Tobias Heilig
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the authors may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/


#include <stdlib.h>
#include <stdint.h>

#include "list.h"


#define XOR(A, B)   (list_node *)(((uintptr_t) A) ^ ((uintptr_t) B))


/* list list_node */
typedef struct _list_node {
    /* any element */
    Any element;
    /* XOR of next and previous list_node */
    struct _list_node *link;

} list_node;


/* list datastructure */
typedef struct {
    /* list length */
    size_t len;
    /* list head */
    list_node *head;
    /* list tail */
    list_node *tail;

} list;


int list_init (List *l);
int list_free (List l);
int list_first (List l, Any *element);
int list_last (List l, Any *element);
int list_lpop (List l, Any *element);
int list_rpop (List l, Any *element);
int list_lpush (List l, Any element);
int list_rpush (List l, Any element);
int list_at (List l, size_t index, Any *element);
int list_pop_at (List l, size_t index, Any *element);
int list_push_at (List l, size_t index, Any element);
int list_reverse (List l);
int list_foreach (List l, PFIter f);
int list_len (List l, size_t *len);


/* initialize list */
int
list_init (List *l)
{

    list *list = malloc (sizeof (list));

    if (!list)
        return LIST_OUT_OF_MEMORY;

    list->len = 0;
    list->head = NULL;
    list->tail = NULL;

    *l = list;

    return LIST_OK;

}


/* delete list */
int
list_free (List l)
{

    list_node *prev;
    list_node *cur;
    list_node *next;

    list *list = l;

    if (!list)
        return LIST_INVALID;

    prev = NULL;
    cur = list->head;

    while (cur) {
        next = XOR (prev, cur->link);
        prev = cur;

        free (cur);

        cur = next;
    }

    free (list);

    return LIST_OK;

}


/* retreive first element from list */
int
list_first (List l, Any *element)
{

    list *list = l;

    if (!list) {
        // cannot retreive element
        *element = NULL;

        return LIST_INVALID;
    }

    if (!list->head) {
        // cannot retreive element
        *element = NULL;

        return LIST_EMPTY;
    }

    // retreive element
    *element = list->head->element;

    return LIST_OK;

}


/* retreive last element from list */
int
list_last (List l, Any *element)
{

    list *list = l;

    if (!list) {
        // cannot retreive element
        *element = NULL;

        return LIST_INVALID;
    }

    if (!list->tail) {
        // cannot retreive element
        *element = NULL;

        return LIST_EMPTY;
    }

    // retreive element from last list_node
    *element = list->tail->element;

    return LIST_OK;

}


/* retreive and remove first element from list */
int
list_lpop (List l, Any *element)
{

    list_node *tmp;

    list *list = l;

    if (!list) {
        // cannot retreive element
        if (element)
            *element = NULL;

        return LIST_INVALID;
    }

    if (!list->head) {
        // cannot retreive element
        if (element)
            *element = NULL;

        return LIST_EMPTY;
    }

    // retreive element
    if (element)
        *element = list->head->element;

    // update head
    tmp = list->head;
    list->head = XOR (NULL, tmp->link);

    // list is empty
    if (!list->head) {
        // update tail as last list_node was removed
        list->tail = NULL;

    } else {
        // update new head link
        list->head->link = XOR (NULL, XOR (tmp, list->head->link));

    }

    free (tmp);

    --list->len;

    return LIST_OK;

}


/* retreive and remove last element from list */
int
list_rpop (List l, Any *element)
{

    list_node *tmp;

    list *list = l;

    if (!list) {
        // cannot retreive element
        if (element)
            *element = NULL;

        return LIST_INVALID;
    }

    if (!list->tail) {
        // cannot retreive element
        if (element)
            *element = NULL;

        return LIST_EMPTY;
    }

    // retreive element
    if (element)
        *element = list->tail->element;

    // update tail
    tmp = list->tail;
    list->tail = XOR (tmp->link, NULL);

    // list is empty
    if (!list->tail) {
        // update head as last list_node was removed
        list->head = NULL;

    } else {
        // update new tail link
        list->tail->link = XOR (NULL, XOR (tmp, list->tail->link));

    }

    free (tmp);

    --list->len;

    return LIST_OK;

}


/* prepend element to list */
int
list_lpush (List l, Any element)
{
    list_node *new_list_node;

    list *list = l;

    if (!list)
        return LIST_INVALID;

    new_list_node = malloc (sizeof (list_node));

    if (!new_list_node)
        return LIST_OUT_OF_MEMORY;

    // prepare new list_node
    new_list_node->element = element;
    new_list_node->link = XOR (NULL, list->head);

    // list is empty
    if (!list->head) {
        // single list_node in list is both head and tail
        list->head = list->tail = new_list_node;

    } else {
        // update old head link
        list->head->link = XOR (new_list_node, XOR (NULL, list->head->link));

        list->head = new_list_node;

    }

    ++list->len;

    return LIST_OK;

}


/* append element to list */
int
list_rpush (List l, Any element)
{

    list *list = l;

    if (!list)
        return LIST_INVALID;

    list_node *new_list_node = malloc (sizeof (list_node));

    if (!new_list_node)
        return LIST_OUT_OF_MEMORY;

    // prepare new list_node
    new_list_node->element = element;
    new_list_node->link = XOR (list->tail, NULL);

    // list is empty
    if (!list->tail) {
        // single list_node in list is both tail and head
        list->tail = list->head = new_list_node;

    } else {
        // update old tail link
        list->tail->link = XOR (new_list_node, XOR (list->tail->link, NULL));

        list->tail = new_list_node;

    }

    ++list->len;

    return LIST_OK;

}


/* retreive element at index from list */
int
list_at (List l, size_t index, Any *element)
{

    int i, rev;

    list_node *prev;
    list_node *cur;
    list_node *next;

    list *list = l;

    if (!list)
        return LIST_INVALID;

    if (!list->len)
        return LIST_EMPTY;

    if (index < 0 || index >= list->len)
        return LIST_INDEX_OUT_OF_RANGE;

    if (index == 0)
        return list_first (l, element);

    if (index == list->len - 1)
        return list_last (l, element);

    // traverse list in reverse if element resides in second half of list
    rev = index > (list->len >> 1);

    if (rev) {
        index = list->len - 1 - index;
        list_reverse (l);
    }

    prev = NULL;
    cur = list->head;
    i = 0;

    while (cur) {
        next = XOR (prev, cur->link);

        if (i == index) {
            // get element
            if (element)
                *element = cur->element;

            if (rev)
                list_reverse (l);

            return LIST_OK;
        }

        prev = cur;
        cur = next;
        ++i;
    }

    return LIST_INVALID;

}


/* retreive and remove element at index from list */
int
list_pop_at (List l, size_t index, Any *element)
{

    int i, rev;

    list_node *prev;
    list_node *cur;
    list_node *next;

    list *list = l;

    if (!list)
        return LIST_INVALID;

    if (!list->len)
        return LIST_EMPTY;

    if (index < 0 || index >= list->len)
        return LIST_INDEX_OUT_OF_RANGE;

    if (index == 0)
        return list_lpop (l, element);

    if (index == list->len - 1)
        return list_rpop (l, element);

    // traverse list in reverse if element resides in second half of list
    rev = index > (list->len >> 1);

    if (rev) {
        index = list->len - 1 - index;
        list_reverse (l);
    }

    prev = NULL;
    cur = list->head;
    i = 0;

    while (cur) {
        next = XOR (prev, cur->link);

        if (i == index) {
            // remove list_node
            prev->link = XOR (next, XOR (prev->link, cur));
            next->link = XOR (prev, XOR (next->link, cur));

            // get element
            if (element)
                *element = cur->element;

            free (cur);

            --list->len;

            if (rev)
                list_reverse (l);

            return LIST_OK;
        }

        prev = cur;
        cur = next;
        ++i;
    }

    return LIST_INVALID;

}


/* insert element at index from list */
int
list_push_at (List l, size_t index, Any element)
{

    int i, rev;

    list_node *new_list_node;
    list_node *prev;
    list_node *cur;
    list_node *next;

    list *list = l;

    if (!list)
        return LIST_INVALID;

    if (index < 0 || index > list->len)
        return LIST_INDEX_OUT_OF_RANGE;

    if (index == 0 || !list->len)
        return list_lpush (l, element);

    if (index == list->len)
        return list_rpush (l, element);

    new_list_node = malloc (sizeof (list_node));

    if (!new_list_node)
        return LIST_OUT_OF_MEMORY;

    // traverse list in reverse if element resides in second half of list
    rev = index > (list->len >> 1);

    if (rev) {
        index = list->len - index;
        list_reverse (l);
    }

    prev = NULL;
    cur = list->head;
    i = 0;

    while (cur) {
        next = XOR (prev, cur->link);

        if (i == index) {
            // prepare new list_node
            new_list_node->element = element;
            new_list_node->link = XOR (prev, cur);

            // insert list_node
            prev->link = XOR (new_list_node, XOR (prev->link, cur));
            cur->link = XOR (new_list_node, next);

            ++list->len;

            if (rev)
                list_reverse (l);

            return LIST_OK;
        }

        prev = cur;
        cur = next;
        ++i;
    }

    return LIST_INVALID;

}


/* reverse list */
int
list_reverse (List l)
{

    list_node *tmp;

    list *list = l;

    if (!list)
        return LIST_INVALID;

    if (!list->len)
        return LIST_EMPTY;

    tmp = list->head;
    list->head = list->tail;
    list->tail = tmp;

    return LIST_OK;

}


/* iterate list and call f for each element */
int
list_foreach (List l, PFIter f)
{

    list_node *prev;
    list_node *cur;
    list_node *next;

    list *list = l;

    if (!list)
        return LIST_INVALID;

    if (!list->len)
        return LIST_EMPTY;

    prev = NULL;
    cur = list->head;

    while (cur) {
        next = XOR (prev, cur->link);

        // call f for each element
        (*f) (cur->element);

        prev = cur;
        cur = next;
    }

    return LIST_OK;

}


/* retreive length of list */
int
list_len (List l, size_t *len)
{

    list *list = l;

    if (!list)
        return LIST_INVALID;

    *len = list->len;

    return LIST_OK;

}

