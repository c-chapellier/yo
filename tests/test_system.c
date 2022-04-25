#include "../headers/tinymt32.h"
#include <CUnit/Basic.h>
#include "../src/system.c"
void test_gf_256_full_add_vector() {
    uint8_t *symbol_1 = (uint8_t *) malloc(7*sizeof(uint8_t)) ;
    uint8_t *symbol_2 = (uint8_t *) malloc(7*sizeof(uint8_t)) ;
    uint8_t *add_vecteur = (uint8_t *) malloc(7*sizeof(uint8_t)) ;
    uint32_t symbol_size = 7 ; 
    symbol_1[0] = 53 ;
    symbol_1[1] = 255 ;
    symbol_1[2] = 0 ;
    symbol_1[3] = 85 ; 
    symbol_1[4] = 0 ;
    symbol_1[5] = 250 ;
    symbol_1[6] = 255 ;

    symbol_2[0] = 85 ;
    symbol_2[1] = 204;
    symbol_2[2] = 170;
    symbol_2[3] = 170 ;
    symbol_2[4] = 0 ;
    symbol_2[5] = 122 ;
    symbol_2[6] =  1 ; 


    
    add_vecteur[0] = 96 ;
    add_vecteur[1] = 51;
    add_vecteur[2] = 170;
    add_vecteur[3] = 255 ;
    add_vecteur[4] = 0 ;
    add_vecteur[5] = 128 ; 
    add_vecteur[6] = 254 ; 
    uint8_t *x = gf_256_full_add_vector(symbol_1,symbol_2,symbol_size) ;
    for(int i = 0 ; i < 7; i++) {
        CU_ASSERT_EQUAL(add_vecteur[i], x[i]) ;
    }
    free(x) ;
    free(add_vecteur) ;
    free(symbol_1) ;
    free(symbol_2) ;
}
void test_gf_256_mul_vector() {
    uint8_t *symbol_1 = (uint8_t *) malloc(4*sizeof(uint8_t)) ;
    uint8_t test [] = {0,0,0,0,85,0,250,255,170,0,233,227,255,0,19,28} ;


    uint32_t symbol_size = 4 ; 
    symbol_1[0] = 85 ; 
    symbol_1[1] = 0 ;
    symbol_1[2] = 250 ;
    symbol_1[3] = 255 ;
    int v = 0 ; 
    for(uint8_t i = 0 ; i < 4 ; i++) {
        uint8_t *result = gf_256_mul_vector(symbol_1,i,symbol_size) ; 
        for(int l = 0 ; l < 4 ;l++) {
            CU_ASSERT_EQUAL(result[l],test[v + l])
            }
        free(result) ;
        v = v + 4 ; 
        }
    free(symbol_1) ; 
}

