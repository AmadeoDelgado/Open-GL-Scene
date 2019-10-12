
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on may of 2014                                            *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <cassert>
#include "View.hpp"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>


using namespace sf;
using namespace example;

int main()
{
	// Se crea la ventana de SFML, que es donde se creará el contexto de OpenGL:


	Window window(VideoMode(800, 600), "OpenGL Examples: Simple Cube", Style::Default, ContextSettings(32));

	// Una vez se ha creado el contexto de OpenGL ya se puede inicializar Glew:

	GLenum glew_initialization = glewInit();

	assert(glew_initialization == GLEW_OK);

	// Glew se inicializa antes de crear view porque view ya usa extensiones de OpenGL:

	View view(800, 600);

	window.setVerticalSyncEnabled(true);

	bool running = true;



	do
	{
		//camera variables
		float lastX = 400, lastY = 300, yaw = 0, pitch = 0;
		bool firstImput=false;
		


		Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{

			case Event::Closed:
			{
				running = false;
				break;
			}

			case Event::Resized:
			{
				Vector2u window_size = window.getSize();

				view.resize(window_size.x, window_size.y);

				break;
			}

			case Event::MouseMoved:
			{


			}


			case Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::S)
				{
					
					view.camera.SetPosition(view.camera.GetPos() - view.camera.GetSpeed() * view.camera.GetFront());
				}
				if (event.key.code == sf::Keyboard::W)
				{
					view.camera.SetPosition(view.camera.GetPos() + view.camera.GetSpeed() * view.camera.GetFront());
				}

				if (event.key.code == sf::Keyboard::A)
				{
					
					view.camera.SetPosition(view.camera.GetPos() - glm::normalize((glm::cross(view.camera.GetFront(), view.camera.GetUp()))*view.camera.GetSpeed()));
				}
				if (event.key.code == sf::Keyboard::D)
				{
					
					view.camera.SetPosition(view.camera.GetPos() + glm::normalize((glm::cross(view.camera.GetFront(), view.camera.GetUp()))*view.camera.GetSpeed()));
				}

				

			}

			
			}

			
		}

		glClear(GL_COLOR_BUFFER_BIT);

		view.update();
		view.render();

		window.display();
	}
		while (running);

		return (EXIT_SUCCESS);
	}

