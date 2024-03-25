cmake_minimum_required(VERSION 3.27)
project(diffusion_layer C)

set(CMAKE_C_STANDARD 17)

# Add the directory where the cJSON.h header is located
include_directories(/Users/ik/Library/Mobile Documents/com~apple~CloudDocs/Yousuf/MacbookAir/cJSON)
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
        api.h
        aead.h
        word.h
        cJSON/cJSON.c
)




