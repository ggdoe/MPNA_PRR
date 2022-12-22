cmake_minimum_required(VERSION 3.10)
project(prr)
find_package(MPI REQUIRED)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER mpicc)

find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)
find_package(OpenMP)

set(FLAGS -Wall -flto -g)
set(LIBS m ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} OpenMP::OpenMP_C)

add_executable(prr calcul_residu.c main.c projection.c prr.c resolution_sev.c retour_espace_depart.c tools.c)
target_compile_options(prr PRIVATE ${FLAGS})
target_link_libraries(prr PRIVATE ${LIBS})

add_executable(prr_mpi calcul_residu.c main.c projection.c prr.c resolution_sev.c retour_espace_depart.c tools.c)
target_compile_definitions(prr_mpi PRIVATE MULTIPRR)
target_compile_options(prr_mpi PRIVATE ${FLAGS})
target_link_libraries(prr_mpi PRIVATE ${LIBS})
target_link_libraries(prr_mpi PRIVATE ${MPI_C_LIBRARIES})
