cmake_minimum_required(VERSION 3.27)
project(diffusion_layer C)

set(CMAKE_C_STANDARD 17)

add_executable(diffusion_layer main.c
        constants.h
        utils.c
        utils.h
        permutations.c
        permutations.h
        permutations.h
        round.c
        round.h
        aead.c
        aead.h
        api.h)
