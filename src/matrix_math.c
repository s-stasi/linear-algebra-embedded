#include "../include/matrix_math.h"
#include "../include/operation_state.h"

OperationState_t matrix_set_zero(Matrix_t *A) {
  float *pA = A->data;

  uint16_t dim = A->cols * A->rows;

  for (int i = 0; i <dim; i++) {
    pA[i] = 0.0f;
  }

  return ok;
}

OperationState_t matrix_set_identity(Matrix_t *A) {
  if (A->cols != A->rows) return matrix_not_square;

  float *pA = A->data;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->rows; j++) {
      if (i == j) pA[i * A->cols + j] = 1.0f;
      else  pA[i * A->cols + j] = 0.0f;
    }
  }

  return ok;
}

OperationState_t matrix_copy(const Matrix_t *A, Matrix_t *B) {
  if (A->rows != B->rows || A->cols != B->cols) return matrix_wrong_dimensions;

  const float* __restrict pA = A->data;
  float* __restrict pB = B->data;

  uint16_t dim = A->cols * A->rows;

  for (int i = 0; i < dim; i++) {
    pB[i] = pA[i];
  }

  return ok;
}

OperationState_t matrix_trace(const Matrix_t *A, float *result) {
  if (A->cols != A->rows) return matrix_not_square;

  const float* __restrict pA = A->data;

  *result = 0;

  for (int i = 0; i < A->rows; i++) {
    *result += pA[i * A->cols + i];
  }
  
  return ok;
}

OperationState_t matrix_add(const Matrix_t *A, const Matrix_t *B, Matrix_t *C)
{
  if (A->cols != B->cols || A->rows != B->rows || C->cols != A->cols || C->rows != A->rows)
    return matrix_wrong_dimensions;

  uint16_t size = A->rows * A->cols;
  const float* __restrict pA = A->data;
  const float* __restrict pB = B->data;
  float* __restrict pC = C->data;

  uint16_t i = 0;
  for (i = 0; i <= size - 4; i += 4)
  {
    pC[i] = pA[i] + pB[i];
    pC[i + 1] = pA[i + 1] + pB[i + 1];
    pC[i + 2] = pA[i + 2] + pB[i + 2];
    pC[i + 3] = pA[i + 3] + pB[i + 3];
  }

  for (; i < size; i++)
  {
    pC[i] = pA[i] + pB[i];
  }

  return ok;
}

OperationState_t matrix_add_constant(const Matrix_t *A, const float b, Matrix_t *C) {
  if (A->cols != C->cols || A->rows != C->rows) return matrix_wrong_dimensions;

  const float* __restrict pA = A->data;
  float* __restrict pC = C->data;

  uint16_t dim = A->cols * A->rows;

  for (int i = 0; i < dim; i++) {
    pC[i] = pA[i] + b;
  }
  
  return ok;
}

OperationState_t matrix_subtract(const Matrix_t *A, const Matrix_t *B, Matrix_t *C)
{
  if (A->cols != B->cols || A->rows != B->rows || C->cols != A->cols || C->rows != A->rows)
    return matrix_wrong_dimensions;

  uint16_t size = A->rows * A->cols;
  const float* __restrict pA = A->data;
  const float* __restrict pB = B->data;
  float* __restrict pC = C->data;

  uint16_t i = 0;
  for (i = 0; i <= size - 4; i += 4)
  {
    pC[i] = pA[i] - pB[i];
    pC[i + 1] = pA[i + 1] - pB[i + 1];
    pC[i + 2] = pA[i + 2] - pB[i + 2];
    pC[i + 3] = pA[i + 3] - pB[i + 3];
  }

  for (; i < size; i++)
  {
    pC[i] = pA[i] - pB[i];
  }

  return ok;
}

OperationState_t matrix_multiply(const Matrix_t* A, const Matrix_t* B, Matrix_t* C) {
  if (A->cols != B->rows || C->rows != A->rows || C->cols != B->cols) return matrix_wrong_dimensions;

  const float* __restrict pA = A->data;
  const float* __restrict pB = B->data;
  float* __restrict pC = C->data;

  for (uint16_t i = 0; i < A->rows; i++) {
    for (uint16_t j = 0; j < B->cols; j++) {
      uint16_t Cindex = i * C->cols + j;

      float somma = 0;
      for (uint16_t k = 0; k < A->cols; k++) {
        uint16_t Aindex = i * A->cols + k;
        uint16_t Bindex = k * B->cols + j;

        somma = somma + (pA[Aindex] * pB[Bindex]);
      }
      pC[Cindex] = somma;
    }
  }

  return ok;
}

