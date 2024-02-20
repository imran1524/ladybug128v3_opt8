cmake_minimum_required(VERSION 3.27)
project(diffusion_layer C)

set(CMAKE_C_STANDARD 17)

add_executable(diffusion_layer main.c
        diffusion_layer.c
        diffusion_layer.h
        constants.h
        confusion_layer.c
        confusion_layer.h)
