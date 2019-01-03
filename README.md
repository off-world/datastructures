# datastructures

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/fdc6b14a60ab4307b0a04471832acbbe)](https://www.codacy.com/app/off-world/datastructures?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=off-world/datastructures&amp;utm_campaign=Badge_Grade)

a collection of common datastructures written in C


## ToC

1. [Hashmap](#hashmap)

___

### Hashmap

[open addressing](https://en.wikipedia.org/wiki/Open_addressing) hashmap using [linear probing](https://en.wikipedia.org/wiki/Linear_probing). The table size is ensured to stay prime even upon resize to prevent clustering.

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
