<<<<<<< HEAD



## Usage

You may run the program with

```bash
gcc src/* -lm & ./a.out input_binary 
```

test
```bash
diff <(diff <(./a.out input_binary) <(cat input_txt/africa.txt input_txt/medium.txt input_txt/big.txt input_txt/small.txt)) exp.txt

leaks -atExit -- ./a.out input_binary
```


## explication programme

- system.c : contient les fonction d'addition, de multiplication et d'innversion des vecteurs. Contient également la fonction gen_coefs permettant de générer aléatoirement les coéfficients de chaque bloc.
- processe_block.c : contient la fonction find_lost_words qui permet de trouver les symboles sources perdu, la fonction make_linear_system qui crée système linéaire Ax=b. Et pour finir la fonction matrix_solve qui résou le système lineaire. 
- main.c : contient les fonctions make_block, write_block, write_last_block et print_block utilent aux blocs. Contient également la fonction main.

## mono-thread 

le programme print les 4 input en moyenne en 0.023 secondes. Cependant, certains caractères/symboles ne sont pas les bons. 
=======

## Usage

You may run the program with

```bash
gcc src/* -lm & ./a.out input_binary 
```

test
```bash
diff <(diff <(./a.out input_binary) <(cat input_txt/africa.txt input_txt/medium.txt input_txt/big.txt input_txt/small.txt)) exp.txt

leaks -atExit -- ./a.out input_binary
```

To test the average speed of execution of you can execute
```bash
./executionSpeed.sh  NUMBER_TIMES "PARAMETERS"
```
with
- `NUMBER_TIMES` : the numbers of time yo want to execute the code
- `PARAMETERS` : the parameters used to run the code like -f FILE_NAME.txt

## explication programme

- system.c : contient les fonctions d'addition, de multiplication et d'innversion des vecteurs. Contient également la fonction gen_coefs permettant de générer aléatoirement les coéfficients de chaque bloc.
- processe_block.c : contient la fonction find_lost_words qui permet de trouver les symboles sources perdus, la fonction make_linear_system qui crée le système linéaire Ax=b. Et pour finir la fonction matrix_solve qui résout le système lineaire. 
- main.c : contient les fonctions make_block, write_block, write_last_block et print_block utiles aux blocs. Contient également la fonction main.

## mono-thread 

le programme print les 4 input en moyenne en 0.023 secondes. Cependant, certains caractères/symboles ne sont pas les bons. 
>>>>>>> 8bc809f1537ee68a71453f7e26bfc8eb38b4ca21
