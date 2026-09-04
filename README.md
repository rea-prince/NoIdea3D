![C](https://img.shields.io/badge/C-A8B9CC?style=flat&logo=c&logoColor=black)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![SDL2](https://img.shields.io/badge/SDL2-000000?style=flat)

## No Idea 3D
NoIdea3D is a C++ implementation of the Raytracing section from Gabriel Gambetta's book [**Computer Graphics from Scratch**](https://gabrielgambetta.com/computer-graphics-from-scratch/) using SDL2. This spawned out of boredom as I had *no idea* what to do.

<p align="center">
<img width="798" height="798" alt="image" src="https://github.com/user-attachments/assets/4e8a6df8-33fb-413f-9807-4d56cb5429b0" />
</p>

The functions are almost directly translated from pseudo-code to C++, with some minor tweaks. Do note that I wrote most of the functions in C-style and tried to avoid using C++ features and classes.

All the main ray tracing functions are written neatly in `src/baller.cpp` with documentation, separated as they are in the book. You can mess around with the scenes/objects in `include/objects.h`. The default scene is hard coded in the `main` function and is only blitted to the rendered texture once, just before the loop.

Performance-wise, it's not the best, but it works. I have a local branch with camera movement and it runs at around ~30fps with a few objects in the scene.

## Usage

```bash
cmake -G "Ninja" -B build
# or whatever build tool you so desire

cmake --build build
./build/noidea3d.exe
```

> Note that I have yet to test the build on Linux.
