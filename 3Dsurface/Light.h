#pragma once
#include"data.h"
#include"Shader.h"

class Light
{
public:
	Light(const ShaderProgram & shader_program) {
		shader_program_id = shader_program.id;
		intensity = 1.0f;
	}

	Light() {
		shader_program_id = -1;
		intensity = 1.0f;
	}

	void Intensity(const string& _name) {
		intensity_name = _name;
		intensity_location = Uniform(shader_program_id, intensity_name);
	}

	void Intensity(float _intensity) {
		intensity = _intensity;
	}

	void shaderProgram(const ShaderProgram & shader_program) {
		shader_program_id = shader_program.id;
	}

	void Color(const vec3& _color) {
		color = vec4(_color, 1);
	}

	void Color(const vec4& _color) {
		color = _color;

	}

	void Color(const float * _color) {
		color = vec4(_color);
	}

	void Color(const int * _color) {
		color = vec4((float)_color[0] / 255, (float)_color[1] / 255,
			(float)_color[2] / 255, (float)_color[3] / 255);
	}

	void Color(float r, float g, float b, float a) {
		color = vec4(r, g, b, a);
	}

	void Color(float r, float g, float b) {
		color = vec4(r, g, b, 1);
	}

	void Color(const string & _name) {
		color_name = _name;
		color_location = Uniform(shader_program_id, color_name);
	}

protected:
	vec4 color;
	string color_name;
	Uniform color_location;

	float intensity;
	string intensity_name;
	Uniform intensity_location;

	int shader_program_id;
};

class Ambient :public Light {
public:
	Ambient(const ShaderProgram & shader_program) :Light(shader_program) {
		color = vec4(1, 1, 1, 1);
	}

	Ambient() :Light() {
		color = vec4(1, 1, 1, 1);
		color_name = "";
	}

	void Init() {

		if (!color_name.empty())
		{
			color_location = color;
		}

		if (!intensity_name.empty())
		{
			intensity_location = intensity;
		}
	}
	

protected:
	
};

class Specular:public Light
{

public:
	Specular() :Light() {

	}

	Specular(const ShaderProgram & shader_program) :Light(shader_program) {

	}

	void Position(const vec3 & _position) {
		position = _position;
	}

	void Position(const float * _position) {
		position[0] = _position[0];
		position[1] = _position[1];
		position[2] = _position[2];

	}

	void Position(float x, float y, float z) {
		position[0] = x;
		position[1] = y;
		position[2] = z;
	}

	void Position(const string& _name) {
		position_name = _name;
		position_location = Uniform(shader_program_id, position_name);
	}

	void Direction(const vec3 & _direction) {
		direction = _direction;
	}

	void Direction(const float * _direction) {
		direction[0] = _direction[0];
		direction[1] = _direction[1];
		direction[2] = _direction[2];

	}

	void Direction(float x, float y, float z) {
		direction[0] = x;
		direction[1] = y;
		direction[2] = z;
	}

	void Direction(const string& _name) {
		direction_name = _name;
		direction_location = Uniform(shader_program_id, position_name);
	}

	void Init() {

		if (!color_name.empty())
		{
			color_location = color;
		}

		if (!intensity_name.empty())
		{
			intensity_location = intensity;
		}

		if (!position_name.empty())
		{
			position_location = position;
		}

		if (!direction_name.empty())
		{
			direction_location = direction;
		}
	}

protected:
	vec3 position;
	string position_name;
	Uniform position_location;

	vec3 direction;
	string direction_name;
	Uniform direction_location;
};
