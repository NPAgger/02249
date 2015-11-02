/******************************************************************************
 * @file Bit matrix data structure implementation interface.                  *
 * @author Linas Kaminskas and Nis Agger.                                     *
 * @date 29-10-2015                                                           *
 * @modified 01-11-2015                                                       *
 ******************************************************************************/
#ifndef __BITMATRIX_H__
#define __BITMATRIX_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef __WORD_T__
#define __WORD_T__
typedef uint8_t word_t;
#endif /* __WORD_T__ */

typedef struct
{
    uint64_t *row_pos;
    uint64_t *col_pos;
    uint64_t row_dim;
    uint64_t col_dim;
    uint64_t pop_cnt;
        word_t **mat;
} bitmatrix_t;

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

extern bitmatrix_t *bitmatrix_malloc(uint64_t m, uint64_t n);

extern void bitmatrix_free(bitmatrix_t *src);

extern void bitmatrix_fprintf(FILE *dest, bitmatrix_t *mat_ptr);

extern bool bitmatrix_get(bitmatrix_t *dest, uint64_t i, uint64_t j);

extern void bitmatrix_set(bitmatrix_t *dest, bool value, uint64_t i, uint64_t j);

extern bool bitmatrix_equal(bitmatrix_t *a, bitmatrix_t *b);

extern uint64_t bitmatrix_get_pop(bitmatrix_t *src);

extern void bitmatrix_set_pop(uint64_t pop_cnt, bitmatrix_t *src);

extern void bitmatrix_not(bitmatrix_t *dest, bitmatrix_t *src);

extern void bitmatrix_and(bitmatrix_t *dest, bitmatrix_t *src_a, bitmatrix_t *src_b);

extern void bitmatrix_ior(bitmatrix_t *dest, bitmatrix_t *src_a, bitmatrix_t *src_b);

extern void bitmatrix_xor(bitmatrix_t *dest, bitmatrix_t *src_a, bitmatrix_t *src_b);

extern void bitmatrix_rrot(bitmatrix_t *src, uint64_t row, uint64_t rot_amnt, rot_dir_t dir);

extern void bitmatrix_crot(bitmatrix_t *src, uint64_t col, uint64_t rot_amnt, rot_dir_t dir);

#endif /* __BITMATRIX_H__ */

