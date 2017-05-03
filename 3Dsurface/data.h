#pragma once
#include <vector>
#include <iostream>
#include <cmath>
#define PI       (float)3.14159265358979323846 

using namespace std;
template<class T> void zero(T * pnt, int size) {
	for (size_t i = 0; i < size; i++)
		pnt[i] = 0;
}




typedef vector<float> vectorf;
class vec {
public:
	vectorf _data;
	size_t size;
	float * data;

	vec() {
		_data = vectorf();
		size = 0;
		data = 0;
	}

	vec(size_t _size) {
		_data = vectorf(_size);
		size = _size;
		data = _data.data();
	}

	vec(const float * _vec, size_t _size) {
		_data = vectorf(_size);
		size = _size;
		for (size_t i = 0; i < size; i++)
		{
			_data[i] = _vec[i];
		}
		data = _data.data();
	}

	float & operator[](size_t index) {
		return _data[index];
	}
	float operator[](size_t index)  const {
		return _data[index];
	}
	float   operator*(const vec & _vec) {
		float res;
		if (size == _vec.size)
		{
			for (size_t i = 0; i < size; i++)
			{
				res += _data[i] * _vec[i];
			}
		}
		return res;
	}

	vec operator-(const vec & _v) {
		vec res(size);
		if (size == _v.size)
			for (size_t i = 0; i < size; i++)
			{
				res[i] = _data[i] - _v[i];
			}
		return res;
	}

	vec operator*(float val) {
		vec res(size);
		for (size_t i = 0; i < size; i++)
		{
			res[i] = _data[i] * val;
		}
		return res;
	}

	void normalize() {

		float	res = 0;
		for (size_t i = 0; i < size; i++)
		{
			res += _data[i] * _data[i];
		}
		res = sqrtf(res);
		if (res != 0.0f) {
			(*this) = (*this) * (1 / res);
		}
	}
};

class vec2 :public vec {
public:
	vec2() :vec(2) {}

	vec2(float x, float y) :vec(2) {
		_data[0] = x;
		_data[1] = y;
	}

	vec2(float * _data) :vec(_data, 2) {}
};

class vec3 :public vec {
public:
	vec3() :vec(3) {
	}

	vec3(const vec2 & v, float third) :vec(v.data, 3) {
		_data[2] = third;
	}

	vec3(float x, float y, float z) : vec(3) {
		_data[0] = x;
		_data[1] = y;
		_data[2] = z;
	}

	vec3(const float * _data) :vec(_data, 3) {
	}

	vec3 cross(const vec3 & _vec) const {
		vec3 res;
		res[0] = _data[1] * _vec[2] - _data[2] * _vec[1];
		res[1] = _data[2] * _vec[0] - _data[0] * _vec[2];
		res[2] = _data[0] * _vec[1] - _data[1] * _vec[0];
		return res;
	}

	vec3     operator*(float val) {
		vec3 res;
		for (size_t i = 0; i < size; i++)
		{
			res[i] = _data[i] * val;
		}
		return res;
	}

	vec3     operator-(const vec3 & v) const
	{
		vec3 res;
		res[0] = _data[0] - v[0];
		res[1] = _data[1] - v[1];
		res[2] = _data[2] - v[2];
		return res;
	}

	vec3     operator+(const vec3 & v) const {
		vec3 res;
		res[0] = _data[0] + v[0];
		res[1] = _data[1] + v[1];
		res[2] = _data[2] + v[2];
		return res;
	}

	float operator*(const vec3 & v) const {
		float res;
		res = _data[0] * v[0] + _data[1] * v[1] + _data[2] * v[2];
		return res;
	}
};

class vec4 : public vec {
public:
	vec4() :vec(4) {
	}

	vec4(const vec3 & v, float fourth) :vec(v.data, 4) {
		_data[3] = fourth;
	}

	vec4(const float * _data) :vec(_data, 4) {
	}

	vec4(float x, float y,float z, float w) :vec(4) {
		_data[0] = x;
		_data[1] = y;
		_data[2] = z;
		_data[3] = w;
	}
};

ostream& operator<< (ostream& os, vec & v)
{

	for (size_t j = 0; j < v.size; j++)
	{
		os << v[j] << " ";
	}
	os << endl;

	return os;
}


