#ifndef __BITVECTOR_H__
#define __BITVECTOR_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef __WORD_T__
#define __WORD_T__
typedef uint64_t word_t;
#endif /* __WORD_T__ */

typedef struct
{
    uint64_t vec_pos;
    uint64_t bit_pos;
    uint64_t malloc_size;
    uint64_t vector_size;
      word_t *vec;
} bitvector_t;

#ifndef __ROT_DIR_T__
#define __ROT_DIR_T__
typedef enum
{
    rot_dir_right = -1,
    rot_dir_left = 1,
    rot_dir_up = 1,
    rot_dir_down = -1
} rot_dir_t;
#endif /* __ROT_DIR_T__ */

extern bitvector_t *bitvector_malloc(uint64_t n);

extern void bitvector_free(bitvector_t *src);

extern void bitvector_set(bitvector_t *dest, bool value, uint64_t bit);

extern bool bitvector_get(bitvector_t *dest, uint64_t bit);

extern void bitvector_add(bitvector_t *dest, bitvector_t *src_a, bitvector_t *src_b);

extern void bitvector_sub(bitvector_t *dest, bitvector_t *src_a, bitvector_t *src_b);

extern void bitvector_not(bitvector_t *dest, bitvector_t *src);

extern void bitvector_and(bitvector_t *dest, bitvector_t *src_a, bitvector_t *src_b);

extern void bitvector_or(bitvector_t *dest, bitvector_t *src_a, bitvector_t *src_b);

extern void bitvector_xor(bitvector_t *dest, bitvector_t *src_a, bitvector_t *src_b);

extern void bitvector_lshift(bitvector_t *dest, bitvector_t *src, uint64_t shft_amnt);

extern void bitvector_rshift(bitvector_t *dest, bitvector_t *src, uint64_t shft_amnt);

extern void bitvector_lrot(bitvector_t *dest, bitvector_t *src, uint64_t rot_amnt);

extern void bitvector_rrot(bitvector_t *dest, bitvector_t *src, uint64_t rot_amnt);

extern void bitvector_rot(bitvector_t *dest, bitvector_t *src, uint64_t rot_amnt, rot_dir_t dir);

#endif /* __BITVECTOR_H__ */

