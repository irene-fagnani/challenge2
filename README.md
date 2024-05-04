# Challenge2: A sparse matrix.

### Introduction
In large scale computations it is often necessary to exploit sparsity to be able to operate with large matrices. Indeed, the space used up by full storage of a matrix grows as N quadratic and can rapidly fill up all your memory. Not only, if the matrix is sparse, operations like matrix vector product can be made more efficient if we avoid the useless multiplications by the zero elements. This repository hosts a template class designed to deal with sparse matrices.

### Features

- **Sparse Matrix Compression and Decompression**: Efficiently compress and decompress sparse matrices to optimize storage and computation.

- **Matrix Market (.mtx) File Support**: Read sparse matrices from Matrix Market (.mtx) files, providing seamless integration with standard datasets.

- **Flexible Storage Options**: Store matrices in either row-wise or column-wise formats, providing flexibility based on your application's requirements.

- **Matrix Operations**:
  - **Matrix-Matrix Multiplication**: Perform matrix multiplication efficiently.
  - **Matrix-Vector Multiplication**: Compute matrix-vector products swiftly.
  - **Matrix Norms**: Calculate various matrix norms including infinity norm, one norm, and Frobenius norm.
  - **Time Execution**: Measure the execution time of matrix operations for performance analysis.

- **Complex Number Support**: Designed to seamlessly handle operations with `std::complex` types, ensuring compatibility with complex-valued matrices and vectors.

- **User-Controlled Output**: Provide options to the user to choose whether to print results after each operation or not, enhancing usability and control.

- **File Selection with GetPot**: Utilize `GetPot` to enable users to select the Matrix Market (`.mtx`) file they want to operate on, enhancing flexibility and ease of use.

- **Compression Ratio Calculation**: Calculate the percentage reduction in storage size achieved through compression, facilitating evaluation of compression effectiveness.

- **Doxygen documentation**: Generate the Doxygen documentation in a doc folder, thanks to Doxygen comments and the Doxyfile.

### Repository content
+ [include](https://github.com/irene-fagnani/challenge2/tree/main/include): Contains the header files, MatrixClass.hpp, MatrixClass_implementations.hpp, MatrixClass_Utilities.hpp, Utilities.hpp.
+ [src](https://github.com/irene-fagnani/challenge2/tree/main/src): Contains source code files, main.cpp and GetPot.
+ [Makefile](https://github.com/irene-fagnani/challenge2/blob/main/Makefile): Provides basic setup for compiling the C++ project.
+ [Doxyfile](https://github.com/irene-fagnani/challenge2/blob/main/Doxyfile): Describes the settings to be used by the documentation system doxygen.
+ [lnsp_131.mtx](https://github.com/irene-fagnani/challenge2/blob/main/lnsp_131.mtx): matrix used in the main.cpp


Content of [MatrixClass.hpp](https://github.com/irene-fagnani/challenge2/blob/main/MatrixClass.hpp)

+ Template class and declarations of all members of the class, in the algebra namespace.


Content of  [MatrixClass_implementations.hpp](https://github.com/irene-fagnani/challenge2/blob/main/MatrixClass_implementations.hpp)

+ Definition of all the member function of the MatrixClass template class:
    - Const and non-const operator().
    - Compress and uncompress methods.
    - Resize matrix method.
    - Read matrix from a mtx file.
    - Two operator* (one to manage matrix * matrix multiplication, one to manage matrix * vector multiplication).
    - A method to compute the norm, in three different ways based on the value of the NormMethod template.
    - A method to print the matrix.


Content of [MatrixClass_Utilities.hpp](https://github.com/irene-fagnani/challenge2/blob/main/MatrixClass_Utilities.hpp)

+ Enums `StorageOrder` and `NormMethod`, used to specify the order of storage of elements in a MatrixClass object (row wisely or column wisely) and to specify the wanted method to compute the norm.
+ The friend function operator<, used to confront the key of the map in the two different cases (column wise or row wise storage)
+ Structs to verify is a number is complex or not.

Content of [Utilities.hpp](https://github.com/irene-fagnani/challenge2/blob/main/Utilities.hpp)
+ A function to print vectors
+ Two functions to count the time of the multiplication. (overload)

Content of [main.cpp](https://github.com/irene-fagnani/challenge2/blob/main/main.cpp)
+ It contains the main function. The case with "lnsp_131.mtx" matrix is setted as default. For each case, uncompressed and compressed, a set of operations are done: matrix * vector, matrix * vector, with vector as an MatrixClass object, and matrix * matrix. Also, all 3 types of norms are computed. The part with the smallest matrix is ​​commented because I believe the decrease in multiplication time going from the uncompressed case to the compressed one is not notable. However, in this part I show also how the methods of the class works also with `std::complex` types.

### Requirements
+ C++ compiler compatible with C++20 standard
+ Git for cloning the repository
+ Make utility for compiling the code

### Running Locally

Clone the project:
```bash
    git clone git@github.com:irene-fagnani/challenge2.git
```
Compile the code:
```bash
    make
```

Compile the code and generate Doxygen documentation:
```bash
    make doc
```

Run the challenge:
```bash
    ./main
```

Run the challenge and print all the results of the multiplications:
```bash
    ./main print=true
```

Run the challenge with a new_file.mtx
```bash
    ./main filename="new_file.mtx"
```

