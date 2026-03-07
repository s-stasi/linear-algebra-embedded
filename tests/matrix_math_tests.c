#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MATRIX_MATH_IMPLEMENTATION
#include "../include/matrix_math.h"

#define TOLERANCE 1e-4f

bool check_float(float a, float b)
{
  return fabsf(a - b) < TOLERANCE;
}

bool check_matrix(const Matrix_t *A, const float *expected)
{
  uint16_t size = A->rows * A->cols;
  for (uint16_t i = 0; i < size; i++)
  {
    if (!check_float(A->data[i], expected[i]))
    {
      printf("Mismatch at index %d: expected %f, got %f\n", i, expected[i], A->data[i]);
      return false;
    }
  }
  return true;
}

void print_result(const char *test_name, bool passed)
{
  if (passed)
  {
    printf("[PASS] %s\n", test_name);
  }
  else
  {
    printf("[FAIL] %s\n", test_name);
  }
}

int main(void)
{
  // Add
  float a_add[4] = {1.0f, 2.0f, 3.0f, 4.0f};
  float b_add[4] = {5.0f, 6.0f, 7.0f, 8.0f};
  float c_add[4] = {0};
  float exp_add[4] = {6.0f, 8.0f, 10.0f, 12.0f};
  Matrix_t A_add = {2, 2, a_add};
  Matrix_t B_add = {2, 2, b_add};
  Matrix_t C_add = {2, 2, c_add};
  matrix_add(&A_add, &B_add, &C_add);
  print_result("Add", check_matrix(&C_add, exp_add));

  // Subtract
  float c_sub[4] = {0};
  float exp_sub[4] = {-4.0f, -4.0f, -4.0f, -4.0f};
  Matrix_t C_sub = {2, 2, c_sub};
  matrix_subtract(&A_add, &B_add, &C_sub);
  print_result("Subtract", check_matrix(&C_sub, exp_sub));

  // Multiply
  float a_mul[6] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
  float b_mul[6] = {7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f};
  float c_mul[4] = {0};
  float exp_mul[4] = {58.0f, 64.0f, 139.0f, 154.0f};
  Matrix_t A_mul = {2, 3, a_mul};
  Matrix_t B_mul = {3, 2, b_mul};
  Matrix_t C_mul = {2, 2, c_mul};
  matrix_multiply(&A_mul, &B_mul, &C_mul);
  print_result("Multiply", check_matrix(&C_mul, exp_mul));

  // Constant
  float c_const[4] = {0};
  float exp_const[4] = {2.0f, 4.0f, 6.0f, 8.0f};
  Matrix_t C_const = {2, 2, c_const};
  matrix_multiply_constant(&A_add, 2.0f, &C_const);
  print_result("Constant", check_matrix(&C_const, exp_const));

  // Determinant2
  float a_det2[4] = {4.0f, 6.0f, 3.0f, 8.0f};
  Matrix_t A_det2 = {2, 2, a_det2};
  float det2 = 0;
  matrix_determinant(&A_det2, &det2, NULL);
  print_result("Determinant2", check_float(det2, 14.0f));

  // Determinant3
  float a_det3[9] = {1.0f, 2.0f, 3.0f, 0.0f, 1.0f, 4.0f, 5.0f, 6.0f, 0.0f};
  Matrix_t A_det3 = {3, 3, a_det3};
  float det3 = 0;
  matrix_determinant(&A_det3, &det3, NULL);
  print_result("Determinant3", check_float(det3, 1.0f));

  // DeterminantN
  float a_detN[16] = {2.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 3.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 4.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 5.0f};
  float scratch_detN[16] = {0};
  Matrix_t A_detN = {4, 4, a_detN};
  Matrix_t Scratch_detN = {4, 4, scratch_detN};
  float detN = 0;
  matrix_determinant(&A_detN, &detN, &Scratch_detN);
  print_result("DeterminantN", check_float(detN, 120.0f));

  // Inverse2
  float a_inv2[4] = {4.0f, 3.0f, 3.0f, 2.0f};
  float res_inv2[4] = {0};
  float exp_inv2[4] = {-2.0f, 3.0f, 3.0f, -4.0f};
  Matrix_t A_inv2 = {2, 2, a_inv2};
  Matrix_t Res_inv2 = {2, 2, res_inv2};
  matrix_inverse(&A_inv2, &Res_inv2, NULL);
  print_result("Inverse2", check_matrix(&Res_inv2, exp_inv2));

  // Inverse3
  float a_inv3[9] = {1.0f, 2.0f, 3.0f, 0.0f, 1.0f, 4.0f, 5.0f, 6.0f, 0.0f};
  float res_inv3[9] = {0};
  float exp_inv3[9] = {-24.0f, 18.0f, 5.0f, 20.0f, -15.0f, -4.0f, -5.0f, 4.0f, 1.0f};
  Matrix_t A_inv3 = {3, 3, a_inv3};
  Matrix_t Res_inv3 = {3, 3, res_inv3};
  matrix_inverse(&A_inv3, &Res_inv3, NULL);
  print_result("Inverse3", check_matrix(&Res_inv3, exp_inv3));

  // InverseN
  float a_invN[16] = {2.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 4.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 5.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 10.0f};
  float res_invN[16] = {0};
  float scratch_invN[16] = {0};
  float exp_invN[16] = {0.5f, 0.0f, 0.0f, 0.0f,
                        0.0f, 0.25f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.2f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.1f};
  Matrix_t A_invN = {4, 4, a_invN};
  Matrix_t Res_invN = {4, 4, res_invN};
  Matrix_t Scratch_invN = {4, 4, scratch_invN};
  matrix_inverse(&A_invN, &Res_invN, &Scratch_invN);
  print_result("InverseN", check_matrix(&Res_invN, exp_invN));

  // Transpose
  float a_trans[6] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
  float res_trans[6] = {0};
  float exp_trans[6] = {1.0f, 4.0f, 2.0f, 5.0f, 3.0f, 6.0f};
  Matrix_t A_trans = {2, 3, a_trans};
  Matrix_t Res_trans = {3, 2, res_trans};
  matrix_transpose(&A_trans, &Res_trans);
  print_result("Transpose", check_matrix(&Res_trans, exp_trans));

  // TransposeSelf
  float a_tself[9] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  float exp_tself[9] = {1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  Matrix_t A_tself = {3, 3, a_tself};
  matrix_transpose_self(&A_tself);
  print_result("TransposeSelf", check_matrix(&A_tself, exp_tself));

  return 0;
}