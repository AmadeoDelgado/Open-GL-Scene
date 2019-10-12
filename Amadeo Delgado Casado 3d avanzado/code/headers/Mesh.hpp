#pragma once

#include <SFML/OpenGL.hpp>
#include <vector>
#include "Shader.hpp"
#include "Camera.hpp"



	class Mesh
	{
	private:

		

		enum
		{
			COORDINATES_VBO,
			COLORS_VBO,
			INDICES_IBO,
			VBO_COUNT
		};

		// Arrays de datos del cubo base:

		Mesh * father = NULL;

		std::vector <glm::vec3> temporal_coods;
		std::vector <glm::vec3> temporal_normals;
		std::vector <glm::vec2> temporal_uvCoords;

		
		std::vector<GLubyte> index;
	

		std::vector<GLfloat> objectData;



	private:

		GLuint vbo_ids[VBO_COUNT];      // Ids de los VBOs que se usan
		GLuint vao_id;                  // Id del VAO del cubo

		unsigned int VBO, VAO, EBO;
		unsigned int texture;


	public:

		// 0 normal, 1 transparent, 2 skybox
		size_t type = 0;

		glm::mat4 modelViewMatrix;
		glm::vec3 rotation{ 0.f, 0.f, 0.f };
		glm::vec3 meshposition{ 0.f, 0.f, 0.f };

		Mesh(const char *,const char* textureDir );
		~Mesh();

		void render(example::Camera &, const char* fragShader = nullptr, const char* vertexShader = nullptr);
		void update(example::Camera &); 
		void rotate(glm::vec3);        
		void position(glm::vec3);
		void SetParent(Mesh *);

		
		


	};

