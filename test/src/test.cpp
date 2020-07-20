#include "sdl2++/init.hpp"

using namespace sdl2;

int main(int, char**) {
    SDL2 sdl2(sdl2_init_flags::EVERYTHING);
    if (!sdl2)
        return EXIT_FAILURE;
    
    IMG img(img_init_flags::ALL);
    if (!img)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}