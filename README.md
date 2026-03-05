# Embedded Linear Algebra Library

A lightweight, highly optimized C library for matrix and vector operations, specifically designed for microcontrollers and embedded systems. This library provides the mathematical foundation required for implementing advanced control systems, such as the Extended Kalman Filter (EKF).

## Core Architecture

The library is designed around constraints typical of embedded environments:

* **Zero Dynamic Allocation:** The library does not use `malloc` or `free`. All memory for matrices and vectors must be statically allocated or provided by the caller, preventing heap fragmentation and ensuring deterministic execution times.
* **Single-Header Implementation:** Designed for seamless integration. The implementation is conditionally compiled using preprocessor macros.
* **Hardware Optimization:** Leverages the `__restrict` keyword to prevent pointer aliasing, allowing the compiler to heavily optimize memory access. It also includes explicit loop unrolling and fast paths for low-dimensional operations (e.g., 2D and 3D vectors).
* **Safety First:** Strict dimensional validation is performed prior to any computation to prevent memory corruption.

## Integration

To use the library in your project, include the headers in your files. In exactly **one** source file (e.g., your `main.c` or a dedicated `math_impl.c`), define the implementation macros before including the headers:

```c
#define MATRIX_MATH_IMPLEMENTATION
#include "matrix_math.h"

#define VECTOR_MATH_IMPLEMENTATION
#include "vector_math.h"

```

In all other files where you need the math functions, simply include the headers normally:

```c
#include "matrix_math.h"
#include "vector_math.h"

```

## Data Structures

The library operates on a unified error-handling state and distinct structures for matrices and vectors.

* `OperationState_t`: A global enumeration returning the execution status (e.g., `ok`, `matrix_wrong_dimensions`, `matrix_singular`).
* `Matrix_t`: Contains `rows`, `cols`, and a `float* data` pointer.
* `Vector_t`: Contains `length` and a `float* data` pointer. Represents a mathematical column vector.
* `Vector2_t` / `Vector3_t`: Fixed-size structures for spatial coordinates and basic kinematics.

## Feature Overview

### Matrix Operations (`matrix_math.h`)

* Addition, Subtraction, and Scalar Multiplication.
* Matrix Multiplication.
* Transposition.
* Determinant calculation (using LU decomposition for generic sizes, with optimized fast paths for 2x2 and 3x3).
* Matrix Inversion.
* Trace, Identity, and Zero initializations.

### Vector Operations (`vector_math.h`)

* Vector Addition, Subtraction, and Scalar Multiplication.
* Dot Product (with safe loop unrolling).
* Outer Product (generates a matrix from two vectors).
* Matrix-Vector and Vector-Matrix multiplications.
* Safe casting and conversions between `Vector_t` and `Matrix_t`.
