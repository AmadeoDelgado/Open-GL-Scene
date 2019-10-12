
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on may of 2014                                            *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "View.hpp"
#include <iostream>
#include <cassert>

                        // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr

namespace example
{

    using namespace std;

	
	

	View::View(int width, int height)
    {

        // Se establece la configuración básica:
		
        glEnable     (GL_CULL_FACE);
        glClearColor (1.f, 0.5f, 0.f, 1.f);

		drawables.push_back(new Mesh("..\\..\\assets\\skybox.obj", "..\\..\\assets\\skybox.jpg"));
		drawables.push_back(new Mesh("..\\..\\assets\\floor.obj","..\\..\\assets\\GroundTexture.jpg"));			
		drawables.push_back(new Mesh("..\\..\\assets\\water.obj", "..\\..\\assets\\water.jpg"));
		drawables.push_back(new Mesh("..\\..\\assets\\sun.obj", "..\\..\\assets\\sun.jpg"));

		
		drawables[2]->SetParent(drawables[1]);
		drawables[2]->type = 1;
		drawables[0]->type = 2;
		

		
        resize (width, height);
    }

    void View::update ()
    {
		


		for (auto mesh : drawables)
		{
			mesh->update(camera);
		}


		
		drawables[3]->rotate({ 30,30,0 });
		drawables[3]->position({ 0,20,10 });

		drawables[1]->position({ 0,-10,0 });
		drawables[1]->rotate({ 0,30,0 });
		
		drawables[0]->position(camera.GetPos());

		
		
		
		drawables[2]->position({ -7,-1,-6 });
		

		

		
       
    }

    void View::render ()
    {
		glBindFramebuffer(GL_FRAMEBUFFER, postrpro.FBO);

		glEnable(GL_DEPTH_TEST);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		for (auto d : drawables) {

			switch (d->type)
			{
				//Normal 
			case 0:
				d->render(camera);
				break;
				//transparent
			case 1:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				d->render(camera);
				glDisable(GL_BLEND);
				break;
				//Skybox
			case 2:
				const char * vertex_shader_code =
					"#version 330 core\n"
					""
					"uniform mat4 model_view_matrix;"
					"uniform mat4 projection_matrix;"
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
					"Normal = aNormal;"
					"TexCoord = vec2(aTexCoord.x, aTexCoord.y);"
					"}";

				const char * fragment_shader_code =
					"#version 330 core\n"
					""
					""
					"in vec3 FragPos;"
					"in vec3 Normal;"
					"in vec2 TexCoord;"
					""
					"out vec4 FragColor;"
					""
					"uniform sampler2D texture1;"
					""
					"void main()"
					"{"
					""
					"FragColor = texture(texture1, TexCoord);"
					"}";
				d->render(camera,fragment_shader_code,vertex_shader_code);
				break;
			
			}
			
		}

		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

		// clear all relevant buffers

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(postrpro.program_id);
		glBindVertexArray(postrpro.VAO);

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, postrpro.texture);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
    }

		void View::resize (int width, int height)
		{
			glm::mat4 projection_matrix = camera.Calculate_projectionMatrix(width, height);

			glUniformMatrix4fv (projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

			postrpro.resize(width, height);

			glViewport (0, 0, width, height);
		}

   

}
