#include "../headers/system.h"

typedef struct
{
    DIR *input_dir;
    char input_dir_path[PATH_MAX];
    FILE *output_stream;
    uint8_t nb_threads;
    bool verbose;
} args_t;

args_t args;

void usage(char *prog_name)
{
    fprintf(stderr, "USAGE:\n");
    fprintf(stderr, "    %s [OPTIONS] input_dir\n", prog_name);
    fprintf(stderr, "    input_dir: path to the directory containing the instance files with the encoded messages\n");
    fprintf(stderr, "    -f output_file: path to the output file containing all decoded messages\n");
    fprintf(stderr, "    -n n_threads (default: 4): set the number of computing threads that will be used to execute the RLC algorithm\n");
    fprintf(stderr, "    -v : enable debugging messages. If not set, no such messages will be displayed (except error messages on failure)\n");
}

int parse_args(args_t *args, int argc, char *argv[])
{
    memset(args, 0, sizeof(args_t));

    // Default values of the arguments
    args->nb_threads = 4;
    args->verbose = false;
    args->output_stream = stdout;
    int opt;
    while ((opt = getopt(argc, argv, "n:vf:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            args->nb_threads = atoi(optarg);
            if (args->nb_threads == 0)
            {
                fprintf(stderr, "The number of computing threads must be a positive integer, got: %s\n", optarg);
                return -1;
            }
            break;
        case 'v':
            args->verbose = true;
            break;
        case 'f':
            args->output_stream = fopen(optarg, "w");
            if (args->output_stream == NULL)
            {
                fprintf(stderr, "Impossible to open the output file %s: %s\n", optarg, strerror(errno));
                return -1;
            }
            break;
        case '?':
            usage(argv[0]);
            return 1;
        default:
            usage(argv[0]);
        }
    }

    if (optind == argc)
    {
        fprintf(stderr, "You must provide an input directory containing the instance files!\n");
        return -1;
    }

    // Source: https://stackoverflow.com/questions/11736060/how-to-read-all-files-in-a-folder-using-c
    if (NULL == (args->input_dir = opendir(argv[optind])))
    {
        fprintf(stderr, "Impossible to open the directory containing the input instance files %s: %s\n", argv[optind], strerror(errno));
        return -1;
    }
    // The following line is not very secure... Ask Prof. Legay and/or wait for LINGI2144 for more information :-)
    strcpy(args->input_dir_path, argv[optind++]);

    return 0;
}
    // """
    // Construit le bloc sur base des données et de la taille d'un bloc

    // :param data: les données du bloc en format binaire. Si le fichier d'input est bien formé, celui-ci est découpé
    //              `size` symboles de taille `word_size` bytes, suivis de `redundancy` symboles de taille `word_size`
    // :param size: le nombre de symboles sources dans un bloc
    // :return block: le block construit, sous la forme d'une matrice (une ligne = un symbole)
    // """
    // trouvé le type de data 
block_t make_block(uint8_t* data, message_t msg, int size){

    block_t bloc ; 

    // printf("ok = %d\n", size);
    
    bloc.bloc = (uint8_t**) malloc((size + msg.redundancy) * sizeof(uint8_t*)) ; 
    for(int i = 0 ; i < size + msg.redundancy ; i++)  {
        bloc.bloc[i] = (uint8_t*) malloc(msg.word_size * sizeof(uint8_t)) ;
        for(int j = 0 ; j < msg.word_size; j++) {
            bloc.bloc[i][j] = (uint8_t) data[i * msg.word_size + j] ;
        }
    }
    return bloc;
}

    // """
    // Fonction d'aide. Retourne un string stocké en binaire dans le bloc passé en argument

    // :param block: le bloc en question
    // :param size: la taille du bloc
    // :return s: le string du bloc converti en binaire
    // """
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
    // """
    // Ecrit dans le fichier `output_file` le bloc en binaire

    // :param output_file: le descripteur de fichier de sortie
    // :param block: le bloc en question
    // :param size: la taille du bloc
    // :param word_size: la taille de chaque symbole du bloc
    // """

// void write_block(FILE *output_file,block_t block){

//     for (int i = 0 ; i< block.size  ; i++) {
//         for (int j  ; j < message.word_size ; j++){
//             if (output_file == stdout or output_file == stderr){       // pas sur pour le if
//                 char x  =(char) block[i][j] ; 
//                 printf("%c \n",x) ; 
//             }
//             fwrite() ; 
//             output_file.write(int(block[i][j]).to_bytes(1, 'big'));
//         }

//     }
// }

#define BIG_TO_LITTLE_ENDIAN_32BITS(n) (((n)>>24)&0xff) | (((n)<<8)&0xff0000) | (((n)>>8)&0xff00) | (((n)<<24)&0xff000000);
#define BIG_TO_LITTLE_ENDIAN_64BITS(n) (((n)>>24)&0xff) | (((n)<<8)&0xff0000) | (((n)>>8)&0xff00) | (((n)<<24)&0xff000000);
    // """
    // Récupère les informations du bloc `data`, comme spécifiées dans l'énoncé

    // :param data: les 24 premieers bytes brutes du fichier
    // :return block_size: la taille d'un bloc - le nombre de symboles sources dans le bloc
    // :return word_size: la taille d'un symbole 'complet' dans un bloc
    // :return redundancy: le nombre de symboles de redondance dans le bloc
    // :message_size: la taille (en bytes) du fichier initial que nous souhaitons récupérer.
    //             Cette valeur ne prend en compte que les données du fichier, donc sans symbole de réparation
    //             ni les informations repries ci-dessus
    // """

uint64_t to_le_64(uint64_t n)
{
    uint64_t t = n;
    uint8_t *an = (uint8_t *)&n, *at = (uint8_t *)&t;

    for (int i = 0; i < 8; ++i)
        at[i] = an[7 - i];

    return t;
}

message_t get_file_infos(uint8_t *data)
{
    message_t msg, *msg_tmp = (message_t *)data;

    msg.seed = BIG_TO_LITTLE_ENDIAN_32BITS(msg_tmp->seed);
    msg.block_size = BIG_TO_LITTLE_ENDIAN_32BITS(msg_tmp->block_size);
    msg.word_size = BIG_TO_LITTLE_ENDIAN_32BITS(msg_tmp->word_size);
    msg.redundancy = BIG_TO_LITTLE_ENDIAN_32BITS(msg_tmp->redundancy);
    msg.message_size = to_le_64(msg_tmp->message_size);
    msg.coeffs = gen_coefs(msg.seed,msg.redundancy,msg.block_size);
    return msg;
}

void write_block(int output_file, block_t block, int size, int word_size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < word_size; j++)
        {
            // if output_file == sys.stdout or output_file == sys.stderr:
            printf("%c", block.bloc[i][j]);
            // else:
            //     output_file.write(int(block[i][j]).to_bytes(1, 'big'));
        }
    }
    // printf("\n");
}

