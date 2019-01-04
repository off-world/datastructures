# datastructures

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/fdc6b14a60ab4307b0a04471832acbbe)](https://www.codacy.com/app/off-world/datastructures?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=off-world/datastructures&amp;utm_campaign=Badge_Grade)

a collection of common datastructures written in C

## ToC

1.  [Hashmap](#hashmap)

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
| iterate | `O(n)`                                                               |

#### Space Complexity

`O(n)`

space consumption depends heavily on growth rate and load factor threshold. A higher growth rate and smaller threshold result in higher memory usage but overall better performance due to smaller probability of hash collisions. By default growth rate is 2 and load factor threshold is 0.5 resulting in an average load between 0.25 and 0.5.

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
    
    hm_iter (h, hashmap_print);
    
    hm_free (h);
}
```
