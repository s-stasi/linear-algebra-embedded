#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>

#include "operation_state.h"

typedef struct
{
  uint8_t rows;
  uint8_t cols;
  float *data;
} Matrix_t;

#ifdef __cplusplus
extern "C"
{
#endif

  OperationState_t matrix_set_zero(Matrix_t *A);
  OperationState_t matrix_set_identity(Matrix_t *A);
  OperationState_t matrix_copy(const Matrix_t *A, Matrix_t *B);
  OperationState_t matrix_trace(const Matrix_t *A, float *result);
  OperationState_t matrix_multiply_elementwise(const Matrix_t *A, const Matrix_t *B, Matrix_t *C);
  OperationState_t matrix_add_constant(const Matrix_t *A, const float b, Matrix_t *C);
  OperationState_t matrix_add(const Matrix_t *A, const Matrix_t *B, Matrix_t *C);
  OperationState_t matrix_subtract(const Matrix_t *A, const Matrix_t *B, Matrix_t *C);
  OperationState_t matrix_multiply(const Matrix_t *A, const Matrix_t *B, Matrix_t *C);
  OperationState_t matrix_multiply_constant(const Matrix_t *A, const float b, Matrix_t *C);
  OperationState_t matrix_determinant(const Matrix_t *A, float *result, Matrix_t *scratchBuffer);
  OperationState_t matrix_inverse(const Matrix_t *A, Matrix_t *B, Matrix_t *scratchBuffer);
  OperationState_t matrix_transpose(const Matrix_t *A, Matrix_t *B);
  OperationState_t matrix_transpose_self(Matrix_t *A);

#ifdef __cplusplus
}
#endif

#endif // MATRIX_MATH_H
