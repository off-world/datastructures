/**
 * stack.c
 *
 * implementation of a simple stack.
 *
 * Copyright (c) 2019, Tobias Heilig
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this stack of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this stack of conditions and the following disclaimer in the
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

#include "stack.h"


typedef struct _stack_node {
    /* any element */
    Any element;
    /* address of next stack node */
    struct _stack_node *next;

} stack_node;


typedef struct {
    /* stack size */
    size_t size;
    /* address of topmost stack node */
    stack_node *top;

} stack;


typedef struct {
    /* address of next stack node */
    stack_node *next;

} stack_iterator;



/* initialize stack */
int
stack_init (Stack *s)
{

    stack *stack;
    
    stack = malloc (sizeof (*stack));

    if (!stack)
        return STACK_OUT_OF_MEMORY;

    stack->size = 0;
    stack->top = NULL;

    *s = stack;

    return STACK_OK;

}


/* delete stack */
int
stack_free (Stack s)
{

    stack_node *tmp;

    stack *stack = s;

    if (!stack)
        return STACK_INVALID;

    while (stack->top) {
        // remove top node
        tmp = stack->top;
        stack->top = stack->top->next;

        free (tmp);
    }

    free (stack);

    return STACK_OK;

}


/* retreive topmost element from stack */
int
stack_peek (const Stack s, Any *element)
{

    stack *stack = s;

    if (!stack) {
        // cannot retreive element
        *element = NULL;

        return STACK_INVALID;
    }

    if (!stack->top) {
        // cannot retreive element
        *element = NULL;

        return STACK_EMPTY;
    }

    // retreive element
    *element = stack->top->element;

    return STACK_OK;

}


/* pop element from stack */
int
stack_pop (Stack s, Any *element)
{

    stack_node *tmp;

    stack *stack = s;

    if (!stack) {
        // cannot retreive element
        if (element)
            *element = NULL;

        return STACK_INVALID;
    }

    if (!stack->top) {
        // cannot retreive element
        if (element)
            *element = NULL;

        return STACK_EMPTY;
    }

    // retreive element
    if (element)
        *element = stack->top->element;

    // remove top node
    tmp = stack->top;
    stack->top = stack->top->next;

    free (tmp);

    --stack->size;

    return STACK_OK;

}


/* push element onto stack */
int
stack_push (Stack s, const Any element)
{

    stack_node *new_node;

    stack *stack = s;

    if (!stack)
        return STACK_INVALID;

    new_node = malloc (sizeof (stack_node));

    if (!new_node)
        return STACK_OUT_OF_MEMORY;

    // push new node
    new_node->element = element;
    new_node->next = stack->top;

    stack->top = new_node;

    ++stack->size;

    return STACK_OK;

}


/* retreive size of stack */
int
stack_size (const Stack s, size_t *size)
{

    stack *stack = s;

    if (!stack)
        return STACK_INVALID;

    *size = stack->size;

    return STACK_OK;

}


/* initialize stack iterator */
int
stack_iter_init (Iterator *it, const Stack s)
{

    stack_iterator *iter;

    stack *stack = s;

    if (!stack)
        return STACK_INVALID;

    iter = malloc (sizeof (stack_iterator));

    if (!iter)
        return STACK_OUT_OF_MEMORY;

    iter->next = stack->top;

    *it = iter;

    return STACK_OK;

}

/* delete stack iterator */
int
stack_iter_free (Iterator it)
{

    stack_iterator *iter = it;

    if (!iter)
        return STACK_INVALID;

    free (iter);

    return STACK_OK;

}


/* test for next element in stack iterator */
int
stack_iter_has_next (const Iterator it)
{

    stack_iterator *iter = it;

    if (!iter)
        return STACK_INVALID;

    if (!iter->next)
        return STACK_ITERATOR_EXHAUSTED;

    return STACK_OK;

}


/* retreive next element from stack iterator */
int
stack_iter_next (Iterator it, Any *element)
{

    stack_iterator *iter = it;

    if (!iter)
        return STACK_INVALID;

    if (!iter->next) {
        // no next element
        *element = NULL;

        return STACK_ITERATOR_EXHAUSTED;
    }

    // retreive element
    *element = iter->next->element;

    // increment iterator
    iter->next = iter->next->next;

    return STACK_OK;

}


/* reset stack iterator */
int
stack_iter_reset (Iterator it, const Stack s)
{

    stack *stack = s;
    stack_iterator *iter = it;

    if (!stack || !iter)
        return STACK_INVALID;

    // reset stack iterator
    iter->next = stack->top;

    return STACK_OK;

}


