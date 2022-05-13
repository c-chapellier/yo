#include "../headers/system.h"

lost_words* find_lost_words( block_t new_block, message_t msg, int size){
    lost_words *new_lost_world = (lost_words*) malloc(sizeof(lost_words));
    if (new_lost_world == NULL){
        new_lost_world -> unknown_indexes = NULL;
        new_lost_world -> unknowns = 0;
        return new_lost_world;
    }
    new_lost_world->unknown_indexes = (bool *) malloc(sizeof(bool) *size);
    uint8_t unknowns = 0;

    for(int i = 0;  i < size ; i++){
        int count = 0;
        for(int j = 0; j < msg.word_size ; j++){
            count += new_block.bloc[i][j];
        }
        if(count == 0){
            new_lost_world->unknown_indexes[i] = true;
            unknowns ++;
        }
        else{
            new_lost_world->unknown_indexes[i] = false ;
        }
    }
    new_lost_world->unknowns = unknowns;
    if(args.verbose) {
        printf("unknown_indexes = ");
        for(int i = 0;  i <msg.block_size ; i++)
            printf("%d",new_lost_world->unknown_indexes[i]);
        printf("\n");
    }
    return new_lost_world;
}

linear_system make_linear_system(lost_words *new_lost_world,block_t new_bloc, message_t msg, uint8_t size){ 
    linear_system a_b;
    a_b.nb_unk = new_lost_world->unknowns;

    uint8_t **a = (uint8_t**) malloc(new_lost_world->unknowns * sizeof(uint8_t*));
    if (a == NULL){
        a_b.nb_unk = 0;
        a_b.A = NULL;
        a_b.B = NULL;
        return a_b;
    }
    for(int i = 0 ;i < new_lost_world->unknowns; i ++){
        a[i]= (uint8_t*) malloc(new_lost_world->unknowns * sizeof(uint8_t));
    }
    uint8_t **b = (uint8_t**) malloc(new_lost_world->unknowns * sizeof(uint8_t*));
    if (b == NULL){
        a_b.nb_unk = 0;
        a_b.A = NULL;
        a_b.B = NULL;
        return a_b;
    }
    for (int i = 0 ;i < new_lost_world->unknowns; i ++)
        b[i] = new_bloc.bloc[i+size] ;
    for (int i = 0 ; i < new_lost_world->unknowns; i ++){
        int temp = 0;
        for (int j = 0 ;j < size; j ++){
            if (new_lost_world->unknown_indexes[j]){
                a[i][temp] = msg.coeffs[i][j];
                temp ++;
            }
            else {
                uint8_t *mult = gf_256_mul_vector(new_bloc.bloc[j], msg.coeffs[i][j], msg.word_size);
                if (mult == NULL){
                    a_b.nb_unk = 0;
                    a_b.A = NULL;
                    a_b.B = NULL;
                    return a_b;
                }
                uint8_t *add =gf_256_full_add_vector(b[i],mult, msg.word_size);
                if (add == NULL){
                    a_b.nb_unk = 0;
                    a_b.A = NULL;
                    a_b.B = NULL;
                    return a_b;
                }
                memcpy(b[i],add,msg.word_size *sizeof(uint8_t));
                free(mult) ;
                free(add);
            }
        }
    }
    a_b.A = a;
    a_b.B = b;
    if(args.verbose){
        printf(" \n >> Système linéaire make_lineaire \n");
        printf("A: \n");
        for (int i = 0; i < new_lost_world->unknowns; i++){
            for (int j = 0; j < new_lost_world->unknowns; j++)
                printf("%d \t",a_b.A[i][j]);
            printf("\n");
        }
        printf("B: \n");
        for (int i = 0; i < new_lost_world->unknowns; i++){
            for (int j = 0; j < msg.word_size; j++)
                printf("%d \t",a_b.B[i][j]);
            printf("\n") ;
        }

        printf("Size : %d \n",a_b.nb_unk);
    }
    return a_b;
}

