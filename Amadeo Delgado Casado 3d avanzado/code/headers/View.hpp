
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on may of 2014                                            *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef VIEW_HEADER
#define VIEW_HEADER

    #include <GL/glew.h>            // Debe incluirse antes que gl.h
	#include <glm/glm.hpp>  

	#include "Mesh.hpp"
	#include "Camera.hpp"
    #include <string>
	#include <vector>

#include "PostProcess.h"

    namespace example
    {

        class View
        {
		private:  

            GLint  projection_matrix_id;	
			PostProcess postrpro;

        public:

			///List of meshes
			std::vector<Mesh*> drawables;

			Camera camera;

            View(int width, int height);
			
			///Calls the update of every mesh
            void   update ();

			///Renders the meshes (0 = Default, 1 = transparent, Skybox = 2)
            void   render ();

			///Resizes camera view and the frambuffer
            void   resize (int width, int height);

        };

    }

#endif
