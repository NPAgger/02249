/******************************************************************************
 * @file Bit matrix implementation testing file.                              *
 * @author Linas Kaminskas and Nis Agger.                                     *
 * @date 29-10-2015                                                           *
 * @modified 01-11-2015                                                       *
 ******************************************************************************/
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitmatrix.h"

#define DEBUG 2

uint64_t maxu64t(uint64_t a, uint64_t b);

void bitmatrix_printf(bitmatrix_t *mat_ptr);

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

/* todo : Add assertions for each test block.    */
/* todo : Make the DEBUG macro not being a mess. */
int main(int argc, char *argv[])
{
    auto uint64_t i = 0, j = 0, k = 0;
    
    #if DEBUG
    char *proc_name = argv[0];
    #endif /* DEBUG */

    #if DEBUG == 2
    auto uint64_t mat_m = 10, mat_n = 10;
    auto uint64_t row_rot_amnt = 1;
    auto uint64_t col_rot_amnt = 1;
    auto rot_dir_t rrot_dir = rot_dir_right;
    auto rot_dir_t crot_dir = rot_dir_down;
    auto uint64_t mat_amnt = 3;
    auto bitmatrix_t *m[3] = {NULL};
    #endif /* DEBUG == 2 */

    /* note : Test with a diagonal bit matrix. */  
    #if DEBUG == 2 
    for (i = 0; i < mat_amnt; ++i) {
        m[i] = bitmatrix_malloc(mat_m,mat_n);
        
        if (m[i] != NULL) {
            #ifdef DEBUG
            printf("%s: %s[%"PRIu64"] %s\n",proc_name,"bitmatrix m",i,"allocated");
            #endif /* DEBUG */
        }
    }

    for (i = 0; i < mat_amnt; ++i) {
        for (j = 0; j < maxu64t(mat_m,mat_n); ++j)
            bitmatrix_set(m[i],true,j,j);

        bitmatrix_printf(m[i]);
    }
    #endif /* DEBUG == 2 */

    #if DEBUG == 2 
    for (i = 0; i < 1; ++i) {
        bitmatrix_printf(m[i]);
        for (j = 0; j < mat_m; ++j) {
            bitmatrix_rrot(m[i],j,row_rot_amnt,rrot_dir);
            #ifdef DEBUG
            printf("%s: %s[%"PRIu64"] %"PRIu64"'th %s %"PRIu64"\n",proc_name,"bitmatrix m",i,j,"row rotated by",row_rot_amnt);
            #endif /* DEBUG */
        }
        bitmatrix_printf(m[i]);
        #ifdef DEBUG
        for (k = 0; k < m[i]->col_dim; ++k)
            printf("%"PRIu64" ",m[i]->col_pos[k]);
        printf("\n");
        #endif /* DEBUG */
    }
    #endif /* DEBUG == 2 */  
    
    #if DEBUG == 2 
    for (i = 0; i < 1; ++i) {
        for (j = 0; j < mat_n; ++j) {
            bitmatrix_crot(m[i],j,col_rot_amnt,crot_dir);
            #ifdef DEBUG
            printf("%s: %s[%"PRIu64"] %"PRIu64"'th %s %"PRIu64"\n",proc_name,"bitmatrix m",i,j,"col rotated by",col_rot_amnt);
            #endif /* DEBUG */
        }
        bitmatrix_printf(m[i]);
        #ifdef DEBUG
        for (k = 0; k < m[i]->row_dim; ++k)
            printf("%"PRIu64" ",m[i]->row_pos[k]);
        printf("\n");
        #endif /* DEBUG */
    }
    #endif /* DEBUG == 2 */  

    #if DEBUG == 2 
    for (i = 0; i < 1; ++i) {
        bitmatrix_not(m[i],m[i]);
        #ifdef DEBUG
        printf("%s: %s[%"PRIu64"]\n",proc_name,"negating bitmatrix m",i);
        #endif /* DEBUG */
        bitmatrix_printf(m[i]);
    }
    #endif /* DEBUG == 2 */  

    #if DEBUG == 2 
    bitmatrix_and(m[2],m[0],m[1]);
    #ifdef DEBUG
    printf("%s: %s[%"PRIu64"] %s[%"PRIu64"] %s[%"PRIu64"]\n",
           proc_name,"bitwise and'ing bitmatrix m",
           (uint64_t) 1,"and m",(uint64_t) 2,"to m",(uint64_t) 3);
    #endif /* DEBUG */
    bitmatrix_printf(m[0]);
    bitmatrix_printf(m[1]);
    bitmatrix_printf(m[2]);
    #endif /* DEBUG == 2 */  
    
    #if DEBUG == 2 
    bitmatrix_ior(m[2],m[0],m[1]);
    #ifdef DEBUG
    printf("%s: %s[%"PRIu64"] %s[%"PRIu64"] %s[%"PRIu64"]\n",
           proc_name,"bitwise or'ing bitmatrix m",
           (uint64_t) 1,"and m",(uint64_t) 2,"to m",(uint64_t) 3);
    #endif /* DEBUG */
    bitmatrix_printf(m[0]);
    bitmatrix_printf(m[1]);
    bitmatrix_printf(m[2]);
    #endif /* DEBUG == 2 */  
    
    #if DEBUG == 2 
    bitmatrix_xor(m[2],m[0],m[1]);
    #ifdef DEBUG
    printf("%s: %s[%"PRIu64"] %s[%"PRIu64"] %s[%"PRIu64"]\n",
           proc_name,"bitwise or'ing bitmatrix m",
           (uint64_t) 1,"and m",(uint64_t) 2,"to m",(uint64_t) 3);
    #endif /* DEBUG */
    bitmatrix_printf(m[0]);
    bitmatrix_printf(m[1]);
    bitmatrix_printf(m[2]);
    #endif /* DEBUG == 2 */  

    #if DEBUG == 2 
    for (i = 0; i < mat_amnt; ++i) {
        bitmatrix_free(m[i]);   
        #ifdef DEBUG
        printf("%s: %s[%"PRIu64"] %s\n",proc_name,"bitmatrix m",i,"deallocated");
        #endif /* DEBUG */
    }
    #endif /* DEBUG == 2 */  

    return EXIT_SUCCESS;
}

