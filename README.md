# Minctes (Minimal C Testing)

## About
Minctes is a minimal testing framework for C99+. Mostly created for educational purposes.

## Creating tests
### Registering a test
To create a test, first create a file with the following naming scheme:
```BASH
[filename].t.c
```
Files with this naming scheme will be discovered for tests.

Within this file, define test functions using the following macro:
```C
MINCTES([test_name]){
    //Test logic
}
```
### Test Helpers
Provided are the following macros to aid in writing tests. Using these allows you to fail tests while continuing to process further tests.
```C
MINCTES_ASSERT([condition])
```
```C
MINCTES_EQUALS([actual],[expected],[string_format])
```

## Pipeline
* Run 
```BASH
minctes discover [src_folder] [output_folder]
```
This will generate a discovered_tests.h file.

* Create a library archive of your project to run tests against.

* Run
```BASH
minctes build [c_compiler] [discovered_tests.h] [your_library.a] [output_folder]
```
This will build a test_runner executable. Note that only compilers using the Clang/GCC syntax are currently supported. For example:
```BASH
clang [main.c] -o [output]
```

* Execute test_runner 