/******************************************************************************
 * @file Algorithm implementation a Madragon solver.                          *
 * @author Linas Kaminskas and Nis Agger.                                     *
 * @date 29-10-2015                                                           *
 * @modified 01-11-2015                                                       *
 ******************************************************************************/
#ifndef __ALGORITHM_C__
#define __ALGORITHM_C__

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "algorithm.h"
#include "bitmatrix.h"

inline operation_t *operation_malloc(uint64_t k)
{
    auto operation_t *ops_ptr = calloc((size_t) 1, sizeof(*ops_ptr));

    if (k == 0) {
        operation_free(ops_ptr);
        return NULL;
    }

    ops_ptr->row_pos = calloc((size_t) k, sizeof(*ops_ptr));
    ops_ptr->col_pos = calloc((size_t) k, sizeof(*ops_ptr));
    ops_ptr->direction = calloc((size_t) k, sizeof(*ops_ptr));

    if (ops_ptr->row_pos == NULL || ops_ptr->col_pos == NULL || ops_ptr->direction == NULL) {
        operation_free(ops_ptr);
        return NULL;
    }

    ops_ptr->amount = 0;

    return ops_ptr;
}

inline void operation_free(operation_t *ops_ptr)
{
    if (ops_ptr == NULL)
        return;

    free(ops_ptr->row_pos);
    free(ops_ptr->col_pos);
    free(ops_ptr->direction);
    free(ops_ptr);
    
    return;
}

inline void operation_reconstruct(FILE *out_file, madragon_t *instance)
{
    auto uint64_t k;

    bitmatrix_t *t = bitmatrix_malloc(instance->m,instance->n);
    bitmatrix_ior(t,instance->a[0],t);

    for (k = 0; k < instance->o->amount; ++k) {
        if (instance->o->row_pos[k] != 0)
            bitmatrix_rrot(t,instance->o->row_pos[k],instance->o->direction[k],rot_dir_right);   
        else if (instance->o->col_pos[k] != 0)
            bitmatrix_crot(t,instance->o->col_pos[k],instance->o->direction[k],rot_dir_up);   
        if (instance->o->row_pos != 0 || instance->o->col_pos != 0) {
            bitmatrix_fprintf(out_file,t);
            fprintf(out_file,"\n\n");
        }
    }

    bitmatrix_fprintf(out_file,instance->a[1]);   
    fprintf(out_file,"\n\n");

    return;
}

/* algorithm : Algorithm for solving Madragon instances.               */
/*      todo : Currently, uses brute-force: try the whole state space. */
/*      todo : Currently not working. */
bool algorithm(madragon_t *instance)
{
    auto bool decision_ans = false;
    auto uint64_t i, j, k, l;
    auto bitmatrix_t *diff = bitmatrix_malloc(instance->m, instance->n);
    auto bitmatrix_t *zero = bitmatrix_malloc(instance->m, instance->n);

    if (diff == NULL || zero == NULL) {
        bitmatrix_free(diff);
        bitmatrix_free(zero);
        return decision_ans;
    }

    for (k = 0; k <= instance->k; ++k) {
        for (i = 0; i < instance->m; ++i) {
            for (j = 0; j < instance->n; ++j) {
                bitmatrix_and(diff,instance->a[0],instance->a[1]);
                if (bitmatrix_equal(diff,zero)) {
                    decision_ans = (k <= instance->k);
                    i = instance->m;
                    j = instance->n;
                    k = instance->k + 1;
                } else {
                    bitmatrix_crot(instance->a[0],j,1,rot_dir_up);                    
                }
            }
            bitmatrix_rrot(instance->a[0],i,1,rot_dir_right);                    
        }
    }

    bitmatrix_free(diff);
    bitmatrix_free(zero);

    return decision_ans;
}

#endif /* __ALGORITHM_C__ */

