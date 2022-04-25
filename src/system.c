
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
    uint8_t ** matrix = (uint8_t**) malloc(nss);
    uint32_t i;
    for (i = 0; i < nss; i++){
        matrix[i] =(uint8_t*) malloc(nrs);
        uint32_t j;
        for (j = 0; j < nrs; j++) {
            matrix[i][j] = (uint8_t)tinymt32_generate_uint32(&prng); ;
        }
    }
    return matrix;
}

// perds de l'information -> chiffres < 0 -> positif (ex -1 -> 255)
void gf_256_gaussian_elimination(uint8_t **A, uint8_t **b, uint32_t symbol_size, uint32_t system_size)
{
    if(symbol_size != system_size) {
        printf("Système infinis");
        exit(1) ;
        
    }
	int n = system_size;
    float *x = malloc(n * sizeof (float));

    float **augmented = malloc((n + 2) * sizeof (float *));
	for (int i = 0; i < (n + 2); ++i)
		augmented[i] = malloc((n + 2) * sizeof (float));

    for (int i = 0; i < (n + 2); ++i)
        for (int j = 0; j < (n + 2); ++j)
            augmented[i][j] = 0;
    
    // checker si on ne eut pas réunir les doubles boucles for
    // IMPLENTER EN VECTORIEL
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            augmented[1 + i][1 + j] = A[i][j];

    for (int i = 0; i < n; ++i)
        augmented[1 + i][n + 1] = (*b)[i];

    
	
    for (int i = 1; i <= n - 1; ++i)
    {

        for (int j = i + 1; j <= n; ++j)
        {
            float ratio = augmented[j][i] / augmented[i][i]; // remplacer par fonction mult et inv 
                
            for (int k = 1; k <= n + 1; ++k)
                augmented[j][k] = augmented[j][k] - ratio*augmented[i][k];
        }
	}
    for (int i = 1; i <= n ; ++i)
		if (augmented[i][i] == 0.0) printf("Mathematical Error! \n"), exit(1);

	x[n] = augmented[n][n + 1] / augmented[n][n];
	
	for (int i = n - 1; i >= 1; i--)
	{
		x[i] = augmented[i][n + 1];
		for (int j = i + 1; j <= n; ++j)
		  	x[i] = x[i] - augmented[i][j]*x[j];
		x[i] = x[i] / augmented[i][i];
	}

	for (int i = 1; i <= n; ++i)
        (*b)[i - 1] = (uint8_t)x[i];
    free(x) ;
    for (int i = 0; i< n +2; i++) {
        free(augmented[i]) ;
    }
    free(augmented);
}
// tout est fait dans test e main aussi est dans test, quand vous faites une fonction fetes tout de suites les testes
// et oublie pas de retirer vos print d'essaie 
