#include <CUnit/Basic.h>
#include "../src/get_info.c"
#include "../src/system.c"
void test_make_bloc(){
    block_t new_bloc ; 
    uint8_t **new_matrix = (uint8_t**) malloc(sizeof(uint8_t*) * 7) ; 
    for (size_t i = 0; i < 7; i++)
    {
        new_matrix[i] = (uint8_t*) malloc(sizeof(uint8_t) * 3) ;
    }
    new_matrix[0][0] = 73 ;
    new_matrix[0][1] = 32 ;
    new_matrix[0][2] = 108 ;
    new_matrix[1][0] = 111 ;
    new_matrix[1][1] = 118 ;
    new_matrix[1][2] = 101 ;
    new_matrix[2][0] = 32 ;
    new_matrix[2][1] = 67 ;
    new_matrix[2][2] = 32 ;
    new_matrix[3][0] = 151 ;
    new_matrix[3][1] = 140 ;
    new_matrix[3][2] = 120 ;
    new_matrix[4][0] = 15 ;
    new_matrix[4][1] = 96 ;
    new_matrix[4][2] = 173 ;
    new_matrix[5][0] = 70;
    new_matrix[5][1] = 82 ;
    new_matrix[5][2] = 232 ;  
    new_matrix[6][0] = 214 ;   
    new_matrix[6][1] = 245 ;   
    new_matrix[6][2] = 65 ;   
    new_bloc.bloc = new_matrix ;
    FILE *input_file = fopen("input_binary/small.bin", "r");
    fseek(input_file, 0, SEEK_END);
    long fsize = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    uint8_t *binary_data = malloc(fsize);
    fread(binary_data, fsize, 1, input_file);
    message_t msg = get_file_infos(binary_data);
    binary_data = &binary_data[24];
    int step = msg.word_size * (msg.block_size + msg.redundancy);
    block_t current_block = make_block((uint8_t *)&binary_data[0 * step], msg, msg.block_size);
    for (size_t i = 0; i < 7; i++)
    {
        CU_ASSERT_NSTRING_EQUAL(current_block.bloc[i],new_bloc.bloc[i],3) ;
    }
    for (size_t i = 0; i < 7; i++){
        for (size_t j = 0; j < 3; j++)
        {
            printf("%d \t", current_block.bloc[i][j]) ;
        }
        printf("\n") ;
    }
    
    
    
    for (size_t i = 0; i < 7; i++)
    {
        free(new_bloc.bloc[i]) ;
    }
    free(new_bloc.bloc) ;
}
void test_get_file_infos(){
    FILE *input_file = fopen("input_binary/small.bin", "r");
    fseek(input_file, 0, SEEK_END);
    long fsize = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    uint8_t *binary_data = malloc(fsize);
    fread(binary_data, fsize, 1, input_file);
    message_t msg ;
    msg.seed = 42 ; 
    msg.block_size = 3 ; 
    msg.word_size = 3 ; 
    msg.redundancy = 4 ; 
    msg.message_size = 23 ;  
    message_t msg_test = get_file_infos(binary_data);
    CU_ASSERT_EQUAL(msg.seed,msg_test.seed) ;
    CU_ASSERT_EQUAL(msg.block_size,msg_test.word_size) ;
    CU_ASSERT_EQUAL(msg.word_size,msg_test.word_size) ;
    CU_ASSERT_EQUAL(msg.redundancy,msg_test.redundancy) ;
    CU_ASSERT_EQUAL(msg.message_size,msg_test.message_size) ;
}
int main() {
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry");
    CU_pSuite test_input = CU_add_suite("test_input", NULL, NULL);
    if (CU_get_error() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    CU_add_test(test_input, "test_make_bloc",test_make_bloc);
    //CU_add_test(test_input, "test_get_file_infos",test_get_file_infos);
    CU_basic_run_tests();
    CU_cleanup_registry();  
    return 0 ; 
}