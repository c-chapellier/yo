
test () {
    
    if ( diff <(./a.out input_binary) <(cat input_txt/medium.txt) )
    then
        echo -e "\033[0;32m[OK]\033[0m "
    else
        echo -e "\033[0;31m[ERROR]\033[0m"
    fi
}

test