void write_last_block(int output_file, block_t block, uint8_t size, uint8_t word_size,uint8_t last_word_size){
    for (int i = 0; i < size - 1 ; i++)
    {
        for (int j = 0; j < word_size; j++)
        {
            // if output_file == sys.stdout or output_file == sys.stderr:
            printf("%c", block.bloc[i][j]);
            // else:
            //     output_file.write(int(block[i][j]).to_bytes(1, 'big'));
        }
    }
    for (int j = 0; j < last_word_size; j++){
        printf("%c", block.bloc[size - 1][j]);
    }
    // printf("\n") ;
}

int main(int argc, char *argv[])
{
    args_t args;
    int err = parse_args(&args, argc, argv);
    if (err == -1)
    {
        exit(EXIT_FAILURE);
    }
    else if (err == 1)
    {
        exit(EXIT_SUCCESS);
    }

    // The following lines (and every code already present in this skeleton) can be removed, it is just an example to show you how to use the program arguments
    // fprintf(stderr, "\tnumber of threads executing the RLC decoding algorithm in parallel: %" PRIu32 "\n", args.nb_threads);
    // fprintf(stderr, "\tverbose mode: %s\n", args.verbose ? "enabled" : "disabled");

    // This is an example of how to open the instance files of the input directory. You may move/edit it during the project
    struct dirent *directory_entry;
    FILE *input_file;
    while ((directory_entry = readdir(args.input_dir)))
    {
        // Ignore parent and current directory
        if (!strcmp(directory_entry->d_name, "."))
        {
            continue;
        }
        if (!strcmp(directory_entry->d_name, ".."))
        {
            continue;
        }

        // Add the directory path to the filename to open it
        char full_path[PATH_MAX];
        memset(full_path, 0, sizeof(char) * PATH_MAX);
        strcpy(full_path, args.input_dir_path);
        strcat(full_path, "/");
        strcat(full_path, directory_entry->d_name);

        input_file = fopen(full_path, "r");
        if (input_file == NULL)
        {
            fprintf(stderr, "Failed to open the input file %s: %s\n", full_path, strerror(errno));
            goto file_read_error;
        }
        if (args.verbose)
        {
            // This is a simple example of how to use the verbose mode
            fprintf(stderr, "Successfully opened the file %s\n", full_path);
        }
        
        // lire fichier tranformer en blocs, decode le message 
        
        // TODO: parse the input binary file, decode the encoded message with RLC and write the output in the output stream following the statement

        // read file into binary_data
        fseek(input_file, 0, SEEK_END);
        long fsize = ftell(input_file);
        fseek(input_file, 0, SEEK_SET);

        uint8_t *binary_data = malloc(fsize);
        fread(binary_data, fsize, 1, input_file);

        // get variables
        message_t msg = get_file_infos(binary_data);

        if (args.verbose)
        {
            printf("seed = %d\n", msg.seed);
            printf("block_size = %d\n", msg.block_size);
            printf("word_size = %d\n", msg.word_size);
            printf("redundancy = %d\n", msg.redundancy);
            printf("message_size = %llu\n", msg.message_size);
            // printf("coefs = %d\n", msg.coefs[][]);
            // print(binary_data);
        }

        binary_data = &binary_data[24];

        int step = msg.word_size * (msg.block_size + msg.redundancy);  //# Taille de bloc en bytes 

        // # Ecrit la taille du titre et du contenu du message
        // # Ecrit le nom du fichier dans l'output
        // if output_fd == sys.stdout or output_fd == sys.stderr:
        // printf("%lu ", strlen(directory_entry->d_name));
        // printf("%d ", msg.message_size);
        // printf("%s\n", directory_entry->d_name);
        // else:
        //     output_fd.write(len(filename).to_bytes(4, "big"))
        //     output_fd.write(message_size.to_bytes(8, "big"))
        //     output_fd.write(filename.encode("ASCII"))

        //# Contient le nombre de blocs complets (sans le dernier bloc s'il n'est pas complet)
        // printf("fsize - 24 = %d\n", fsize - 24);
        int nb_blocks = ceil((float)(fsize - 24) / (msg.word_size * (msg.block_size + msg.redundancy)));
        // printf("nb_blocks = %d\n", nb_blocks);

        int contains_uncomplete_block = 0;
        if (msg.message_size != nb_blocks * msg.block_size * msg.word_size)  //# Dernier bloc non complet (i.e., moins de block_size symboles)
        {
            nb_blocks -= 1;
            contains_uncomplete_block = 1;
        }

        int readed = 0; // # Nombre de bytes lus jusqu'à présent
        for (int i = 0; i < nb_blocks; ++i)
        {
            block_t current_block = make_block((uint8_t *)&binary_data[i * step], msg, msg.block_size);
            if (args.verbose)
            {
                print_block("block 1", current_block, msg); 
            }
            block_t response = process_block(current_block, msg, msg.block_size);
            if (args.verbose)
            {
                print_block("block 2", response, msg); 
            }
            
            write_block(0, response, msg.block_size, msg.word_size);

            readed += step;
        }

        // printf("\n-----------1-------------\n");

        int readed_symbols = msg.block_size * msg.word_size * nb_blocks;
        int nb_remaining_symbols = ((fsize - 24 - readed) / msg.word_size) - msg.redundancy;

        // printf("readed_symbols: %d\n", readed_symbols);
        // printf("nb_remaining_symbols: %d\n", nb_remaining_symbols);

        if (contains_uncomplete_block)
        {
            block_t last_block = make_block(&binary_data[readed], msg, nb_remaining_symbols);
            block_t decoded = process_block(last_block, msg, nb_remaining_symbols);
            // # Le dernier symbole peut ne pas etre complet et contenir du padding
            // # => on utilise la taille totale du fichier pour retirer le padding
            int padding = readed_symbols + nb_remaining_symbols * msg.word_size - msg.message_size;
            int true_length_last_symbol = msg.word_size - padding;
            // if (args.verbose)
            //     print(last_block);
                
            write_last_block(1, decoded, nb_remaining_symbols, msg.word_size, true_length_last_symbol);
        }

        // Close this instance file
        fclose(input_file);
    }

    // Close the input directory and the output file
    err = closedir(args.input_dir);
    if (err < 0)
    {
        fprintf(stderr, "Error while closing the input directory containing the instance files\n");
    }
    if (args.output_stream != stdout)
    {
        fclose(args.output_stream);
    }
    return 0;

file_read_error:
    err = closedir(args.input_dir);
    if (err < 0)
    {
        fprintf(stderr, "Error while closing the input directory containing the instance files\n");
    }
    if (args.output_stream != stdout)
    {
        fclose(args.output_stream);
    }
    exit(EXIT_FAILURE);
}