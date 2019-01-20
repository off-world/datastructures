/**
 * list.h
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


#include <stddef.h>


/* ok */
#define LL_OK                    1

/* invalid or uninitialized list */
#define LL_INVALID               0

/* empty list */
#define LL_EMPTY                -1

/* index out of list range */
#define LL_INDEX_OUT_OF_RANGE   -2

/* out of memory */
#define LL_OUT_OF_MEMORY        -3


/* pointer to the internally managed list datastructure */
typedef void *List;

/* element type */
typedef void *Any;

/* iteration callback */
typedef void (*PFIter) (Any);


/* initialize list */
extern int ll_init (List *l);

/* delete list */
extern int ll_free (List l);

/* retreive first element from list */
extern int ll_first (List l, Any *element);

/* retreive last element from list */
extern int ll_last (List l, Any *element);

/* retreive and remove first element from list */
extern int ll_lpop (List l, Any *element);

/* retreive and remove last element from list */
extern int ll_rpop (List l, Any *element);

/* prepend element to list */
extern int ll_lpush (List l, Any element);

/* append element to list */
extern int ll_rpush (List l, Any element);

/* retreive element at index from list */
extern int ll_at (List  l, size_t index, Any *element);

/* retreive and remove element at index from list */
extern int ll_pop_at (List  l, size_t index, Any *element);

/* insert element at index from list */
extern int ll_push_at (List  l, size_t index, Any element);

/* reverse list */
extern int ll_reverse (List l);

/* iterate list and call f for each element */
extern int ll_foreach (List l, PFIter f);

/* retreive length of list */
extern int ll_len (List l, size_t *len);


