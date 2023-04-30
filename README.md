# recap

## Prerequisites
1. Project build in `c++20`, therefore `gcc` in `13.1` version or higher is required:
    ```bash
    gcc --version
    # gcc (GCC) 13.1.0
    ```
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