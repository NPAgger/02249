#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitvector.h"

#define DEBUG 1

void bitvector_printf(bitvector_t *vec_ptr);

inline void bitvector_printf(bitvector_t *vec_ptr)
{
    auto uint64_t i;

    for (i = 0; i < vec_ptr->vector_size; ++i)
        printf("%d",bitvector_get(vec_ptr,i));    
    
    printf("\n");

    return;
}

int main(int argc, char *argv[])
{
    auto uint64_t i = 0, j = 0;
    
    #if DEBUG
    char *proc_name = argv[0];
    #endif /* DEBUG */

    #if DEBUG == 1
    auto uint64_t vec_size = 65;
    auto uint64_t shift_amnt = 7;
    auto uint64_t vec_amnt = 3;
    auto bitvector_t *t[3] = {NULL};
    #endif /* DEBUG == 1 */
    
    #if DEBUG == 1
    for (i = 0; i < vec_amnt; ++i) {
        t[i] = bitvector_malloc(vec_size);
        if (t[i] != NULL) {
            #ifdef DEBUG
            printf("%s: %s[%"PRIu64"] %s\n",proc_name,"bitvector t",i,"allocated");
            #endif /* DEBUG */
        }
        bitvector_set(t[i],true,1);
        bitvector_set(t[i],true,7);
    }
    #endif /* DEBUG == 1 */

    #if DEBUG == 1
    printf("%s: %s\n",proc_name,"bitvector value");
    #endif /* DEBUG == 1 */ 

    #if DEBUG == 1
    /* note : Test left shifting of bitvectors. */
    for (i = 0; i < vec_size; ++i) {
        bitvector_printf(t[0]);
        bitvector_rot(t[0],t[0],shift_amnt,left);
    }

    /* note : Test right shifting of bitvectors. */
    for (i = 0; i < vec_size; ++i) {
        bitvector_printf(t[0]);
        bitvector_rot(t[0],t[0],shift_amnt,right);
    }
    #endif /* DEBUG == 1 */ 
    
    #if DEBUG == 1
    /* note : Test bitwise operators on bitvectors. */
    bitvector_xor(t[2],t[0],t[1]);
    bitvector_printf(t[2]);
    #endif /* DEBUG == 1 */ 

    #if DEBUG == 1
    for (i = 0; i < vec_amnt; ++i) {
        bitvector_free(t[i]);
        t[i] = NULL;
        printf("%s: %s[%"PRIu64"] %s\n",proc_name,"bitvector t",i,"deallocated");
    }
    #endif /* DEBUG == 1 */

    return EXIT_SUCCESS;
}

