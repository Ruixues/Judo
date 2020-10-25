# How To build?

Environment Request:  
-   CMake
-   CXX Compiler (recommend Clang)
-   LLVM 10

First run

```
mkdir build && cd build
```

Then call cmake to generate make files.
We suggest you to use Ninja.

```
cmake -G "Ninja" ../
```

Then Run ninja

```
Ninja
```

After compiling,you will get the binary file of Judo.

Enjoy it!