#version 330

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;

out vec2 TextureCoordinate;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	gl_Position = projection * model * vec4(pos.xy, 0.0, 1.0);
	
	TextureCoordinate = tex;
}