#include "../include/MUD_main.h"

#ifdef MUD_DRAW_H_

string VertexShader = 
	"#version 120\n"

	"attribute vec4 vertex;\n"

	"varying vec2 tex_coord;\n"

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
	
	"#define SHAPE_CIRCLE 1\n"
	"#define SHAPE_BOX 2\n"
	"#define SHAPE_ROUNDED_BOX 3\n"

	"varying vec2 tex_coord;\n"

	"uniform sampler2D color_map;\n"
	"uniform vec4 color;\n"
	"uniform float borderRadius;\n"

	"uniform int shape;\n"

	"void circle()\n"
	"{\n"
	"	float v = length((tex_coord * 2.0) - 1.0);\n"
	"	if (v > 1.0) discard;\n"
	"	v = ((1.0 - v) * 0.25) + 0.75;\n"
	"	gl_FragColor = color * v;\n"
	"}\n"
	"void box()\n"
	"{\n"
	"	gl_FragColor = color;\n"
	"}\n"
	"void roundedBox()\n"
	"{\n"
	"	vec2 c = (tex_coord * 2.0) - 1.0;\n"
	"	float v = length(c);\n"

	"	v = ((1.0 - v) * 0.25) + 0.75;\n"
	"	gl_FragColor = color * v;\n"
	"}\n"
	
	"void main()\n"
	"{\n"
	"	switch (shape)\n"
	"	{\n"
	"	case SHAPE_CIRCLE: circle(); break;\n"
	"	case SHAPE_BOX: box(); break;\n"
	"	case SHAPE_ROUNDED_BOX: roundedBox(); break;\n"

	"	default: break;\n"
	"	}\n"
	"}\n";

void DrawSprite(uint sprite, int x, int y, uint width, uint height, float rotation, MALib::COLOR color)
{
	glUniform1i(Uniforms.color_map, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite);

	glUniform2f(Uniforms.position, float(x - ScreenRect.x0), float(y - ScreenRect.y0));
	glUniform2f(Uniforms.dimensions, float(width), float(height));
	glUniform1f(Uniforms.rotation, MALib::ToRadians(rotation));
	glUniform4f(Uniforms.color, color.r, color.g, color.b, color.a);
	glUniform1i(Uniforms.shape, 2);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void DrawCircle(int x, int y, uint radius, MALib::COLOR color)
{
	glUniform2f(Uniforms.position, float(x - ScreenRect.x0), float(y - ScreenRect.y0));
	glUniform2f(Uniforms.dimensions, float(radius) * 2.0f, float(radius) * 2.0f);
	glUniform4f(Uniforms.color, color.r, color.g, color.b, color.a);
	glUniform1i(Uniforms.shape, 1);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void DrawBox(int x, int y, uint width, uint height, float rotation, MALib::COLOR color)
{
	glUniform2f(Uniforms.position, float(x - ScreenRect.x0), float(y - ScreenRect.y0));
	glUniform2f(Uniforms.dimensions, float(width), float(height));
	glUniform1f(Uniforms.rotation, MALib::ToRadians(rotation));
	glUniform4f(Uniforms.color, color.r, color.g, color.b, color.a);
	glUniform1i(Uniforms.shape, 2);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void DrawRoundedBox(int x, int y, uint width, uint height, float borderRadius, float rotation, MALib::COLOR color)
{
	glUniform2f(Uniforms.position, float(x - ScreenRect.x0), float(y - ScreenRect.y0));
	glUniform2f(Uniforms.dimensions, float(width), float(height));
	glUniform1f(Uniforms.rotation, MALib::ToRadians(rotation));
	glUniform4f(Uniforms.color, color.r, color.g, color.b, color.a);
	glUniform1f(Uniforms.borderRadius, borderRadius);
	glUniform1i(Uniforms.shape, 3);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

#endif