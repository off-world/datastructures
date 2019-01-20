/**
 * stack.h
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


#include <stddef.h>


/* ok */
#define STACK_OK                    1

/* invalid or uninitialized stack */
#define STACK_INVALID               0

/* empty stack */
#define STACK_EMPTY                -1

/* out of memory */
#define STACK_OUT_OF_MEMORY        -2


/* pointer to the internally managed stack datastructure */
typedef void *Stack;

/* element type */
typedef void *Any;


/* initialize stack */
extern int stack_init (Stack *s);

/* delete stack */
extern int stack_free (Stack s);

/* retreive topmost element from stack */
extern int stack_peek (Stack s, Any *element);

/* pop element from stack */
extern int stack_pop (Stack s, Any *element);

/* push element onto stack */
extern int stack_push (Stack s, Any element);

/* retreive size of stack */
extern int stack_size (Stack s, size_t *size);


