#include <iostream>
#include <string.h>

#pragma once

//Proxy Slice + Slice Stepping


template <class TypeM>
struct MatrixLink {
	
	TypeM* arr;
	int swidth, sheight;
	int counter;
	bool isTransposed = false;
	int height, width;

	~MatrixLink() {
	}

};

template <class TypeM>
class Matrix {
private:

	//MatrixLink<TypeM>* matrix;

	void eventClear() {

		if (matrix != NULL) {

			matrix->counter--;
			if (matrix->counter == 0) {
				delete(matrix);				
			}	

			matrix = NULL;
		}
	}

	void matrixMove(MatrixLink<TypeM>* frommatrix) {
		
		if (matrix != frommatrix) {

			eventClear();

			if (frommatrix != NULL) {

				frommatrix->counter++;
				matrix = frommatrix;
			}
		}
	}

	void matrixCopy(MatrixLink<TypeM>* frommatrix) {

		if (matrix != frommatrix) {

			MatrixLink<TypeM>* tmp;
			int size = frommatrix->swidth * frommatrix->sheight;
			
			tmp->swidth = frommatrix->swidth;
			tmp->sheight = frommatrix->sheight;
			tmp->width = frommatrix->width;
			tmp->height = frommatrix->height;
			tmp->isTransposed = frommatrix->isTransposed;

			memcpy(tmp->arr, frommatrix->arr, sizeof(TypeM)*size);

			eventClear();
			matrix = tmp;
		}
	}

public:

	MatrixLink<TypeM>* matrix;

	void transpose();
	int index(int i, int j);

	static Matrix cmul(Matrix mt, TypeM val);
	static Matrix mul(Matrix mt1, Matrix mt2);
	static Matrix sum(Matrix mt1, Matrix mt2);
	static Matrix mget(Matrix mt, int startY, int startX, int nheight, int nwidth, int step=1);
	static Matrix mset(Matrix mt1, Matrix mt2, int startY, int startX, int step=1);

	Matrix cmul(TypeM val);
	Matrix mul(Matrix mt);
	Matrix sum(Matrix mt);
	Matrix mget(int startY, int startX, int nheight, int nwidth, int step=1);
	Matrix mset(Matrix mt, int startY, int startX, int step=1);

	int getWidth();
	int getHeight();

	//construct
	Matrix(int height, int width=1);

	~Matrix() {

		matrix->counter--;
		if (matrix->counter < 0) {
			//std::cout << "Matrix Links empty!" << std::endl;
		}
	}

	//proxy class
	class Proxy {
	public:

		Matrix* mtx;
		int mi;

		Proxy(Matrix* matr, int i) {
			mtx = matr;
			mi = i;
		}

		TypeM& operator[](int j) {
			return mtx->matrix->arr[mtx->index(mi, j)];
		}

	};

	//exception class
	class Exception: public std::exception {

	public:

		std::string errorInfo;

		Exception(std::string info) {
			errorInfo = "Matrix Error: " + info;
		}

		virtual const char* what() const throw() {
			return errorInfo.c_str();
		}
	};


	//operators:
	Proxy operator[](int i) {
		return Proxy(this, i);
	}


	Matrix operator=(Matrix mtx) {

		matrixMove(mtx.matrix);

		return mtx;
	}

};

template <class TypeM>
class Slice {
private:

	int height, width, step, startY, startX;


public:
	int index(int i, int j);
	Matrix<TypeM>* matrix;

	Slice(Matrix<TypeM>* m, int nstartY, int nstartX, int sheight, int swidth, int ntep=1);

	int getWidth();
	int getHeight();
	int getStepSize();

	Slice mul(TypeM val);
	Matrix<TypeM> getAsMatrix();

	static Slice mul(Slice slc, TypeM val);
	static Matrix<TypeM> getAsMatrix(Slice slc);



	class SProxy {
	public:

		Slice* slc;
		int mi;

		SProxy(Slice* slic, int i) {
			slc = slic;
			mi = i;
		}

		TypeM& operator[](int j) {
			return slc->matrix->matrix->arr[slc->index(mi, j)];
		}
	};

	SProxy operator[](int i) {
		return SProxy(this, i);
	}


	//exception class
	class Exception: public std::exception {

	public:

		std::string errorInfo;

		Exception(std::string info) {
			errorInfo = "Slice Error: " + info;
		}

		virtual const char* what() const throw() {
			return errorInfo.c_str();
		}
	};

};


template <class TypeM>
Matrix<TypeM> operator+(Matrix<TypeM>& mt1, Matrix<TypeM>& mt2);

template <class TypeM>
Matrix<TypeM> operator-(Matrix<TypeM>& mt1, Matrix<TypeM>& mt2);

template <class TypeM>
Matrix<TypeM> operator*(TypeM c, Matrix<TypeM>& mt);

template <class TypeM>
Matrix<TypeM> operator*(Matrix<TypeM>& mt, TypeM c);

template <class TypeM>
bool operator==(Matrix<TypeM> mt1, Matrix<TypeM> mt2);


template <class TypeM>
Slice<TypeM> operator*(Slice<TypeM>& slc, TypeM c);
