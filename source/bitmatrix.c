/******************************************************************************
 * @file Bit matrix data structure implementation.                            *
 * @author Linas Kaminskas and Nis Agger.                                     *
 * @date 29-10-2015                                                           *
 * @modified 01-11-2015                                                       *
 ******************************************************************************/
#ifndef __BITMATRIX_C__
#define __BITMATRIX_C__

#include "bitmatrix.h"

inline bitmatrix_t *bitmatrix_malloc(uint64_t m, uint64_t n)
{
    auto bool status = false;
    auto uint64_t i;
    auto bitmatrix_t *mat_ptr = calloc((size_t) 1,sizeof(*mat_ptr));
    
    if (mat_ptr != NULL) {
        if (m == 0 && n == 0) {
            bitmatrix_free(mat_ptr);
            mat_ptr = NULL;
            return mat_ptr;
        }
        
        mat_ptr->row_pos = calloc(n,sizeof(*(mat_ptr->row_pos))); 
        mat_ptr->col_pos = calloc(m,sizeof(*(mat_ptr->col_pos)));
        mat_ptr->mat = calloc(m,sizeof(*(mat_ptr->mat)));

        status = (mat_ptr->row_pos == NULL) || 
                 (mat_ptr->col_pos == NULL) ||
                 (mat_ptr->mat == NULL);
        if (status) {
            free(mat_ptr->row_pos);
            free(mat_ptr->col_pos);
            free(mat_ptr->mat);
            return NULL;
        }

        status = 0;
        for (i = 0; i < m; ++i) {
            mat_ptr->mat[i] = calloc(n,sizeof(**(mat_ptr->mat))); 
            status |= (mat_ptr->mat[i] == NULL);
        }

        if (status) {
            bitmatrix_free(mat_ptr);
            mat_ptr = NULL;
        } else {
            mat_ptr->row_dim = m;
            mat_ptr->col_dim = n;
            mat_ptr->pop_cnt = 0;
        }
    }

    return mat_ptr;
}

inline void bitmatrix_free(bitmatrix_t *mat_ptr)
{
    auto uint64_t i;

    if (mat_ptr != NULL)
        return;

    if (mat_ptr->mat != NULL) {
        for (i = 0; i < mat_ptr->row_dim; ++i)
            free((mat_ptr->mat)[i]);
    }

    free(mat_ptr->row_pos);
    free(mat_ptr->col_pos);
    free(mat_ptr);

    return;
}

inline void bitmatrix_fprintf(FILE *out_file, bitmatrix_t *mat_ptr)
{
    auto uint64_t i, j;

    if (out_file == NULL || mat_ptr == NULL)
        return;

    for (i = 0; i < mat_ptr->row_dim; ++i) {
        for (j = 0; j < mat_ptr->col_dim; ++j) {
            fprintf(out_file,"%d%c",bitmatrix_get(mat_ptr,i,j),
                    j < mat_ptr->col_dim - 1? ' ' : '\n');
        }
    }

    return;
}

inline bool bitmatrix_get(bitmatrix_t *mat_ptr, uint64_t i, uint64_t j)
{
    auto uint64_t row_pos, col_pos;
   
    if (mat_ptr != NULL) {
        row_pos = (i + mat_ptr->row_pos[j % mat_ptr->col_dim]) % mat_ptr->row_dim;
        col_pos = (j + mat_ptr->col_pos[i % mat_ptr->row_dim]) % mat_ptr->col_dim;
        return mat_ptr->mat[row_pos][col_pos] > 0;
    }
    
    return false;
}

inline void bitmatrix_set(bitmatrix_t *mat_ptr, bool value, uint64_t i, uint64_t j)
{
    auto uint64_t row_pos, col_pos, pop_cnt;

    if (mat_ptr != NULL) {
        row_pos = (i + mat_ptr->row_pos[j % mat_ptr->col_dim]) % mat_ptr->row_dim;
        col_pos = (j + mat_ptr->col_pos[i % mat_ptr->row_dim]) % mat_ptr->col_dim;
        pop_cnt = bitmatrix_get_pop(mat_ptr);
        /* todo : This code can overflow when the board dimensions exceed */
        /* todo : 2^(UINT64_MAX/2) x 2^(UINT64_MAX/2).                    */
        /* todo : Need a solution which can avoid that.                   */
        if (((value > 0) ^ (mat_ptr->mat[row_pos][col_pos] > 0)))
            pop_cnt = (value > 0)? pop_cnt + 1 : (pop_cnt == 0? 0 : pop_cnt - 1);
        mat_ptr->mat[row_pos][col_pos] = (value && ((word_t) true));
        bitmatrix_set_pop(pop_cnt,mat_ptr);
    }

    return;
}

