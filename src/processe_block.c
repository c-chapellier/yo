#include "../headers/system.h"

    
/**
 *
 * sur base d'un bloc, trouve les symboles sources perdus et les répertorie dans `unknown_indexes`.
 * Un symbole est considéré comme perdu dans le bloc si le symbole ne contient que des 0
 * @param new_block: le bloc en question.
 * @param msg: le message bianire donné
 * @param size: la taille du bloc
 * @return: un tableau de taille 'size' faisqnt un mapping avec les symboles sources.(l'éntrée 'i' est 'True' si le symbole source 'i' est perdu)
 *          et le nombre de symboles sources perdu.
 */

lost_words find_lost_words( block_t new_block, message_t msg, int size){
    lost_words new_lost_world ; 
    bool *unknown_indexes = (bool*) malloc(sizeof(bool) * size) ;
    uint8_t unknowns = 0 ; 

    for(int i = 0;  i < size ; i++){
        int count = 0 ;
        for(int j = 0; j < msg.word_size ; j++){
            count += new_block.bloc[i][j] ; 
        }
        if(count == 0){
            unknown_indexes[i] = true ; 
            unknowns ++ ; 
        }
        else{
            unknown_indexes[i] = false ; 
        }
    }
    if(args.verbose) {
        printf("unknown_indexes = ");
        for(int i = 0;  i <msg.block_size ; i++)
            printf("%d",unknown_indexes[i]) ;
        printf("\n");
    }
    
    new_lost_world.unknown_indexes =unknown_indexes ;
    new_lost_world.unknowns = unknowns ;
    return new_lost_world; 
    
}
    
/**
 *
 * Construit un système linéaire Ax=b à partir des blocs donnés en argument.
 * @param new_lost_world : un tableau de taille 'size' faisqnt un mapping avec les symboles sources.(l'éntrée 'i' est 'True' si le symbole source 'i' est perdu) Et le nombre de symboles sources perdu.
 * @param new_bloc: le bloc en question.
 * @param msg: le message bianire donné
 * @param size: la taille du bloc
 * @return: le système linéaire en question
 */

linear_system make_linear_system(lost_words new_lost_world,block_t new_bloc, message_t msg, uint8_t size){ // bool *unknown_indexes, uint8_t nb_unk

    linear_system a_b ;
    a_b.nb_unk = new_lost_world.unknowns ;

    uint8_t **a = (uint8_t**) malloc(new_lost_world.unknowns * sizeof(uint8_t*)) ;
    for(int i = 0 ;i < new_lost_world.unknowns ; i ++) {
         a[i]= (uint8_t*) malloc(new_lost_world.unknowns * sizeof(uint8_t)) ;
    }
    uint8_t **b = (uint8_t**) malloc(new_lost_world.unknowns * sizeof(uint8_t*)) ;
    for(int i = 0 ;i < new_lost_world.unknowns ; i ++) 
        b[i] = new_bloc.bloc[i+size] ;   
    for(int i = 0 ;i < new_lost_world.unknowns ; i ++) {
        int temp = 0 ;
        for(int j = 0 ;j < size; j ++) {

            if(new_lost_world.unknown_indexes[j] ) {
                a[i][temp] = msg.coeffs[i][j] ; 
                temp ++ ;
            }
            else{
                b[i] = gf_256_full_add_vector(b[i], gf_256_mul_vector(new_bloc.bloc[j], msg.coeffs[i][j], msg.word_size), msg.word_size) ;
                }
        } 
    }
    a_b.A = a ;
    a_b.B = b ;   
    if(args.verbose) {
        printf(" \n >> Système linéaire make_lineaire \n") ; 
        printf("A: \n") ; 
        for (int i = 0; i < new_lost_world.unknowns; i++)
        {
            for (int j = 0; j < new_lost_world.unknowns; j++){
                printf("%d \t",a_b.A[i][j]) ;
            }
                printf("\n") ;
        }
        printf("B: \n") ;
        for (int i = 0; i < new_lost_world.unknowns; i++)
        {
            for (int j = 0; j < msg.word_size; j++){
                printf("%d \t",a_b.B[i][j]) ;
            }
            printf("\n") ;
        }
    
        printf("Size : %d \n",a_b.nb_unk) ; 
        }
    return a_b ; 
}


