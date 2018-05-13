#include <sstream>
#include <math.h>
#include "Matrix.h"

template <class TypeM>
void Matrix<TypeM>::transpose() {

	isTransposed = not isTransposed;

	int tmp = width;
	width = height;
	height = tmp;
}

template <class TypeM>
int Matrix<TypeM>::index(int i, int j) {

	if (not isTransposed) {
		
		if (i >= sheight or j >= swidth or i < 0 or j < 0) {
			throw Exception("Wrong indexes");
			return -1;
		}

		return i*swidth + j;
	
	} else {

		if (i >= swidth or j >= sheight or i < 0 or j < 0) {
			throw Exception("Wrong indexes");
			return -1;
		}
		
		return j*swidth + i;
	}
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::cmul(Matrix mt, TypeM c) {

	//std::cout << "Width and Height: " << mt.width << " " << mt.height << std::endl;
	for (int i = 0; i < mt.height; i++) {
		for (int j = 0; j < mt.width; j++) {

			//std::cout << "Indexes M: " << i << " " << j << " : " << mt[i][j] << std::endl;
			mt[mt.index(i, j)] = mt[i][j] * c; 
		}
	}

	return mt;
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::cmul(TypeM c) {

	Matrix mt(sheight, swidth);	
	
	mt.arr = arr;
	mt.isTransposed = isTransposed;

	mt.width = width;
	mt.height = height;

	return Matrix::cmul(mt, c);
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::sum(Matrix mt1, Matrix mt2) {

	int w1, w2, h1, h2;

	w1 = mt1.width;
	w2 = mt2.width;
	
	h1 = mt1.height;
	h2 = mt2.height;


	if (not (w1 == w2) and not (h1 == h2)) {
		throw Exception("Can't sum matrixes with different size");
		return mt1;
	}

	for (int i = 0; i < h1; i++) {
		for (int j = 0; j < w1; j++) {
			//std::cout << "Indexes: " << i << " " << j << " : " << mt1[i][j] << " " << mt2[i][j] << std::endl;
			mt1[mt1.index(i, j)] = mt1[i][j] + mt2[i][j];
		}
	}

	return mt1;

}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::sum(Matrix mt) {

	Matrix mt2(sheight, swidth);	
	mt2.arr = arr;
	mt2.isTransposed = isTransposed;

	mt2.width = width;
	mt2.height = height;

	return Matrix::sum(mt, mt2);
} 

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mul(Matrix mt1, Matrix mt2) {

	if (mt1.width != mt2.height) {
		throw Exception("Wrong Matrix Multiplication sizes");
	}

	int w = mt2.width;
	int h = mt1.height;
	int m = mt1.width;

	Matrix mt(h, w);

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			TypeM sum = 0;
			for (int k = 0; k < m; k++) {
				sum += mt1[i][k] * mt2[k][j];
			}

			mt[mt.index(i, j)] = sum;
		}
	}

	return mt;
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mul(Matrix mt2) {

	Matrix mt(sheight, swidth);	
	mt.arr = arr;
	mt.isTransposed = isTransposed;

	mt.width = width;
	mt.height = height;

	return Matrix::mul(mt, mt2);
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mget(Matrix mt, int startY, int startX, int nheight, int nwidth) {
	
	if ((startX + nwidth) <= 0 or 
		(startX + nwidth) > mt.width or 
		(startY + nheight) <= 0 or 
		(startY + nheight) > mt.height) 
	{

		throw Exception("Wrong matrix slice size");
		return mt;
	}

	Matrix mtn(nheight, nwidth);

	for (int i = startY; i < startY + nheight; i++) {
		for (int j = startX; j < startX + nwidth; j++) {

			mtn[mtn.index(i-startY, j-startX)] = mt[i][j];
		}
	}

	return mtn;
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mget(int startY, int startX, int nheight, int nwidth) {

	Matrix mt(sheight, swidth);	
	mt.arr = arr;
	mt.isTransposed = isTransposed;

	mt.width = width;
	mt.height = height;

	return Matrix::mget(mt, startY, startX, nheight, nwidth);
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mset(Matrix mt1, Matrix mt2, int startY, int startX) {

	if ((startX + mt2.width) <= 0 or 
		(startX + mt2.width) > mt1.width or 
		(startY + mt2.height) <= 0 or
		(startY + mt2.height) > mt1.height)	
	{

		throw Exception("Can't put sliced matrix inside this matrix");
		return mt1;
	}

	for (int i = startY; i < startY + mt2.height; i++) {
		for (int j = startX; j < startX + mt2.width; j++) {
			mt1[mt1.index(i, j)] = mt2[i-startY][j-startX];
		}
	}

	return mt1;

}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mset(Matrix mt2, int startY, int startX) {

	Matrix mt(sheight, swidth);	
	mt.arr = arr;
	mt.isTransposed = isTransposed;

	mt.width = width;
	mt.height = height;

	return Matrix::mset(mt, mt2, startY, startX);
}




template <class TypeM>
Matrix<TypeM>::Matrix(int mheight, int mwidth) {

	if (mheight < 0 or mwidth < 0) {
		throw Exception("Negative size");
	}

	sheight = mheight;
	swidth = mwidth;

	height = sheight;
	width = swidth;

	arr = new TypeM[height*width];

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

	/*Matrix<float> mt(3, 6);

	for (int i = 0; i < mt.height; i++) {
		for (int j = 0; j < mt.width; j++) {
			mt[mt.index(i, j)] = i*mt.width + j + 1;
		}
	}

	mt = mt.cmul(3.141);

	for (int i = 0; i < mt.height; i++) {
		for (int j = 0; j < mt.width; j++) {
			std::cout << mt[i][j] << " ";
		}
		std::cout << std::endl;
	}

	mt.transpose();
	std::cout << std::endl;

	for (int i = 0; i < mt.width; i++) {
		for (int j = 0; j < mt.height; j++) {
			std::cout << mt[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	*/




	/*Matrix<int> mt1(2, 3);
	Matrix<int> mt2(3, 2);

	for (int i = 0; i < mt1.height; i++) {
		for (int j = 0; j < mt1.width; j++) {
			mt1[mt1.index(i, j)] = i*mt1.width + j + 1;
		}
	}


	for (int i = 0; i < mt2.height; i++) {
		for (int j = 0; j < mt2.width; j++) {
			mt2[mt2.index(i, j)] = (i*mt2.width + j + 1) * (i*mt2.width + j + 1);
		}
	}

	using namespace std;

	mt2.transpose();
	
	mt2.cmul(-1);
	cout << endl;
	
	for (int i = 0; i < mt1.height; i++) {
		for (int j = 0; j < mt1.width; j++) {
			cout << mt1[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;
	for (int i = 0; i < mt2.height; i++) {
		for (int j = 0; j < mt2.width; j++) {
			cout << mt2[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;
	Matrix<int> mt3 = mt1 - mt2;

	for (int i = 0; i < mt3.height; i++) {
		for (int j = 0; j < mt3.width; j++) {
			cout << mt3[i][j] << " ";
		}
		cout << endl;
	}*/




	/*Matrix<float> mt(2, 3);

	for (int i = 0; i < mt.height; i++) {
		for (int j = 0; j < mt.width; j++) {
			mt[mt.index(i, j)] = i*mt.width + j + 1;
		}
	}

	mt = mt*((float) 2);
	using namespace std;

	for (int i = 0; i < mt.height; i++) {
		for (int j = 0; j < mt.width; j++) {
			cout << mt[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;

	mt = ((float) -0.5)*mt;
	mt.transpose();
	mt = ((float) -1)*mt;

	for (int i = 0; i < mt.height; i++) {
		for (int j = 0; j < mt.width; j++) {
			cout << mt[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;
	cout << mt[0][1]*5 << endl; */

	/*Matrix<int> mt1(2, 3);
	Matrix<int> mt2(3, 2);

	mt1[mt1.index(0, 0)] = 1;
	mt1[mt1.index(0, 1)] = 2;
	mt1[mt1.index(0, 2)] = -3;
	mt1[mt1.index(1, 0)] = -1;
	mt1[mt1.index(1, 1)] = 3;
	mt1[mt1.index(1, 2)] = 5;

	mt2[mt2.index(0, 0)] = 2;
	mt2[mt2.index(0, 1)] = 3;
	mt2[mt2.index(1, 0)] = -1;
	mt2[mt2.index(1, 1)] = 3;
	mt2[mt2.index(2, 0)] = 3;
	mt2[mt2.index(2, 1)] = -4;

	for (int i = 0; i < mt1.height; i++) {
		for (int j = 0; j < mt1.width; j++) {
			std::cout << mt1[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (int i = 0; i < mt2.height; i++) {
		for (int j = 0; j < mt2.width; j++) {
			std::cout << mt2[i][j] << " ";
		}
		std::cout << std::endl;
	}


	Matrix<int> mt3 = mt1 * mt2;
	mt3 = mt3*mt3;
	std::cout << std::endl;

	for (int i = 0; i < mt3.height; i++) {
		for (int j = 0; j < mt3.width; j++) {
			std::cout << mt3[i][j] << " ";
		}
		std::cout << std::endl;
	}*/

	Matrix<int> mt(4, 4);

	for (int i = 0; i < mt.height; i++) {
		for (int j = 0; j < mt.width; j++) {
			mt[mt.index(i, j)] = i*mt.width + j + 1;
		}
	}

	for (int i = 0; i < mt.height; i++) {
		for (int j = 0; j < mt.width; j++) {
			std::cout << mt[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

	Matrix<int> slice = mt.mget(1, 1, 2, 2);
	for (int i = 0; i < slice.height; i++) {
		for (int j = 0; j < slice.width; j++) {
			std::cout << slice[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
	for (int i = 0; i < slice.height; i++) {
		for (int j = 0; j < slice.width; j++) {
			slice[slice.index(i, j)] = -1;
		}
	}

	mt = mt.mset(slice, 1, 1);
	for (int i = 0; i < mt.height; i++) {
		for (int j = 0; j < mt.width; j++) {
			std::cout << mt[i][j] << " ";
		}
		std::cout << std::endl;
	}
}