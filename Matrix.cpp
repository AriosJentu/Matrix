#include <sstream>
#include <math.h>
#include "Matrix.h"

template <class TypeM>
int Matrix<TypeM>::getWidth() {
	return matrix->width;
}
template <class TypeM>
int Matrix<TypeM>::getHeight() {
	return matrix->height;
}

template <class TypeM>
void Matrix<TypeM>::transpose() {

	matrix->isTransposed = not matrix->isTransposed;

	/*int tmp = width;
	width = height;
	height = tmp;*/

	std::swap(matrix->width, matrix->height);
}

template <class TypeM>
int Matrix<TypeM>::index(int i, int j) {

	if (not matrix->isTransposed) {
		
		if (i >= matrix->sheight or j >= matrix->swidth or i < 0 or j < 0) {
			throw Exception("Wrong indexes");
			return -1;
		}

		return i*matrix->swidth + j;
	
	} else {

		if (i >= matrix->swidth or j >= matrix->sheight or i < 0 or j < 0) {
			throw Exception("Wrong indexes");
			return -1;
		}
		
		return j*matrix->swidth + i;
	}
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::cmul(Matrix mt, TypeM c) {

	//std::cout << "Width and Height: " << mt.width << " " << mt.height << std::endl;
	for (int i = 0; i < mt.matrix->height; i++) {
		for (int j = 0; j < mt.matrix->width; j++) {

			//std::cout << "Indexes M: " << i << " " << j << " : " << mt[i][j] << std::endl;
			mt[i][j] = mt[i][j] * c; 
		}
	}

	return mt;
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::cmul(TypeM c) {

	Matrix mt(matrix->sheight, matrix->swidth);	
	
	mt.matrix->arr = matrix->arr;
	mt.matrix->isTransposed = matrix->isTransposed;

	mt.matrix->width = matrix->width;
	mt.matrix->height = matrix->height;

	return Matrix::cmul(mt, c);
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::sum(Matrix mt1, Matrix mt2) {

	int w1, w2, h1, h2;

	w1 = mt1.matrix->width;
	w2 = mt2.matrix->width;
	
	h1 = mt1.matrix->height;
	h2 = mt2.matrix->height;


	if (not (w1 == w2) and not (h1 == h2)) {
		throw Exception("Can't sum matrixes with different size");
		return mt1;
	}

	for (int i = 0; i < h1; i++) {
		for (int j = 0; j < w1; j++) {
			//std::cout << "Indexes: " << i << " " << j << " : " << mt1[i][j] << " " << mt2[i][j] << std::endl;
			mt1[i][j] = mt1[i][j] + mt2[i][j];
		}
	}

	return mt1;

}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::sum(Matrix mt) {

	Matrix mt2(matrix->sheight, matrix->swidth);	
	mt2.matrix->arr = matrix->arr;
	mt2.matrix->isTransposed = matrix->isTransposed;

	mt2.matrix->width = matrix->width;
	mt2.matrix->height = matrix->height;

	return Matrix::sum(mt, mt2);
} 

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mul(Matrix mt1, Matrix mt2) {

	if (mt1.matrix->width != mt2.matrix->height) {
		throw Exception("Wrong Matrix Multiplication sizes");
	}

	int w = mt2.matrix->width;
	int h = mt1.matrix->height;
	int m = mt1.matrix->width;

	Matrix mt(h, w);

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			TypeM sum = 0;
			for (int k = 0; k < m; k++) {
				sum += mt1[i][k] * mt2[k][j];
			}

			mt[i][j] = sum;
		}
	}

	return mt;
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mul(Matrix mt2) {

	Matrix mt(matrix->sheight, matrix->swidth);	
	mt.matrix->arr = matrix->arr;
	mt.matrix->isTransposed = matrix->isTransposed;

	mt.matrix->width = matrix->width;
	mt.matrix->height = matrix->height;

	return Matrix::mul(mt, mt2);
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mget(Matrix mt, int startY, int startX, int nheight, int nwidth) {
	
	if ((startX + nwidth) <= 0 or 
		(startX + nwidth) > mt.matrix->width or 
		(startY + nheight) <= 0 or 
		(startY + nheight) > mt.matrix->height) 
	{

		throw Exception("Wrong matrix slice size");
		return mt;
	}

	Matrix mtn(nheight, nwidth);

	for (int i = startY; i < startY + nheight; i++) {
		for (int j = startX; j < startX + nwidth; j++) {

			mtn[i-startY][j-startX] = mt[i][j];
		}
	}

	return mtn;
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mget(int startY, int startX, int nheight, int nwidth) {

	Matrix mt(matrix->sheight, matrix->swidth);	
	mt.matrix->arr = matrix->arr;
	mt.matrix->isTransposed = matrix->isTransposed;

	mt.matrix->width = matrix->width;
	mt.matrix->height = matrix->height;

	return Matrix::mget(mt, startY, startX, nheight, nwidth);
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mset(Matrix mt1, Matrix mt2, int startY, int startX) {

	if ((startX + mt2.matrix->width) <= 0 or 
		(startX + mt2.matrix->width) > mt1.matrix->width or 
		(startY + mt2.matrix->height) <= 0 or
		(startY + mt2.matrix->height) > mt1.matrix->height)	
	{

		throw Exception("Can't put sliced matrix inside this matrix");
		return mt1;
	}

	for (int i = startY; i < startY + mt2.matrix->height; i++) {
		for (int j = startX; j < startX + mt2.matrix->width; j++) {
			mt1[i][j] = mt2[i-startY][j-startX];
		}
	}

	return mt1;

}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mset(Matrix mt2, int startY, int startX) {

	Matrix mt(matrix->sheight, matrix->swidth);	
	mt.matrix->arr = matrix->arr;
	mt.matrix->isTransposed = matrix->isTransposed;

	mt.matrix->width = matrix->width;
	mt.matrix->height = matrix->height;

	return Matrix::mset(mt, mt2, startY, startX);
}




template <class TypeM>
Matrix<TypeM>::Matrix(int mheight, int mwidth) {

	if (mheight < 0 or mwidth < 0) {
		throw Exception("Negative size");
	}

	matrix = new MatrixLink<TypeM>;

	matrix->sheight = mheight;
	matrix->swidth = mwidth;

	matrix->height = matrix->sheight;
	matrix->width = matrix->swidth;

	matrix->arr = new TypeM[matrix->height*matrix->width];

}

template <class TypeM>
Matrix<TypeM> operator+(Matrix<TypeM>& mt1, Matrix<TypeM>& mt2) {
	return mt1.sum(mt2);
}

template <class TypeM>
Matrix<TypeM> operator-(Matrix<TypeM>& mt1, Matrix<TypeM>& mt2) {
	return mt1.sum(mt2.cmul(-1));
}

template <class TypeM>
Matrix<TypeM> operator*(TypeM c, Matrix<TypeM>& mt) {
	return mt.cmul(c);
}

template <class TypeM>
Matrix<TypeM> operator*(Matrix<TypeM>& mt, TypeM c) {
	return mt.cmul(c);
}

template <class TypeM>
Matrix<TypeM> operator*(Matrix<TypeM>& mt1, Matrix<TypeM>& mt2) {
	return mt1.mul(mt2);
}












int main() {

	//test1
	Matrix<float> mt(3, 6);

	for (int i = 0; i < mt.getHeight(); i++) {
		for (int j = 0; j < mt.getWidth(); j++) {
			mt[i][j] = i*mt.getWidth() + j + 1;
		}
	}

	mt = mt.cmul(3.141);

	for (int i = 0; i < mt.getHeight(); i++) {
		for (int j = 0; j < mt.getWidth(); j++) {
			std::cout << mt[i][j] << " ";
		}
		std::cout << std::endl;
	}

	mt.transpose();
	std::cout << std::endl;

	for (int i = 0; i < mt.getHeight(); i++) {
		for (int j = 0; j < mt.getWidth(); j++) {
			std::cout << mt[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	



	//test2 

	Matrix<int> mt1(2, 3);
	Matrix<int> mt2(3, 2);

	for (int i = 0; i < mt1.getHeight(); i++) {
		for (int j = 0; j < mt1.getWidth(); j++) {
			mt1[i][j] = i*mt1.getWidth() + j + 1;
		}
	}


	for (int i = 0; i < mt2.getHeight(); i++) {
		for (int j = 0; j < mt2.getWidth(); j++) {
			mt2[i][j] = (i*mt2.getWidth() + j + 1) * (i*mt2.getWidth() + j + 1);
		}
	}

	using namespace std;

	mt2.transpose();
	
	mt2.cmul(-1);
	cout << endl;
	
	for (int i = 0; i < mt1.getHeight(); i++) {
		for (int j = 0; j < mt1.getWidth(); j++) {
			cout << mt1[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;
	for (int i = 0; i < mt2.getHeight(); i++) {
		for (int j = 0; j < mt2.getWidth(); j++) {
			cout << mt2[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;
	Matrix<int> mt3 = mt1 - mt2;

	for (int i = 0; i < mt3.getHeight(); i++) {
		for (int j = 0; j < mt3.getWidth(); j++) {
			cout << mt3[i][j] << " ";
		}
		cout << endl;
	}



	//test3

	Matrix<float> mtmd(2, 3);

	for (int i = 0; i < mtmd.getHeight(); i++) {
		for (int j = 0; j < mtmd.getWidth(); j++) {
			mtmd[i][j] = i*mtmd.getWidth() + j + 1;
		}
	}

	mtmd = mtmd*((float) 2);
	using namespace std;

	for (int i = 0; i < mtmd.getHeight(); i++) {
		for (int j = 0; j < mtmd.getWidth(); j++) {
			cout << mtmd[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;

	mtmd = ((float) -0.5)*mtmd;
	mtmd.transpose();
	mtmd = ((float) -1)*mtmd;

	for (int i = 0; i < mtmd.getHeight(); i++) {
		for (int j = 0; j < mtmd.getWidth(); j++) {
			cout << mtmd[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;
	cout << mtmd[0][1]*5 << endl; 


	//test4

	Matrix<int> mtd1(2, 3);
	Matrix<int> mtd2(3, 2);

	mtd1[0][0] = 1;
	mtd1[0][1] = 2;
	mtd1[0][2] = -3;
	mtd1[1][0] = -1;
	mtd1[1][1] = 3;
	mtd1[1][2] = 5;

	mtd2[0][0] = 2;
	mtd2[0][1] = 3;
	mtd2[1][0] = -1;
	mtd2[1][1] = 3;
	mtd2[2][0] = 3;
	mtd2[2][1] = -4;

	for (int i = 0; i < mtd1.getHeight(); i++) {
		for (int j = 0; j < mtd1.getWidth(); j++) {
			std::cout << mtd1[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (int i = 0; i < mtd2.getHeight(); i++) {
		for (int j = 0; j < mtd2.getWidth(); j++) {
			std::cout << mtd2[i][j] << " ";
		}
		std::cout << std::endl;
	}


	Matrix<int> mtd3 = mtd1 * mtd2;
	mtd3 = mtd3*mtd3;
	std::cout << std::endl;

	for (int i = 0; i < mtd3.getHeight(); i++) {
		for (int j = 0; j < mtd3.getWidth(); j++) {
			std::cout << mtd3[i][j] << " ";
		}
		std::cout << std::endl;
	}

	Matrix<int> mtxn(4, 4);

	for (int i = 0; i < mtxn.getHeight(); i++) {
		for (int j = 0; j < mtxn.getWidth(); j++) {
			mtxn[i][j] = i*mtxn.getWidth() + j + 1;
		}
	}

	for (int i = 0; i < mtxn.getHeight(); i++) {
		for (int j = 0; j < mtxn.getWidth(); j++) {
			std::cout << mtxn[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

	Matrix<int> slice = mtxn.mget(1, 1, 2, 2);
	for (int i = 0; i < slice.getHeight(); i++) {
		for (int j = 0; j < slice.getWidth(); j++) {
			std::cout << slice[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
	for (int i = 0; i < slice.getHeight(); i++) {
		for (int j = 0; j < slice.getWidth(); j++) {
			slice[i][j] = -1;
		}
	}

	mtxn = mtxn.mset(slice, 1, 1);
	for (int i = 0; i < mtxn.getHeight(); i++) {
		for (int j = 0; j < mtxn.getWidth(); j++) {
			std::cout << mtxn[i][j] << " ";
		}
		std::cout << std::endl;
	}

}