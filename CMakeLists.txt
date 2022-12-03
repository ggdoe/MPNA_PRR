cmake_minimum_required(VERSION 3.10)
project(prr)
set(CMAKE_C_STANDARD 11)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)
# find_package(OpenMP)

add_executable(prr calcul_residu.c main.c projection.c prr.c resolution_sev.c retour_espace_depart.c tools.c)
target_compile_options(prr PRIVATE -Wall -flto -g)
target_link_libraries(prr PRIVATE m) #math.h
target_link_libraries(prr PRIVATE ${BLAS_LIBRARIES})
target_link_libraries(prr PRIVATE ${LAPACK_LIBRARIES})
# target_link_libraries(prr PRIVATE OpenMP::OpenMP_C)