template<class T> class mat {

public:
	vector<T> data;
	size_t width;
	size_t height;
	T * pdata;


	mat() {
		width = 0;
		height = 0;
		data = vector<T>();
		pdata = 0;
	}

	mat(size_t _width, size_t _height) {
		width = _width;
		height = _height;
		data = vector<T>(width * height);
		pdata = data.data();
	}

	mat(const T* _data, size_t _width, size_t _height) {
		width = _width;
		height = _height;
		data = vector<T>(width * height);
		pdata = data.data();
		for (size_t i = 0; i <width * height; i++)
		{
			data[i] = _data[i];
		}
	}

	T * operator[](size_t row) {

		if (row < height) {
			return (data.data() + row * width);
		}
		return 0;

	}

	

	mat<T>  operator*(mat<T> & m)
	{
		mat<T> tmp(height, m.width);
		if (m.width == height && m.height == width)
		{

			for (size_t i = 0; i < height; i++)
			{
				for (size_t j = 0; j < width; j++)
				{
					for (size_t k = 0; k < width; k++)
					{
						tmp[i][j] += (*this)[i][k] * m[k][j];
					}
				}
			}
		}

		return tmp;
	}

	mat<T> & operator=(mat<T> & m)
	{
		if (m.width != width && m.height != height) {
			width = m.width;
			height = m.height;
		}
		data = m.data;
		pdata = data.data();
		return *this;
	}

	
};

class mat2 :public mat<float> {
public:
	mat2() :mat<float>(2, 2) {
	}

	mat2 & operator=(mat<float>   & m) {
		if (m.width != width && m.height != height) {
			width = m.width;
			height = m.height;
		}
		data = m.data;
		pdata = data.data();
		return *this;
	}
};

class mat3 :public mat<float> {
public:
	mat3() :mat<float>(3, 3) {
	}

	mat3 & operator=(mat<float>   & m) {
		if (m.width != width && m.height != height) {
			width = m.width;
			height = m.height;
		}
		data = m.data;
		pdata = data.data();
		return *this;
	}
};

class mat4 :public mat<float> {
public:
	mat4() :mat<float>(4, 4) {}

	mat4 & operator=(mat<float>   & m) {
		if (m.width != width && m.height != height) {
			width = m.width;
			height = m.height;
		}
		data = m.data;
		pdata = data.data();
		return *this;
	}
};

class transform :public mat4 {
public:
	transform() :mat4() {
		(*this)[0][0] = 1;
		(*this)[1][1] = 1;
		(*this)[2][2] = 1;
		(*this)[3][3] = 1;
	}

	transform & operator=(mat<float>   & m) {
		if (m.width != width && m.height != height) {
			width = m.width;
			height = m.height;
		}
		data = m.data;
		pdata = data.data();
		return *this;
	}

	void scale(float sx, float sy, float sz) {
		(*this)[0][0] = sx;
		(*this)[1][1] = sy;
		(*this)[2][2] = sz;
	}

	void scale(vec3 & s) {
		(*this)[0][0] = s[0];
		(*this)[1][1] = s[1];
		(*this)[2][2] = s[2];
	}

	void translate(float tx, float ty, float tz) {
		(*this)[0][3] = tx;
		(*this)[1][3] = ty;
		(*this)[2][3] = tz;
	}

	void translate(vec3 & t) {
		(*this)[0][3] = t[0];
		(*this)[1][3] = t[1];
		(*this)[2][3] = t[2];
	}

	void rotateZ(float angle) {
		(*this)[0][0] = cos(angle * PI / 180);
		(*this)[0][1] = -sin(angle * PI / 180);
		(*this)[1][0] = sin(angle * PI / 180);
		(*this)[1][1] = cos(angle * PI / 180);
	}

	void rotateY(float angle) {
		(*this)[0][0] = cos(angle * PI / 180);
		(*this)[0][2] = -sin(angle * PI / 180);
		(*this)[2][0] = sin(angle * PI / 180);
		(*this)[2][1] = cos(angle * PI / 180);
	}

	void rotateX(float angle) {
		(*this)[1][1] = cos(angle * PI / 180);
		(*this)[1][2] = -sin(angle * PI / 180);
		(*this)[2][1] = sin(angle * PI / 180);
		(*this)[2][2] = cos(angle * PI / 180);
	}

