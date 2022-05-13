#ifndef PROCESS_BLOCK_H
#define PROCESS_BLOCK_H

typedef struct {
    uint8_t **bloc;
    uint8_t first_elment_bloc;
    uint8_t size;

} block_t;

typedef struct {
    uint32_t seed;
    uint32_t block_size;
    uint32_t word_size;
    uint32_t redundancy;
    uint64_t message_size;
    uint8_t **coeffs;
} message_t;

typedef struct {
    uint8_t **A;
    uint8_t **B;
    uint8_t nb_unk;
} linear_system;

typedef struct {
    bool *unknown_indexes;
    uint8_t unknowns;

} lost_words;

/**
 *
 * @brief: base d'un bloc, la fonction trouve les symboles sources perdus et les répertories dans `unknown_indexes`.
 * Un symbole est considéré comme perdu dans le bloc si le symbole ne contient que des 0.
 *
 * @param new_block: Le bloc en question.
 * @param msg: Le message binaire donné.
 * @param size: La taille du bloc.
 * @return: Un tableau de taille 'size' faisant un mapping avec les symboles sources (l'entrée 'i' est 'True' si le symbole source 'i' est perdu).
 *          Et le nombre de symboles sources perdu.
 */
lost_words *find_lost_words( block_t new_block, message_t msg, int size);

/**
 * @brief: un système linéaire Ax=b à partir des blocs donnés en argument.
 * @param new_lost_world : un tableau de taille 'size' faisant un mapping avec les symboles sources (l'entrée 'i' est 'True' si le symbole source 'i' est perdu). Et le nombre de symboles sources perdu.
 * @param new_bloc: Le bloc en question.
 * @param msg: Le message binaire donné.
 * @param size: La taille du bloc.
 * @return: Le système linéaire en question.
 */
linear_system make_linear_system(lost_words *new_lost_world,block_t new_bloc, message_t msg, uint8_t size);

/**
 *@brief:la méthode de Gauss-Jordan pour résoudre le système linéaire Ax=B.
 * @param a_b: Le système linéaire.
 * @param msg: Le message bianire donné.
 * @return 0 if the function work, -1 if not
 */
int  matrix_solve(linear_system a_b, message_t msg);

/**
 *
 * @brief: base d'un bloc, trouve les inconnues (i.e., symboles sources perdus) et construit le système linéaire
 * correspondant. Cette version simple considère qu'il y a toujours autant d'inconnues que de symboles de redondance,
 * c'est-à-dire qu'on va toujours construire un système avec autant d'équations que de symboles de redondances.
 *
 * @param new_bloc: le bloc en question.
 * @param msg: le message bianire donné.
 * @param size: la taille du bloc.
 * @return: 0 si ça fonctionne, -1 si elle ne passe pas les valeurs de retours.
 */
int process_block(block_t new_block, message_t msg, int size);

#endif /* PROCESS_BLOCK_H */