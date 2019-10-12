#ifndef SHADER_HEADER
#define SHADER_HEADER

#include <string>
#include <GL/glew.h>            // Debe incluirse antes que gl.h
#include <glm/glm.hpp>  

namespace example {

	class Shader
	{
	public:

		///Default vertex shader with ilumination and transparency
		const std::string   vertex_shader_code =
			"#version 330 core\n"
			""
			"uniform mat4 model_view_matrix;"
			"uniform mat4 projection_matrix;"
			"uniform mat4 normal_matrix;"
			""
			"layout(location = 0) in vec3 aPos;"
			"layout(location = 1) in vec3 aNormal;"
			"layout(location = 2) in vec2 aTexCoord;"
			""
			"out vec3 Normal;"
			"out vec2 TexCoord;"
			"out vec3 FragPos;"
			""
			"void main()"
			"{"
			"gl_Position = projection_matrix * model_view_matrix * vec4(aPos, 1.0);"
			"FragPos = vec3(model_view_matrix * vec4(aPos, 1.0));"
			"Normal = vec3(normal_matrix * vec4(aNormal,1.0));"
			"TexCoord = vec2(aTexCoord.x, aTexCoord.y);"
			"}";;
		
		///Default fragment shader with ilumination and transparency
		const std::string fragment_shader_code =
			"#version 330 core\n"
			""
			"float ambientStrength = 0.1f;"
			""
			"in vec3 FragPos;"
			"in vec3 Normal;"
			"in vec2 TexCoord;"
			""
			"out vec4 FragColor;"
			""
			"uniform sampler2D texture1;"
			"uniform vec4 light_color;"
			"uniform vec3 lightPos;"
			""
			"void main()"
			"{"
			"vec3 norm = normalize(Normal);"
			"vec3 lightDir = normalize(lightPos - FragPos);"
			"float diff = max(dot(norm, lightDir), 0.0);"
			"vec4 diffuse = diff * light_color;"
			""
			"FragColor = texture(texture1, TexCoord) * (diffuse + ambientStrength )* vec4(1,1,1,0.7f);"
			"}";

		GLint  model_view_matrix_id;
		GLint  projection_matrix_id;
		GLint  light_color;
		GLint  lightPos;
		
		///Compiles and links the shaders
		GLuint compile_shaders();

		void   show_compilation_error(GLuint  shader_id);
		void   show_linkage_error(GLuint program_id);

		///Default constructor
		inline Shader() {  };
		///Constructor with custom shaders
		inline Shader(const char*frag,const char * vert) : fragment_shader_code(frag),vertex_shader_code(vert){  };
		

	private:

	};

}
#endif // SHADER_HEADER

