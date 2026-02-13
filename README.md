# Minctes (Minimal C Testing)

## About
Minctes is a minimal testing framework for C99+. Mostly created for educational purposes.

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