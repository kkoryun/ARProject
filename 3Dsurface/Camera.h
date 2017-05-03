#pragma once
#include"data.h"
#include"GL\glew.h"

class Camera {
private:
	vec3 position;
	vec3 target;
	vec3 up;
	float zoom, farZ, nearZ, FoV, aspect;
	int alpha, beta;
public:
	transform viewMat;
	transform perspectiveMat;

	Camera() {
		farZ = nearZ = FoV = aspect = alpha = beta = 0;
		zoom = 1;
	}

	void Pos(const vec3 & _position) {
		SetView(_position, target, up);
	}

	void Pos(const float * _position) {
		SetView(vec3(_position), target, up);
	}

	void Targ(const vec3 & _target) {
		SetView(position, _target, up);
	}

	void Up(const vec3 & _up) {
		SetView(position, target, _up);
	}

	void spMotion(float _zoom, float dx, float dy) {
		zoom = _zoom;
		position[0] = zoom * std::cos(dx * 3.14 / 180)*std::cos(dy* 3.14 / 180);
		position[1] = zoom * std::sin(dx * 3.14 / 180)*std::cos(dy * 3.14 / 180);
		position[2] = zoom * (0 - std::sin(dy* 3.14 / 180));
		SetView(position, target, up);
	}

	void Motion(int dx, int dy) {

		if (dx || dy) {

			alpha -= dx;
			beta += dy;
			//if (alpha > 360) alpha = 360;
			//if (alpha < 0) alpha = 0;
			/*if (beta < -90) beta = -90;
			if (beta > 90) beta = 90;*/

			target = target - position;

			target[0] = cos(alpha * PI / 180) * sin(beta * PI / 180);
			target[1] = cos(beta * PI / 180);
			target[2] = sin(alpha * PI / 180) *  sin(beta * PI / 180);

			target = target + position;

			viewMat.uvn(position, target, up);
		}
	}

	void MoveForward() {
		vec3 tmp = target - position;
		position = target;
		target = position + tmp;
		viewMat.uvn(position, target, up);
		cout << "position " << position << endl;
		cout << "target " << target << endl;
	}

	void MoveBack() {
		vec3 tmp = target - position;
		target = position;
		position = position - tmp;
		viewMat.uvn(position, target, up);
		cout << "position " << position << endl;
		cout << "target " << target << endl;
	}

	void SetPerspective(float _FoV, float _aspect, float _near, float _far) {
		FoV = _FoV;
		aspect = _aspect;
		nearZ = _near;
		farZ = _far;
		perspectiveMat.perspective(FoV, aspect, nearZ, farZ);
	}

	void SetView(const vec3 & _position, const vec3 & _target, const vec3 & _up) {
		position = _position;
		target = _target;
		up = _up;
		viewMat.uvn(position, target, up);
	}

	void SetView(const float * _position, const float* _target, const float* _up) {
		SetView(vec3(_position), vec3(_target), vec3(_up));
	}

};