# HYPER-ORGANISM

Distributed Autonomous Cluster Agent Synthesizer — Digital Biology in Silicon.

A C++17 digital evolution system that simulates biological evolution to generate and optimize machine code (x86-64 assembly) for performance. The project treats computer programs as living organisms and applies genetic algorithms, immune system dynamics, ecological modeling, and distributed cluster migration to evolve high-performance code.

## Features

- **Genetic Algorithm Engine** — Tournament selection, block crossover, and multiple mutation strategies (instruction reorder, vectorization, NOP insertion, immediate mutation)
- **Simulated Sandbox Execution** — Fitness evaluation without `mmap`/execute of random machine code, analyzing byte patterns to estimate cycles, throughput, and correctness
- **Immune System** — Genome encryption/decryption, integrity checksums, parasite detection, self-repair, and self-destruct mechanisms
- **Dynamic Environment** — Shifting optimization goals, catastrophic events, and adaptive pressure
- **Ecosystem Modeling** — Species classification by fitness, energy flow, biodiversity index, and humus collection from dead organisms
- **Distributed Cluster** — RDMA bridge for organism migration across cluster nodes
- **Visualization** — ASCII population display, memory map, and fitness landscape rendering
- **Champion Export** — Export the best-evolved organism to `.asm`, `.bin`, and C `.h` files
- **Serialization** — JSON output of organism data including generation, architecture, code size, fitness breakdown, and hash

## Architecture

```
src/
├── core/              # Core data types, instruction model, pipeline model, fitness
├── isa/               # ISA backends (x86-64 AVX-512), encoding/decoding
├── sandbox/           # Safe execution sandbox with exception handling
├── eval/              # Benchmarking and validation
├── mutation/          # Mutator, instruction mutator, loop vectorizer, reorderer, genetic control
├── crossover/         # Crossover, block crossover, multi-parent crossover
├── pool/              # Genetic pool (population management)
├── ecology/           # Ecosystem with species and energy flow
├── garbage/           # Garbage collector (humus collection)
├── immune/            # Immune system: encryption, checksums, parasite detection
├── environment/       # Dynamic environment with catastrophes and pressure
├── distributed/       # Node controller and RDMA bridge for cluster migration
├── io/                # Serializer (JSON) and genome I/O
├── visualization/     # ASCII visualizer and champion exporter
└── main.cpp           # Evolution loop and cluster simulation
```

## Data Model

- `MachineCode` — `std::vector<uint8_t>` bytes with target ISA
- `FitnessScore` — cycles, throughput, correctness, energy estimate, composite score
- `Organism` — machine code, fitness, generation, parents, architecture, hash, metadata
- `Population` — vector of organisms with best/avg fitness and diversity metrics

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
```

### Options

- `ENABLE_MPI` — Enable MPI distributed support (OFF by default)
- `ENABLE_ARM64` — Enable ARM64 SVE backend (OFF by default)
- `ENABLE_TESTS` — Build tests (ON by default)

## Run

```bash
./build/src/hyper_organism_main
```

The program runs a 50-generation evolution cycle with 100 organisms, then simulates a 3-node cluster with organism migration.

## Example Output

```
╔══════════════════════════════════════════════════════════╗
║           HYPER-ORGANISM v1.0.0                          ║
║   Distributed Autonomous Cluster Agent Synthesizer       ║
║   Digital Biology in Silicon                             ║
╚══════════════════════════════════════════════════════════╝

=== Starting Evolution Cycle ===
Population: 100 organisms
Generations: 50
Target: x86-64 AVX-512

Gen 0 | Best: 0.00855737 | Avg: 0.00828509 | Diversity: 0.974063 | ...
Gen 49 | Best: 0.0128967 | Avg: 0.0128946 | Diversity: 0.223273 | ...

=== Evolution Complete ===
Best fitness: 0.0128967
Champion exported to champion.asm, champion.bin, champion.h
```

## Requirements

- C++17 compiler (GCC 9+ or Clang 10+)
- CMake 3.16+
- Optional: MPI library for distributed mode

## License

See LICENSE file for details.