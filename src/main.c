#include "../include/client.h"
#include "../include/simple.h"
#include "../include/complexe.h"

int main(int argc, char **argv) {
    if(argc != 3 && argc != 2){
        return -1;
    }
    if(argv[1][0] == 'c'){
        client();
    }else if(argv[1][0] == 's'){
        if(argv[2][0] == 's'){
            simple();
        }else if(argv[2][0] == 'c'){
            complexe();
        }
    }
    return 1;
}