void test_tinymt32_gen_42(){
    tinymt32_t prng;
    memset(&prng, 0, sizeof(tinymt32_t));
    prng.mat1 = 0x8f7011ee;
    prng.mat2 = 0xfc78ff1f;
    prng.tmat = 0x3793fdff;
    tinymt32_init(&prng, 42);

    uint8_t expected_res[] = {
        171, 165, 55, 61, 69, 143, 152, 158, 168, 64, 5, 91, 215, 134, 85, 17, 29, 41, 122, 169, 75, 0, 84, 85, 231, 156, 156, 201, 100, 17, 96, 137, 207, 72, 15, 182, 183, 154, 222, 92, 143, 153, 105, 58, 56, 239, 37, 75, 40, 133, 106, 199, 200, 144, 136, 175, 219, 87, 100, 59, 62, 26, 208, 19, 194, 227, 31, 187, 176, 225, 189, 120, 189, 78, 13, 112, 12, 47, 80, 39, 149, 170, 184, 23, 227, 247, 99, 218, 163, 140, 58, 248, 171, 12, 99, 5, 78, 178, 155, 127, 138, 37, 12, 54, 142, 80, 96, 206, 24, 23, 127, 242, 106, 86, 135, 185, 255, 157, 1, 91, 213, 135, 75, 87, 216, 233, 11, 7, 235, 168, 31, 229, 231, 29, 51, 105, 163, 139, 156, 104, 119, 247, 158, 210, 158, 15, 120, 27, 237, 28, 31, 23, 212, 240, 44, 95, 201, 157, 168, 14, 20, 10, 222, 208, 34, 137, 217, 221, 111, 155, 4, 97, 119, 31, 132, 108, 62, 135, 76, 148, 18, 249, 169, 95, 162, 18, 68, 109, 50, 102, 216, 62, 204, 16, 160, 5, 205, 223, 75, 187, 239, 64, 64, 135, 1, 160, 141, 0, 132, 64, 59, 35, 24, 245, 245, 132, 126, 134, 41, 236, 88, 155, 108, 12, 100, 214, 87, 67, 172, 46, 71, 217, 97, 36, 37, 26, 87, 204, 17, 159, 91, 98, 64, 78, 87, 134, 18, 165, 233, 187, 71, 90, 71, 52, 203, 208, 145, 248, 42, 228, 214, 100, 243, 155, 183, 250, 126, 133, 243, 75, 236, 91, 177, 213, 20, 166, 198, 41, 112, 255, 152, 84, 32, 27, 76, 182, 47, 39, 226, 11, 226, 87, 162, 62, 51, 237, 107, 147, 230, 127, 149, 222, 94, 29, 194, 250, 60, 19, 215, 38, 187, 6, 254, 186, 227, 57, 158, 134, 230, 239, 185, 211, 91, 118, 187, 251, 221, 194, 54, 15, 90, 181, 219, 232, 180, 182, 94, 86, 43, 114, 132, 195, 222, 144, 118, 90, 135, 118, 204, 98, 116, 235, 198, 181, 229, 37, 208, 204, 55, 82, 123, 172, 185, 228, 57, 248, 248, 144, 154, 1, 172, 228, 233, 203, 137, 17, 4, 226, 23, 229, 119, 168, 223, 145, 203, 71, 54, 252, 34, 26, 36, 89, 35, 246, 2, 175, 191, 162, 60, 49, 171, 0, 2, 46, 248, 193, 114, 96, 174, 188, 240, 100, 238, 73, 142, 31, 14, 42, 161, 58, 193, 142, 116, 147, 9, 130, 60, 95, 155, 133, 200, 245, 250, 179, 29, 55, 208, 79, 110, 152, 214, 50, 115, 115, 224, 184, 74, 73, 209, 151, 127, 167, 22, 111, 127, 1, 93, 95, 196, 49, 17, 211, 71, 141, 45, 127, 2, 151, 190, 205, 1, 90, 76, 59, 176, 146, 33, 237, 71, 27, 227, 97, 70, 24, 87, 209, 237, 105, 8, 93, 81, 144, 171, 189, 16, 9, 34, 196, 201, 205, 179, 224, 34, 122, 118, 117, 199, 169, 109, 65, 180, 219, 186, 39, 221, 87, 151, 246, 243, 29, 172, 162, 233, 226, 96, 253, 140, 203, 181, 167, 49, 200, 72, 23, 232, 58, 45, 60, 14, 128, 78, 228, 204, 152, 80, 101, 105, 178, 44, 99, 218, 209, 97, 54, 5, 178, 246, 247, 232, 244, 130, 146, 67, 2, 219, 175, 142, 158, 174, 21, 26, 168, 203, 86, 242, 93, 239, 164, 26, 64, 150, 184, 71, 165, 152, 109, 163, 3, 42, 16, 213, 73, 160, 188, 74, 104, 219, 167, 197, 207, 227, 108, 3, 144, 150, 49, 174, 233, 119, 151, 161, 33, 242, 249, 94, 57, 101, 61, 241, 93, 86, 240, 245, 111, 87, 110, 230, 134, 205, 255, 37, 163, 156, 93, 150, 68, 131, 11, 8, 24, 46, 196, 151, 88, 89, 93, 241, 28, 194, 15, 176, 91, 60, 92, 126, 238, 251, 95, 153, 174, 108, 251, 118, 202, 216, 96, 207, 176, 7, 174, 1, 68, 116, 0, 252, 225, 248, 0, 85, 247, 222, 93, 57, 61, 224, 53, 210, 1, 150, 47, 165, 152, 185, 253, 6, 213, 158, 90, 29, 210, 130, 39, 11, 125, 1, 75, 16, 145, 203, 247, 8, 140, 72, 168, 199, 40, 59, 222, 183, 133, 102, 65, 161, 156, 163, 8, 27, 136, 185, 146, 44, 128, 64, 8, 73, 124, 241, 51, 157, 250, 110, 114, 213, 36, 153, 83, 24, 59, 38, 33, 9, 46, 232, 221, 136, 181, 11, 234, 247, 177, 206, 34, 232, 136, 10, 73, 190, 50, 198, 140, 132, 107, 83, 71, 146, 210, 198, 4, 104, 118, 246, 200, 168, 179, 136, 116, 215, 60, 71, 250, 225, 56, 199, 96, 181, 110, 138, 57, 115, 121, 123, 103, 136, 82, 21, 212, 236, 49, 26, 88, 239, 204, 147, 213, 9, 4, 112, 82, 12, 230, 193, 43, 10, 250, 172, 29, 101, 26, 101, 122, 96, 113, 164, 29, 43, 32, 5, 255, 124, 10, 43, 190, 207, 19, 210, 61, 143, 106, 229, 73, 63, 110, 38, 112, 243, 99, 121, 43, 148, 221, 141, 253, 90, 238, 58, 240, 172, 79, 8, 101, 139, 251, 216, 8, 67, 68, 164, 119, 160, 76, 54, 239, 1, 204, 208, 105, 93, 26, 199, 139, 231, 102, 183, 70, 168, 193, 179, 154, 83, 38, 11, 110, 174, 140, 14, 15, 35, 43, 217, 31, 174, 124, 237, 144, 185, 47, 48, 51, 170, 189, 156, 37, 234, 121, 10, 220, 17, 198, 81, 168, 93, 11, 240, 121, 97, 105, 188, 114, 133, 188, 20, 45, 130, 214, 241, 24, 100, 134, 112, 225, 203, 121, 141, 239, 31, 125, 13, 215, 251, 91, 165, 178, 160, 86, 152, 207, 185, 134, 21, 127, 101, 208, 217, 159, 134, 242, 35, 231, 123, 183, 172, 84, 111, 132, 74, 89, 1, 85, 217, 138, 206, 139, 222, 224, 126, 221, 96, 118, 138, 81
    };

    for (int i = 0; i < 1000; ++i)
    {
        uint8_t coef = (uint8_t)tinymt32_generate_uint32(&prng);
        CU_ASSERT_EQUAL(coef, expected_res[i]);
    }
}
void test_gf_256_inv_vector(){
    uint8_t test [] = {0,0,0,0,85,0,250,255,164,0,125,241,51,0,86,85} ;
    uint8_t *symbol_1 = (uint8_t *) malloc(4*sizeof(uint8_t)) ;
    uint32_t symbol_size = 4 ; 
    symbol_1[0] = 85 ; 
    symbol_1[1] = 0 ;
    symbol_1[2] = 250 ;
    symbol_1[3] = 255 ;
    int v = 0 ; 
    for(uint8_t i = 0 ; i < 4 ; i++) {
        uint8_t *result = gf_256_inv_vector(symbol_1,i,symbol_size) ; 
        for(int l = 0 ; l < 4 ;l++) {
            CU_ASSERT_EQUAL(result[l],test[v + l])
            }
        free(result) ;
        v = v + 4 ; 
        }
    free(symbol_1) ; 
}

