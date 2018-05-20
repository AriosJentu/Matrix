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
	std::swap(matrix->width, matrix->height);
}

template <class TypeM>
int Matrix<TypeM>::index(int i, int j) {

	// std::cout << "Debug: " << i << " " << j << " : " << matrix << " " << matrix->sheight << " " << matrix->swidth << std::endl;
	if (not matrix->isTransposed) {
		
		if (i >= matrix->sheight or j >= matrix->swidth or i < 0 or j < 0) {
			// std::cout << "NOT TRANSPOSED " << (i >= matrix->sheight) << " " << (j >= matrix->swidth) << " : " << i << " " << j << " " << matrix->sheight << " " << matrix->swidth << " ENDL" << std::endl;
			throw Exception("Wrong indexes ");
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


	if (not (w1 == w2) or not (h1 == h2)) {
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
Matrix<TypeM> Matrix<TypeM>::mget(Matrix mt, int startY, int startX, int nheight, int nwidth, int step) {
	
	if ((startX + (nwidth-1)*step) <= 0 or 
		(startX + (nwidth-1)*step) > mt.matrix->width or 
		(startY + (nheight-1)*step) <= 0 or 
		(startY + (nheight-1)*step) > mt.matrix->height

		or (step <= 0) )
	{

		throw Exception("Wrong matrix slice size");
		return mt;
	}

	Matrix mtn(nheight, nwidth);

	for (int i = startY; i < startY + nheight*step; i += step) {
		for (int j = startX; j < startX + nwidth*step; j += step) {

			mtn[(i-startY)/step][(j-startX)/step] = mt[i][j];
		}
	}

	return mtn;
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mget(int startY, int startX, int nheight, int nwidth, int step) {

	Matrix mt(matrix->sheight, matrix->swidth);	
	mt.matrix->arr = matrix->arr;
	mt.matrix->isTransposed = matrix->isTransposed;

	mt.matrix->width = matrix->width;
	mt.matrix->height = matrix->height;

	return Matrix::mget(mt, startY, startX, nheight, nwidth, step);
}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mset(Matrix mt1, Matrix mt2, int startY, int startX, int step) {

	if (
		step <= 0 or
		(startX + (mt2.matrix->width-1)*step) <= 0 or 
		(startX + (mt2.matrix->width-1)*step) > mt1.matrix->width or 
		(startY + (mt2.matrix->height-1)*step) <= 0 or
		(startY + (mt2.matrix->height-1)*step) > mt1.matrix->height)	
	{

		throw Exception("Can't put sliced matrix inside this matrix");
		return mt1;
	}

	for (int i = startY; i < startY + mt2.matrix->height*step; i+=step) {
		for (int j = startX; j < startX + mt2.matrix->width*step; j+=step) {
			mt1[i][j] = mt2[(i-startY)/step][(j-startX)/step];
		}
	}

	return mt1;

}

template <class TypeM>
Matrix<TypeM> Matrix<TypeM>::mset(Matrix mt2, int startY, int startX, int step) {

	Matrix mt(matrix->sheight, matrix->swidth);	
	mt.matrix->arr = matrix->arr;
	mt.matrix->isTransposed = matrix->isTransposed;

	mt.matrix->width = matrix->width;
	mt.matrix->height = matrix->height;

	return Matrix::mset(mt, mt2, startY, startX, step);
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

template <class TypeM>
bool operator==(Matrix<TypeM> mt1, Matrix<TypeM> mt2) {

	bool isValid = (
		mt1.getWidth() == mt2.getWidth()
	) and (
		mt1.getHeight() == mt2.getHeight()
	);

	for (int i = 0; i < mt1.getHeight(); i++) {
		for (int j = 0; j < mt1.getWidth(); j++) {
			if (not isValid) {
				return false;
			} else {
				isValid = mt1[i][j] == mt2[i][j];
			}

		}
		if (not isValid) {
			return false;
		}
	}

	return true;
}




template <class TypeM>
int Slice<TypeM>::getWidth() {
	return width;
}

template <class TypeM>
int Slice<TypeM>::getHeight() {
	return height;
}

template <class TypeM>
int Slice<TypeM>::getStepSize() {
	return step;
}
 
 
template <class TypeM>
int Slice<TypeM>::index(int i, int j) {
	
	if (i >= height or j >= width or i < 0 or j < 0) {
		throw Exception("Wrong indexes");
		return -1;
	}
	return matrix->index(startY + i*step, startX + j*step);
}

template <class TypeM>
Matrix<TypeM> Slice<TypeM>::getAsMatrix() {
	return matrix->mget(startY, startX, height, width, step);
}

template <class TypeM>
Matrix<TypeM> Slice<TypeM>::getAsMatrix(Slice<TypeM> slc) {
	return slc.getAsMatrix();
}


template <class TypeM>
Slice<TypeM> Slice<TypeM>::mul(Slice<TypeM> slc, TypeM val) {
	for (int i = 0; i < slc.getHeight(); i++) {
		for (int j = 0; j < slc.getWidth(); j++) {
			slc[i][j] = slc[i][j]*val;
		}
	}
	return slc;
}

template <class TypeM>
Slice<TypeM> Slice<TypeM>::mul(TypeM val) {
		
	Slice slc(matrix, startY, startX, height, width, step);	

	return Slice::mul(slc, val);

}



template <class TypeM>
Slice<TypeM>::Slice(Matrix<TypeM>* m, int nstartY, int nstartX, int sheight, int swidth, int nstep) {

	matrix = m;
	if ((nstartY + (sheight-1)*nstep >= matrix->getHeight()) or (nstartX + (swidth-1)*nstep >= matrix->getWidth())) {
		throw Exception("Wrong matrix slice size");
	}

	startY = nstartY;
	startX = nstartX;
	height = sheight;
	width = swidth;
	step = nstep;
}

template <class TypeM>
Slice<TypeM> operator*(Slice<TypeM>& slc, TypeM c) {
	return slc.mul(c);
}



/*
int main() {
	Matrix<int> a(5, 5);
	for (int i = 0; i < a.getHeight(); i++) {
		for (int j = 0; j < a.getWidth(); j++) {
			a[i][j] = i*a.getWidth() + j + 2;
			std::cout << a[i][j] << ' ';
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

	Matrix<int> k = a.mget(1, 0, 4, 4, 1);
	for (int i = 0; i < k.getHeight(); i++) {
		for (int j = 0; j < k.getWidth(); j++) {
			std::cout << k[i][j] << ' ';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	Matrix<int> b(2, 2);

	//a.mset(b, 0, 2, 2);
	for (int i = 0; i < a.getHeight(); i++) {
		for (int j = 0; j < a.getWidth(); j++) {
			std::cout << a[i][j] << ' ';
		}
		std::cout << std::endl;
	}

	Slice<int> slc(&a, 0, 0, 3, 3, 2);
	
	for (int i = 0; i < slc.getHeight(); i++) {
		for (int j = 0; j < slc.getWidth(); j++) {
			slc[i][j] = 1;
		}
		
	}
	slc = slc*(-9);

	std::cout << std::endl;
	for (int i = 0; i < a.getHeight(); i++) {
		for (int j = 0; j < a.getWidth(); j++) {
			std::cout << a[i][j] << " ";
		}
		std::cout << std::endl;
	}

	Matrix<int> c = slc.getAsMatrix();

}
*/



/*
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

	for (int i = 0; i < matrix.getHeight(); i++) {
		for (int j = 0; j < matrix.getWidth(); j++) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}

	matrix2.transpose();
	for (int i = 0; i < matrix2.getHeight(); i++) {
		for (int j = 0; j < matrix2.getWidth(); j++) {
			std::cout << matrix2[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
*/