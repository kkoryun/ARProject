
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

//#define _DEBUG_
#include "Shader.h"
#include "Buffers.h"
#include "Texture.h"
#include "data.h"
#include "Camera.h"
#include "Landscape.h"
#include "Light.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_position1_callback(GLFWwindow* window, double x, double y);

void mouse_callback(GLFWwindow* window, int x, int y, int z);
bool press, press_mem;
int cursorX = 0, cursorY = 0, cursordx = 0, cursordy = 0;
const GLuint WIDTH = 800, HEIGHT = 600;
int width, height;
GLfloat verticesposition[] = {
	-1.0f, -1.0f, 0.0f, //0
	0.0f, -1.0f, 0.0f, //1
	-1.0f, 0.0f, 0.0f, //2
	0.0f,  0.0f, 0.0f, //3
	-1.0f, 1.0f, 0.0f, //4
	0.0f, 1.0f, 0.0f //5
};

GLfloat verticescoord[] = {
	0.0f,0.0f,//0
	1.0f,0.0f,//1
	0.0f,1.0f,//2
	1.0f,1.0f,//3
	0.0f,0.0f,//4
	1.0f,0.0f,//5
	/*0.0f,1.0f,
	1.0f,1.0f,
	0.0f,0.0f,
	1.0f,0.0f,
	0.0f,1.0f,
	1.0f,1.0f*/
};
GLuint verticeindex[]{
	0,1,2,1,2,3,2,3,4,3,4,5
};
GLfloat cameraposition[] = { 0.0f,3.0f,0.0f };
GLfloat cameratarget[] = { 0.0f,0.0f,0.0f };
GLfloat cameraup[] = { 0.0f,1.0f,0.0f };
Camera cam;
Ambient light;
Specular light1;
GLFWwindow* window;

int set_context() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetCursorPosCallback(window, cursor_position1_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

}