void test_gf_256_gaussian_elimination_negatif() {
    uint8_t **a = malloc(2 * sizeof (uint8_t *));
	for (int i = 0; i < 2; ++i)
		a[i] = malloc(2 * sizeof (uint8_t));
	a[0][0] = 2;
	a[0][1] = 1;
	a[1][0] = 3;
	a[1][1] = 1;

	uint8_t *b = malloc(2 * sizeof (uint8_t));
	b[0] = 3;
	b[1] = 2;

    uint8_t *answear = malloc(2 * sizeof (uint8_t));
    answear[0] = 255 ; 
    answear[1] = 5 ; 


    gf_256_gaussian_elimination(a, &b, 2, 2);
    CU_ASSERT_EQUAL(*b,*answear) ; 
    free(answear) ;
    free(b) ;
}
// pas encore bonne doit modivier pour le determinant 
uint8_t* test_gf_256_gaussian_elimination_no_solution() {
    uint8_t **a = malloc(2 * sizeof (uint8_t *));
	for (int i = 0; i < 2; ++i)
		a[i] = malloc(2 * sizeof (uint8_t));
	a[0][0] = 2;
	a[0][1] = 1;
	a[1][0] = 2;
	a[1][1] = 1;

	uint8_t *b = malloc(2 * sizeof (uint8_t));
	b[0] = 3;
	b[1] = 2;
    gf_256_gaussian_elimination(a, &b, 2, 2);
    return b ; 
  
}

