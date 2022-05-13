#include "../headers/system.h"

block_t make_block(uint8_t* data, message_t msg, int size){
    block_t bloc ;    
    bloc.bloc = (uint8_t**) malloc((size + msg.redundancy) * sizeof(uint8_t*));
    if (bloc.bloc == NULL){
        bloc.first_elment_bloc = 0;
        bloc.size = 0;
        return bloc;
    }
    for(int i = 0 ; i < size + msg.redundancy ; i++)  {
        bloc.bloc[i] = (uint8_t*) malloc(msg.word_size * sizeof(uint8_t)) ;
        for(int j = 0 ; j < msg.word_size; j++) {
            bloc.bloc[i][j] = (uint8_t) data[i * msg.word_size + j] ;
        }
    }
    return bloc;
}
void print_block(const char *s, block_t new_block, message_t msg){ // pas important juste dans verbosse regarder après
    printf("%s {\n", s);
    for (int i = 0 ; i < msg.block_size + msg.redundancy; i++){
        for(int j = 0 ; j < msg.word_size ; j++){
            printf("%d\t", new_block.bloc[i][j]); 
        }
        printf("\n");
    }
    printf("}\n");
}
#define BIG_TO_LITTLE_ENDIAN_32BITS(n) (((n)>>24)&0xff) | (((n)<<8)&0xff0000) | (((n)>>8)&0xff00) | (((n)<<24)&0xff000000);
#define BIG_TO_LITTLE_ENDIAN_64BITS(n) (((n)>>24)&0xff) | (((n)<<8)&0xff0000) | (((n)>>8)&0xff00) | (((n)<<24)&0xff000000);

uint64_t to_le_64(uint64_t n){
    uint64_t t = n;
    uint8_t *an = (uint8_t *)&n, *at = (uint8_t *)&t;

    for (int i = 0; i < 8; ++i)
        at[i] = an[7 - i];

    return t;
}

message_t get_file_infos(uint8_t *data){
    message_t msg, *msg_tmp = (message_t *)data;

    msg.seed = BIG_TO_LITTLE_ENDIAN_32BITS(msg_tmp->seed);
    msg.block_size = BIG_TO_LITTLE_ENDIAN_32BITS(msg_tmp->block_size);
    msg.word_size = BIG_TO_LITTLE_ENDIAN_32BITS(msg_tmp->word_size);
    msg.redundancy = BIG_TO_LITTLE_ENDIAN_32BITS(msg_tmp->redundancy);
    msg.message_size = to_le_64(msg_tmp->message_size);
    msg.coeffs = gen_coefs(msg.seed,msg.redundancy,msg.block_size);
    return msg;
}