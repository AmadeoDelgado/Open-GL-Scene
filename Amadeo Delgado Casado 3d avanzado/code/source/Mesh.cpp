#define TINYOBJLOADER_IMPLEMENTATION
#define _USE_MATH_DEFINES

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



#include <GL/glew.h>  
#include "Mesh.hpp"
#include <SFML/OpenGL.hpp>

#include "tiny_obj_loader.h"
#include <iostream>
#include <string>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>   



Mesh::Mesh(const char* filedir, const char* textureDir)
{
	//load 3d model

	
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filedir);

	if (!warn.empty()) {
		std::cout << warn << std::endl;
	}

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}

	// Loop over shapes

	for (size_t s = 0; s < shapes.size(); s++) 
	{
		for (size_t c = 0; c < attrib.vertices.size(); c += 3)
		{
			float coord[3];
			for (size_t i = 0; i < 3; ++i) 
			{
				coord[i] = attrib.vertices[c+i];
			}
			temporal_coods.push_back( glm::vec3{ coord[0],coord[1],coord[2] });
		}
		
		for (size_t c = 0; c < attrib.normals.size(); c += 3)
		{
			float normal[3];
			for (size_t i = 0; i < 3; ++i)
			{
				normal[i] = attrib.normals[c + i];
			}
			temporal_normals.push_back(glm::vec3{ normal[0],normal[1],normal[2] });
		}
		
		for (size_t c = 0; c < attrib.texcoords.size(); c += 2)
		{
			float uvs[2];
			for (size_t i = 0; i < 2; ++i)
			{
				uvs[i] = attrib.texcoords[c + i];
			}
			temporal_uvCoords.push_back(glm::vec2{ uvs[0],uvs[1] });
		}
	}


		for (auto i : shapes[0].mesh.indices)
		{
			index.push_back(i.vertex_index);
			
		}



		for (auto index : shapes[0].mesh.indices)
		{
			
			objectData.push_back(temporal_coods[index.vertex_index].x);
			objectData.push_back(temporal_coods[index.vertex_index].y);
			objectData.push_back(temporal_coods[index.vertex_index].z);

			objectData.push_back(temporal_normals[index.normal_index].x);
			objectData.push_back(temporal_normals[index.normal_index].y);
			objectData.push_back(temporal_normals[index.normal_index].z);

			objectData.push_back(temporal_uvCoords[index.texcoord_index].x);
			objectData.push_back(temporal_uvCoords[index.texcoord_index].y);

	
		}

		std::cout << sizeof(float) << std::endl;
	

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, objectData.size() * sizeof(GLfloat),objectData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLubyte), index.data(), GL_STATIC_DRAW);

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	
	// normals attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	
	// texture coord attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	
	//load texture

	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping / filtering options(on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load image
	int width, height, nrChannels;
	unsigned char *data = stbi_load(textureDir, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	}


Mesh::~Mesh()
{
	// Se liberan los VBOs y el VAO usados:

	glDeleteVertexArrays(1, &vao_id);
	glDeleteBuffers(VBO_COUNT, vbo_ids);
}

void Mesh::render(example::Camera & camera, const char* fragShader, const char* vertexshader)
{
	GLuint program_id;
	if (fragShader == nullptr || vertexshader == nullptr)
	{
		example::Shader shader;
		program_id =  shader.compile_shaders();

		glUseProgram(program_id);

		GLint model_view_matrix_id = glGetUniformLocation(program_id, "model_view_matrix");
		GLint projection_matrix_id = glGetUniformLocation(program_id, "projection_matrix");
		GLint light_color = glGetUniformLocation(program_id, "light_color");
		GLint lightPos = glGetUniformLocation(program_id, "lightPos");

		//Rotated normal matrix for ilumination
		GLint normal_matrix_id = glGetUniformLocation(program_id, "normal_matrix");

		glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
		glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(camera.Calculate_projectionMatrix(800, 600)));

		glUniformMatrix4fv(normal_matrix_id, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(modelViewMatrix))));

		glUniform4f(light_color, 1.0f, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPos, 1.0f, 1.0f, 1.0f);

	}

	else
	{
		example::Shader shader{fragShader,vertexshader};
		program_id = shader.compile_shaders();

		glUseProgram(program_id);

		GLint model_view_matrix_id = glGetUniformLocation(program_id, "model_view_matrix");
		GLint projection_matrix_id = glGetUniformLocation(program_id, "projection_matrix");

		glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
		glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(camera.Calculate_projectionMatrix(800, 600)));
	}
	 
	

	glEnable(GL_CULL_FACE);
	glBindVertexArray(VAO);
	// bind Texture
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, objectData.size());
	
	glBindVertexArray(0);
}

void Mesh::update(example::Camera & camera )
{
	
	modelViewMatrix = camera.Calculate_modelViewMatrix();
	
}

void Mesh::rotate(glm::vec3 nrotation)
{	
	rotation += nrotation;
	if (!father) {
		modelViewMatrix = glm::rotate(modelViewMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		modelViewMatrix = glm::rotate(modelViewMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		modelViewMatrix = glm::rotate(modelViewMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
	}

	else {
		modelViewMatrix = glm::rotate(father->modelViewMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		modelViewMatrix = glm::rotate(father->modelViewMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		modelViewMatrix = glm::rotate(father->modelViewMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
	}

}

void Mesh::position(glm::vec3 nposition)
{
	if(!father)
	modelViewMatrix = glm::translate(modelViewMatrix, nposition);
	else modelViewMatrix = glm::translate(father->modelViewMatrix, nposition);
	
}

void Mesh::SetParent(Mesh * parent)
{
	father = parent;
}

