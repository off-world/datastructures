/**
 * hashmap.c
 *
 * implementation of an open addressing hashmap using linear probing.
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
#include <string.h>

#include "hashmap.h"


/* initial table size */
#define INITIAL_SIZE 257

/* exceeding this ratio between bindings and
 * table size will trigger a resize operation */
#define LOAD_FACTOR_THRESHOLD 0.5

/* factor by which the table size will
 * grow on resize operations */
#define GROWTH_RATE 2

/* interval between probes */
#define LINEAR_PROBING_INTERVAL 1

/* maximum probing tries */
#define LINEAR_PROBING_MAX_SEQUENCE 16


/* key-value pair */
typedef struct {
    /* unique key */
    Key key;
    /* any value */
    Any value;

} binding;


/* hashmap datastructure */
typedef struct {
    /* maximum count of bindings */
    size_t size;
    /* current count of bindings */
    size_t load;
    /* hashtable */
    binding *table;

} hashmap;


/* hashing algorithm */
static inline unsigned int
hash (Key k)
{

    unsigned int hash = 5381;
    int c;

    while ( (c = *k++) )
        hash = ((hash << 5) + hash) ^ c;

    return hash;

}


/* find next prime after given number */
static size_t
next_prime (size_t n)
{

    for (;; ++n) {

        if (n % 2 == 0 || n % 3 == 0)
            goto NEXT;

        size_t div = 6;
        while (div * div - (div << 1) + 1 <= n) {
            if (n % (div - 1) == 0 || n % (div + 1) == 0)
                goto NEXT;

            div += 6;
        }

        return n;

NEXT:
        continue;

    }
}

/* find empty slot */
static int
find_slot (hashmap *map, Key key, size_t *index)
{

    size_t i, idx;

    // get slot index for key
    idx = hash (key) % map->size;

    // linear probing
    for (i = 0; i < LINEAR_PROBING_MAX_SEQUENCE; ++i) {

        // slot at index has no binding or keys match
        if (!map->table[idx].key || strcmp (map->table[idx].key, key) == 0) {
            // retreive index
            *index = idx;

            return HM_OK;
        }

        idx = (idx + LINEAR_PROBING_INTERVAL) % map->size;

    }

    return HM_PROBING_FAILED;

}

/* grow the table and rehash all keys */
static int
resize (hashmap *map)
{

    int ret;

    size_t i, idx;

    size_t old_size;
    binding *old_table;

    // backup old table
    old_size = map->size;
    old_table = map->table;

    // grow by given factor then round up to next prime
    map->size = next_prime (GROWTH_RATE * old_size);
    // allocate new table
    map->table = calloc (map->size, sizeof (binding));

    if (!map->table) {
        // restore
        map->table = old_table;
        map->size = old_size;

        return HM_OUT_OF_MEMORY;
    }

    // rehash
    for (i = 0; i < old_size; ++i) {

        // slot at index has binding
        if (old_table[i].key) {
            // get slot index for key
            ret = find_slot (map, old_table[i].key, &idx);

            if (!ret) {
                // free previously allocated resources
                free (map->table);

                // restore
                map->table = old_table;
                map->size = old_size;

                return HM_PROBING_FAILED;
            }

            // insert binding
            map->table[idx].key = old_table[i].key;
            map->table[idx].value = old_table[i].value;
        }
    }

    free (old_table);

    return HM_OK;

}


/* initialize a hashmap */
int
hm_init (Hashmap *hm)
{

    hashmap *map = malloc (sizeof (hashmap));

    if (!map)
        return HM_OUT_OF_MEMORY;

    map->table = calloc (INITIAL_SIZE,  sizeof (binding));

    if (!map->table) {
        // free previously allocated resources
        hm_free (map);

        return HM_OUT_OF_MEMORY;
    }

    map->size = INITIAL_SIZE;
    map->load = 0;

    *hm = map;

    return HM_OK;

}


