#pragma once
#include <GL\glew.h>
#include"Img.h"
class Texture {

public:
	GLuint   textID;
	Bitmap * textImg;

	Texture(string  textureFileName = "") {
		textID = -1;
		if (textureFileName != "") {
			LoadTexture(textureFileName);
		}
	}

	void LoadTexture(string  textureFileName) {
			textImg = new Bitmap(textureFileName);
			glGenTextures(1, &textID);
			glBindTexture(GL_TEXTURE_2D, textID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,textImg->width,textImg->height, 0,
				GL_BGR, GL_UNSIGNED_BYTE, textImg->data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Bind() {
		glBindTexture(GL_TEXTURE_2D, textID);
	}

	void Unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void MinMap() {
		/*glBindTexture(GL_TEXTURE_2D, textID);
		glGenerateMipmap(GL_TEXTURE_2D);*/
	}

	~Texture() {
		delete textImg;
	}
};