	void perspective(float FoV, float aspect, float zNear, float zFar) {

		(*this)[0][0] = 1.0f / (tanf(FoV / 2) * aspect);
		(*this)[0][1] = 0.0f;
		(*this)[0][2] = 0.0f;
		(*this)[0][3] = 0.0f;

		(*this)[1][0] = 0.0f;
		(*this)[1][1] = 1.0f / tanf(FoV / 2);
		(*this)[1][2] = 0.0f;
		(*this)[1][3] = 0.0f;

		(*this)[2][0] = 0.0f;
		(*this)[2][1] = 0.0f;
		(*this)[2][2] = -(zNear + zFar) / (zFar - zNear);
		(*this)[3][2] = (2.0f * zFar * zNear) / (zNear - zFar);

		(*this)[3][0] = 0.0f;
		(*this)[3][1] = 0.0f;
		(*this)[2][3] = -1.0f;
		(*this)[3][3] = 0.0f;

	}

	void uvn(const vec3 & position,const vec3 & target,const vec3 & up) {
		vec3 f = target - position ;
		f.normalize();
		vec3 s = up.cross(f);
		s.normalize();
		vec3 u = f.cross(s);

		(*this)[0][0] = s[0];
		(*this)[1][0] = s[1];
		(*this)[2][0] = s[2];
		(*this)[3][0] = 0.0f;

		(*this)[0][1] = u[0];
		(*this)[1][1] = u[1];
		(*this)[2][1] = u[2];
		(*this)[3][1] = 0.0f;

		(*this)[0][2] = -f[0];
		(*this)[1][2] = -f[1];
		(*this)[2][2] = -f[2];
		(*this)[3][2] = 0.0f;

		(*this)[3][0] = -(position * s);
		(*this)[3][1] = -(position * u);
		(*this)[3][2] = (position * f);
		(*this)[3][3] = 1.0f;

	}

};

typedef mat<float> matf;

ostream& operator<< (ostream& os, matf & m)
{
	for (size_t i = 0; i < m.height; i++)
	{
		for (size_t j = 0; j < m.width; j++)
		{
			os << m[i][j] << " ";
		}
		os << endl;
	}
	return os;
}

namespace other {
	template<class T, size_t _size> class vec
	{
	public:
		vec() {
			size = _size;
			data = std::vector<T>(size);
		}

		vec(const T* _data, size_t data_size) {
			size = data_size;
			data = std::vector<T>(size);
			for (size_t i = 0; i < size; i++)
			{
				data[i] = _data[i];
			}
		}

		vec(std::vector<T> _data) {
			data = _data;
			size = _data.size();
		}

#pragma region OPERATORS
		//operators
		T & operator[](size_t index) {
			if (index < size)
				return data[index];
		}

		T operator[](size_t index)const {
			if (index < size)
				return data[index];
		}

		vec<T, _size>     operator*(float val) {
			vec<T, _size> res;
			for (size_t i = 0; i < size; i++)
			{
				res[i] = _data[i] * val;
			}
			return res;
		}

		T     operator*(vec<T, _size> & v) {
			T res;
			for (size_t i = 0; i < size; i++)
			{
				res += _data[i] * v[i];
			}
			return res;
		}

		vec<T, _size>     operator-(const vec<T, _size> & v) const
		{
			vec<T, _size> res;
			for (size_t i = 0; i < size; i++)
			{
				res[i] = data[i] - v[i];
			}
			return res;
		}

		vec<T, _size>     operator+(const vec<T, _size> & v) const {
			vec res(size);
			for (size_t i = 0; i < size; i++)
			{
				res[i] = data[i] + v[i];
			}
			return res;
		}

		vec<T, _size>     operator-() const {}

		friend ostream & operator<<(ostream & os, const vec<T, _size> & v) {
			for (size_t i = 0; i < _size; i++)
			{
				os << v[i] << " ";
			}
			return os;
		}
		//
#pragma endregion

#pragma region MATH_FUNC
		void normalize() {

		}

		void cross() {

		}
#pragma endregion

	size_t size;
		std::vector<T> data;
	private:

	};

	class vec3 : public vec<float, 3> {

	};
		typedef vec<float, 2> vec2;
	//typedef vec<float, 3> vec3;
	typedef vec<float, 4> vec4;

	
}
