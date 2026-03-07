#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "../include/operation_state.h"
#include "../include/matrix_math.h"

#define VECTOR_MATH_IMPLEMENTATION
#include "../include/vector_math.h"

// Assertion macros
#define ASSERT_TRUE(condition)                             \
  if (!(condition))                                        \
  {                                                        \
    printf("[FAIL]: %s at line %d\n", #condition, __LINE__); \
    return false;                                          \
  }

#define ASSERT_FLOAT_EQ(expected, actual)                                                           \
  if (fabs((expected) - (actual)) > 1e-5)                                                           \
  {                                                                                                 \
    printf("[FAIL]: Expected %f, got %f at line %d\n", (float)(expected), (float)(actual), __LINE__); \
    return false;                                                                                   \
  }

#define ASSERT_STATUS_EQ(expected, actual)                                                   \
  if ((expected) != (actual))                                                                \
  {                                                                                          \
    printf("[FAIL]: Expected status %d, got %d at line %d\n", (expected), (actual), __LINE__); \
    return false;                                                                            \
  }

// ---------------------------------------------------------
// Test Cases
// ---------------------------------------------------------

bool test_vector_add()
{
  float d1[] = {1.0f, 2.0f, 3.0f};
  float d2[] = {0.5f, 1.5f, 2.5f};
  float res[3] = {0};

  Vector_t v1 = {3, d1};
  Vector_t v2 = {3, d2};
  Vector_t vRes = {3, res};

  // Success path
  ASSERT_STATUS_EQ(ok, vector_add(&v1, &v2, &vRes));
  ASSERT_FLOAT_EQ(1.5f, vRes.data[0]);
  ASSERT_FLOAT_EQ(3.5f, vRes.data[1]);
  ASSERT_FLOAT_EQ(5.5f, vRes.data[2]);

  // Error path: wrong dimensions
  Vector_t vBad = {2, d1};
  ASSERT_STATUS_EQ(vector_wrong_dimension, vector_add(&v1, &vBad, &vRes));
  ASSERT_STATUS_EQ(vector_wrong_dimension, vector_add(&v1, &v2, &vBad));

  return true;
}

bool test_vector_subtract()
{
  float d1[] = {5.0f, 5.0f, 5.0f};
  float d2[] = {1.0f, 2.0f, 3.0f};
  float res[3] = {0};

  Vector_t v1 = {3, d1};
  Vector_t v2 = {3, d2};
  Vector_t vRes = {3, res};

  // Success path
  ASSERT_STATUS_EQ(ok, vector_subtract(&v1, &v2, &vRes));
  ASSERT_FLOAT_EQ(4.0f, vRes.data[0]);
  ASSERT_FLOAT_EQ(3.0f, vRes.data[1]);
  ASSERT_FLOAT_EQ(2.0f, vRes.data[2]);

  // Error path: wrong dimensions
  Vector_t vBad = {2, d1};
  ASSERT_STATUS_EQ(vector_wrong_dimension, vector_subtract(&v1, &vBad, &vRes));

  return true;
}

bool test_vector_multiply_constant()
{
  float d[] = {1.0f, -2.0f, 3.0f};
  float res[3] = {0};

  Vector_t v = {3, d};
  Vector_t vRes = {3, res};

  // Success path
  ASSERT_STATUS_EQ(ok, vector_multiply_constant(&v, 2.5f, &vRes));
  ASSERT_FLOAT_EQ(2.5f, vRes.data[0]);
  ASSERT_FLOAT_EQ(-5.0f, vRes.data[1]);
  ASSERT_FLOAT_EQ(7.5f, vRes.data[2]);

  // Error path: wrong dimensions
  Vector_t vBad = {2, res};
  ASSERT_STATUS_EQ(vector_wrong_dimension, vector_multiply_constant(&v, 2.5f, &vBad));

  return true;
}

bool test_vector_dot_product()
{
  float res_val = 0.0f;

  // Test fast path: length 2
  float d2_1[] = {1.0f, 2.0f};
  float d2_2[] = {3.0f, 4.0f};
  Vector_t v2_1 = {2, d2_1};
  Vector_t v2_2 = {2, d2_2};
  ASSERT_STATUS_EQ(ok, vector_dot_product(&v2_1, &v2_2, &res_val));
  ASSERT_FLOAT_EQ(11.0f, res_val); // 3 + 8 = 11

  // Test fast path: length 3
  float d3_1[] = {1.0f, 2.0f, 3.0f};
  float d3_2[] = {2.0f, 2.0f, 2.0f};
  Vector_t v3_1 = {3, d3_1};
  Vector_t v3_2 = {3, d3_2};
  ASSERT_STATUS_EQ(ok, vector_dot_product(&v3_1, &v3_2, &res_val));
  ASSERT_FLOAT_EQ(12.0f, res_val); // 2 + 4 + 6 = 12

  // Test general path with unrolling: length 5
  float d5_1[] = {1.0f, 1.0f, 1.0f, 1.0f, 2.0f};
  float d5_2[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
  Vector_t v5_1 = {5, d5_1};
  Vector_t v5_2 = {5, d5_2};
  ASSERT_STATUS_EQ(ok, vector_dot_product(&v5_1, &v5_2, &res_val));
  ASSERT_FLOAT_EQ(20.0f, res_val); // 1 + 2 + 3 + 4 + 10 = 20

  // Error path: wrong dimensions
  ASSERT_STATUS_EQ(vector_wrong_dimension, vector_dot_product(&v2_1, &v3_1, &res_val));

  return true;
}

bool test_vector_outer_product()
{
  float d_u[] = {1.0f, 2.0f};       // length 2
  float d_v[] = {3.0f, 4.0f, 5.0f}; // length 3
  Vector_t u = {2, d_u};
  Vector_t v = {3, d_v};

  float m_data[6] = {0};
  Matrix_t C = {2, 3, m_data}; // Expected: 2 rows, 3 cols

  // Note: This test may fail with your current implementation due to the rows/cols swap bug.
  // It enforces the mathematically correct dimension check.
  ASSERT_STATUS_EQ(ok, vector_outer_product(&u, &v, &C));
  ASSERT_FLOAT_EQ(3.0f, C.data[0]);  // u[0]*v[0]
  ASSERT_FLOAT_EQ(4.0f, C.data[1]);  // u[0]*v[1]
  ASSERT_FLOAT_EQ(5.0f, C.data[2]);  // u[0]*v[2]
  ASSERT_FLOAT_EQ(6.0f, C.data[3]);  // u[1]*v[0]
  ASSERT_FLOAT_EQ(8.0f, C.data[4]);  // u[1]*v[1]
  ASSERT_FLOAT_EQ(10.0f, C.data[5]); // u[1]*v[2]

  // Error path: null pointer
  ASSERT_STATUS_EQ(matrix_null, vector_outer_product(&u, &v, NULL));

  // Error path: wrong dimensions
  Matrix_t C_bad = {3, 2, m_data};
  ASSERT_STATUS_EQ(matrix_wrong_dimensions, vector_outer_product(&u, &v, &C_bad));

  return true;
}

bool test_matrix_multiply_vector()
{
  float m_data[] = {
      1.0f, 2.0f, 3.0f,
      4.0f, 5.0f, 6.0f};
  Matrix_t M = {2, 3, m_data};

  float v_data[] = {1.0f, 1.0f, 1.0f};
  Vector_t V = {3, v_data};

  float res_data[2] = {0};
  Vector_t Res = {2, res_data};

  // Success path
  ASSERT_STATUS_EQ(ok, matrix_multiply_vector(&M, &V, &Res));
  ASSERT_FLOAT_EQ(6.0f, Res.data[0]);
  ASSERT_FLOAT_EQ(15.0f, Res.data[1]);

  // Error path: wrong dimensions
  Vector_t V_bad = {2, res_data};
  ASSERT_STATUS_EQ(matrix_wrong_dimensions, matrix_multiply_vector(&M, &V_bad, &Res));

  return true;
}

bool test_vector_multiply_matrix()
{
  float v_data[] = {1.0f, 2.0f};
  Vector_t V = {2, v_data};

  float m_data[] = {
      1.0f, 2.0f, 3.0f,
      4.0f, 5.0f, 6.0f};
  Matrix_t M = {2, 3, m_data};

  float res_data[3] = {0};
  Vector_t Res = {3, res_data};

  // Success path
  ASSERT_STATUS_EQ(ok, vector_multiply_matrix(&V, &M, &Res));
  ASSERT_FLOAT_EQ(9.0f, Res.data[0]);  // 1*1 + 2*4
  ASSERT_FLOAT_EQ(12.0f, Res.data[1]); // 1*2 + 2*5
  ASSERT_FLOAT_EQ(15.0f, Res.data[2]); // 1*3 + 2*6

  // Error path: wrong dimensions
  Vector_t Res_bad = {2, res_data};
  ASSERT_STATUS_EQ(matrix_wrong_dimensions, vector_multiply_matrix(&V, &M, &Res_bad));

  return true;
}

bool test_vector_matrix_conversions()
{
  float v_data[] = {1.0f, 2.0f, 3.0f};
  Vector_t v = {3, v_data};

  float m_data_col[3] = {0};
  float m_data_row[3] = {0};
  float m_data_bad[4] = {0};
  Matrix_t M_col = {3, 1, m_data_col};
  Matrix_t M_row = {1, 3, m_data_row}; // Valid row matrix
  Matrix_t M_bad = {3, 2, m_data_bad}; // Invalid matrix shape

  // Test vector_to_matrix success
  ASSERT_STATUS_EQ(ok, vector_to_matrix(&v, &M_col));
  ASSERT_FLOAT_EQ(1.0f, M_col.data[0]);
  ASSERT_FLOAT_EQ(3.0f, M_col.data[2]);

  // Test vector_to_matrix error (must target Nx1)
  ASSERT_STATUS_EQ(matrix_wrong_dimensions, vector_to_matrix(&v, &M_row));

  // Prepare data for matrix_to_vector tests
  M_col.data[1] = 5.0f;
  M_row.data[1] = 7.0f;

  // Test matrix_to_vector success (from column matrix)
  ASSERT_STATUS_EQ(ok, matrix_to_vector(&M_col, &v));
  ASSERT_FLOAT_EQ(5.0f, v.data[1]);

  // Test matrix_to_vector success (from row matrix)
  ASSERT_STATUS_EQ(ok, matrix_to_vector(&M_row, &v));
  ASSERT_FLOAT_EQ(7.0f, v.data[1]);

  // Test matrix_to_vector error (invalid shape or size)
  ASSERT_STATUS_EQ(vector_wrong_dimension, matrix_to_vector(&M_bad, &v));

  return true;
}

// ---------------------------------------------------------
// Main Runner
// ---------------------------------------------------------

int main()
{
  printf("Running vector_math tests...\n");

  int passed = 0;
  int total = 0;

#define RUN_TEST(test)             \
  do                               \
  {                                \
    total++;                       \
    if (test())                    \
    {                              \
      passed++;                    \
      printf("[PASS]: %s\n", #test); \
    }                              \
    else                           \
    {                              \
      printf("[FAIL]: %s\n", #test); \
    }                              \
  } while (0)

  RUN_TEST(test_vector_add);
  RUN_TEST(test_vector_subtract);
  RUN_TEST(test_vector_multiply_constant);
  RUN_TEST(test_vector_dot_product);
  RUN_TEST(test_vector_outer_product);
  RUN_TEST(test_matrix_multiply_vector);
  RUN_TEST(test_vector_multiply_matrix);
  RUN_TEST(test_vector_matrix_conversions);

  printf("--------------------------------------------------\n");
  printf("Test Results: %d / %d passed.\n", passed, total);

  if (passed == total)
  {
    printf("Coverage target met successfully.\n");
    return 0;
  }
  else
  {
    printf("Coverage or validation failure detected.\n");
    return 1;
  }
}