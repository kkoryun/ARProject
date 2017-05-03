#version 330 core

uniform sampler2D grass;
uniform sampler2D dirt;
uniform sampler2D snow;
uniform sampler2D rock1;
uniform sampler2D rock2;

in float _position;
in vec2  _texture_coord;
in vec3 _normal;
float    _tangent;

out vec4 result_color;

struct AmbientLight
{
    vec4  color;
    float intensity;
};

struct DiffuseLight{
    vec4  color;
    vec3  direction;
    float intensity;
 };

uniform AmbientLight ambient_light;
uniform DiffuseLight diffuse_light;

void getColor() 
{
 if(_position >= 0.0f && _position <= 0.0f)
 {
 result_color = texture(dirt, _texture_coord); 
 }
 if(_position> 0.0 && _position <=2.0)
 {
 result_color = mix( texture(dirt, _texture_coord),texture(grass,_texture_coord),_position/ 2.0);
 }
 if(_position> 2.0 && _position <=3.0)
 {
 result_color=texture(grass, _texture_coord); 
 }
 if(_position > 3 && _position <= 5 )
 {
 result_color= mix( texture(grass, _texture_coord),texture(rock1, _texture_coord),(_position-3)/2.0);
 }
 if(_position > 5 && _position < 10 )
 {
 result_color=texture(rock1, _texture_coord); 
 }
 if(_position >= 10 && _position <= 12)
 {
 result_color = mix( texture(rock1, _texture_coord),texture(rock2, _texture_coord),(_position-10)/2.0);
 }
 if(_position > 12 && _position <= 25)
 {
 result_color=texture(rock2, _texture_coord);
 }
 if(_position > 25 && _position <= 28 ){
 result_color = mix( texture(rock2, _texture_coord),texture(snow, _texture_coord),(_position-25)/3.0);
 }
 if(_position > 28)
 {
 result_color=texture(snow, _texture_coord);
 }
}

void main()
{
vec4 ambient_color =  ambient_light.color * ambient_light.intensity;
vec4 diffuse_color;
getColor();
_tangent = dot(normalize(_normal) , -diffuse_light.direction);

 if (_tangent > 0){
   diffuse_color = diffuse_light.color * diffuse_light.intensity * _tangent;
 }
else{
  diffuse_color = vec4(0,0,0,0) ;
}
result_color = result_color * (ambient_color + diffuse_color);
}