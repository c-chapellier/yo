#include "../headers/system.h"
#include "../headers/portable_semaphore.h"

args_t args;

sem_t *sem;
int nb_blocks, block_number, next_block_to_write;
int readed, step;
uint8_t *binary_data;
message_t msg;

void usage(char *prog_name){
    fprintf(stderr, "USAGE:\n");
    fprintf(stderr, "    %s [OPTIONS] input_dir\n", prog_name);
    fprintf(stderr, "    input_dir: path to the directory containing the instance files with the encoded messages\n");
    fprintf(stderr, "    -f output_file: path to the output file containing all decoded messages\n");
    fprintf(stderr, "    -n n_threads (default: 4): set the number of computing threads that will be used to execute the RLC algorithm\n");
    fprintf(stderr, "    -v : enable debugging messages. If not set, no such messages will be displayed (except error messages on failure)\n");
}

int parse_args(args_t *args, int argc, char *argv[]){
    memset(args, 0, sizeof(args_t));

    // Default values of the arguments
    args->nb_threads = 4;
    args->verbose = false;
    args->output_stream = stdout;
    int opt;
    while ((opt = getopt(argc, argv, "n:vf:")) != -1){
        switch (opt){
        case 'n':
            args->nb_threads = atoi(optarg);
            if (args->nb_threads == 0){
                fprintf(stderr, "The number of computing threads must be a positive integer, got: %s\n", optarg);
                return -1;
            }
            break;
        case 'v':
            args->verbose = true;
            break;
        case 'f':
            args->output_stream = fopen(optarg, "w");
            if (args->output_stream == NULL){
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

    if (optind == argc){
        fprintf(stderr, "You must provide an input directory containing the instance files!\n");
        return -1;
    }

    // Source: https://stackoverflow.com/questions/11736060/how-to-read-all-files-in-a-folder-using-c
    if (NULL == (args->input_dir = opendir(argv[optind]))){
        fprintf(stderr, "Impossible to open the directory containing the input instance files %s: %s\n", argv[optind], strerror(errno));
        return -1;
    }
    // The following line is not very secure... Ask Prof. Legay and/or wait for LINGI2144 for more information :-)
    strcpy(args->input_dir_path, argv[optind++]);

    return 0;
}


void write_block(int output_file, block_t block, uint32_t size, uint32_t word_size){
    for (int i = 0; i < size; i++){
        for (int j = 0; j < word_size; j++){
            fprintf(args.output_stream,"%c", block.bloc[i][j]);
        }
    }
    
}

void *thread_control(void *unused)
{
    sem_wait(sem);
    ++block_number;
    int i = block_number;
    sem_post(sem);

    while (i < nb_blocks)
    {
        block_t current_block = make_block((uint8_t *)&binary_data[24 +(i * step)], msg, msg.block_size);
        if (current_block.bloc == NULL){
            printf("There is an error at the construction of the current_block\n");
            exit(-1);
        }

        int prog = process_block(current_block, msg, msg.block_size);
        if (prog == -1){
            printf("there is an error at the creation of the new_losts_word\n");
            exit(-1);
        }
        if (prog == -2){
            printf("There is an error at the creation of the linear system\n");
            exit(-1);
        }
        if (prog == -3){
            printf("There is a problem when we try to operate on vectors\n");
            exit(-1);
        }
        if (prog == -4){
            printf("There is a problem when we try to solve the system\n");
            exit(-1);
        }

        if (args.verbose){
        }

        while (i != next_block_to_write)
        {
            //wait
        }

        sem_wait(sem);
        write_block(0, current_block, msg.block_size, msg.word_size);
        ++next_block_to_write;
        sem_post(sem);

        // On free le bloc 'current_block' 
        for (int i = 0 ; i<msg.redundancy+msg.block_size; i++){
            free(current_block.bloc[i]);
        }
        free(current_block.bloc);

        readed += step;

        sem_wait(sem);
        ++block_number;
        i = block_number;
        sem_post(sem);
    }
    return NULL;
}

/**
*Ecrit dans le fichier `output_file` le bloc en binaire
* @param output_file: le descripteur de fichier de sortie
* @param block: le bloc en question
* @param size: la taille du bloc
* @param word_size: la taille de chaque symbole du bloc
*/
void write_last_block(int output_file, block_t block, uint32_t size, uint32_t word_size, uint32_t last_word_size){
    for (int i = 0; i < size - 1 ; i++){
        for (int j = 0; j < word_size; j++){
            fprintf(args.output_stream,"%c", block.bloc[i][j]);}}

    for (int j = 0; j < last_word_size; j++){
        fprintf(args.output_stream,"%c", block.bloc[size - 1][j]);
    }

    fprintf(args.output_stream, "\n");
}
int main(int argc, char *argv[]){

    float temps;
    clock_t t1, t2;
    t1 = clock();

    int err = parse_args(&args, argc, argv);
    if (err == -1){
        exit(EXIT_FAILURE);
    }
    else if (err == 1){
        exit(EXIT_SUCCESS);
    }

    pthread_t threads[args.nb_threads];
    sem = sem_init(1);

    // The following lines (and every code already present in this skeleton) can be removed, it is just an example to show you how to use the program arguments
    // fprintf(stderr, "\tnumber of threads executing the RLC decoding algorithm in parallel: %" PRIu32 "\n", args.nb_threads);
    // fprintf(stderr, "\tverbose mode: %s\n", args.verbose ? "enabled" : "disabled");

    // This is an example of how to open the instance files of the input directory. You may move/edit it during the project
    struct dirent *directory_entry;
    FILE *input_file;
    while ((directory_entry = readdir(args.input_dir))){
        // Ignore parent and current directory
        if (!strcmp(directory_entry->d_name, ".")){
            continue;
        }
        if (!strcmp(directory_entry->d_name, "..")){
            continue;
        }

        // Add the directory path to the filename to open it
        char full_path[PATH_MAX];
        memset(full_path, 0, sizeof(char) * PATH_MAX);
        strcpy(full_path, args.input_dir_path);
        strcat(full_path, "/");
        strcat(full_path, directory_entry->d_name);

        input_file = fopen(full_path, "r");
        if (input_file == NULL){
            fprintf(stderr, "Failed to open the input file %s: %s\n", full_path, strerror(errno));
            goto file_read_error;
        }
        if (args.verbose){
            // This is a simple example of how to use the verbose mode
            fprintf(stderr, "Successfully opened the file %s\n", full_path);
        }
        
        // lire fichier tranformer en blocs, decode le message 
        
        // TODO: parse the input binary file, decode the encoded message with RLC and write the output in the output stream following the statement

        // read file into binary_data
        fseek(input_file, 0, SEEK_END);
        long fsize = ftell(input_file);
        fseek(input_file, 0, SEEK_SET);

        binary_data = malloc(fsize); // il est free
        if(fread(binary_data, 1, fsize, input_file) == fsize){
            if(args.verbose){fprintf(stdout, "succecfully open file %s , \n", full_path);}

        }
        else {
        if(feof(input_file)){
            fprintf(stderr, "Errr while reading file %s", full_path);
        }
        else if (ferror(input_file)){perror("error while opening");}}

        //get variables
        msg = get_file_infos(binary_data);

        if (args.verbose){
            printf("seed = %d\n", msg.seed);
            printf("block_size = %d\n", msg.block_size);
            printf("word_size = %d\n", msg.word_size);
            printf("redundancy = %d\n", msg.redundancy);
            //printf("message_size = %llu\n", msg.message_size);
            // printf("coefs = %d\n", msg.coefs[][]);
            // print(binary_data);
        }

        step = msg.word_size * (msg.block_size + msg.redundancy);  //# Taille de bloc en bytes 

        //Ecrit la taille du titre et du contenu du message
        // Ecrit le nom du fichier dans l'output

        //fprintf(args.output_stream, "%lu ", strlen(directory_entry->d_name));
        //fprintf("%d ", msg.message_size);
        //fprintf("%c\n", directory_entry->d_name);
        // else:
        //     output_fd.write(len(filename).to_bytes(4, "big"))
        //     output_fd.write(message_size.to_bytes(8, "big"))
        //     output_fd.write(filename.encode("ASCII"))

        //fprintf(args.output_stream,"" )

        //# Contient le nombre de blocs complets (sans le dernier bloc s'il n'est pas complet)
        // printf("fsize - 24 = %d\n", fsize - 24);
        nb_blocks = ceil((float)(fsize - 24) / (msg.word_size * (msg.block_size + msg.redundancy)));
        int contains_uncomplete_block = 0;
        if (msg.message_size != nb_blocks * msg.block_size * msg.word_size){  //# Dernier bloc non complet (i.e., moins de block_size symboles)
            nb_blocks -= 1;
            contains_uncomplete_block = 1;
        }

        readed = 0; // # Nombre de bytes lus jusqu'à présent
        block_number = -1;
        next_block_to_write = 0;

        for (int i = 0; i < args.nb_threads; ++i)
            pthread_create(&threads[i], NULL, thread_control, NULL);

        for (int i = 0; i < args.nb_threads; ++i)
            pthread_join(threads[i], NULL);
            

        // printf("\n-----------1-------------\n");

        int readed_symbols = msg.block_size * msg.word_size * nb_blocks;
        int nb_remaining_symbols = ((fsize - 24 - readed) / msg.word_size) - msg.redundancy;

        // printf("readed_symbols: %d\n", readed_symbols);
        // printf("nb_remaining_symbols: %d\n", nb_remaining_symbols);
        if (contains_uncomplete_block){
            block_t last_block = make_block(&binary_data[24 +readed], msg, nb_remaining_symbols);
            if (last_block.bloc == NULL){
                printf("There is a problem on the last block when we try to make it\n");
                return -1;
            }
            int last_prog = process_block(last_block, msg, nb_remaining_symbols);
            if (last_prog == -1){
                printf("there is an error with the last block, at the creation of the new_losts_word\n");
                return -1;
            }
            if (last_prog == -2){
                printf("There is an error with the last block, at the creation of the linear system\n");
                return -1;
            }
            if (last_prog == -3){
                printf("There is a problem with the last block, when we try to operate on vectors\n");
                return -1;
            }
            if (last_prog == -4){
                printf("There is a problem with the last block when we try to solve the system\n");
                return -1;
            }
            // # Le dernier symbole peut ne pas etre complet et contenir du padding
            // # => on utilise la taille totale du fichier pour retirer le padding
            
            if (args.verbose){
                print_block("before processe_block",last_block,msg) ;
            }
            // process_block(last_block, msg, msg.block_size);

            if (args.verbose){
                print_block("after processe_block",last_block,msg) ;
            }
            int padding = readed_symbols + nb_remaining_symbols * msg.word_size - msg.message_size;
            int true_length_last_symbol = msg.word_size - padding;
            // if (args.verbose)
            //     print(last_block);
                
            write_last_block(1, last_block, nb_remaining_symbols, msg.word_size, true_length_last_symbol);

            // free du dernier bloc 'last_block'
            for (int i=0; i<nb_remaining_symbols +msg.redundancy; i++){
                free (last_block.bloc[i]);
            }free(last_block.bloc);
        }

        for (int i=0; i<msg.redundancy; i++){
            free(msg.coeffs[i]);
        }
        free(msg.coeffs);
        // Close this instance file
        free(binary_data);
        fclose(input_file);


    }

    // Close the input directory and the output file
    err = closedir(args.input_dir);
    if (err < 0){
        fprintf(stderr, "Error while closing the input directory containing the instance files\n");
    }
    if (args.output_stream != stdout){
        fclose(args.output_stream);
    }
    t2 = clock();
    temps = (float)(t2-t1)/CLOCKS_PER_SEC;
    printf("temps = %f\n", temps);

    sem_destroy(sem);

    return 0;

file_read_error:
    err = closedir(args.input_dir);
    if (err < 0){
        fprintf(stderr, "Error while closing the input directory containing the instance files\n");
    }
    if (args.output_stream != stdout){
        fclose(args.output_stream);
    }
    exit(EXIT_FAILURE);
}