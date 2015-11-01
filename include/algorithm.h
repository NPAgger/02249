/******************************************************************************
 * @file Algorithm implementation interface.                                  *
 * @author Linas Kaminskas and Nis Agger.                                     *
 * @date 29-10-2015                                                           *
 * @modified 01-11-2015                                                       *
 ******************************************************************************/
#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitmatrix.h"

#ifndef MADRAGON_INST_COUNT
#define MADRAGON_INST_COUNT 2
#endif /* MADRAGON_INST_COUNT */

/* operation_t : Structure for tracking moves.                    */
/* todo : Create an initialisation and destruction functions      */
/* todo : for the operation_t, this is useful for reconstruction. */
typedef struct
{
    uint64_t *row_pos;
    uint64_t *col_pos;
    rot_dir_t *direction;
    uint64_t amount;
} operation_t;

/* madragon_t : The instance type for Madragon. */
typedef struct
{
    bitmatrix_t *a[MADRAGON_INST_COUNT];
    operation_t *o;
    uint64_t m;
    uint64_t n;
    uint64_t k;
} madragon_t;

/* madragon_t : Algorithm for solving Madragon instances. */
extern bool algorithm(madragon_t *instance);

#endif /* __ALGORITHM_H__ */

