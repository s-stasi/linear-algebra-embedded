#include "../include/vector_math.h"
#include "../include/operation_state.h"

LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_dot_product(const Vector_t *u, const Vector_t *v, float *result)) {
  if (u->length != v->length) return vector_wrong_dimension;

  *result = 0.0f;

  if (u->length == 2) *result = u->data[0]*v->data[0] + u->data[1]*v->data[1];
  else if (u->length == 3) *result = u->data[0]*v->data[0] + u->data[1]*v->data[1] + u->data[2]*v->data[2];

  else{
    uint8_t i;
    for (i = 0; i + 4 < u->length; i += 4) {
      *result += u->data[i] * v->data[i];
      *result += u->data[i + 1] * v->data[i + 1];
      *result += u->data[i + 2] * v->data[i + 2];
      *result += u->data[i + 3] * v->data[i + 3];
    }

    for (; i < u->length; i++) {
      *result += u->data[i] * v->data[i];
    }
  }

  return ok;
}

LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_outer_product(const Vector_t *u, const Vector_t *v, Matrix_t *C)) {
  if (C == NULL) return matrix_null;
  if (v->length != C->cols || u->length != C->rows) return matrix_wrong_dimensions;

  float *pC = C->data;
  const float* __restrict pu = u->data;
  const float* __restrict pv = v->data;

  for (uint8_t i = 0; i < u->length; i++){
   for (uint8_t j = 0; j < v->length; j++) {
      pC[i * v->length + j] = pu[i]*pv[j];
    }
  }

  return ok;
}

LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t matrix_multiply_vector(const Matrix_t *A, const Vector_t *v, Vector_t *result)) {
  if (A->cols != v->length || A->rows != result->length) return matrix_wrong_dimensions;

  const float* __restrict pA = A->data;
  const float* __restrict pV = v->data;
  float* __restrict pRes = result->data;

  for (uint16_t i = 0; i < A->rows; i++) {
    float sum = 0.0f;
    
    for (uint16_t j = 0; j < A->cols; j++) {
      sum += pA[i * A->cols + j] * pV[j];
    }
    
    pRes[i] = sum;
  }

  return ok;
}

LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_multiply_matrix(const Vector_t *v, const Matrix_t *A, Vector_t *result)) {
  if (v->length != A->rows || result->length != A->cols) return matrix_wrong_dimensions;

  const float* __restrict pV = v->data;
  const float* __restrict pA = A->data;
  float* __restrict pRes = result->data;

  for (uint16_t j = 0; j < A->cols; j++) {
    float sum = 0.0f;
    
    for (uint16_t i = 0; i < A->rows; i++) {
      sum += pV[i] * pA[i * A->cols + j];
    }
    
    pRes[j] = sum;
  }

  return ok;
}

LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_add(const Vector_t *u, const Vector_t *v, Vector_t *result)) {
  if (u->length != v->length || u->length != result->length) return vector_wrong_dimension;

  const float* __restrict pU = u->data;
  const float* __restrict pV = v->data;
  float* __restrict pRes = result->data;

  for (uint8_t i = 0; i < u->length; i++) {
    pRes[i] = pU[i] + pV[i];
  }

  return ok;
}

LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_subtract(const Vector_t *u, const Vector_t *v, Vector_t *result)) {
  if (u->length != v->length || u->length != result->length) return vector_wrong_dimension;

  const float* __restrict pU = u->data;
  const float* __restrict pV = v->data;
  float* __restrict pRes = result->data;

  for (uint8_t i = 0; i < u->length; i++) {
    pRes[i] = pU[i] - pV[i];
  }

  return ok;
}

LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_multiply_constant(const Vector_t *v, const float b, Vector_t *result)) {
  if (v->length != result->length) return vector_wrong_dimension;

  const float* __restrict pV = v->data;
  float* __restrict pRes = result->data;

  for (uint8_t i = 0; i < v->length; i++) {
    pRes[i] = pV[i] * b;
  }

  return ok;
}

LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_to_matrix(const Vector_t *v, Matrix_t *M)) {
  // Controlliamo che la matrice di destinazione sia un vettore colonna Nx1
  if (M->rows != v->length || M->cols != 1) return matrix_wrong_dimensions;

  const float* __restrict pV = v->data;
  float* __restrict pM = M->data;

  for (uint8_t i = 0; i < v->length; i++) {
    pM[i] = pV[i];
  }

  return ok;
}

LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t matrix_to_vector(const Matrix_t *M, Vector_t *v)) {
  // La matrice deve essere una colonna (Nx1) o una riga (1xN)
  if ((M->cols != 1 && M->rows != 1) || (M->cols * M->rows != v->length)) {
    return vector_wrong_dimension;
  }

  const float* __restrict pM = M->data;
  float* __restrict pV = v->data;

  for (uint8_t i = 0; i < v->length; i++) {
    pV[i] = pM[i];
  }

  return ok;
}

LINEAR_ALGEBRA_QUICKACCESS_CODE(OperationState_t vector_copy(const Vector_t *u, const Vector_t *v)) {
  if (u->length != v->length) return vector_wrong_dimension;

  for (uint8_t i = 0; i < v->length; i++) {
    v->data[i] = u->data[i];
  }

  return ok;
}