/**
 *
 * Utilise la méthode de Gauss-Jordan pour résoudre le système linéaire Ax=B
 * @param a_b: le système linéaire
 * @param msg: le message bianire donné
 */

void matrix_solve(linear_system a_b, message_t msg) {
    for(int k = 0; k < a_b.nb_unk ; k ++) {
        for(int i = k + 1 ; i <  a_b.nb_unk ; i ++) {
            uint8_t factor = gf256_mul_table[a_b.A[i][k]][gf256_inv_table[a_b.A[k][k]]] ;
            for( int j = 0 ; j < a_b.nb_unk ; j++) {
                a_b.A[i][j] = a_b.A[i][j] ^ gf256_mul_table[a_b.A[k][j]][factor] ;
                } 
            a_b.B[i] = gf_256_full_add_vector(a_b.B[i], gf_256_mul_vector(a_b.B[k], factor,msg.word_size),msg.word_size) ;
        }
    }
    uint8_t *factor_tab = (uint8_t*) malloc(sizeof(uint8_t) * msg.word_size) ; 
    for(int i = a_b.nb_unk - 1 ; i > -1 ; i --) {
        for(int k = 0 ; k < msg.word_size ; k ++){
            factor_tab[k] = 0 ;
            
        }
        for(int j = i + 1 ; j < a_b.nb_unk ; j ++ ){
            factor_tab = gf_256_full_add_vector(factor_tab, gf_256_mul_vector(a_b.B[j], a_b.A[i][j], msg.word_size),msg.word_size) ;
        }
        a_b.B[i] = gf_256_inv_vector(gf_256_full_add_vector(a_b.B[i], factor_tab,msg.word_size ), a_b.A[i][i], msg.word_size) ;
    }
    if(args.verbose) {
        printf(" \n >> Système linéaire matrix_solve \n") ; 
        printf("A : \n") ; 
        for (int i = 0; i < a_b.nb_unk; i++){
            for (int j = 0; j < a_b.nb_unk; j++){
                printf("%d \t",a_b.A[i][j]) ;
            }
            printf("\n") ;
        }
        printf("B : \n") ;
        for (int i = 0; i < a_b.nb_unk; i++){
            for (int j = 0; j < msg.word_size; j++){
                printf("%d \t",a_b.B[i][j]) ;
            }
                printf("\n") ;
        }
        printf("\n") ;
    }
}

/**
 *
 * Sur base d'un bloc, trouve les inconnues (i.e., symboles sources perdus) et construit le système linéaire
 * correspondant. Cette version simple considère qu'il y a toujours autant d'inconnues que de symboles de redondance
 * c'est-à-dire qu'on va toujours construire un système avec autant d'équations que de symboles de redondances.
 *
 * @param new_bloc: le bloc en question.
 * @param msg: le message bianire donné
 * @param size: la taille du bloc
 * @return: retourne le bloc résolu
 */

block_t process_block(block_t new_block, message_t msg, int size) {

    lost_words new_lost_world = find_lost_words(new_block, msg, size) ;

    // for (int i = 0; i < size; ++i)
    //     new_lost_world.unknown_indexes[i] ? printf("True ") : printf("False ");
    // printf("\n");

    linear_system a_b = make_linear_system(new_lost_world,new_block, msg, size) ;
    matrix_solve(a_b, msg); 
    uint8_t temp = 0 ; 

    for(int i = 0; i < size; i ++) {
        if(new_lost_world.unknown_indexes[i]) {
            new_block.bloc[i] = a_b.B[temp] ; 
            temp ++ ;
        }
    }

    return new_block ; 
}
