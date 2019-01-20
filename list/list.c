/** * list.c
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


#define XOR(A, B)   (node *)(((uintptr_t) A) ^ ((uintptr_t) B))


/* list node */
typedef struct _node {
    /* any element */
    Any element;
    /* XOR of next and previous node */
    struct _node *link;

} node;


/* list datastructure */
typedef struct {
    /* list length */
    size_t len;
    /* list head */
    node *head;
    /* list tail */
    node *tail;

} list;


int ll_init (List *l);
int ll_free (List l);
int ll_first (List l, Any *element);
int ll_last (List l, Any *element);
int ll_lpop (List l, Any *element);
int ll_rpop (List l, Any *element);
int ll_lpush (List l, Any element);
int ll_rpush (List l, Any element);
int ll_at (List l, size_t index, Any *element);
int ll_pop_at (List l, size_t index, Any *element);
int ll_push_at (List l, size_t index, Any element);
int ll_reverse (List l);
int ll_foreach (List l, PFIter f);
int ll_len (List l, size_t *len);


/* initialize list */
int
ll_init (List *l)
{

    list *lst = calloc (1, sizeof (list));

    if (!lst)
        return LL_OUT_OF_MEMORY;

    *l = lst;

    return LL_OK;

}


/* delete list */
int
ll_free (List l)
{

    node *prev;
    node *cur;
    node *next;

    list *lst = l;

    if (!lst)
        return LL_INVALID;

    prev = NULL;
    cur = lst->head;

    while (cur) {
        next = XOR (prev, cur->link);
        prev = cur;

        free (cur);

        cur = next;
    }

    free (lst);

    return LL_OK;

}


/* retreive first element from list */
int
ll_first (List l, Any *element)
{

    list *lst = l;

    if (!lst) {
        // cannot retreive element
        *element = NULL;

        return LL_INVALID;
    }

    if (!lst->head) {
        // cannot retreive element
        *element = NULL;

        return LL_EMPTY;
    }

    // retreive element from first node
    *element = lst->head->element;

    return LL_OK;

}


/* retreive last element from list */
int
ll_last (List l, Any *element)
{

    list *lst = l;

    if (!lst) {
        // cannot retreive element
        *element = NULL;

        return LL_INVALID;
    }

    if (!lst->tail) {
        // cannot retreive element
        *element = NULL;

        return LL_EMPTY;
    }

    // retreive element from last node
    *element = lst->tail->element;

    return LL_OK;

}


/* retreive and remove first element from list */
int
ll_lpop (List l, Any *element)
{

    int ret;
    node *tmp;

    list *lst = l;

    if (!lst)
        return LL_INVALID;

    if (!lst->head)
        return LL_EMPTY;

    // get first element
    if (element) {
        ret = ll_first (l, element);

        if (!ret)
            return ret;
    }

    // update head
    tmp = lst->head;
    lst->head = XOR (NULL, tmp->link);

    // list is empty
    if (!lst->head) {
        // update tail as last node was removed
        lst->tail = NULL;

    } else {
        // update new head link
        lst->head->link = XOR (NULL, XOR (tmp, lst->head->link));

    }

    free (tmp);

    --lst->len;

    return LL_OK;

}


/* retreive and remove last element from list */
int
ll_rpop (List l, Any *element)
{

    int ret;
    node *tmp;

    list *lst = l;

    if (!lst)
        return LL_INVALID;

    if (!lst->head)
        return LL_EMPTY;

    // get last element
    if (element) {
        ret = ll_last (l, element);

        if (!ret)
            return ret;
    }

    // update tail
    tmp = lst->tail;
    lst->tail = XOR (tmp->link, NULL);

    // list is empty
    if (!lst->tail) {
        // update head as last node was removed
        lst->head = NULL;

    } else {
        // update new tail link
        lst->tail->link = XOR (NULL, XOR (tmp, lst->tail->link));

    }

    free (tmp);

    --lst->len;

    return LL_OK;

}


/* prepend element to list */
int
ll_lpush (List l, Any element)
{
    node *new_node;

    list *lst = l;

    if (!lst)
        return LL_INVALID;

    new_node = malloc (sizeof (node));

    if (!new_node)
        return LL_OUT_OF_MEMORY;

    // prepare new node
    new_node->element = element;
    new_node->link = XOR (NULL, lst->head);

    // list is empty
    if (!lst->head) {
        // single node in list is both head and tail
        lst->head = lst->tail = new_node;

    } else {
        // update old head link
        lst->head->link = XOR (new_node, XOR (NULL, lst->head->link));

        lst->head = new_node;

    }

    ++lst->len;

    return LL_OK;

}


/* append element to list */
int
ll_rpush (List l, Any element)
{

    list *lst = l;

    if (!lst)
        return LL_INVALID;

    node *new_node = malloc (sizeof (node));

    if (!new_node)
        return LL_OUT_OF_MEMORY;

    // prepare new node
    new_node->element = element;
    new_node->link = XOR (lst->tail, NULL);

    // list is empty
    if (!lst->tail) {
        // single node in list is both tail and head
        lst->tail = lst->head = new_node;

    } else {
        // update old tail link
        lst->tail->link = XOR (new_node, XOR (lst->tail->link, NULL));

        lst->tail = new_node;

    }

    ++lst->len;

    return LL_OK;

}