int matrix_solve(linear_system a_b, message_t msg){
    for (int k = 0; k < a_b.nb_unk ; k ++){
        for (int i = k + 1 ; i <  a_b.nb_unk ; i ++){
            uint8_t factor = gf256_mul_table[a_b.A[i][k]][gf256_inv_table[a_b.A[k][k]]];
            for (int j = 0 ; j < a_b.nb_unk ; j++) {
                a_b.A[i][j] = a_b.A[i][j] ^ gf256_mul_table[a_b.A[k][j]][factor];}
            uint8_t *mult = gf_256_mul_vector(a_b.B[k], factor,msg.word_size);
            if (mult == NULL) return -1;
            uint8_t *add = gf_256_full_add_vector(a_b.B[i],mult,msg.word_size);
            if (add == NULL) return -1;
            memcpy(a_b.B[i],add, sizeof(uint8_t)* msg.word_size);
            free(mult);
            free(add);
        }
    }
    uint8_t *factor_tab = (uint8_t*) malloc(sizeof(uint8_t) * msg.word_size);
    if (factor_tab == NULL) return -2;

    for (int i = a_b.nb_unk - 1 ; i > -1 ; i--){
        for (int k = 0 ; k < msg.word_size ; k ++){
            factor_tab[k] = 0 ;
        }
        for (int j = i + 1 ; j < a_b.nb_unk ; j ++ ){
            uint8_t *mult = gf_256_mul_vector(a_b.B[j], a_b.A[i][j], msg.word_size) ;
            uint8_t  *add = gf_256_full_add_vector(factor_tab,mult,msg.word_size);
            memcpy(factor_tab,add, sizeof(uint8_t) * msg.word_size);
            free(mult) ;
            free(add);
        }
        uint8_t *add = gf_256_full_add_vector(a_b.B[i], factor_tab,msg.word_size ) ;
        //uint8_t *inv = gf_256_inv_vector(add, a_b.A[i][i], msg.word_size) ;
        //memcpy(a_b.B[i],inv,msg.word_size *sizeof(uint8_t));
        a_b.B[i] = gf_256_inv_vector(add, a_b.A[i][i], msg.word_size) ;
        //free(inv);
        free(add) ;
    }
    free(factor_tab) ; 

    if(args.verbose){
        printf(" \n >> Système linéaire matrix_solve \n");
        printf("A : \n");
        for (int i = 0; i < a_b.nb_unk; i++){
            for (int j = 0; j < a_b.nb_unk; j++){
                printf("%d \t",a_b.A[i][j]);
            }
            printf("\n");
        }
        printf("B : \n");
        for (int i = 0; i < a_b.nb_unk; i++){
            for (int j = 0; j < msg.word_size; j++){
                printf("%d \t",a_b.B[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    return 0;
}

int process_block(block_t new_block, message_t msg, int size) {
    lost_words *new_lost_world = find_lost_words(new_block, msg, size);
    if (new_lost_world -> unknown_indexes == NULL){
        return -1;
    }
    linear_system a_b = make_linear_system(new_lost_world,new_block, msg, size);
    if (a_b.A == NULL && a_b.B == NULL) return -2;
    int p = matrix_solve(a_b, msg);
    if (p == -1) return -3;
    if (p== -2) return -4;
    uint8_t temp = 0;

    for (int i = 0; i < size; i ++){
        if(new_lost_world->unknown_indexes[i]){
            memcpy(new_block.bloc[i],a_b.B[temp],sizeof(uint8_t) *msg.word_size);
            temp ++;
        }
    }
    for (size_t i = 0; i < new_lost_world->unknowns ; i++){
        //if (new_lost_world->unknown_indexes[i] != true)
        //{
            free(a_b.B[i]) ;
       // }
        
        free(a_b.A[i]) ;
    }
    free(a_b.B);
    free(a_b.A);
    free(new_lost_world->unknown_indexes);
    free(new_lost_world);
    return 0;
}