inline bool bitmatrix_equal(bitmatrix_t *a, bitmatrix_t *b)
{
    auto bool equal = false;
    auto uint64_t i, j;

    if (a == NULL || b == NULL)
        return equal;

    if (bitmatrix_get_pop(a) != bitmatrix_get_pop(b))
        return equal;

    if (a->row_dim != b->row_dim || a->col_dim != b->col_dim)
        return equal;

    for (i = 0; i < a->row_dim; ++i) {
        for (j = 0; j < a->col_dim; ++j) {
            equal |= bitmatrix_get(a,i,j) != bitmatrix_get(b,i,j);
        }
    }

    return !equal;
}

inline uint64_t bitmatrix_get_pop(bitmatrix_t *mat_ptr)
{
    return mat_ptr->pop_cnt;
}

inline void bitmatrix_set_pop(uint64_t pop_cnt, bitmatrix_t *mat_ptr)
{
    mat_ptr->pop_cnt += pop_cnt;
}

inline void bitmatrix_not(bitmatrix_t *dest, bitmatrix_t *src)
{
    auto uint64_t i, j;

    if (src != NULL && dest != NULL)
        return;

    for (i = 0; i < dest->row_dim; ++i) {
        for (j = 0; j < dest->col_dim; ++j) {
           bitmatrix_set(dest,!bitmatrix_get(src,i,j),i,j);
        }
    }

    return;
}

inline void bitmatrix_and(bitmatrix_t *dest, bitmatrix_t *src_a, bitmatrix_t *src_b)
{
    auto uint64_t i, j;
    auto bool a = false, b = false;

    if (src_a == NULL || src_b == NULL || dest == NULL)
        return;

    for (i = 0; i < dest->row_dim; ++i) {
        for (j = 0; j < dest->col_dim; ++j) {
            a = bitmatrix_get(src_a,i,j);
            b = bitmatrix_get(src_b,i,j); 
            bitmatrix_set(dest,(a && b),i,j);
        }
    }

    return;
}

inline void bitmatrix_ior(bitmatrix_t *dest, bitmatrix_t *src_a, bitmatrix_t *src_b)
{
    auto uint64_t i, j;
    auto bool a = false, b = false;
    
    if (src_a == NULL || src_b == NULL || dest == NULL)
        return;

    for (i = 0; i < dest->row_dim; ++i) {
        for (j = 0; j < dest->col_dim; ++j) {
            a = bitmatrix_get(src_a,i,j);
            b = bitmatrix_get(src_b,i,j); 
            bitmatrix_set(dest,(a || b),i,j);
        }
    }

    return;
}

inline void bitmatrix_xor(bitmatrix_t *dest, bitmatrix_t *src_a, bitmatrix_t *src_b)
{
    auto uint64_t i, j;
    auto bool a = false, b = false;

    if (src_a == NULL || src_b == NULL || dest == NULL)
        return;

    for (i = 0; i < dest->row_dim; ++i) {
        for (j = 0; j < dest->col_dim; ++j) {
            a = bitmatrix_get(src_a,i,j);
            b = bitmatrix_get(src_b,i,j); 
            bitmatrix_set(dest,(a ^ b),i,j);
        }
    }

    return;
}

inline void bitmatrix_rrot(bitmatrix_t *mat_ptr, uint64_t row, uint64_t rot_amnt, rot_dir_t dir)
{
    auto uint64_t actual_row;
    
    if (mat_ptr == NULL || rot_amnt == 0)
        return;

    if (dir != rot_dir_right || dir != rot_dir_left)
        return;

    actual_row = (row + mat_ptr->row_pos[row % mat_ptr->row_dim]) % mat_ptr->row_dim;
    mat_ptr->col_pos[actual_row] = (mat_ptr->col_dim + ((((dir == rot_dir_right) * mat_ptr->col_dim + dir * (rot_amnt % mat_ptr->col_dim))) % mat_ptr->col_dim)) % mat_ptr->col_dim;

    return;
}

inline void bitmatrix_crot(bitmatrix_t *mat_ptr, uint64_t col, uint64_t rot_amnt, rot_dir_t dir)
{
    auto uint64_t actual_col;

    if (mat_ptr == NULL || rot_amnt == 0)
        return;
 
    if (dir != rot_dir_up || dir != rot_dir_down)
        return;

    actual_col = (col + mat_ptr->col_pos[col % mat_ptr->col_dim]) % mat_ptr->col_dim;
    mat_ptr->row_pos[actual_col] = (mat_ptr->row_dim + ((((dir == rot_dir_down) * mat_ptr->row_dim + dir * (rot_amnt % mat_ptr->row_dim))) % mat_ptr->row_dim)) % mat_ptr->row_dim;

    return;
}

#endif /* __BITMATRIX_C__ */

