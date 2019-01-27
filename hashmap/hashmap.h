/**
 * hashmap.h
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


/* ok */
#define MAP_OK                    1

/* invalid or uninitialized hashmap */
#define MAP_INVALID               0

/* no binding for key */
#define MAP_KEY_NOT_FOUND        -1

/* out of memory */
#define MAP_OUT_OF_MEMORY        -2

/* cannot find slot */
#define MAP_PROBING_FAILED       -3

/* hashmap iterator exhausted */
#define MAP_ITERATOR_EXHAUSTED    0


/* pointer to the internally managed hashmap datastructure */
typedef void *Hashmap;

/* pointer to the internally managed hashmap iterator */
typedef void *Iterator;

/* key type */
typedef char *Key;

/* value type */
typedef void *Any;


/* initialize hashmap */
extern int map_init (Hashmap *hm);

/* delete hashmap */
extern int map_free (Hashmap hm);

/* retreive value from hashmap */
extern int map_lookup (const Hashmap hm, const Key key, Any *value);

/* update key or create new binding if not exists */
extern int map_insert (Hashmap hm, const Key key, const Any value);

/* remove binding from hashmap */
extern int map_remove (Hashmap hm, const Key key);

/* retreive current count of bindings from hashmap*/
extern int map_count (const Hashmap hm, size_t *count);

/* initialize hashmap iterator */
extern int map_iter_init (Iterator *it, const Hashmap hm);

/* delete hashmap iterator */
extern int map_iter_free (Iterator it);

/* test for next binding in hashmap iterator */
extern int map_iter_has_next (const Iterator it);

/* retreive next binding from hashmap iterator */
extern int map_iter_next (Iterator it, Key *key, Any *value);

/* reset hashmap iterator */
extern int map_iter_reset (Iterator it, const Hashmap hm);


