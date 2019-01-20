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
#define HM_OK                1

/* invalid or uninitialized hashmap */
#define HM_INVALID           0

/* no binding for key */
#define HM_KEY_NOT_FOUND    -1

/* out of memory */
#define HM_OUT_OF_MEMORY    -2

/* cannot find slot */
#define HM_PROBING_FAILED   -3


/* pointer to the internally managed hashmap datastructure */
typedef void *Hashmap;

/* key type */
typedef char *Key;

/* value type */
typedef void *Any;

/* iteration callback */
typedef void (*PFIter) (Key, Any);


/* initialize hashmap */
extern int hm_init (Hashmap *hm);

/* delete hashmap */
extern int hm_free (Hashmap hm);

/* retreive value from hashmap */
extern int hm_lookup (Hashmap hm, Key key, Any *value);

/* update key or create new binding if not exists */
extern int hm_insert (Hashmap hm, Key key, Any value);

/* remove binding from hashmap */
extern int hm_remove (Hashmap hm, Key key);

/* iterate hashmap and call f for each key-value pair */
extern int hm_foreach (Hashmap hm, PFIter f);

/* retreive current count of bindings from hashmap*/
extern int hm_count (Hashmap hm, size_t *count);

