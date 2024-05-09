cmake_minimum_required(VERSION 3.27)
project(ladybug128v2 C)

set(CMAKE_C_STANDARD 17)

# Add the directory where the cJSON.h header is located
include_directories(cJSON/cJSON)
add_executable(ladybug128v2 main.c
        constants.h
        utils.c
        utils.h
        crypto_aead.h
        permutations.c
        permutations.h
        round.c
        round.h
        aead.c
        api.h
        encrypt.h
        word.h
        cJSON/cJSON.c
)




