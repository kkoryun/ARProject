#pragma once
#include <GL\glew.h>
#include<string>
#include<fstream>
#include<iostream>
#include"Texture.h"
#include"data.h"

using namespace std;

class Shader
{
public:
	
	GLuint  shader_id;
	GLint   status;
	string  shader_code;
	string  log_info;
	string file_name;


	Shader() {
		shader_code = "";
		shader_id = 0;
		status = 1;
		log_info = "";
	}

	void loadShaderCode(string filename) {
		file_name = filename;
		ifstream is(filename);
		string tmp;
		while (getline(is, tmp))
			shader_code += tmp + "\n";
		is.close();
	}

	void initShader() {
		const GLchar * str = shader_code.c_str();
		glShaderSource(shader_id, 1, &str, NULL);
		glCompileShader(shader_id);
	}

	void checkStatus() {
		GLchar * loginfo;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			int length = 0;
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
			if (length) {
				loginfo = new GLchar[length];
				glGetShaderInfoLog(shader_id, length, NULL, loginfo);
				log_info = loginfo;
				delete[] loginfo;
			}
		};
	}

	void print_status() {
		if (!log_info.empty()) {
			cout <<"COMPILE_ERROR in  "<<file_name<<endl<< log_info << endl;
		}
	}
	~Shader() {
		glDeleteShader(shader_id);
	}
};

class SHFragment :public Shader {
public:
	SHFragment(string filename = "") :Shader() {
		if (filename != "") {
			loadShaderCode(filename);
			shader_id = glCreateShader(GL_FRAGMENT_SHADER);
			initShader();
		}
	}

};

class SHVertex :public Shader
{
public:
	SHVertex(string filename = "") :Shader() {
		if (filename != "") {
			loadShaderCode(filename);
			shader_id = glCreateShader(GL_VERTEX_SHADER);
			initShader();
		}
	}

};

class ShaderProgram {
public:

	ShaderProgram() {
		status = 1;
		log_info = "";
		vert = 0;
		frag = 0;
		id = 0;
	}

	ShaderProgram(string  vertname, string  fragname) {
		status = 1;
		log_info = "";
		vert = new SHVertex(vertname);
		frag = new SHFragment(fragname);
		id = glCreateProgram();
		Init();

	}

	void Init() {
		if (vert != 0 && frag != 0) {
			glAttachShader(id, vert->shader_id);
			glAttachShader(id, frag->shader_id);
			glLinkProgram(id);
		}
	}

	void Init(SHVertex * _vert, SHFragment * _frag) {
		vert = _vert;
		frag = _frag;

	}

	void LinkStatus() {
		glGetProgramiv(id, GL_LINK_STATUS, &status);
		GLchar * loginfo;
		if (!status)
		{
			int length = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
			if (length) {
				loginfo = new GLchar[length];
				glGetShaderInfoLog(id, length, NULL, loginfo);
				log_info = loginfo;
				delete[] loginfo;
			}
		};
	}

	void CheckStatus() {
		vert->checkStatus();
		frag->checkStatus();
		LinkStatus();
	}

	void Print_Status() {
		if (!log_info.empty()) {
			vert->print_status();
			frag->print_status();
			cout << "LINK_ERROR" << endl << log_info << endl;
		}
	}

	void Use() {
		glUseProgram(id);
	}

	~ShaderProgram() {
		if (vert != 0) delete vert;
		if (frag != 0) delete frag;
	}

	GLuint id;
	SHVertex   * vert;
	SHFragment * frag;
	string     log_info;
	GLint   status;
};

class Uniform
{
public:
	GLint id;
	GLuint program_id;
	string value_name;

	Uniform() {
		id = -1;
		program_id = -1;
		value_name = "";
	}

	Uniform(GLuint _progID, string _name) {
		program_id = _progID;
		value_name = _name;
	}

	Uniform(const ShaderProgram & _prog, string  _name) {
		program_id = _prog.id;
		value_name = _name;
	}

	Uniform & operator=(const GLfloat * _vec) {
		id = glGetUniformLocation(program_id, value_name.c_str());
		if (id != -1)
			glUniform4f(id, _vec[0], _vec[1], _vec[2], _vec[3]);
		return *this;
	}

	Uniform & operator=(const vec2 & _vec) {
		id = glGetUniformLocation(program_id, value_name.c_str());
		if (id != -1)
			glUniform2f(id, _vec[0], _vec[1]);
		return *this;
	}

	Uniform & operator=(const vec3 & _vec) {
		id = glGetUniformLocation(program_id, value_name.c_str());
		if (id != -1)
			glUniform3f(id, _vec[0], _vec[1], _vec[2]);
		return *this;
	}

	Uniform & operator=(const vec4 & _vec) {
		id = glGetUniformLocation(program_id, value_name.c_str());
		if (id != -1)
			glUniform4f(id, _vec[0], _vec[1], _vec[2], _vec[3]);
		return *this;
	}

	Uniform & operator=(const mat2 & _mat) {
		id = glGetUniformLocation(program_id, value_name.c_str());
		if (id != -1)
			glUniformMatrix2fv(id, 1, GL_FALSE, _mat.pdata);
		return *this;
	}

	Uniform & operator=(const mat3 & _mat) {
		id = glGetUniformLocation(program_id, value_name.c_str());
		if (id != -1)
			glUniformMatrix3fv(id, 1, GL_FALSE, _mat.pdata);
		return *this;
	}

	Uniform & operator=(const mat4 & _mat) {
		id = glGetUniformLocation(program_id, value_name.c_str());
		if (id != -1)
			glUniformMatrix4fv(id, 1, GL_FALSE, _mat.pdata);
		return *this;
	}

	Uniform & operator=(const transform & _mat) {
		id = glGetUniformLocation(program_id, value_name.c_str());
		if (id != -1)
			glUniformMatrix4fv(id, 1, GL_FALSE, _mat.pdata);
		return *this;
	}

	Uniform & operator=(GLfloat val) {
		id = glGetUniformLocation(program_id, value_name.c_str());
		if (id != -1)
			glUniform1f(id, val);
		return *this;
	}

	Uniform & operator=(GLint val) {
		id = glGetUniformLocation(program_id,value_name.c_str());
		if (id != -1)
			glUniform1i(id, val);
		return *this;
	}

	Uniform & operator=(GLuint val) {
		id = glGetUniformLocation(program_id, value_name.c_str());
		if (id != -1)
			glUniform1i(id, val);
		return *this;
	}

};

class Sampler {
public:
	Texture * texture2D;
	Uniform   location;
	string    name;
	int       program_id;
	int       number;

	Sampler() {
		texture2D = 0;
	    location = -1;
		name = "";
		program_id = -1;
		number = -1;
	}

	Sampler(const ShaderProgram & _program ,const string & _sampler_name, int _number = 0) {
	    name = _sampler_name;
		program_id = _program.id;
		number = _number;
		location = Uniform(program_id, name);
		texture2D = 0;
	}

	Sampler & operator=(string texturename) {
		texture2D = new Texture(texturename);
		return *this;
	}

	Sampler & operator=(const char * texturename) {
		texture2D = new Texture(texturename);
		return *this;
	}

	Sampler & operator=( Texture * texture) {
		texture2D = texture;
		return *this;
	}

	void Bind() {
		if (texture2D) {
			glActiveTexture(GL_TEXTURE0 + number);
			texture2D->Bind();
			location = number;
		}
		
	}

	~Sampler() {
		if (texture2D)
			delete texture2D;
	}

};
