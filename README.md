# Getting Started with Parlaylib
[ParlayLib](https://github.com/cmuparlay/parlaylib) is a toolkit for programming parallel algorithms on shared-memory multicore computers.

This repository helps you get started to integrate ParlayLib into a C++ executable on Linux and Windows. A single example source file is provided, which builds
from command line on Linux. For Windows, a Visual Studion 2022 project is provided, which compiles the same source file. Steps toward getting executables building
and running are provided.

## Getting Started on Linux
To install g++ which supports C++17:
```
sudo apt update
sudo apt upgrade
# reboot the system
sudo apt install build-essential
```
To obtain ParlayLib and set it up:
```
# go to the directory where parlaylib will live
git clone https://github.com/cmuparlay/parlaylib.git
cd parlaylib/
mkdir -p build/Release && cd build/Release
sudo apt  install cmake
cmake -DCMAKE_BUILD_TYPE=Release -DPARLAY_BENCHMARK=On ../..
cmake --build .
```
ParlayLib recommends installing a high performance memory allocator: jemalloc
```
# go to the directory where jemalloc will live
git clone https://github.com/jemalloc/jemalloc.git
sudo apt-get install autoconf
./configure
./autogen.sh
make
sudo make install
```
ParlayLib benchmark can be run with or without jemalloc:
```
# with jemalloc
LD_PRELOAD=/usr/local/lib/libjemalloc.so ./benchmark/bench_standard --benchmark_repetitions=20 > benchmark_results_with_jemalloc.txt
# without jemalloc
./benchmark/bench_standard --benchmark_repetitions=20 > benchmark_results.txt
```
To compile the example C++ source file on Linux (or WSL on Windows):
```
# On Linux where parlaylib and parlaylib_getting_started repos are at the same directory level
git clone https://github.com/DragonSpit/parlaylib_getting_started.git
cd parlaylib_getting_started/parlay_proj
g++ -I ../../parlaylib/include parlaylib_proj.cpp -std=c++20 -O3 -o parlaylib_proj
```
If parlaylib was cloned into a different directory, then change "-I" command line option to point to that directory.

To run the executable
```
./parlaylib_proj 100000000
```

## Getting Started on Windows
Install [Visual Studio 2022](https://visualstudio.microsoft.com/vs/whatsnew/) - either free (Community) or paid version (Professional or Enterprise).
[Intel OneAPI](https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit-download.html) can be installed to obtain Intel C++ compiler for Visual Studio 2022.
Both compilers (Microsoft or Intel) can be used to build this project. To switch between compilers, select "Project/Intel-Compiler" from Visual Studio 2022 menu.
Install [Git for Windows](https://github.com/git-guides/install-git).

To obtain ParlayLib and set it up, open a "Command Prompt" window:
```
# go to the directory where parlaylib will live (e.g. c:\repos)
git clone https://github.com/cmuparlay/parlaylib.git
```

In "parlaylib_proj" directory, Visual Studio 2022 solution/project (parlaylib_proj.sln) is provided to build an executable using either Microsoft's compiler or Intel compiler. Right-click on `parlaylib_proh.sln` file in Explorer and select "Open with Visual Studion 2022".
Once Visual Studio 2022 comes up, select "Built/Rebuild-Solution" to build the executable.

If Intel compiler was used, then copy `libmmd.dll` from Intel compiler (C:\Program Files (x86)\Intel\oneAPI\compiler\2024.0\bin) to the directory the executable is in (parlaylib_getting_started\parlaylib_proj\x64\Release).
To run the built executable, open "Command Prompt" and navigate to the directory Visual Studio 2022 shown in the Output window where the executable was placed - path should end with x64\Release.
```
parlaylib_proj 100000000
```