int main()
{

	set_context();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	Landscape land("hm5.bmp");
	land.Scaling(2, 0.5, 2);

	ShaderProgram landShader("land.vert", "land.frag");
	landShader.CheckStatus();
	landShader.Print_Status();

#pragma region buf
	vector<float> _texture_coord;
	vector<float> _points_position;
	vector<float> _points_normal;
	for (size_t i = 0; i < land.height - 1; i += 1)
	{
		for (size_t j = 0; j < 3 * land.width - 3; j += 3)
		{
			//
			//  p3          p3 - p4 
			//   | \           \ |  
			//  p1 - p2          p2

			//p1
			_points_position.push_back(land.points[i][j + 0]);
			_points_position.push_back(land.points[i][j + 1]);
			_points_position.push_back(land.points[i][j + 2]);
			//p2
			_points_position.push_back(land.points[i][j + 3]);
			_points_position.push_back(land.points[i][j + 4]);
			_points_position.push_back(land.points[i][j + 5]);
			//p3
			_points_position.push_back(land.points[i + 1][j + 0]);
			_points_position.push_back(land.points[i + 1][j + 1]);
			_points_position.push_back(land.points[i + 1][j + 2]);
			//p2
			_points_position.push_back(land.points[i][j + 3]);
			_points_position.push_back(land.points[i][j + 4]);
			_points_position.push_back(land.points[i][j + 5]);
			//p3
			_points_position.push_back(land.points[i + 1][j + 0]);
			_points_position.push_back(land.points[i + 1][j + 1]);
			_points_position.push_back(land.points[i + 1][j + 2]);
			//p4
			_points_position.push_back(land.points[i + 1][j + 3]);
			_points_position.push_back(land.points[i + 1][j + 4]);
			_points_position.push_back(land.points[i + 1][j + 5]);
		}
	}

	int tail_factor = 10;
	for (size_t i = 0; i < land.height - 1; i += 1)
	{
		for (size_t j = 0, k = 0; j < 3 * land.width - 3; j += 3, k++)
		{
			_texture_coord.push_back(0.0f + 1.0f / tail_factor *(k % tail_factor));
			//cout << 0.0f + 1.0f / tail_factor *(k % tail_factor) << ", ";
			_texture_coord.push_back(0.0f + 1.0f / tail_factor *(i % tail_factor));
			//cout << _texture_coord[k + 1] << "; ";

			_texture_coord.push_back(1.0f / tail_factor + 1.0f / tail_factor *(k % tail_factor));
			//cout << _texture_coord[k + 2] << ", ";
			_texture_coord.push_back(0.0f + 1.0f / tail_factor *(i % tail_factor));
			//cout << _texture_coord[k + 3] << "; ";

			_texture_coord.push_back(0.0f + 1.0f / tail_factor *(k % tail_factor));
			//cout << _texture_coord[k + 4] << ", ";
			_texture_coord.push_back(1.0f / tail_factor + 1.0f / tail_factor *(i % tail_factor));
			//cout << _texture_coord[k + 5] << "; ";

			_texture_coord.push_back(1.0f / tail_factor + 1.0f / tail_factor *(k % tail_factor));
			//cout << _texture_coord[k + 6] << ", ";
			_texture_coord.push_back(0.0f + 1.0f / tail_factor *(i % tail_factor));
			//cout << _texture_coord[k + 7] << "; ";

			_texture_coord.push_back(0.0f + 1.0f / tail_factor *(k % tail_factor));
			//cout << _texture_coord[k + 8] << ", ";
			_texture_coord.push_back(1.0f / tail_factor + 1.0f / tail_factor *(i % tail_factor));
			//cout << _texture_coord[k + 9] << "; ";

			_texture_coord.push_back(1.0f / tail_factor + 1.0f / tail_factor *(k % tail_factor));
			//cout << _texture_coord[k + 10] << ", ";
			_texture_coord.push_back(1.0f / tail_factor + 1.0f / tail_factor *(i % tail_factor));
			//cout << _texture_coord[k + 11] << "; ";
			//cout << endl;
		}

	}
	for (size_t i = 0; i < land.height - 1; i += 1)
	{
		for (size_t j = 0; j < 3 * land.width - 3; j += 3)
		{
			//
			//  p3          p3 - p4 
			//   | \           \ |  
			//  p1 - p2          p2

			//p1
			//_points_position.push_back(land.points[i][j + 0]);
			//_points_position.push_back(land.points[i][j + 1]);
			//_points_position.push_back(land.points[i][j + 2]);
			////p2
			//_points_position.push_back(land.points[i][j + 3]);
			//_points_position.push_back(land.points[i][j + 4]);
			//_points_position.push_back(land.points[i][j + 5]);
			////p3
			//_points_position.push_back(land.points[i + 1][j + 0]);
			//_points_position.push_back(land.points[i + 1][j + 1]);
			//_points_position.push_back(land.points[i + 1][j + 2]);
			////p2
			//_points_position.push_back(land.points[i][j + 3]);
			//_points_position.push_back(land.points[i][j + 4]);
			//_points_position.push_back(land.points[i][j + 5]);
			////p3
			//_points_position.push_back(land.points[i + 1][j + 0]);
			//_points_position.push_back(land.points[i + 1][j + 1]);
			//_points_position.push_back(land.points[i + 1][j + 2]);
			////p4
			//_points_position.push_back(land.points[i + 1][j + 3]);
			//_points_position.push_back(land.points[i + 1][j + 4]);
			//_points_position.push_back(land.points[i + 1][j + 5]);
		}
	}

#pragma endregion 

	VBO points_position, texture_coord;
	VAO vao;
	points_position.Init(_points_position.data(), _points_position.size());
	texture_coord.Init(_texture_coord.data(), _texture_coord.size());

	vao.AddVertexBuffer(points_position, 0, 3);
	vao.AddVertexBuffer(texture_coord, 1, 2);


	//size_t * landInd = new size_t[(land.height - 1) * (land.width - 1) * 6];
	//for (size_t i = 0, j = 0; i < (land.width - 1) * (land.height - 1) * 6; i += 6, j++)
	//{
	//	landInd[i + 0] = j* land.width + j;
	//	landInd[i + 1] = (j + 1)* land.width + j;
	//	landInd[i + 2] = j* land.width + (j + 1);
	//	landInd[i + 3] = (j + 1)* land.width + j;
	//	landInd[i + 4] = j * land.width + (j + 1);
	//	landInd[i + 5] = (j + 1)* land.width + (j + 1);
	//}

	/*vector<int> index;
	for (size_t i = 0; i < land.height - 1; i++)
	{
		for (size_t j = 0; j < land.width - 1; j++)
		{
			index.push_back(i* land.width + j);
			index.push_back((i + 1)* land.width + j);
			index.push_back(i* land.width + (j + 1));
			index.push_back((i + 1)* land.width + j);
			index.push_back(i * land.width + (j + 1));
			index.push_back((i + 1)* land.width + (j + 1));
		}
	}*/

	/*matf landTextc(land.width * 2, land.height);
	for (size_t i = 0; i < landTextc.height - 1; i += 2)
	{
		for (size_t j = 0; j < landTextc.width - 3; j += 4)
		{
			landTextc[i][j] = 0.0f;
			landTextc[i][j + 1] = 0.0f;
			landTextc[i][j + 2] = 1.0f;
			landTextc[i][j + 3] = 0.0f;

			landTextc[i + 1][j] = 0.0f;
			landTextc[i + 1][j + 1] = 1.0f;
			landTextc[i + 1][j + 2] = 1.0f;
			landTextc[i + 1][j + 3] = 1.0f;
		}
	}*/

	/*VBO landPos(land.points);
	VBO landText(landTextc);
	EBO landEbo((GLuint*)index.data(), (land.height - 1) * (land.width - 1) * 6);
	VAO landVao;
	landVao.AddVertexBuffer(&landPos, 0, 3);
	landVao.AddVertexBuffer(&landText, 1, 2);
	landVao.AddIndexBuffer(&landEbo);*/

	ShaderProgram mainshader("vert.glsl", "frag.glsl");
	mainshader.CheckStatus();
	mainshader.Print_Status();




	VBO position;
	position.Init(verticesposition, 18);
	VBO textcoord;
	textcoord.Init(verticescoord, 12);
	EBO posindex;
	posindex.Init(verticeindex, 12);

	VAO mesh;
	mesh.AddVertexBuffer(position, 0, 3);
	mesh.AddVertexBuffer(textcoord, 1, 2);
	mesh.AddIndexBuffer(posindex);

	/*Sampler * text = new Sampler[5];

	text[0] = Sampler(mainshader, "grass", 0);
	text[1] = Sampler(mainshader, "dirt", 1);
	text[2] = Sampler(mainshader, "rock1", 2);
	text[3] = Sampler(mainshader, "snow", 3);
	text[4] = Sampler(mainshader, "rock2", 4);


	text[0] = "grass.bmp";
	text[1] = "dirt.bmp";
	text[2] = "rock1.bmp";
	text[3] = "snow.bmp";
	text[4] = "rock2.bmp";*/


	Sampler text1(landShader, "grass", 0);
	text1 = "grass.bmp";
	Sampler text2(landShader, "dirt", 1);
	text2 = "grass1.bmp";
	Sampler text3(landShader, "snow", 2);
	text3 = "snow.bmp";
	Sampler text4(landShader, "rock1", 3);
	text4 = "rock1.bmp";
	Sampler text5(landShader, "rock2", 4);
	text5 = "rock2.bmp";



	Uniform viewMat(landShader, "view");
	Uniform projMat(landShader, "projection");


	cam.SetView(vec3(cameraposition), vec3(cameratarget), vec3(cameraup));
	cam.SetPerspective(90.0f, (float)width / (float)height, 0.1f, 1000.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	light.shaderProgram(landShader);
	light.Color("ambient_light.color");
	light.Color(1, 1, 1, 1);
	light.Intensity("ambient_light.intensity");
	light.Intensity(0.5f);

	float dx = 1, dy = 1;
	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//mainshader.Use();
		landShader.Use();


		cam.Motion(cursordx, cursordy);
		//cam.Pos(cameraposition);
		//cam.Move(50,dx , dy);
		cursordx = 0;
		cursordy = 0;
		viewMat = cam.viewMat;
		projMat = cam.perspectiveMat;

		text1.Bind();
		text2.Bind();
		text3.Bind();
		text4.Bind();
		text5.Bind();
		light.Init();
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, text1.texture2D->textID);
		unsigned int id = glGetUniformLocation(landShader.progID, "grass");
		glUniform1i(id, 0);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, text2.texture2D->textID);
		glUniform1i(glGetUniformLocation(landShader.progID, "dirt"), 1);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, text3.texture2D->textID);
		glUniform1i(glGetUniformLocation(landShader.progID, "snow"), 2);

		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, text4.texture2D->textID);
		glUniform1i(glGetUniformLocation(landShader.progID, "rock1"), 3);

		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, text5.texture2D->textID);
		glUniform1i(glGetUniformLocation(landShader.progID, "rock2"), 4);*/
		/*text2.Bind();
		text1.Bind();*/


		/*text[0].Bind();
		text[1].Bind();
		text[2].Bind();
		text[3].Bind();
		text[4].Bind();*/

		//mesh.Draw(0, 12);
		vao.Draw(0, _points_position.size());
		//land.Draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_W) {
		cam.MoveForward();
	}
	if (key == GLFW_KEY_S) {
		cam.MoveBack();
	}
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		press = 1;
	}
	else {
		press = 0;
		press_mem = 0;
		cursorX = 0;
		cursorY = 0;
	}
}

void cursor_position1_callback(GLFWwindow* window, double x, double y)
{
	if (press) {
		cursorX = x;
		cursorY = y;
		press = 0;
		press_mem = 1;
	}

	if (press_mem) {
		cursordx = x - cursorX;
		cursordy = cursorY - y;
		cursorX = x;
		cursorY = y;
		//	cout << "cursordx " << cursordx << " cursordy " << cursordy << endl;
	}
}
