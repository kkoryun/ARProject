#version 330 core

uniform sampler text2D;

in float _position;
in vec2 _texture_coord;
in vec3 _normal;

struct DirectionalLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
    vec3 Direction;
};

out vec4 result_color;

void main {

 vec4 AmbientColor = vec4(gDirectionalLight.Color, 1.0f) *
                gDirectionalLight.AmbientIntensity;

float DiffuseFactor = dot(normalize(_normal), -gDirectionalLight.Direction);

vec4 DiffuseColor;
 
if (DiffuseFactor > 0){
DiffuseColor = vec4(gDirectionalLight.Color, 1.0f) * gDirectionalLight.DiffuseIntensity *
           DiffuseFactor;
}
else{
    DiffuseColor = vec4(0, 0, 0, 0);

result_color = texture2D(text2D, _texture_coord) * (AmbientColor + DiffuseColor);

}