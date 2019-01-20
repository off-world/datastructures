# datastructures

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/fdc6b14a60ab4307b0a04471832acbbe)](https://www.codacy.com/app/off-world/datastructures?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=off-world/datastructures&amp;utm_campaign=Badge_Grade)

a collection of common datastructures written in C

## ToC

1.  [Hashmap](#hashmap)
2.  [List](#list)

___

### Hashmap

[open addressing](https://en.wikipedia.org/wiki/Open_addressing) hashmap using [linear probing](https://en.wikipedia.org/wiki/Linear_probing). table size is ensured to stay prime even upon resize to prevent clustering. default hash algorithm is [djb2](http://www.cse.yorku.ca/~oz/hash.html).

#### Time Complexity

|         |                                                                      |
|---------|----------------------------------------------------------------------|
| insert  | `O(1)` [amortized](https://en.wikipedia.org/wiki/Amortized_analysis) |
| lookup  | `O(1)`                                                               |
| remove  | `O(1)`                                                               |
| count   | `O(1)`                                                               |
| foreach | `O(n)`                                                               |

#### Space Complexity

`O(n)`

space consumption depends heavily on growth rate and load factor threshold. A higher growth rate and lower threshold result in higher memory usage but overall better performance due to smaller probability of hash collisions. By default growth rate is 2 and load factor threshold is 0.5 resulting in an average load between 0.25 and 0.5.

#### Example

```C
#include <stdio.h>

#include "hashmap.h"

static void hashmap_print (Key key, Any value) {
    printf("%s => %s\n", (char*)key, (char*)value);
}

int
main (int argc, char **argv)
{
    Hashmap h;
    
    Key k = "key";
    Any v = "val";
    
    hm_init (&h);
    
    hm_insert (h, k, v);
    hm_lookup (h, k, &v);
    
    hm_foreach (h, hashmap_print);
    
    hm_free (h);
    
    return 0;
}
```

### List

[XOR linked list](https://en.wikipedia.org/wiki/XOR_linked_list), a memory efficient implementation of a doubly linked list. Each node only stores one address field containing the bitwise XOR of the memory addresses from the previous and next node.

#### Time Complexity

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
| foreach | `O(n)` |

#### Space Complexity

`O(n)`

### Example

```C
#include <stdio.h>

#include "list.h"

static void list_print (Any value) {
    printf("%s ", (char*)value);
}

int
main (int argc, char **argv)
{
    List l;
    
    Any v;
    
    ll_init (&l);
    
    ll_lpush (l, "b");
    ll_lpush (l, "a");
    ll_rpush (l, "d");
    ll_push_at (l, 2, "c");
    
    ll_reverse (l);
    
    ll_first (l, &v);
    
    ll_pop_at (l, 2, &v);
    ll_lpop (l, &v);
    
    ll_foreach (l, list_print);
    
    ll_free (l);
    
    return 0;
}
```
