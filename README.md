# Embedded Linear Algebra Library

A lightweight, highly optimized C library for matrix and vector operations, specifically designed for microcontrollers and embedded systems. This library provides the mathematical foundation required for implementing advanced control systems, such as Kalman Filters and LQR controllers.

## Core Architecture

The library is designed for the constraints of embedded environments:

* **Modular Structure**: Clean separation between headers (`.h`) and source files (`.c`), removing the need for implementation macros.
* **Memory Safety**: Uses the `__restrict` keyword to prevent pointer aliasing, allowing the compiler to optimize memory access.
* **Static-Friendly**: While the higher-level filters might use initialization buffers, the core math functions operate on pre-allocated memory provided by the caller, ensuring deterministic execution times.
* **Dimensional Validation**: Strict validation is performed prior to any computation to prevent memory corruption and out-of-bounds access.

## Integration

The library is now structured as a standard C library. To integrate it:

1. Add `lib/linear-algebra-embedded/include` to your project's include paths.
2. Compile and link the following files:
   * `src/matrix_math.c`
   * `src/vector_math.c`

```c
#include "matrix_math.h"
#include "vector_math.h"

```

## Data Structures

The library operates on a unified error-handling state and distinct structures:

* **`OperationState_t`**: An enumeration returning the execution status (e.g., `ok`, `matrix_wrong_dimensions`, `matrix_singular`).
* **`Matrix_t`**: Contains `rows`, `cols`, and a `float* data` pointer.
* **`Vector_t`**: Contains `length` and a `float* data` pointer.

## Feature Overview

### Matrix Operations (`matrix_math.h`)

* **Arithmetic**: Addition, Subtraction, and Scalar Multiplication.
* **Multiplication**: Optimized matrix-matrix multiplication.
* **Linear Algebra**: Transposition, Determinant calculation (with fast paths for 2x2 and 3x3), and Matrix Inversion.
* **Initialization**: Functions for Trace, Identity, and Zero matrices.

### Vector Operations (`vector_math.h`)

* **Arithmetic**: Vector Addition, Subtraction, and Scalar Multiplication.
* **Products**: Dot Product and Outer Product (generating a matrix from two vectors).
* **Transformations**: Matrix-Vector and Vector-Matrix multiplications.

## Optimization Notes

The library includes explicit loop unrolling for common operations and leverages `__restrict` to help the compiler generate efficient SIMD instructions (like ARM NEON or CMSIS-DSP) where available.

## License

This project is licensed under the MIT License.