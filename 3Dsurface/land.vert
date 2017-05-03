#version 330 core


uniform mat4 view;
uniform mat4 projection;



layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coord;
layout (location = 2) in vec3 normal;

out float _position;
out vec2 _texture_coord;
out vec3 _normal;

void main()
{
	_texture_coord = texture_coord;
	_position = position.y;
    _normal = normal;

    gl_Position = projection * view * vec4(position,1.0);
}