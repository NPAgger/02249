/******************************************************************************
 * @file Madragon main program.                                               *
 * @author Linas Kaminskas and Nis Agger.                                     *
 * @date 29-10-2015                                                           *
 * @modified 01-11-2015                                                       *
 ******************************************************************************/
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "algorithm.h"
#include "bitvector.h"
#include "bitmatrix.h"

#ifndef VERBOSE
#define VERBOSE 0
#endif /* VERBOSE */

#ifndef DEBUG
#define DEBUG 1
#endif /* DEBUG */

/* minu64t : Fast way of finding a minimum between two uint64_t. */
uint64_t minu64t(uint64_t a, uint64_t b);

/* minu64t : Fast way of finding a maximum between two uint64_t. */
uint64_t maxu64t(uint64_t a, uint64_t b);

/* bitmatrix_printf : For printing bit matrices.                 */
void bitmatrix_printf(bitmatrix_t *mat_ptr);

/* initialise_madragon : Initiliase a Madragon game instance.    */
uint32_t initialise_madragon(madragon_t *dest, FILE *src);

/* destroy_madragon : Destroy a Madragon game instance.          */
uint32_t destroy_madragon(madragon_t *src);

int main(int argc, char *argv[])
{
    #if DEBUG == 1
    auto uint64_t k = 0;
    #endif /* DEBUG == 1 */
    auto madragon_t inst;
 
    #ifdef VERBOSE
    char *proc_name = argv[0];
    #endif /* VERBOSE */

    if (argc != 2) {
        #ifdef VERBOSE
        printf("%s: %s\n",proc_name,"missing input file as argument");
        #endif /* VERBOSE */
        return EXIT_FAILURE;
    }
        
    auto char *file_name = argv[1];
    auto FILE *data_file = fopen(file_name,"r");
    
    if (data_file == NULL) {
        #ifdef VERBOSE
        printf("%s: %s %s %s\n",proc_name,"file",file_name,"could not be opened or does not exist");
        #endif /* VERBOSE */
        return EXIT_FAILURE;
    }

    #if VERBOSE == 1
    printf("%s: %s\n",proc_name,"reading data");
    #endif /* VERBOSE == 1 */ 
    
    initialise_madragon(&inst,data_file);
    
    #if VERBOSE == 1
    printf("%s: %s\n",proc_name,"read data");
    #endif /* VERBOSE == 1 */
    
    #if VERBOSE == 1
    printf("%s: rows: %"PRIu64" columns: %"PRIu64" moves: %"PRIu64"\n",proc_name,inst.m,inst.n,inst.k);
    #endif /* VERBOSE == 1 */

    #if VERBOSE == 1
    printf("%s: %s\n",proc_name,"closing file");
    #endif /* VERBOSE == 1 */
    fclose(data_file);
    
    #if DEBUG == 1
    printf("\n");
    for (k = 0; k < MADRAGON_INST_COUNT; ++k) {
        bitmatrix_printf(inst.a[k]);
        printf("\n");
    }
    #endif /* DEBUG == 1 */

    auto bool decision_ans;
    decision_ans = algorithm(&inst);

    destroy_madragon(&inst);

    return !decision_ans;
}

inline uint64_t minu64t(uint64_t a, uint64_t b)
{
    return b ^ ((a ^ b) & -(a < b));
}

inline uint64_t maxu64t(uint64_t a, uint64_t b)
{
    return a ^ ((a ^ b) & -(a < b));
}

inline void bitmatrix_printf(bitmatrix_t *mat_ptr)
{
    auto uint64_t i, j;

    for (i = 0; i < mat_ptr->row_dim; ++i) {
        for (j = 0; j < mat_ptr->col_dim; ++j) {
            printf("%d",bitmatrix_get(mat_ptr,i,j));
        }

        printf("\n");
    }

    return;
}

uint32_t initialise_madragon(madragon_t *instance, FILE *data_file)
{
    auto char data;
    auto uint32_t status;
    auto uint64_t i = 0, j = 0, k = 0;
 
    if (instance == NULL || data_file == NULL)
        return 1;

    /* note : Make some sanity checks to see if the dimensions are proper. */
    status = fscanf(data_file,"%"SCNu64"",&(instance->m));
    status = fscanf(data_file,"%"SCNu64"",&(instance->n));
    status = fscanf(data_file,"%"SCNu64"",&(instance->k));

    status = 0;
    for (k = 0; k < MADRAGON_INST_COUNT; ++k) { 
        instance->a[k] = bitmatrix_malloc(instance->m,instance->n);
        status |= (instance->a[k] == NULL);
    }

    if (status)
        return destroy_madragon(instance);

    k = 0;
    while(!feof(data_file)) {
        fscanf(data_file,"%c",&data);
                
        #ifdef DEBUG
        printf("%c",data);
        #endif /* DEBUG */ 
        if (data == 'b' || data == 'w') {
            bitmatrix_set(instance->a[k],(data == 'w'),i,j);
            j = (uint64_t) (j + 1) % instance->n;
            i = (uint64_t) (i + (j == 0)) % instance->m;
            k = (uint64_t) (k + (i == 0 && j == 0)) % MADRAGON_INST_COUNT;
        }
    }

    return 0;
}

uint32_t destroy_madragon(madragon_t *instance)
{
    auto uint64_t i;
    if (instance == NULL)
        return 1;

    for (i = 0; i < MADRAGON_INST_COUNT; ++i)
        bitmatrix_free(instance->a[i]);

    return 0;
}

