# Core types and data structures for HYPER-ORGANISM

set(SOURCES
    core/types.cpp
    core/organism.cpp
    core/instruction.cpp
    core/pipeline_model.cpp
    core/fitness.cpp
    sandbox/executor.cpp
    sandbox/memory_guard.cpp
    sandbox/timeout_guard.cpp
    eval/benchmark.cpp
    eval/validator.cpp
    mutation/mutator.cpp
    mutation/instruction_mutator.cpp
    mutation/loop_vectorizer.cpp
    mutation/reorderer.cpp
    crossover/crossover.cpp
    crossover/block_crossover.cpp
    pool/genetic_pool.cpp
    pool/persistence.cpp
    distributed/node_controller.cpp
    distributed/mpi_bridge.cpp
    distributed/rdma_bridge.cpp
    io/serializer.cpp
    io/genome_io.cpp
    main.cpp
)

add_library(hyper_organism STATIC ${SOURCES})
target_include_directories(hyper_organism PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_compile_options(hyper_organism PRIVATE -march=native)

if(ENABLE_MPI)
    find_package(MPI REQUIRED)
    target_link_libraries(hyper_organism PUBLIC MPI::MPI_CXX)
    target_compile_definitions(hyper_organism PUBLIC HYPER_ORGANISM_ENABLE_MPI)
endif()

if(ENABLE_ARM64)
    target_compile_definitions(hyper_organism PUBLIC HYPER_ORGANISM_ENABLE_ARM64)
endif()

add_executable(hyper_organism_main main.cpp)
target_link_libraries(hyper_organism_main hyper_organism)
target_include_directories(hyper_organism_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)