OperationState_t matrix_multiply_elementwise(const Matrix_t *A, const Matrix_t *B, Matrix_t *C) {
  if (A->cols != B->cols || A->rows != B->rows || A->cols != C->cols || A->rows != C->rows) return matrix_wrong_dimensions;

  uint16_t dim = A->cols * A->rows;

  const float* __restrict pA = A->data;
  const float* __restrict pB = B->data;
  float* __restrict pC = C->data;

  for (int i = 0; i < dim; i++) {
    pC[i] = pA[i] * pB[i];
  }

  return ok;
}

OperationState_t matrix_multiply_constant(const Matrix_t *A, const float b, Matrix_t *C) {
  if (A->rows != C->rows || A->cols != C->cols) return matrix_wrong_dimensions;

  const float* __restrict pA = A->data;
  float* __restrict pC = C->data;

  for (uint16_t i = 0; i < A->rows; i++) {
    for (uint16_t j = 0; j < A->cols; j++) {
      uint16_t index = i * A->cols + j;

      pC[index] = pA[index] * b;
    }
  }

  return ok;
}

OperationState_t matrix_determinant(const Matrix_t *A, float *result, Matrix_t *scratchBuffer) {
  if (A->cols != A->rows) return matrix_not_square;

  const float *pA = A->data;
  
  uint16_t size = A->cols*A->cols;
  
  
  if (A->cols == 2) {
    *result = pA[0] * pA[3] - pA[1] * pA[2];
  }
  else if (A->cols == 3) {
    float a_val = pA[0] * pA[4] * pA[8];
    float b_val = pA[1] * pA[5] * pA[6];
    float c_val = pA[2] * pA[3] * pA[7];
    
    float d_val = pA[2] * pA[4] * pA[6];
    float e_val = pA[1] * pA[3] * pA[8];
    float f_val = pA[0] * pA[5] * pA[7];
    
    *result = a_val + b_val + c_val - d_val - e_val - f_val;
  }
  else {
    if (scratchBuffer == NULL) return matrix_null;
    if (A->cols != scratchBuffer->cols || A->rows != scratchBuffer->rows) return matrix_wrong_dimensions;
    float *sbd = scratchBuffer->data;

    for (uint16_t k = 0; k < size; k++) {
      sbd[k] = pA[k];
    }
    
    float det_sign = 1.0f;

    for (uint8_t j = 0; j < A->cols; j++) {
      float diag = sbd[j * A->cols + j];

      float pivot = fabs(diag);
      uint8_t pivotRow = j;

      for (uint8_t i = j; i < A->cols; i++){
        float current_val = fabs(sbd[i * A->cols + j]);
        if(current_val > pivot) {
          pivot = current_val;
          pivotRow = i;
        }
      }

      if (pivot < 1e-6f) {
          *result = 0.0f;
          return ok;
      }

      if (pivotRow != j) {
        for (uint8_t col = 0; col < A->cols; col ++) {
          float tmp = sbd[j * A->cols + col];
          sbd[j * A->cols + col] = sbd[pivotRow * A->cols + col];
          sbd[pivotRow * A->cols + col] = tmp;
        }

        det_sign *= -1;
      }

      pivot = sbd[j * A->cols + j];
      for (int m = j + 1; m < A->cols; m++) {
        float mult = sbd[m * A->cols + j] / pivot;

        for (uint8_t col = j; col < A->cols; col++) {
          sbd[m * A->cols + col] -= mult * sbd[j * A->cols + col];
        }
      }

    }

    float final_det = det_sign;
    for (uint8_t i = 0; i < A->cols; i++) {
        final_det *= sbd[i * A->cols + i];
    }
    
    *result = final_det;
  }

  return ok;
}

