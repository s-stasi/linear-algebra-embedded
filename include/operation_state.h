#ifndef OPERATION_STATE_H
#define OPERATION_STATE_H
typedef enum
{
  matrix_null,
  matrix_wrong_dimensions,
  matrix_determinant_null,
  matrix_not_square,
  matrix_singular,
  vector_wrong_dimension,
  ok
} OperationState_t;
#endif // OPERATION_STATE_H