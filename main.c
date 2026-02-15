#include "mortar.h"

int main(int argc, char* argv[]) {
    
    mortar_init_info info = mortar_init();

    mortar_quit(&info);

    return 0;
}