int main()
{
    // uint8_t matrix [][] ={{0,0,0},{58,41,0},{48,218,196},{135,164,243},{122,252,234},{80,117,232}} ;
    // for (int i = 0; i < 6; i++) {
    //     for(int l = 0 ;l < 3; l++) {
    //         printf("%d \t", matrix[i][l]);
    //     }
    //     printf("\n") ;
    // }

    //Structure du main pour les testes, elle consistes juste a changer les mots de la fonction comme voir en bas 
    // -------- test uint8_t **gen_coefs --------
    // if (CU_initialize_registry() != CUE_SUCCESS)
    //     errx(EXIT_FAILURE, "can't initialize test registry");
    // CU_pSuite gen_co = CU_add_suite("gen_coeff", NULL, NULL);
    // if (CU_get_error() != CUE_SUCCESS)
    //     errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    // CU_add_test(gen_co, "answear_negatif",test_tinymt32_gen_42);
    // CU_basic_run_tests();
    // CU_cleanup_registry();
    

    
    
    // -------- test gf_256_gaussian_elimination --------
    // det = 0 
    //     uint8_t **a = malloc(2 * sizeof (uint8_t *));
	// for (int i = 0; i < 2; ++i)
	// 	a[i] = malloc(2 * sizeof (uint8_t));
	// a[0][0] = 2;
	// a[0][1] = 1;
	// a[1][0] = 2;
	// a[1][1] = 1;

	// uint8_t *b = malloc(2 * sizeof (uint8_t));
	// b[0] = 3;
	// b[1] = 2;
    // gf_256_gaussian_elimination(a, &b, 2, 2);

    // if (CU_initialize_registry() != CUE_SUCCESS)
    //     errx(EXIT_FAILURE, "can't initialize test registry");
    // CU_pSuite gaussienSuite = CU_add_suite("gaussien", NULL, NULL);
    // if (CU_get_error() != CUE_SUCCESS)
    //     errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    // CU_add_test(gaussienSuite, "answear_negatif",test_gf_256_gaussian_elimination_negatif);
    // CU_basic_run_tests();
    // CU_cleanup_registry();

    // if (CU_initialize_registry() != CUE_SUCCESS)
    //     errx(EXIT_FAILURE, "can't initialize test registry");
    // CU_pSuite gaussienSuite = CU_add_suite("gaussien", NULL, NULL);
    // if (CU_get_error() != CUE_SUCCESS)
    //     errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    // CU_add_test(gaussienSuite, "answear_negatif",test_gf_256_gaussian_elimination_negatif);
    // CU_basic_run_tests();
    // CU_cleanup_registry();
    
    
    
    // // -------- test_tinymt32_gen_42 --------
    // if (CU_initialize_registry() != CUE_SUCCESS)
    //     errx(EXIT_FAILURE, "can't initialize test registry");
    // CU_pSuite gen_coefSuite = CU_add_suite("gen_coef", NULL, NULL);
    // if (CU_get_error() != CUE_SUCCESS)
    //     errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    // CU_add_test(gen_coefSuite, "correct_coeffs", test_tinymt32_gen_42);
    // CU_basic_run_tests();
    // CU_cleanup_registry();
    
    
    // // --------test_gf_256_full_add_vector--------
    // if (CU_initialize_registry() != CUE_SUCCESS)
    //     errx(EXIT_FAILURE, "can't initialize test registry");
    // CU_pSuite add_vectorSuite = CU_add_suite("add_vector", NULL, NULL);
    // if (CU_get_error() != CUE_SUCCESS)
    //     errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    // CU_add_test(add_vectorSuite, "correct_coeffs", test_gf_256_full_add_vector);
    // CU_basic_run_tests();
    // CU_cleanup_registry();

    
    

    

    // --------test_gf_256_mul_vector--------
    
    // if (CU_initialize_registry() != CUE_SUCCESS)
    //     errx(EXIT_FAILURE, "can't initialize test registry");
    // CU_pSuite mul_vectorSuite = CU_add_suite("mul_vector", NULL, NULL);
    // if (CU_get_error() != CUE_SUCCESS)
    //     errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    // CU_add_test(mul_vectorSuite, "correct_coeffs", test_gf_256_mul_vector);
    // CU_basic_run_tests();
    // CU_cleanup_registry();
    

    // // --------test_gf_256_inv_vector--------
    // if (CU_initialize_registry() != CUE_SUCCESS)
    //     errx(EXIT_FAILURE, "can't initialize test registry");
    // CU_pSuite inv_vectorSuite = CU_add_suite("inv_vector", NULL, NULL);
    // if (CU_get_error() != CUE_SUCCESS)
    //     errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    // CU_add_test(inv_vectorSuite, "correct_coeffs", test_gf_256_inv_vector);
    // CU_basic_run_tests();
    // CU_cleanup_registry();
    
     return 0;


    

}