#pragma once
#include "data.h"
#include "Img.h"


class Landscape
{

public:

	Landscape() {
		scaleX = scaleY = scaleZ = 1;
		width = height = 0;
	}

	Landscape(string height_map_file_name) {
		scaleX = scaleY = scaleZ = 1;
		height_map_name = height_map_file_name;
		create_points_mat(height_map_name);
		create_landscape();

	};

	~Landscape() {
	}

	void Draw() {
		vao.Bind();
		vao.Draw(0, (height - 1) * (width - 1) * 6);
	}

	void Create(string height_map_file_name) {
		create_points_mat(height_map_file_name);
		create_landscape();
	}

	void Scaling(float scale) {
		scaleX = scaleY = scaleZ = scale;
		scaling();
		create_points_mat(height_map_name);
		create_landscape();
	}

	void Scaling(float scalex, float scaley, float scalez) {
		scaleX = scalex;
		scaleY = scaley;
		scaleZ = scalez;
		scaling();
	}

	mat<float> points;
	float scaleX, scaleY, scaleZ;
	int width, height; // in points = (x,y,z)

	VBO points_position;
	VBO texture_coord;
	EBO vertex_index;
	VAO vao;

	string height_map_name;

private:
	vector<int> index;

	void create_landscape() {

		vector<float> _texture_coord;
		vector<float> _points_position;

		for (size_t i = 0; i < height - 1; i += 1)
		{
			for (size_t j = 0; j < 3 * width - 3; j += 3)
			{


				_texture_coord.push_back(0.0f);
				_texture_coord.push_back(0.0f);

				_texture_coord.push_back(1.0f);
				_texture_coord.push_back(0.0f);

				_texture_coord.push_back(0.0f);
				_texture_coord.push_back(1.0f);

				_texture_coord.push_back(1.0f);
				_texture_coord.push_back(0.0f);

				_texture_coord.push_back(0.0f);
				_texture_coord.push_back(1.0f);

				_texture_coord.push_back(1.0f);
				_texture_coord.push_back(1.0f);

				//
				//  p3 - p4
				//   | \ |
				//  p1 - p2

				//p1
				_points_position.push_back(points[i][j + 0]);
				_points_position.push_back(points[i][j + 1]);
				_points_position.push_back(points[i][j + 2]);
				//p2
				_points_position.push_back(points[i][j + 3]);
				_points_position.push_back(points[i][j + 4]);
				_points_position.push_back(points[i][j + 5]);
				//p3
				_points_position.push_back(points[i + 1][j + 0]);
				_points_position.push_back(points[i + 1][j + 1]);
				_points_position.push_back(points[i + 1][j + 2]);
				//p2
				_points_position.push_back(points[i][j + 3]);
				_points_position.push_back(points[i][j + 4]);
				_points_position.push_back(points[i][j + 5]);
				//p3
				_points_position.push_back(points[i + 1][j + 0]);
				_points_position.push_back(points[i + 1][j + 1]);
				_points_position.push_back(points[i + 1][j + 2]);
				//p4
				_points_position.push_back(points[i + 1][j + 3]);
				_points_position.push_back(points[i + 1][j + 4]);
				_points_position.push_back(points[i + 1][j + 5]);
			}
		}

		points_position.Init(_points_position.data(), _points_position.size());
		texture_coord.Init(_texture_coord.data(), _texture_coord.size());

			vao.AddVertexBuffer(points_position, 0, 3);
			vao.AddVertexBuffer(texture_coord, 1, 2);

	}

	void create_index() {
		for (size_t i = 0; i < height - 1; i++)
		{
			for (size_t j = 0; j < width - 1; j++)
			{
				index.push_back(i* width + j);
				index.push_back((i + 1)* width + j);
				index.push_back(i* width + (j + 1));
				index.push_back((i + 1)* width + j);
				index.push_back(i * width + (j + 1));
				index.push_back((i + 1)* width + (j + 1));
			}
		}
	}

	matf create_texture_coord_mat() {
		matf textc(width * 2, height);
		for (size_t i = 0; i < height - 1; i += 2)
		{
			for (size_t j = 0; j < width - 3; j += 4)
			{
				textc[i][j] = 0.0f;
				textc[i][j + 1] = 0.0f;
				textc[i][j + 2] = 1.0f;
				textc[i][j + 3] = 0.0f;

				textc[i + 1][j] = 0.0f;
				textc[i + 1][j + 1] = 1.0f;
				textc[i + 1][j + 2] = 1.0f;
				textc[i + 1][j + 3] = 1.0f;
			}
		}
		return textc;
	}

	void create_points_mat(string  filename)
	{
		Bitmap height_map_image(filename, COMPACT_GRAY);

		//in pixel
		width = height_map_image.width;
		height = height_map_image.height;

#ifdef _DEBUG_
		cout << "height map data" << endl;
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width; j++)
			{
				cout << (int)height_map_image.data[i * width + j] << " ";
			}
			cout << endl;
		}
#endif // _DEBUG_


		points = matf(width * 3, height);

		for (int i = 0, z = -height / 2; i < height; i++, z++)
		{
			for (int j = 0,k = 0, x = -width / 2; j < width && k < width *3 ; j++, x++,k+=3) {
				points[i][k + 0] = x * scaleX;
				points[i][k + 1] = height_map_image.data[i*width + j] * scaleY;
				points[i][k + 2] = z * scaleZ;
			}
		}

#ifdef _DEBUG_
		cout << "points mat data" << endl;
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width * 3 ; j += 3)
			{
				cout << points[i][j + 0] << " " <<
					    points[i][j + 1] << " " <<
					    points[i][j + 2] << " ";
			}
			cout << endl;
		}
#endif // _DEBUG_

	}

	void scaling() {
		for (size_t i = 0; i < height; i++) {
			for (size_t j = 0; j < width * 3; j += 3) {
				points[i][j + 0] *= scaleX;
				points[i][j + 1] *= scaleY;
				points[i][j + 2] *= scaleZ;
			}
		}
	}

};