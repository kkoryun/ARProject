#pragma once
#include<GL\glew.h>
#include<vector>

enum
{
	BUFFER_EMPTY = 0
};

class BufferObject {
public:
	GLuint id;
	char state;
	BufferObject() {
		glGenBuffers(1, &id);
		state = 0;
	}

	virtual void Bind() = 0;

	virtual void Unbind() = 0;
};


class VBO
{
public:
	GLuint vboID;
	GLubyte *data;//on cpu
	GLsizei size;


	VBO()
	{
		data = 0;
		size = 0;
		glGenBuffers(1, &vboID);
	}

	VBO(const GLfloat * _data, int vertcount)
	{
		data = (GLubyte *)_data;
		size = vertcount * sizeof(GLfloat);
		glGenBuffers(1, &vboID);
		Init();
	}

	VBO(const vec & _vec) {
		data = (GLubyte *)_vec.data;
		size = _vec.size;
		glGenBuffers(1, &vboID);
		Init();
	}

	VBO(const matf & _mat) {
		data = (GLubyte*)_mat.pdata;
		data = (GLubyte*)_mat.data.data();
		size = _mat.height *_mat.width;
		glGenBuffers(1, &vboID);
		Init();
	}

	void Init() const {
		Bind();
		if (data != 0 && size > 0) glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), data, GL_STATIC_DRAW);
		Unbind();
	}

	void Init(const GLfloat * _data, int vertcount) {
		Bind();

		if (data == 0 && size == 0) {
			data = (GLubyte *)_data;
			size = vertcount;
			glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), data, GL_STATIC_DRAW);
		}
		else
		{
			data = (GLubyte *)_data;
			if (size == vertcount) {
				glBufferSubData(vboID, 0, size * sizeof(GLfloat), data);
			}
			else
			{
				glDeleteBuffers(1, &vboID);
				data = 0;
				size = 0;
				glGenBuffers(1, &vboID);
				Init();
			}
		}
		Unbind();
	}

	void Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
	}

	void Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	~VBO() {
		glDeleteBuffers(1, &vboID);
	}

};


class EBO {
public:
	GLuint eboID;
	GLubyte *data;
	GLsizei size;


	EBO() {
		data = 0;
		size = 0;
		glGenBuffers(1, &eboID);
	}

	EBO(const GLuint * _data, int vertcount) {
		data = (GLubyte *)_data;
		size = vertcount * sizeof(GLfloat);
		glGenBuffers(1, &eboID);
		Init();
	}

	EBO(const vec & _vec) {
		data = (GLubyte *)_vec.data;
		size = _vec.size;
		glGenBuffers(1, &eboID);
		Init();
	}

	void Init() {
		Bind();
		if (data != 0 && size > 0)
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		Unbind();
	}

	void Init(const GLuint * _data, int vertcount) {
		Bind();

		if (data == 0 && size == 0) {
			data = (GLubyte *)_data;
			size = vertcount * sizeof(GLint);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}
		else
		{
			data = (GLubyte *)_data;
			if (size == vertcount * sizeof(GLint)) {
				glBufferSubData(eboID, 0, size, data);
			}
			else
			{
				glDeleteBuffers(1, &eboID);
				data = 0;
				size = 0;
				glGenBuffers(1, &eboID);
				Init();
			}
		}
		Unbind();
	}

	void Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	}

	void Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	~EBO() {
		glDeleteBuffers(1, &eboID);
	}

};


class VAO
{
public:
	GLuint vaoID;
	std::vector<VBO> vboarr;
	EBO  ebo;

	VAO() {
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);
	}

	void Bind() const {
		glBindVertexArray(vaoID);
		if (ebo.size) {
			ebo.Bind();
		}
	}

	void Unbind() const {
		glBindVertexArray(0);
		if (ebo.size) {
			ebo.Unbind();
		}
	}

	void AddVertexBuffer(const VBO & vbo, int location, int elemsize) {
		glBindVertexArray(vaoID);
		//vboarr.push_back(vbo);
		vbo.Bind();
		glVertexAttribPointer(location, elemsize, GL_FLOAT, GL_FALSE, elemsize * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(location);
		vbo.Unbind();
		glBindVertexArray(0);
	}

	void AddIndexBuffer(EBO & _ebo) {
		ebo = _ebo;
	}

	void TDraw(int offset, int pointcount) {
		Bind();
		glDrawArrays(GL_TRIANGLES, offset, pointcount);
		Unbind();
	}

	void TsDraw(int offset, int pointcount) {
		Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, offset, pointcount);
		Unbind();
	}

	void eTDraw(int offset, int pointcount) {
		Bind();
		glDrawElements(GL_TRIANGLES, pointcount, GL_UNSIGNED_INT, 0);
		Unbind();
	}

	void eTsDraw(int offset, int pointcount) {
		Bind();
		glDrawElements(GL_TRIANGLE_STRIP, pointcount, GL_UNSIGNED_INT, 0);
		Unbind();
	}

	void Draw(int offset, int pointcount) {
		Bind();
		if (ebo.size) {
			eTDraw(0, pointcount);
		}
		else {
			TDraw(offset, pointcount);
		}
		Unbind();
	}

};
