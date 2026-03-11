#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#define VECTOR2_VIEW(v2_ptr) (&(Vector_t){ .length = 2, .data = (float *)(v2_ptr) })
#define VECTOR3_VIEW(v3_ptr) (&(Vector_t){ .length = 3, .data = (float *)(v3_ptr) })

#include <stdint.h>
#include <stdbool.h>

#include "operation_state.h"
#include "matrix_math.h"

typedef struct {
  float x;
  float y;
} Vector2_t;

typedef struct {
  float x;
  float y;
  float z;
} Vector3_t;

typedef struct {
  uint8_t length;
  float *data;
} Vector_t;

#ifdef __cplusplus
extern "C"
{
#endif

  LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_add(const Vector_t *u, const Vector_t *v, Vector_t *result));
  LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_subtract(const Vector_t *u, const Vector_t *v, Vector_t *result));
  LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_dot_product(const Vector_t *u, const Vector_t *v, float *result));
  LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_outer_product(const Vector_t *u, const Vector_t *v, Matrix_t *C));
  LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_multiply_constant(const Vector_t *v, const float b, Vector_t *result));
  LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t matrix_multiply_vector(const Matrix_t *A, const Vector_t *v, Vector_t *result));
  LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_multiply_matrix(const Vector_t *v, const Matrix_t *A, Vector_t *result));
  LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_copy(const Vector_t *u, const Vector_t *v));

  /* Conversion Helpers */
  LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_to_matrix(const Vector_t *v, Matrix_t *M));
  LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t matrix_to_vector(const Matrix_t *M, Vector_t *v));


#ifdef __cplusplus
}
#endif

#endif // VECTOR_MATH_H
