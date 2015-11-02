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
#include "bitmatrix.h"

#ifndef VERBOSE
#define VERBOSE 0
#endif /* VERBOSE */

#ifndef DEBUG
#define DEBUG 0
#endif /* DEBUG */

/* minu64t : Fast way of finding a minimum between two uint64_t. */
uint64_t minu64t(uint64_t a, uint64_t b);

/* minu64t : Fast way of finding a maximum between two uint64_t. */
uint64_t maxu64t(uint64_t a, uint64_t b);

/* initialise_madragon : Initiliase a Madragon game instance.    */
uint32_t initialise_madragon(madragon_t *dest, FILE *src);

/* destroy_madragon : Destroy a Madragon game instance.          */
uint32_t destroy_madragon(madragon_t *src);

int main(int argc, char *argv[])
{
    #if DEBUG == 1
    auto uint64_t k = 0;
    #endif /* DEBUG == 1 */
    auto bool decision_ans;
    auto madragon_t inst;
 
    #ifdef VERBOSE
    char *proc_name = argv[0];
    #endif /* VERBOSE */

    if (argc < 2) {
        #ifdef VERBOSE
        printf("%s: %s\n",proc_name,"missing input file as argument");
        #endif /* VERBOSE */
        return EXIT_FAILURE;
    }

    if (argc > 3) {
        #ifdef VERBOSE
        printf("%s: %s\n",proc_name,"too many arguments");
        #endif /* VERBOSE */
        return EXIT_FAILURE;
    }

    auto char *data_name = argv[1];
    auto FILE *data_file = fopen(data_name,"r");

    if (data_file == NULL) {
        #ifdef VERBOSE
        printf("%s: %s %s %s\n",proc_name,"file",data_name,"could not be opened or does not exist");
        #endif /* VERBOSE */
        return EXIT_FAILURE;
    }
    
    auto char *out_name = NULL;
    auto FILE *out_file = NULL;

    if (argc == 3) {
        out_name = argv[2];
        out_file = fopen(out_name,"w");
        
        if (out_file == NULL) {
            #ifdef VERBOSE
            printf("%s: %s %s %s\n",proc_name,"file",out_name,"could not be opened or does not exist");
            #endif /* VERBOSE */
        }
    }

    #if VERBOSE == 1
    printf("%s: %s\n",proc_name,"reading data");
    #endif /* VERBOSE == 1 */ 

    if (initialise_madragon(&inst,data_file) != 0) {
        #ifdef VERBOSE
        printf("%s: %s\n",proc_name,"illegal madragon instance");
        #endif /* VERBOSE */
        return EXIT_FAILURE;
    }

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
        bitmatrix_fprintf(stdout,inst.a[k]);
        printf("\n");
        printf("\n");
    }
    #endif /* DEBUG == 1 */

    decision_ans = algorithm(&inst);

    if (out_file != NULL)
        operation_reconstruct(out_file,&inst);

    if (destroy_madragon(&inst) != 0) {
        #ifdef VERBOSE
        printf("%s: %s\n",proc_name,"could not destroy madragon instance");
        #endif /* VERBOSE */
        return EXIT_FAILURE;
    }

    #ifdef VERBOSE
    printf("%s: %s %s\n",proc_name,"the instance is",decision_ans? "solvable" : "unsolvable");
    #endif /* VERBOSE */

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

uint32_t initialise_madragon(madragon_t *instance, FILE *data_file)
{
    auto char data;
    auto uint32_t status;
    auto uint64_t i = 0, j = 0, k = 0;
 
    if (instance == NULL || data_file == NULL)
        return 1;

    status = fscanf(data_file,"%"SCNu64"",&(instance->m));
    if (status == EOF)
        return 1;
    
    status = fscanf(data_file,"%"SCNu64"",&(instance->n));
    if (status == EOF)
        return 1;
    
    status = fscanf(data_file,"%"SCNu64"",&(instance->k));
    if (status == EOF)
        return 1;

    if (instance->m == 0 || instance->n == 0 || instance->k == 0)
        return 1;

    status = 0;
    for (k = 0; k < MADRAGON_INST_COUNT; ++k) { 
        instance->a[k] = bitmatrix_malloc(instance->m,instance->n);
        status |= (instance->a[k] == NULL);
    }
    
    instance->o = operation_malloc(k);
    status |= (instance->o == NULL);

    if (status)
        return destroy_madragon(instance);

    k = 0;
    while(status != EOF) {
        status = fscanf(data_file,"%c",&data);
                
        #if DEBUG == 1
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

    operation_free(instance->o);

    return 0;
}

