# recap
C++ library for library for command line argument parsing.

# How to build
## Prerequisites
1. Project written in `c++20`, therefore `GCC 10.2` or `Clang 12.0.0` and `libstdc++` 10 or `libc++ 7` recommended.
## Building project
1. Run makefile
    ```bash
    make
    ```
## Running tests
1. Clone [googletest](https://github.com/google/googletest/tree/release-1.8.1) into project root directory.
    ```bash
    git clone https://github.com/google/googletest.git
    ```
1. Run tests
    ```bash
    make test
    ```