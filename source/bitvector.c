#ifndef __BITVECTOR_C__
#define __BITVECTOR_C__

#include "bitvector.h"

inline bitvector_t *bitvector_malloc(uint64_t n)
{
    auto bitvector_t *vec_ptr = calloc((size_t) 1,sizeof(*vec_ptr));
    
    if (vec_ptr != NULL) {
        vec_ptr->vec = calloc((n / (8 * sizeof(*(vec_ptr->vec)))) +
                              ((n % (8 * sizeof(*(vec_ptr->vec)))) > 0),
                              sizeof(*(vec_ptr->vec)));
        if (vec_ptr->vec == NULL || n == 0) {
            free(vec_ptr);
            vec_ptr = NULL;
        } else {
            vec_ptr->vec_pos = 0;
            vec_ptr->bit_pos = 0;
            vec_ptr->malloc_size = (n / (8 * sizeof(*(vec_ptr->vec))))
                                 + ((n % (8 * sizeof(*(vec_ptr->vec)))) > 0);
            vec_ptr->vector_size = n;
        }
    }

    return vec_ptr;
}

inline void bitvector_free(bitvector_t *vec_ptr)
{
    if (vec_ptr != NULL)
        free(vec_ptr->vec);

    free(vec_ptr);

    return;
}

inline void bitvector_set(bitvector_t *vec_ptr, bool value, uint64_t bit)
{
    if (vec_ptr != NULL) {
        vec_ptr->vec[(vec_ptr->vec_pos + (bit / (8 * sizeof(*(vec_ptr->vec))))) % vec_ptr->malloc_size] &= ~((word_t) ((value & true)) << ((vec_ptr->bit_pos + bit) % (8 * sizeof(*(vec_ptr->vec)))));
        vec_ptr->vec[(vec_ptr->vec_pos + (bit / (8 * sizeof(*(vec_ptr->vec))))) % vec_ptr->malloc_size] |=  ((word_t) ((value & true)) << ((vec_ptr->bit_pos + bit) % (8 * sizeof(*(vec_ptr->vec)))));
    }

    return;
}

inline bool bitvector_get(bitvector_t *vec_ptr, uint64_t bit)
{
    if (vec_ptr != NULL)
        return (word_t) ((vec_ptr->vec[(vec_ptr->vec_pos + (bit / (8 * sizeof(*(vec_ptr->vec))))) % vec_ptr->malloc_size]) >> ((vec_ptr->bit_pos + bit) % (8 * sizeof(*(vec_ptr->vec))))) & ((word_t) true);
    return false;
}

inline void bitvector_add(bitvector_t *dest, bitvector_t *src_a, bitvector_t *src_b)
{
    auto bool a, b, carry = false;
    auto uint64_t i;

    for (i = 0; i < dest->vector_size; ++i) {
        a = bitvector_get(src_a,i);
        b = bitvector_get(src_b,i);
        bitvector_set(dest,a ^ b ^ carry,i);
        carry = a & b;
    }

    return;
}

inline void bitvector_sub(bitvector_t *dest, bitvector_t *src_a, bitvector_t *src_b)
{
    return;
}

inline void bitvector_not(bitvector_t *dest, bitvector_t *src)
{
    uint64_t i; 

    for (i = 0; i < src->malloc_size; ++i)
        dest->vec[i] = ~src->vec[i];

    return;
}


inline void bitvector_and(bitvector_t *dest, bitvector_t *src_a, bitvector_t *src_b)
{
    auto uint64_t i; 

    for (i = 0; i < src_a->malloc_size; ++i)
        dest->vec[i] = src_a->vec[i] & src_b->vec[i];

    return;
}

inline void bitvector_or(bitvector_t *dest, bitvector_t *src_a, bitvector_t *src_b)
{
    auto uint64_t i; 

    for (i = 0; i < src_a->malloc_size; ++i)
        dest->vec[i] = src_a->vec[i] | src_b->vec[i];

    return;
}

inline void bitvector_xor(bitvector_t *dest, bitvector_t *src_a, bitvector_t *src_b)
{
    auto uint64_t i; 

    for (i = 0; i < src_a->malloc_size; ++i)
        dest->vec[i] = src_a->vec[i] ^ src_b->vec[i];

    return;
}

inline void bitvector_lshift(bitvector_t *dest, bitvector_t *src, uint64_t shft_amnt)
{
    return;
}

inline void bitvector_rshift(bitvector_t *dest, bitvector_t *src, uint64_t shft_amnt)
{
    return;
}

inline void bitvector_lrot(bitvector_t *dest, bitvector_t *src, uint64_t rot_amnt)
{
    dest->vec_pos = (src->vec_pos + (rot_amnt / (8 * sizeof(*(src->vec))))) % src->malloc_size; 
    dest->bit_pos = (src->bit_pos + rot_amnt) % (8 * sizeof(*(src->vec))); 

    return;
}

inline void bitvector_rrot(bitvector_t *dest, bitvector_t * src, uint64_t rot_amnt)
{
    dest->vec_pos = (src->vec_pos - (rot_amnt / (8 * sizeof(*(src->vec))))) % src->malloc_size; 
    dest->bit_pos = (src->bit_pos - rot_amnt) % (8 * sizeof(*(src->vec))); 

    return;
}

inline void bitvector_rot(bitvector_t *dest, bitvector_t * src, uint64_t rot_amnt, rot_dir_t dir)
{
    dest->vec_pos = (src->vec_pos + (dir * (rot_amnt / (8 * sizeof(*(src->vec)))))) % src->malloc_size; 
    dest->bit_pos = (src->bit_pos + (dir * rot_amnt)) % (8 * sizeof(*(src->vec))); 

    return;
}

#endif /* __BITVECTOR_C__ */

