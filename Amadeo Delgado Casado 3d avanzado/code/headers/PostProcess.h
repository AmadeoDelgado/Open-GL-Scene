#pragma once
#include "Shader.hpp"

class PostProcess {

public:

	GLuint program_id;

	unsigned int FBO;
	unsigned int texture;
	unsigned int RBO;
	unsigned int VAO;
	unsigned int VBO;



	float vertices[24] = {

		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};



	const char * vertex_shader =
		"#version 330 core\n"
		""
		""
		"layout(location = 0) in vec2 aPos;"
		"layout(location = 1) in vec2 aTexCoord;"
		""
		"out vec2 TexCoord;"
		""
		"void main()"
		"{"
		"gl_Position = vec4(aPos.x,aPos.y,0,1.0);"
		"TexCoord = vec2(aTexCoord.x, aTexCoord.y);"
		"}";

	const char * frag_shader =
		"#version 330 core\n"
		""
		""
		"out vec4 FragColor;"
		""
		"in vec2 TexCoord;"
		"uniform sampler2D texture1;"
		""
		"void main()"
		"{"
		"vec3 col = texture(texture1,TexCoord).rgb;"
		""
		//Normal
		//"FragColor = vec4(col.r, col.g, col.b,1.0);"
		//Inverse colors
		"FragColor = vec4(1-col.r, 1-col.g, 1-col.b,1.0);"
		"}";

	example::Shader shader{ frag_shader,vertex_shader };
	
	///Default constructor that geneartes the framebuffer
	PostProcess();
	
	///Resizes the frame buffer
	void resize(int lenght, int height);
	
};