OperationState_t matrix_inverse(const Matrix_t *A, Matrix_t *B, Matrix_t *scratchBuffer) {
  if (A->rows != A->cols) return matrix_not_square;
  if (A->rows != B->rows || A->cols != B->cols) return matrix_wrong_dimensions;

  float detA = 0;
  
  OperationState_t opState = matrix_determinant(A, &detA, B);

  if (opState != ok) return opState;
  if (fabsf(detA) < 1e-6f) return matrix_determinant_null;

  const float* __restrict pA = A->data;
  float* __restrict pB = B->data;
  float invDet = 1 / detA;

  if (A->cols == 2) {
    pB[0] = invDet * pA[3];
    pB[1] = invDet * (-pA[1]);
    pB[2] = invDet * (-pA[2]);
    pB[3] = invDet * pA[0];
  }

  else if (A->cols == 3) {
    pB[0] =  (pA[4]*pA[8] - pA[5]*pA[7]) * invDet;
    pB[1] = -(pA[1]*pA[8] - pA[2]*pA[7]) * invDet;
    pB[2] =  (pA[1]*pA[5] - pA[2]*pA[4]) * invDet;
    
    pB[3] = -(pA[3]*pA[8] - pA[5]*pA[6]) * invDet;
    pB[4] =  (pA[0]*pA[8] - pA[2]*pA[6]) * invDet;
    pB[5] = -(pA[0]*pA[5] - pA[2]*pA[3]) * invDet;
    
    pB[6] =  (pA[3]*pA[7] - pA[4]*pA[6]) * invDet;
    pB[7] = -(pA[0]*pA[7] - pA[1]*pA[6]) * invDet;
    pB[8] =  (pA[0]*pA[4] - pA[1]*pA[3]) * invDet;
  }

  else {
    if (scratchBuffer == NULL) return matrix_null;
    if (scratchBuffer->cols != A->cols || scratchBuffer->rows != A->rows) return matrix_wrong_dimensions;
    float* __restrict scb = scratchBuffer->data;

    uint16_t dimension = A->cols * A->rows;
    for(uint16_t i = 0; i < dimension; i++) {
      scb[i] = pA[i];
    }

    for (uint8_t i = 0; i < A->rows; i++) {
      for (uint8_t j = 0; j < A->rows; j++) {
        if (i == j) pB[i * A->cols + j] = 1.0f;
        else pB[i * A->cols + j] = 0.0f;
      }
    }

    for (uint8_t j = 0; j < A->cols; j++) {
      
      float pivot_max = fabsf(scb[j * A->cols + j]);
      uint8_t pivotRow = j;

      for (uint8_t i = j + 1; i < A->cols; i++){
        float current_val = fabsf(scb[i * A->cols + j]);
        if(current_val > pivot_max) {
          pivot_max = current_val;
          pivotRow = i;
        }
      }

      if (pivot_max < 1e-6f) return matrix_singular; 

      if (pivotRow != j) {
        for (uint8_t col = 0; col < A->cols; col++) {
          float buf = pB[pivotRow * B->cols + col];
          pB[pivotRow * B->cols + col] = pB[j * B->cols + col];
          pB[j * B->cols + col] = buf;
          
          buf = scb[pivotRow * B->cols + col];
          scb[pivotRow * B->cols + col] = scb[j * B->cols + col];
          scb[j * B->cols + col] = buf;
        }
      }

      float pivot_val = scb[j * A->cols + j];
      float inv_pivot = 1.0f / pivot_val;
      
      for (uint8_t col = 0; col < A->cols; col++) {
        uint8_t index = j * A->cols + col;
        pB[index] = pB[index] * inv_pivot;
        scb[index] = scb[index] * inv_pivot;
      }

      // GAUSS-JORDAN
      for (uint8_t i = 0; i < A->rows; i++) {
        if (i != j) {
          float multiplier = scb[i * A->cols + j]; 
          
          for (uint8_t col = 0; col < A->cols; col++) {
            uint16_t index_i = i * A->cols + col;
            uint16_t index_j = j * A->cols + col;
            
            scb[index_i] -= multiplier * scb[index_j];
            pB[index_i]  -= multiplier * pB[index_j];
          }
        }
      }
    }
  }

  return ok;
}

OperationState_t matrix_transpose(const Matrix_t *A, Matrix_t *B) {
  if (A->rows != B->cols || A->cols != B->rows) return matrix_wrong_dimensions;

  const float* __restrict pA = A->data;
  float* __restrict pB = B->data;

  for (uint8_t i = 0; i < A->rows; i++) {
    for (uint8_t j = 0; j < A->cols; j++) {
      pB[j * B->cols + i] = pA[i * A->cols + j];
    }
  }

  return ok;
}

OperationState_t matrix_transpose_self(Matrix_t *A) {
  if (A->rows != A->cols) return matrix_not_square; 

  float *pA = A->data;
  uint16_t N = A->cols;

  for (uint16_t r = 0; r < N; r++) {
    for (uint16_t c = r + 1; c < N; c++) {
      
      uint16_t index_upper = r * N + c;
      uint16_t index_lower = c * N + r;
      
      float tmp = pA[index_upper];
      pA[index_upper] = pA[index_lower];
      pA[index_lower] = tmp;
    }
  }

  return ok;
}