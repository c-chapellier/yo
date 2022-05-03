
#include "../headers/system.h"
#include "../headers/tinymt32.h"

uint8_t *gf_256_full_add_vector(uint8_t *symbol_1, uint8_t *symbol_2, uint32_t symbol_size){
    
     uint8_t *result = (uint8_t *) malloc(symbol_size * sizeof(uint8_t));
     for(int i = 0 ; i<symbol_size; i++) {
         result[i] = symbol_1[i] ^ symbol_2[i] ; 
     }
     return  result;
}

uint8_t *gf_256_mul_vector(uint8_t *symbol, uint8_t coef, uint32_t symbol_size){
    uint8_t *result = (uint8_t*) malloc (symbol_size * sizeof(uint8_t));
    
    for (int i = 0; i < symbol_size; i++){
        result[i] = gf256_mul_table[symbol[i]][coef];
    }
    
    return result;
    }

uint8_t *gf_256_inv_vector(uint8_t *symbol, uint8_t coef, uint32_t symbol_size){
    uint8_t *result  = (uint8_t*) malloc (symbol_size * sizeof(uint8_t));
    
    for (int i = 0; i < symbol_size; i++){
        result[i] = gf256_mul_table[symbol[i]][gf256_inv_table[coef]];
    }
    
    return result ;
}

uint8_t **gen_coefs(uint32_t seed, uint32_t nss, uint32_t nrs){
    tinymt32_t prng;
    memset(&prng, 0, sizeof(tinymt32_t));
    // Do not modify these values!
    prng.mat1 = 0x8f7011ee;
    prng.mat2 = 0xfc78ff1f;
    prng.tmat = 0x3793fdff;
    tinymt32_init(&prng, seed);
    uint8_t ** matrix = (uint8_t**) malloc(nss * sizeof (uint8_t *));
    uint32_t i;
    for (i = 0; i < nss; i++){
        matrix[i] =(uint8_t*) malloc(nrs * sizeof (uint8_t));
        uint32_t j;
        for (j = 0; j < nrs; j++) {
            matrix[i][j] = (uint8_t)tinymt32_generate_uint32(&prng) % 256; ;
        }
    }
    return matrix;
}
// tout est fait dans test e main aussi est dans test, quand vous faites une fonction fetes tout de suites les testes
// et oublie pas de retirer vos print d'essaie 