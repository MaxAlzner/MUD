#include "../include/MUD_main.h"

#ifdef MUD_DRAW_H_

string VertexShader = 
	"#version 120\n"

	"in vec4 vertex;\n"

	"out vec2 tex_coord;\n"

	"uniform vec2 screen;\n"
	"uniform vec2 position;\n"
	"uniform vec2 dimensions;\n"
	"uniform float rotation;\n"
	
	"void main()\n"
	"{\n"
	"	vec2 scaled = vertex.xy * dimensions;\n"
	"	vec2 pivot = vec2(cos(rotation), sin(rotation));\n"
	"	vec2 rotated = vec2((scaled.x * pivot.x) - (scaled.y * pivot.y), (scaled.x * pivot.y) + (scaled.y * pivot.x));\n"

	"	vec2 mapped = ((rotated * 2.0) / screen) - 1.0;\n"
	"	mapped += position / (screen / 2.0);\n"

	"	tex_coord = vertex.zw;\n"
	"	gl_Position = vec4(mapped, 0.0, 1.0);\n"
	"}\n";
string FragmentShader = 
	"#version 120\n"

	"in vec2 tex_coord;\n"

	"uniform vec3 color;\n"
	
	"void main()\n"
	"{\n"
	"	float v = length((tex_coord * 2.0) - 1.0);\n"
	//"	if (v > 1.0) discard;\n"

	"	v = ((1.0 - v) * 0.5) + 0.5;\n"
	"	gl_FragColor = vec4(v);\n"
	"}\n";

void DrawSprite(uint sprite, int x, int y, uint width, uint height, float rotation)
{
	glUniform2f(Uniforms.position, float(x), float(y));
	glUniform2f(Uniforms.dimensions, float(width), float(height));
	glUniform1f(Uniforms.rotation, MALib::ToRadians(rotation));
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

#endif