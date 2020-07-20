# SDL2++ - A modern C++20 wrapper around SDL2

## Comparison between using the SDL2 API vs the SDL2++ API
### SDL2
```cpp
int main(int, char**) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        return EXIT_FAILURE;
    
    auto win = SDL_CreateWindow("Hello SDL2", 
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    800, 600,
                    SDL_WINDOW_RESIZABLE);
    if (win == nullptr)
        return EXIT_FAILURE;
    
    auto ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == nullptr)
        return EXIT_FAILURE;
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    
    bool quit = false;
    while(!quit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
        }
        SDL_RenderClear(ren);
        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
```

### SDL2++
```cpp
using namespcae sdl2;

int main(int, char**) {
   	SDL2 const sdl(sdl2_init_flags::EVERYTHING);
    if (!sdl) 
        return EXIT_FAILURE;
    
    window win("Hello SDL2++", window::pos_centered, {800, 800}, window_flags::RESIZABLE);
    if (!win)
        return EXIT_FAILURE;
    
    renderer ren(win, renderer_flags::ACCELERATED);
    if (!ren)
        return EXIT_FAILURE;
    ren.set_draw_color(colors::red);
    
    bool quit = false;
    while (!quit) {
        for (auto const& event : event_queue) {
            if (event.type == SDL_QUIT)
                quit = true;
        }
        ren.clear();
        ren.present();
    }
}
```

## Generating the documentation

In order to generate documentation for the project, you need to configure the build
to use Doxygen. This is easily done, by modifying the workflow shown above as follows:

```bash
mkdir build/ && cd build/
cmake .. -D<project_name>_ENABLE_DOXYGEN=1 -DCMAKE_INSTALL_PREFIX=/absolute/path/to/custom/install/directory
cmake --build . --target doxygen-docs
```

> ***Note:*** *This will generate a `docs/` directory in the **project's root directory**.*

## Running the tests

```bash
cd build          # if not in the build directory already
ctest -C Release  # or `ctest -C Debug` or any other configuration you wish to test

# you can also run tests with the `-VV` flag for a more verbose output (i.e.
#GoogleTest output as well)
```

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our how you can
become a contributor and the process for submitting pull requests to us.

## Authors

* **Nathan Ward** - [@nathansward](https://github.com/NathanSWard)

## License

This project is licensed under the [Unlicense](https://unlicense.org/) - see the
[LICENSE](LICENSE) file for details
