#version 330 core

out vec4 rescolor;

in vec2 textcoord;
in float pos;

uniform sampler2D grass;
uniform sampler2D dirt;
uniform sampler2D snow;
uniform sampler2D rock;


void main()
{
    rescolor=vec4(0,0,0,0);//texture(rock, textcoord); 
	//rescolor =vec4(0.8,0.3,0.2,0);//mix(texture(grass, textcoord), texture(dirt, textcoord), 0.8);//vec4(textcoord,0.0f,0.0f);//vec4(0.8,0.3,0.2,0); 
	
	
	
	
}
