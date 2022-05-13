#ifndef MAKE_BLOCK_H
#define MAKE_BLOCK_H
/**
 * 
 * Construit le bloc sur base des données et de la taille d'un bloc
 * @param data: les données du bloc en format binaire. Si le fichier d'input est bien formé, celui-ci est découpé `size` symboles de taille `word_size` bytes, suivis de `redundancy` symboles de taille `word_size`
 * @param size: le nombre de symboles sources dans un bloc
 * @param msg: le message bianire donné
 * @return: le block construit, sous la forme d'une matrice (une ligne = un symbole)
 */
block_t make_block(uint8_t* data, message_t msg, int size) ;

/**
 * 
 * Fonction d'aide. Retourne un string stocké en binaire dans le bloc passé en argument
 * @param new_block: le bloc en question
 * @param msg: le message bianire donné
 */    
void print_block(const char *s, block_t new_block, message_t msg) ; // pas important juste dans verbosse regarder après
/**
 * Récupère les informations du bloc `data`, comme spécifiées dans l'énoncé
 * @param data: les 24 premiers bytes brutes du fichier
 * @return message_t -> contenant :
 * block_size : la taille d'un bloc - le nombre de symboles sources dans le bloc
 * word_size: la taille d'un symbole 'complet' dans un bloc
 * redundancy: le nombre de symboles de redondance dans le bloc
 * message_size: la taille (en bytes) du fichier initial que nous souhaitons récupérer.
 * coeff une matrice de coeff aleatoire 
 * Cette valeur ne prend en compte que les données du fichier, donc sans symbole de réparation
 * ni les informations repries ci-dessus
 **/
message_t get_file_infos(uint8_t *data) ;

uint64_t to_le_64(uint64_t n) ;

#endif /* MAKE_BLOCK_H */