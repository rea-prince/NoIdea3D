![C](https://img.shields.io/badge/C-A8B9CC?style=flat&logo=c&logoColor=black)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![SDL2](https://img.shields.io/badge/SDL2-000000?style=flat)

## No Idea 3D
NoIdea3D is a C++ implementation of the Raytracing section from Gabriel Gambetta's book [**Computer Graphics from Scratch**](https://gabrielgambetta.com/computer-graphics-from-scratch/) using SDL2. This spawned out of boredom as I had *no idea* what to do.

The functions are almost directly translated from pseudo-code to C++, with some minor tweaks. Do note that I wrote most of the functions in C-style and tried to avoid using C++ features.

## Usage

```bash
# Windows
cmake -G "MinGW makefiles" -B build/

# Unix
cmake -G "Unix makefiles" -B build/

cmake build && cd build && make
./noidea3d
```

> Note that I have yet to test the build on Linux.