/* retreive element at index from list */
int
ll_at (List l, size_t index, Any *element)
{

    int i, rev;

    node *prev;
    node *cur;
    node *next;

    list *lst = l;

    if (!lst)
        return LL_INVALID;

    if (!lst->len)
        return LL_EMPTY;

    if (index < 0 || index >= lst->len)
        return LL_INDEX_OUT_OF_RANGE;

    if (index == 0)
        return ll_first (l, element);

    if (index == lst->len - 1)
        return ll_last (l, element);

    // traverse list in reverse if element resides in second half of list
    rev = index > (lst->len >> 1);

    if (rev) {
        index = lst->len - 1 - index;
        ll_reverse (l);
    }

    prev = NULL;
    cur = lst->head;
    i = 0;

    while (cur) {
        next = XOR (prev, cur->link);

        if (i == index) {
            // get element
            if (element)
                *element = cur->element;

            if (rev)
                ll_reverse (l);

            return LL_OK;
        }

        prev = cur;
        cur = next;
        ++i;
    }

    return LL_INVALID;

}


/* retreive and remove element at index from list */
int
ll_pop_at (List l, size_t index, Any *element)
{

    int i, rev;

    node *prev;
    node *cur;
    node *next;

    list *lst = l;

    if (!lst)
        return LL_INVALID;

    if (!lst->len)
        return LL_EMPTY;

    if (index < 0 || index >= lst->len)
        return LL_INDEX_OUT_OF_RANGE;

    if (index == 0)
        return ll_lpop (l, element);

    if (index == lst->len - 1)
        return ll_rpop (l, element);

    // traverse list in reverse if element resides in second half of list
    rev = index > (lst->len >> 1);

    if (rev) {
        index = lst->len - 1 - index;
        ll_reverse (l);
    }

    prev = NULL;
    cur = lst->head;
    i = 0;

    while (cur) {
        next = XOR (prev, cur->link);

        if (i == index) {
            // remove node
            prev->link = XOR (next, XOR (prev->link, cur));
            next->link = XOR (prev, XOR (next->link, cur));

            // get element
            if (element)
                *element = cur->element;

            free (cur);

            --lst->len;

            if (rev)
                ll_reverse (l);

            return LL_OK;
        }

        prev = cur;
        cur = next;
        ++i;
    }

    return LL_INVALID;

}


/* insert element at index from list */
int
ll_push_at (List l, size_t index, Any element)
{

    int i, rev;

    node *new_node;
    node *prev;
    node *cur;
    node *next;

    list *lst = l;

    if (!lst)
        return LL_INVALID;

    if (index < 0 || index > lst->len)
        return LL_INDEX_OUT_OF_RANGE;

    if (index == 0 || !lst->len)
        return ll_lpush (l, element);

    if (index == lst->len)
        return ll_rpush (l, element);

    new_node = malloc (sizeof (node));

    if (!new_node)
        return LL_OUT_OF_MEMORY;

    // traverse list in reverse if element resides in second half of list
    rev = index > (lst->len >> 1);

    if (rev) {
        index = lst->len - index;
        ll_reverse (l);
    }

    prev = NULL;
    cur = lst->head;
    i = 0;

    while (cur) {
        next = XOR (prev, cur->link);

        if (i == index) {
            // prepare new node
            new_node->element = element;
            new_node->link = XOR (prev, cur);

            // insert node
            prev->link = XOR (new_node, XOR (prev->link, cur));
            cur->link = XOR (new_node, next);

            ++lst->len;

            if (rev)
                ll_reverse (l);

            return LL_OK;
        }

        prev = cur;
        cur = next;
        ++i;
    }

    return LL_INVALID;

}


/* reverse list */
int
ll_reverse (List l)
{

    node *tmp;

    list *lst = l;

    if (!lst)
        return LL_INVALID;

    if (!lst->len)
        return LL_EMPTY;

    tmp = lst->head;
    lst->head = lst->tail;
    lst->tail = tmp;

    return LL_OK;

}


/* iterate list and call f for each element */
int
ll_foreach (List l, PFIter f)
{

    node *prev;
    node *cur;
    node *next;

    list *lst = l;

    if (!lst)
        return LL_INVALID;

    if (!lst->len)
        return LL_EMPTY;

    prev = NULL;
    cur = lst->head;

    while (cur) {
        next = XOR (prev, cur->link);

        // call f for each element
        (*f) (cur->element);

        prev = cur;
        cur = next;
    }

    return LL_OK;

}


/* retreive length of list */
int
ll_len (List l, size_t *len)
{

    list *lst = l;

    if (!lst)
        return LL_INVALID;

    *len = lst->len;

    return LL_OK;

}