/* delete a hashmap */
int
hm_free (Hashmap hm)
{

    hashmap *map = hm;

    if (!map)
        return HM_INVALID;

    free (map->table);
    free (map);

    return HM_OK;

}


/* retreive the value to a given key from the hashmap */
int
hm_lookup (Hashmap hm, Key key, Any *value)
{

    size_t i, idx;

    hashmap *map = hm;

    if (!map)
        return HM_INVALID;

    // get slot index for key
    idx = hash (key) % map->size;

    // linear probing
    for (i = 0; i < LINEAR_PROBING_MAX_SEQUENCE; ++i) {

        // slot at index has no binding
        if (!map->table[idx].key)
            break;

        // slot at index has binding and keys match
        if (strcmp (map->table[idx].key, key) == 0) {
            // retreive value
            *value = map->table[idx].value;

            return HM_OK;
        }

        idx = (idx + LINEAR_PROBING_INTERVAL) % map->size;
    }

    *value = NULL;

    return HM_KEY_NOT_FOUND;

}


/* update the value of a given key or create the binding if not exists */
int
hm_insert (Hashmap hm, Key key, Any value)
{

    int ret;

    size_t idx;

    hashmap *map = hm;

    if (!map)
        return HM_INVALID;

    // load factor exceeds threshold
    if ((float) map->load / (float) map->size >= LOAD_FACTOR_THRESHOLD) {
        // grow table and rehash
        ret = resize (map);

        if (!ret)
            return ret;
    }

    // get slot index for key
    ret = find_slot (map, key, &idx);

    // no slot found
    if (!ret) {
        // make one attempt to resolve collision chain
        ret = resize (map);

        if (!ret)
            return ret;

        // and try again
        ret = find_slot (map, key, &idx);

        // give up if again no slot was found
        if (!ret)
            return HM_PROBING_FAILED;
    }

    // insert binding
    map->table[idx].key = key;
    map->table[idx].value = value;

    ++map->load;

    return HM_OK;

}


/* remove a binding from the hashmap */
int
hm_remove (Hashmap hm, Key key)
{

    size_t i, idx, removed_idx, last_idx;

    hashmap *map = hm;

    if (!map)
        return HM_INVALID;

    // get slot index for key
    idx = hash (key) % map->size;

    // linear probing
    for (i = 0; i < LINEAR_PROBING_MAX_SEQUENCE; ++i) {

        // slot at index has no binding
        if (!map->table[idx].key)
            return HM_KEY_NOT_FOUND;

        // slot at index has binding and keys match
        if (strcmp (map->table[idx].key, key) == 0) {
            // mark for deletion
            removed_idx = idx;
            // skip to end of collision chain
            for (; map->table[idx].key; idx = (idx + LINEAR_PROBING_INTERVAL) % map->size);
            // get last binding for relocation
            last_idx = (idx - LINEAR_PROBING_INTERVAL) % map->size;

            // relocate
            if (last_idx != removed_idx) {
                map->table[removed_idx].key = map->table[last_idx].key;
                map->table[removed_idx].value = map->table[last_idx].value;
            }

            // remove
            map->table[last_idx].key = NULL;
            map->table[last_idx].value = NULL;

            --map->load;

            return HM_OK;
        }

        idx = (idx + LINEAR_PROBING_INTERVAL) % map->size;

    }

    return HM_PROBING_FAILED;
}


/* iterate the hashmap and call f for each key-value pair */
int
hm_iter (Hashmap hm, PFiter f)
{

    size_t i;

    hashmap *map = hm;

    if (!map)
        return HM_INVALID;

    // iterate table
    for (i = 0; i < map->size; ++i) {
        // slot at index has binding
        if (map->table[i].key)
            (*f) (map->table[i].key, map->table[i].value);

    }

    return HM_OK;

}


/* get the current count of bindings from the hashmap */
size_t
hm_count (Hashmap hm)
{

    hashmap *map = hm;

    if (!map)
        return HM_INVALID;

    return map != NULL ? map->load : 0;

}

