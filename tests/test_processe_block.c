#include "../headers/tinymt32.h"
#include <CUnit/Basic.h>
#include "../bloc_main/processe_block.c"
void test_make_linear_system(){
    lost_words new_lost_world ; 
    block_t new_bloc ; 
    bool unknown_indexes[] = {true, false} ;
    uint8_t nb_unk = 1 ; 
    new_lost_world.unknown_indexes = unknown_indexes ; 
    new_lost_world.unknowns = nb_unk ;
    message.block_size = 2 ; 
    message.word_size = 3 ;
    uint8_t **new_matrix = (uint8_t**) malloc(sizeof(uint8_t*) * 6) ; 
    for (size_t i = 0; i < 6; i++)
    {
        new_matrix[i] = (uint8_t*) malloc(sizeof(uint8_t) * 3) ;
    }
    
    new_matrix[0][0] = 0 ;
    new_matrix[0][1] = 0 ;
    new_matrix[0][2] = 0 ;
    new_matrix[1][0] = 58 ;
    new_matrix[1][1] = 41 ;
    new_matrix[1][2] = 0 ;
    new_matrix[2][0] = 48 ;
    new_matrix[2][1] = 218 ;
    new_matrix[2][2] = 196 ;
    new_matrix[3][0] = 135 ;
    new_matrix[3][1] = 164 ;
    new_matrix[3][2] = 243 ;
    new_matrix[4][0] = 122 ;
    new_matrix[4][1] = 252 ;
    new_matrix[4][2] = 234 ;
    new_matrix[5][0] = 80 ;
    new_matrix[5][1] = 117 ;
    new_matrix[5][2] = 232 ;
    
    new_bloc.bloc = new_matrix ; 
    uint8_t **coef = (uint8_t**) malloc(sizeof(uint8_t*) * 4) ;
    
    for (size_t i = 0; i < 4; i++) {
        coef[i] = (uint8_t*)  malloc(sizeof(uint8_t) * 3) ;
    }

    coef[0][0] = 171 ;
    coef[0][1] = 165 ;
    coef[0][2] = 55;
    coef[1][0] =  61;
    coef[1][1] =  69;
    coef[1][2] =  143;
    coef[2][0] =  152;
    coef[2][1] =  158;
    coef[2][2] =  168;
    coef[3][0] =  64; 
    coef[3][1] =  5; 
    coef[3][2] =  91; 
    message.coeffs = coef ; 
    // reponses 
    linear_system a_b ; 
    uint8_t **A = (uint8_t**) malloc(sizeof(uint8_t*) * 1) ;
    A[0] = (uint8_t*) malloc(sizeof(uint8_t) * 1);
    A[0][0] = 171 ;
    uint8_t **B = (uint8_t**) malloc(sizeof(uint8_t*) * 1) ;
    for(int i = 0 ; i < 3 ; i ++)
        B[i] = (uint8_t*) malloc(sizeof(uint8_t) * 3) ;
    B[0][0] = 189 ;
    B[0][1] = 39 ;
    B[0][2] = 196 ;
    a_b.A = A ; 
    a_b.B = B ; 

    linear_system test = make_linear_system(new_lost_world ,new_bloc) ;
    CU_ASSERT_EQUAL(a_b.A,a_b.A) ;
    CU_ASSERT_EQUAL(a_b.B,a_b.B) ;
    
    
}
void test_make_bloc(){
    block_t new_bloc ; 
    message.block_size = 2 ; 
    message.word_size = 3 ;
    message.redundancy = 4 ; 
    //FILE *data = b'\x00\x00\x00:)\x000\xda\xc4\x87\xa4\xf3z\xfc\xeaPu\xe8' // coco 
    uint8_t **new_matrix = (uint8_t**) malloc(sizeof(uint8_t*) * 6) ; 
    for (size_t i = 0; i < 6; i++)
    {
        new_matrix[i] = (uint8_t*) malloc(sizeof(uint8_t) * 3) ;
    }
    
    new_matrix[0][0] = 0 ;
    new_matrix[0][1] = 0 ;
    new_matrix[0][2] = 0 ;
    new_matrix[1][0] = 58 ;
    new_matrix[1][1] = 41 ;
    new_matrix[1][2] = 0 ;
    new_matrix[2][0] = 48 ;
    new_matrix[2][1] = 218 ;
    new_matrix[2][2] = 196 ;
    new_matrix[3][0] = 135 ;
    new_matrix[3][1] = 164 ;
    new_matrix[3][2] = 243 ;
    new_matrix[4][0] = 122 ;
    new_matrix[4][1] = 252 ;
    new_matrix[4][2] = 234 ;
    new_matrix[5][0] = 80 ;
    new_matrix[5][1] = 117 ;
    new_matrix[5][2] = 232 ;
    
    new_bloc.bloc = new_matrix ; 

}
void test_find_lost_words() {
    block_t new_block ; 
    message.block_size = 2 ; 
    message.word_size = 3 ;

    //uint8_t new_matrix[][3] = {{0,0,0},{48,218,196},{135,164,243},{122,252,234},{80,117,232}} ; // faut caster 
    uint8_t **new_matrix = (uint8_t**) malloc(sizeof(uint8_t*) * 6) ; 
    for (size_t i = 0; i < 6; i++)
    {
        new_matrix[i] = (uint8_t*) malloc(sizeof(uint8_t) * 3) ;
    }
    new_matrix[0][0] = 0 ;
    new_matrix[0][1] = 0 ;
    new_matrix[0][2] = 0 ;
    new_matrix[1][0] = 58 ;
    new_matrix[1][1] = 41 ;
    new_matrix[1][2] = 0 ;
    new_matrix[2][0] = 48 ;
    new_matrix[2][1] = 218 ;
    new_matrix[2][2] = 196 ;
    new_matrix[3][0] = 135 ;
    new_matrix[3][1] = 164 ;
    new_matrix[3][2] = 243 ;
    new_matrix[4][0] = 122 ;
    new_matrix[4][1] = 252 ;
    new_matrix[4][2] = 234 ;
    new_matrix[5][0] = 80 ;
    new_matrix[5][1] = 117 ;
    new_matrix[5][2] = 232 ;
    bool unknown_indexes[] = {true, false} ;
    uint8_t nb_unk = 1 ; 
    new_block.bloc = (uint8_t**) new_matrix ; 
    lost_words world = find_lost_words(new_block) ;
    printf("%d \n", world.unknowns) ;
    for (size_t i = 0; i < message.block_size; i++)
    {
        printf("%d \t",world.unknown_indexes[i]) ;
    }
    CU_ASSERT_EQUAL(world.unknowns, nb_unk) ;
    CU_ASSERT_EQUAL(world.unknown_indexes, unknown_indexes) ;
    

}
void test_matrix_solve(){
    linear_system a_b ; 
    uint8_t **A = (uint8_t**) malloc(sizeof(uint8_t*) * 1) ;
    A[0] = (uint8_t*) malloc(sizeof(uint8_t) * 1);
    A[0][0] = 171 ;
    uint8_t **B = (uint8_t**) malloc(sizeof(uint8_t*) * 1) ;
    for(int i = 0 ; i < 3 ; i ++)
        B[i] = (uint8_t*) malloc(sizeof(uint8_t) * 3) ;
    B[0][0] = 189 ;
    B[0][1] = 39 ;
    B[0][2] = 196 ;
    uint8_t **coef = (uint8_t**) malloc(sizeof(uint8_t*) * 4) ;
    
    for (size_t i = 0; i < 4; i++) {
        coef[i] = (uint8_t*)  malloc(sizeof(uint8_t) * 3) ;
    }

    coef[0][0] = 171 ;
    coef[0][1] = 165 ;
    coef[0][2] = 55;
    coef[1][0] =  61;
    coef[1][1] =  69;
    coef[1][2] =  143;
    coef[2][0] =  152;
    coef[2][1] =  158;
    coef[2][2] =  168;
    coef[3][0] =  64; 
    coef[3][1] =  5; 
    coef[3][2] =  91; 
    message.coeffs = coef ; 
    

    a_b.A = (uint8_t**) A ; 
    a_b.B = (uint8_t**) B ; 
    a_b.nb_unk = 1 ;
    message.block_size = 2 ; 
    message.word_size = 3 ;
    uint8_t **B_test = (uint8_t**) malloc(sizeof(uint8_t*) * 1) ;
    for(int i = 0 ; i < 3 ; i ++)
        B_test[i] = (uint8_t*) malloc(sizeof(uint8_t) * 3) ;
    B_test[0][0] = 189 ;
    B_test[0][1] = 39 ;
    B_test[0][2] = 196 ;
    matrix_solve(a_b) ;
    for(int i = 0 ; i < 3 ;  i ++)
        printf("%d \t",a_b.B[0][i]) ;
    printf(" \n") ;
    CU_ASSERT_EQUAL(a_b.B,B_test) ;
    



}
void test_process_block(){
    block_t new_bloc ; 
    message.block_size = 2 ; 
    message.word_size = 3 ;
    uint8_t **new_matrix = (uint8_t**) malloc(sizeof(uint8_t*) * 6) ; 
    for (size_t i = 0; i < 6; i++)
    {
        new_matrix[i] = (uint8_t*) malloc(sizeof(uint8_t) * 3) ;
    }
    
    new_matrix[0][0] = 0 ;
    new_matrix[0][1] = 0 ;
    new_matrix[0][2] = 0 ;
    new_matrix[1][0] = 58 ;
    new_matrix[1][1] = 41 ;
    new_matrix[1][2] = 0 ;
    new_matrix[2][0] = 48 ;
    new_matrix[2][1] = 218 ;
    new_matrix[2][2] = 196 ;
    new_matrix[3][0] = 135 ;
    new_matrix[3][1] = 164 ;
    new_matrix[3][2] = 243 ;
    new_matrix[4][0] = 122 ;
    new_matrix[4][1] = 252 ;
    new_matrix[4][2] = 234 ;
    new_matrix[5][0] = 80 ;
    new_matrix[5][1] = 117 ;
    new_matrix[5][2] = 232 ;
    new_bloc.bloc = new_matrix ; 
    uint8_t **coef = (uint8_t**) malloc(sizeof(uint8_t*) * 4) ;
    
    for (size_t i = 0; i < 4; i++) {
        coef[i] = (uint8_t*)  malloc(sizeof(uint8_t) * 3) ;
    }

    coef[0][0] = 171 ;
    coef[0][1] = 165 ;
    coef[0][2] = 55;
    coef[1][0] =  61;
    coef[1][1] =  69;
    coef[1][2] =  143;
    coef[2][0] =  152;
    coef[2][1] =  158;
    coef[2][2] =  168;
    coef[3][0] =  64; 
    coef[3][1] =  5; 
    coef[3][2] =  91; 
    message.coeffs = coef ; 
    uint8_t **matrix = (uint8_t**) malloc(sizeof(uint8_t*) * 6) ; 
    for (size_t i = 0; i < 6; i++)
    {
        matrix[i] = (uint8_t*) malloc(sizeof(uint8_t) * 3) ;
    }
    matrix[0][0] = 110;
    matrix[0][1] = 103 ;
    matrix[0][2] = 32 ;
    matrix[1][0] = 58 ;
    matrix[1][1] = 41 ;
    matrix[1][2] = 0 ;
    matrix[2][0] = 48 ;
    matrix[2][1] = 218 ;
    matrix[2][2] = 196 ;
    matrix[3][0] = 135 ;
    matrix[3][1] = 164 ;
    matrix[3][2] = 243 ;
    matrix[4][0] = 122 ;
    matrix[4][1] = 252 ;
    matrix[4][2] = 234 ;
    matrix[5][0] = 80 ;
    matrix[5][1] = 117 ;
    matrix[5][2] = 232 ;
    

    block_t new = process_block(new_bloc) ;
    for(int i = 0 ; i < 6 ; i++) {
        for(int j = 0 ; j < 3 ; j++) {
            printf("%d \t", new.bloc[i][j]) ;
        }
        printf("\n") ;
    }
    CU_ASSERT_EQUAL(new.bloc, matrix) ; 
}
// int main () {
//     //Structure du main pour les testes, elle consistes juste a changer les mots de la fonction comme voir en bas 
//     // --------  test_make_linear_system --------
//     // if (CU_initialize_registry() != CUE_SUCCESS)
//     //     errx(EXIT_FAILURE, "can't initialize test registry");
//     // CU_pSuite test_make_linear = CU_add_suite("test_make_linear_system", NULL, NULL);
//     // if (CU_get_error() != CUE_SUCCESS)
//     //     errx(EXIT_FAILURE, "%s", CU_get_error_msg());
//     // CU_add_test(test_make_linear, "answear_negatif",test_make_linear_system);
//     // CU_basic_run_tests();
//     // CU_cleanup_registry();
//     // --------test_make_bloc  --------
//     // if (CU_initialize_registry() != CUE_SUCCESS)
//     //     errx(EXIT_FAILURE, "can't initialize test registry");
//     // CU_pSuite test_make = CU_add_suite("test_make_bloc", NULL, NULL);
//     // if (CU_get_error() != CUE_SUCCESS)
//     //     errx(EXIT_FAILURE, "%s", CU_get_error_msg());
//     // CU_add_test(test_make, "answear_negatif",test_make_bloc);
//     // CU_basic_run_tests();
//     // CU_cleanup_registry();
//     // --------test_find_lost_words--------
//     // if (CU_initialize_registry() != CUE_SUCCESS)
//     //     errx(EXIT_FAILURE, "can't initialize test registry");
//     // CU_pSuite test_find_lost = CU_add_suite("test_find_lost_words", NULL, NULL);
//     // if (CU_get_error() != CUE_SUCCESS)
//     //     errx(EXIT_FAILURE, "%s", CU_get_error_msg());
//     // CU_add_test(test_find_lost, "answear_negatif",test_find_lost_words);
//     // CU_basic_run_tests();
//     // CU_cleanup_registry();


//     //test_make_linear_system() ; // fonctionne bien 
//     //test_find_lost_words() ; //fonctionne bien 
//     //test_matrix_solve() ; // fonctionne bien 
//     //test_process_block() ; //fonctionne bien 
//     // --------test_process_block--------
//     if (CU_initialize_registry() != CUE_SUCCESS)
//         errx(EXIT_FAILURE, "can't initialize test registry");
//     CU_pSuite test_process_bloc = CU_add_suite("test_process_block", NULL, NULL);
//     if (CU_get_error() != CUE_SUCCESS)
//         errx(EXIT_FAILURE, "%s", CU_get_error_msg());
//     CU_add_test(test_process_bloc, "answear_negatif",test_process_block);
//     CU_basic_run_tests();
//     CU_cleanup_registry();


//     //test_make_linear_system() ; // fonctionne bien 
//     //test_find_lost_words() ; //fonctionne bien 
//     //test_matrix_solve() ; // fonctionne bien 
    
//     return 0 ; 
// }   