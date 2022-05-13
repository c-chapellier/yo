#include <CUnit/Basic.h>
#include "../src/process_block.c"
#include "../src/system.c"
#include "../src/get_info.c"
args_t args ;
void test_make_linear_system(){
    lost_words *new_lost_world  = (lost_words*) malloc(sizeof(lost_words));
    block_t new_bloc ; 
    message_t message ;
    bool unknown_indexe[] = {true, false} ;
    uint8_t nb_unk = 1 ;
    new_lost_world -> unknown_indexes = unknown_indexe ; 
    new_lost_world -> unknowns = nb_unk ; 
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
    A[0] = (uint8_t*) malloc(sizeof(uint8_t) * 2);
    A[0][0] = 171 ;
    uint8_t **B = (uint8_t**) malloc(sizeof(uint8_t*) * 1) ;
    B[0] = (uint8_t*) malloc(sizeof(uint8_t) * 3) ;
    B[0][0] = 189 ;
    B[0][1] = 39 ;
    B[0][2] = 196 ;
    a_b.A = A ; 
    a_b.B = B ; 
    linear_system test = make_linear_system(new_lost_world ,new_bloc,message,message.block_size) ;
    for (size_t i = 0; i < 3; i++)
    {
        if(i < 2) {
            CU_ASSERT_EQUAL(test.B[0][i],a_b.B[0][i]) ;
            CU_ASSERT_EQUAL(test.A[0][i],a_b.A[0][i]) ;
            }
        else{
            CU_ASSERT_EQUAL(test.B[0][i],a_b.B[0][i]) ;
        }       
    }
    free(test.A[0])  ;
    free(a_b.A[0])  ;
    free(a_b.A)  ;
    free(test.A)  ;  
    free(test.B[0]) ;
    free(a_b.B[0])  ;
    free(a_b.B)  ;
    free(test.B) ;
    printf("ok1 \n") ;
    for(int i = 0 ; i < 4 ; i ++) 
        free(message.coeffs[i]) ;
    free(message.coeffs) ;
    free(new_lost_world) ;

}
void test_find_lost_words() {
    block_t new_block ; 
    message_t message ;
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
    lost_words *world = find_lost_words(new_block,message,message.block_size) ;
    CU_ASSERT_EQUAL(world -> unknowns, nb_unk) ;
    CU_ASSERT_NSTRING_EQUAL(world ->unknown_indexes, unknown_indexes,2) ;
    for(int i = 0 ; i < 5 ; i ++) 
        free(new_matrix[i]) ; 
    free(new_matrix) ;
}
void test_matrix_solve(){
    linear_system a_b ; 
    message_t message ;
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
    B_test[0][0] = 110 ;
    B_test[0][1] = 103 ;
    B_test[0][2] = 32 ;
    matrix_solve(a_b, message) ;
    for (size_t i = 0; i < 3; i++)
    {
        CU_ASSERT_EQUAL(a_b.B[0][i],B_test[0][i]) ;
    }    
    for(int i = 0 ; i < 4 ; i ++) 
        free(message.coeffs[i]) ;
    free(message.coeffs) ;
    free(a_b.A[0])  ;
    free(a_b.A)  ;
    free(B_test[0]) ;
    free(a_b.B[0])  ;
    free(a_b.B)  ;
    free(B_test) ;
}
void test_process_block(){
    block_t new_bloc ; 
    message_t message ;
    message.block_size = 3 ; 
    message.word_size = 3 ;
    message.redundancy = 4 ;
    uint8_t **new_matrix = (uint8_t**) malloc(sizeof(uint8_t*) * 7) ; 
    for (size_t i = 0; i < 7; i++)
    {
        new_matrix[i] = (uint8_t*) malloc(sizeof(uint8_t) * 3) ;
    }
    
    new_matrix[0][0] = 0 ;
    new_matrix[0][1] = 0 ;
    new_matrix[0][2] = 0 ;
    new_matrix[1][0] = 111;
    new_matrix[1][1] = 118;
    new_matrix[1][2] = 101;
    new_matrix[2][0] = 0 ;
    new_matrix[2][1] = 0 ;
    new_matrix[2][2] = 0 ;
    new_matrix[3][0] = 151 ;
    new_matrix[3][1] = 140 ;
    new_matrix[3][2] = 120 ;
    new_matrix[4][0] = 15 ;
    new_matrix[4][1] = 96 ;
    new_matrix[4][2] = 173 ;
    new_matrix[5][0] = 70;
    new_matrix[5][1] = 82 ;
    new_matrix[5][2] = 203 ;
    new_matrix[6][0] = 214;
    new_matrix[6][1] = 245 ;
    new_matrix[6][2] = 65;
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
    uint8_t **matrix = (uint8_t**) malloc(sizeof(uint8_t*) * 7) ; 
    for (size_t i = 0; i < 7; i++)
    {
        matrix[i] = (uint8_t*) malloc(sizeof(uint8_t) * 3) ;
    }
    matrix[0][0] = 73;
    matrix[0][1] = 32 ;
    matrix[0][2] = 108;
    matrix[1][0] = 111;
    matrix[1][1] = 118;
    matrix[1][2] = 101;
    matrix[2][0] = 32;
    matrix[2][1] = 67 ;
    matrix[2][2] = 32 ;
    matrix[3][0] = 161 ;
    matrix[3][1] = 220 ;
    matrix[3][2] = 88 ;
    matrix[4][0] = 28 ;
    matrix[4][1] = 148 ;
    matrix[4][2] = 28 ;
    matrix[5][0] = 70 ;
    matrix[5][1] = 82 ;
    matrix[5][2] = 203 ;
    matrix[6][0] = 214 ;
    matrix[6][1] = 245 ;
    matrix[6][2] = 65 ;
    print_block("start", new_bloc, message) ;
    process_block(new_bloc , message, message.block_size) ;
    print_block("end", new_bloc, message) ;

    for(int i = 0 ; i < 7 ; i ++){
        CU_ASSERT_NSTRING_EQUAL(matrix[i],new_bloc.bloc[i],3) ;
    }
    for(int i = 0 ; i < 7 ; i ++){
        free(new_bloc.bloc[i]) ;
        free(matrix[i]) ;
    }
    free(matrix) ;
    free(new_bloc.bloc) ; 
    for(int i = 0 ; i < 4 ; i ++) 
        free(message.coeffs[i]) ;
    free(message.coeffs) ;
}
int main () {
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");
    CU_pSuite test_process_bloc = CU_add_suite("test_process_block", NULL, NULL);
    if (CU_get_error() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    CU_add_test(test_process_bloc, "test_make_linear_system",test_make_linear_system);
    CU_add_test(test_process_bloc, "test_find_lost_words",test_find_lost_words);
    CU_add_test(test_process_bloc, "test_matrix_solve",test_matrix_solve);
    CU_add_test(test_process_bloc, "test_process_block",test_process_block);
    CU_basic_run_tests();
    CU_cleanup_registry();    
    return 0 ; 
}   