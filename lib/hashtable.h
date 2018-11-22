/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 		
￼* hashtable.h		
￼* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 		
￼*		
￼* Simple hashtable (with double hashing, resizing)		
￼* Implements:		
￼*   -insert 		
￼*   -remove 		
￼*   -lookup		
￼*		
 * return codes: all ints indicate status (0 -> success ; -1 -> error)		
￼*/		

#ifndef __HASHTABLE_H		
#define __HASHTABLE_H		
		
typedef struct hashtable * hashtable_t;		
   	
// create hashtable		
hashtable_t hashtable_(		
    const ssize_t init_capacity,		
    ssize_t (*hash1)(const ssize_t, const void *),		
    ssize_t (*hash2)(const ssize_t, const void *),		
    int (*equals)(const void *, const void *)		
    );		


// destroy hashtable		
void free_hashtable(hashtable_t);		
   	
// add to the hashtable		
int hashtable_add(hashtable_t, void * item);		
   	
// remove from the hashtable		
void * hashtable_rem(hashtable_t, void * item);		
   	
// lookup: returns void *, the item found (or NULL if not found) 		
// allows for a complex structure with n elements to be indexed by only one of them		
//		
// so, the filled struct can be indexed by one of the elements and, on lookup,		
// the function can be given an incomplete struct (with the same index)		
//		
// This depends on the hash functions given		
void * hashtable_lookup(const hashtable_t, const void * item);		
   	
// find the hashtable capacity		
ssize_t hashtable_capacity(const hashtable_t) ;		
   	
// find the hashtable size		
ssize_t hashtable_size(const hashtable_t) ;		
#endif // __HASHTABLE_H
