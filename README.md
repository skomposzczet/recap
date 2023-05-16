# recap
C++ library for command line argument parsing.

# How to build
## Prerequisites
1. Project written in `c++20`, therefore `GCC 10.2` or `Clang 12.0.0` and `libstdc++10` or `libc++7` recommended.
## Building project
1. Run makefile
    ```bash
    make
    ```
## Installing
1. Install library
    ```bash
    make install
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

# Usage
## Args
To use create an instance of Parser and add arg your app requires.
```c++
Parser parser = ParserBuilder("app_name").get();
parser.add_argument(ArgBuilder("cl_arg").get());
```
Parse command line arguments.
```c++
// ./app --cl_arg value
parser.parse(argc, argv);
```
Then you can access args value by its name.
```c++
auto res = parser.get("cl_arg");
```
Result is `std::optional`. Contains value only if supplied or default value was given at the creation.
```c++
std::cout << res.value_or("was not given any value");
```
### Flags and positional args
Besides named args (i.e. options), flags and positional args are available.
```c++
parser.add_flag(FlagBuilder("flag").get());
parser.add_positional_argument(PositionalArgBuilder("pos_arg").get());

// ./app pos_value -f
parser.parse(argc, argv);
assert(true == parser.was_toggled("flag"));
assert(std::string{"pos_value"} == parser.get("pos_arg").value());
```
> **_NOTE:_**  While creating positional argument it can be assigned default (current highest + 1 starting at 0) or user defined order number using `order()` method. Order only refers to position according to other positional arguments.

> **_NOTE:_** Additionally positional args may be marked as required with `required()` function. While it makes no sense for required args to follow not required (unless implicitly forcing them to be required as well) it is not validated in any way.

## Conditions
Args can be supplied with conditions. Value will only be set if it meets all conditions.
```c++
auto is_whole_upper = [](const std::string& s) {
    return std::all_of(s.begin(), s.end(), [](unsigned char c){ 
        return !std::islower(c); 
    });
};
parser.add_argument(
    ArgBuilder("upper")
        .with_condition(Condition("IsWholeUpper", is_whole_upper))
        .get()
);
try {
    // ./app -u not_upper
    parser.parse(argc, argv);
    assert(false);
} catch(...) {}
```

More complex examples can be found in `examples` directory.