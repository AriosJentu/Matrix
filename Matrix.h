#include <iostream>
#include <string.h>

#pragma once

template <class TypeM>
class Matrix {
private:

	TypeM* arr;
	int swidth, sheight;

	int byIndex(int i, int j);

public:

	int height, width;
	bool isTransposed = false;

	void transpose();
	int index(int i, int j);

	static Matrix cmul(Matrix mt, TypeM val);
	static Matrix mul(Matrix mt1, Matrix mt2);
	static Matrix sum(Matrix mt1, Matrix mt2);
	static Matrix mget(Matrix mt, int startY, int startX, int nheight, int nwidth);
	static Matrix mset(Matrix mt1, Matrix mt2, int startY, int startX);

	Matrix cmul(TypeM val);
	Matrix mul(Matrix mt);
	Matrix sum(Matrix mt);
	Matrix mget(int startY, int startX, int nheight, int nwidth);
	Matrix mset(Matrix mt, int startY, int startX);

	//construct
	Matrix(int height, int width=1);

	//proxy class
	class Proxy {
	public:

		Matrix* mtx;
		int mi;

		Proxy(Matrix* matr, int i) {
			mtx = matr;
			mi = i;
		}

		TypeM operator[](int j) {
			return mtx->arr[mtx->index(mi, j)];
		}

		Proxy operator=(TypeM val) {
			mtx->arr[mi] = val;
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

};

template <class TypeM>
Matrix<TypeM> operator+(Matrix<TypeM>& mt1, Matrix<TypeM>& mt2);

template <class TypeM>
Matrix<TypeM> operator-(Matrix<TypeM>& mt1, Matrix<TypeM>& mt2);

template <class TypeM>
Matrix<TypeM> operator*(TypeM c, Matrix<TypeM>& mt);

template <class TypeM>
Matrix<TypeM> operator*(Matrix<TypeM>& mt, TypeM c);