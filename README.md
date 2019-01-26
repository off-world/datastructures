# datastructures

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/fdc6b14a60ab4307b0a04471832acbbe)](https://www.codacy.com/app/off-world/datastructures?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=off-world/datastructures&amp;utm_campaign=Badge_Grade)

a collection of common datastructures written in C

## ToC

1.  [Hashmap](#hashmap)
2.  [Linked List](#linked-list)
3.  [Stack](#stack)

___

### Hashmap

[open addressing](https://en.wikipedia.org/wiki/Open_addressing) hashmap using [linear probing](https://en.wikipedia.org/wiki/Linear_probing). table size is ensured to stay prime even upon resize to prevent clustering. default hash algorithm is [djb2](http://www.cse.yorku.ca/~oz/hash.html).

#### Time Complexity of Hashmap Operations

|         |                                                                      |
|---------|----------------------------------------------------------------------|
| insert  | `O(1)` [amortized](https://en.wikipedia.org/wiki/Amortized_analysis) |
| lookup  | `O(1)`                                                               |
| remove  | `O(1)`                                                               |
| count   | `O(1)`                                                               |
| iterate | `O(n)`                                                               |

#### Space Complexity for Hashmap

`O(n)`

space consumption depends heavily on growth rate and load factor threshold. A higher growth rate and lower threshold result in higher memory usage but overall better performance due to smaller probability of hash collisions. By default growth rate is 2 and load factor threshold is 0.5 resulting in an average load between 0.25 and 0.5.

#### Hashmap Example

```C
#include <stdio.h>

#include "hashmap.h"

static void hashmap_print (Key key, Any value) {
    printf("%s => %s\n", (char*)key, (char*)value);
}

int
main (void)
{
    Hashmap h;
    
    Key k = "key";
    Any v = "val";
    
    map_init (&h);
    
    map_insert (h, k, v);
    map_lookup (h, k, &v);
    
    map_foreach (h, hashmap_print);
    
    map_free (h);
    
    return 0;
}
```

### Linked List

[XOR linked list](https://en.wikipedia.org/wiki/XOR_linked_list), a memory efficient implementation of a doubly linked list. Each node only stores one address field containing the bitwise XOR of the memory addresses from the previous and next node.

#### Time Complexity of Linked List Operations

|         |        |
|---------|--------|
| first   | `O(1)` |
| last    | `O(1)` |
| lpop    | `O(1)` |
| rpop    | `O(1)` |
| lpush   | `O(1)` |
| rpush   | `O(1)` |
| at      | `O(n)` |
| pop_at  | `O(n)` |
| push_at | `O(n)` |
| reverse | `O(1)` |
| len     | `O(1)` |
| iterate | `O(n)` |

#### Space Complexity for Linked List

`O(n)`

#### Linked List Example

```C
#include <stdio.h>

#include "list.h"

int
main (void)
{
    List l;
    Iterator it;
    
    Any element;
    
    list_init (&l);
    
    list_lpush (l, "b");
    list_lpush (l, "a");
    list_rpush (l, "d");
    list_push_at (l, 2, "c");
    
    list_reverse (l);
    
    list_iter_init (&it, l);
    
    while (list_iter_has_next (it)) {
        list_iter_next (it, &element);
        printf ("%s\n", (char*)element);
    }
    
    list_first (l, &element);
    list_lpop (l, &element);
    list_rpop (l, NULL);
    list_pop_at (l, 1, &element);
    
    list_iter_free (it);
    list_free (l);
    
    return 0;
}
```

### Stack

Simple [Stack](https://en.wikipedia.org/wiki/Stack_(abstract_data_type)).

#### Time Complexity of Stack Operations

|         |        |
|-------  |--------|
| peek    | `O(1)` |
| pop     | `O(1)` |
| push    | `O(1)` |
| size    | `O(1)` |
| iterate | `O(n)` |

#### Space Complexity for Stack

`O(n)`

#### Stack Example

```C
#include <stdio.h>

#include "stack.h"


int
main (void)
{
    Stack s;
    Iterator it;
    
    Any element;
    
    stack_init (&s);
    
    stack_push (s, "a");
    stack_push (s, "b");
    stack_push (s, "c");
    
    stack_iter_init (&it, s);
    
    while (stack_iter_has_next (it)) {
        stack_iter_next (it, &element);
        printf ("%s\n", (char*)element);
    }
    
    stack_peek (s, &element);
    
    stack_pop (s, NULL);
    stack_pop (s, &element);
    
    stack_iter_free (it);
    stack_free (s);
    
    return 0;
}
```
