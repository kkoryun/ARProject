#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 _textcoord;

out vec2 textcoord;
out float pos;


uniform mat4 view;
uniform mat4 projection;

void main()
{
    pos = position.y;
    textcoord = _textcoord;
    gl_Position = projection * view * vec4(position,1.0);
}