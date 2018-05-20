#include "Matrix.cpp"
#include <gtest/gtest.h>

TEST(Test1, TransposeValuesChecker) {
	
	Matrix<int> matrix(2, 3);

	for (int i = 0; i < matrix.getHeight(); i++) {
		for (int j = 0; j < matrix.getWidth(); j++) {

			matrix[i][j] = i*matrix.getWidth() + j;
		}
	}

	Matrix<int> matrix2(3, 2);
	for (int i = 0; i < matrix2.getHeight(); i++) {
		for (int j = 0; j < matrix2.getWidth(); j++) {
			matrix2[i][j] = j*matrix2.getHeight() + i;
		}
	}

	matrix2.transpose();
	ASSERT_EQ(matrix == matrix2, true);

}

TEST(Test2, ExceptionWrongSize) {

	ASSERT_THROW(Matrix<int>(-2, -3), Matrix<int>::Exception);
}

TEST(Test3, ExceptionWrongIndexes) {

	Matrix<int> m(2, 2);
	m[1][1] = 5;

	ASSERT_EQ(m[0][0], 0);
	
	ASSERT_EQ(m[1][1], 5);

	ASSERT_THROW(m[2][2], Matrix<int>::Exception);
}

TEST(Test4, SumAndScalarMultiplication) {

	Matrix<int> diagOne(3, 3);
	for (int i = 0; i < diagOne.getHeight(); i++) {
		diagOne[i][i] = 1;
	}

	Matrix<int> diagTwo(3, 3);
	for (int i = 0; i < diagTwo.getHeight(); i++) {
		diagTwo[i][i] = -2;
	}	

	Matrix<int> sum = diagTwo + diagOne;
	Matrix<int> mul = diagOne * (-1);

	ASSERT_EQ(5, 5);

}

TEST(Test5, SumException) {

	Matrix<int> matrix1(3, 3);
	Matrix<int> matrix2(3, 2);

	ASSERT_THROW(matrix1 + matrix2, Matrix<int>::Exception);
}

TEST(Test6, MultiplicateMatrixes) {

	Matrix<int> matrix1(2, 3);
	Matrix<int> matrix2(3, 2);

	matrix1[0][0] = 5;
	matrix1[0][1] = -2;
	matrix1[0][2] = 6;
	matrix1[1][0] = -3;
	matrix1[1][1] = 4;
	matrix1[1][2] = -1;

	matrix2[0][0] = 3;
	matrix2[0][1] = 4;
	matrix2[1][0] = -2;
	matrix2[1][1] = 4;
	matrix2[2][0] = -1;
	matrix2[2][1] = 6;

	Matrix<int> matrixMul1to2(2, 2);
	Matrix<int> matrixMul2to1(3, 3);

	matrixMul1to2[0][0] = 13;
	matrixMul1to2[0][1] = 48;
	matrixMul1to2[1][0] = -16;
	matrixMul1to2[1][1] = -2;

	matrixMul2to1[0][0] = 3;
	matrixMul2to1[0][1] = 10;
	matrixMul2to1[0][2] = 14;
	matrixMul2to1[1][0] = -22;
	matrixMul2to1[1][1] = 20;
	matrixMul2to1[1][2] = -16;
	matrixMul2to1[2][0] = -23;
	matrixMul2to1[2][1] = 26;
	matrixMul2to1[2][2] = -12;

	ASSERT_EQ(matrix1*matrix2, matrixMul1to2);
	ASSERT_EQ(matrix2*matrix1, matrixMul2to1);
}

TEST(Test7, TransposeMatrixMultiplicationAndMulException) {

	Matrix<int> matrix1(1, 2);
	Matrix<int> matrix2(1, 2);

	matrix1[0][0] = 1;
	matrix1[0][1] = -5;

	matrix2[0][0] = -3;
	matrix2[0][1] = 5;

	ASSERT_THROW(matrix1*matrix2, Matrix<int>::Exception);

	matrix2.transpose();

	Matrix<int> mul = matrix1*matrix2;
	Matrix<int> res(1, 1);
	res[0][0] = -28;

	ASSERT_EQ(mul == res, true);

}

TEST(Test8, SliceGetter) {
	Matrix<int> matrix(5, 5);
	for (int i = 0; i < matrix.getHeight(); i++) {
		for (int j = 0; j < matrix.getWidth(); j++) {
			matrix[i][j] = i*matrix.getWidth() + j + 1;
		}
	}

	Matrix<int> slice = matrix.mget(2, 2, 2, 2);
	Matrix<int> res(2, 2);

	for (int i = 0; i < res.getHeight(); i++) {
		for (int j = 0; j < res.getWidth(); j++) {
			res[i][j] = (i+2)*matrix.getWidth() + (j+2) + 1;
		}
	}

	ASSERT_EQ(res == slice, true);
}

TEST(Test9, SliceGetterException) {

	Matrix<int> matrix(5, 5);

	ASSERT_EQ(matrix.mget(0, 0, 5, 5) == matrix, true);
	ASSERT_THROW(matrix.mget(1, 1, 5, 5), Matrix<int>::Exception);

}

TEST(Test10, SliceSetter) {

	Matrix<int> matrix(5, 5);
	Matrix<int> slice(2, 2);

	for (int i = 0; i < slice.getHeight(); i++) {
		for (int j = 0; j < slice.getWidth(); j++) {
			slice[i][j] = 1;
		}
	}

	Matrix<int> setted = matrix.mset(slice, 0, 0);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			matrix[i][j] = -1;
		}
	}

	matrix = (-1)*matrix;

	ASSERT_EQ(matrix == setted, true);

}

TEST(Test11, SliceSetterException) {

	Matrix<int> matrix(5, 5);
	Matrix<int> slice(5, 5);

	ASSERT_EQ(matrix.mset(slice, 0, 0) == matrix, true);
	ASSERT_THROW(matrix.mset(slice, 1, 1), Matrix<int>::Exception);
}

TEST(Test12, SlicesEditor) {

	Matrix<int> matrix(2, 2);
	Slice<int> slice(&matrix, 0, 0, 2, 2, 1);
	
	slice[0][0] = -1;

	ASSERT_EQ(matrix[0][0] == -1, true);

	slice[0][0] = slice[0][0]*(-10);
	ASSERT_EQ(matrix[0][0] == 10, true);

}

TEST(Test13, SlicesToMatrix) {

	Matrix<int> matrix(3, 3);
	for (int i = 0; i < matrix.getHeight(); i++) {
		for (int j = 0; j < matrix.getWidth(); j++) {
			matrix[i][j] = i*matrix.getWidth() + j + 1;
		}
	}

	Slice<int> slice(&matrix, 0, 0, 2, 2, 2);

	Matrix<int> mtx = slice.getAsMatrix();
	Matrix<int> res(2, 2);
	res[0][0] = 1;
	res[0][1] = 3;
	res[1][0] = 7;
	res[1][1] = 9;

	ASSERT_EQ(mtx == res, true);
}

TEST(Test14, SlicesExceptions) {

	Matrix<int> mtx(3, 3);

	ASSERT_THROW(Slice<int>(&mtx, 0, 0, 5, 5, 1), Slice<int>::Exception);
	ASSERT_THROW(Slice<int>(&mtx, 0, 0, 3, 3, 0), Slice<int>::Exception);

}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
