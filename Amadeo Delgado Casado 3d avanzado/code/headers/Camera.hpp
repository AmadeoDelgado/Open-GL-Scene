#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include <GL/glew.h>            // Debe incluirse antes que gl.h
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>    

 namespace example {
	 ///This class is used for calculate the proyection matrix of the scene, can be moved and rotated
	class Camera
	{
	public:
		Camera()=default;
		
		///Returns the model_view, using "look at" of the camera
		inline glm::mat4 Calculate_modelViewMatrix() {

			return  glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		};

		///Returns the perspective of the camera
		inline glm::mat4 Calculate_projectionMatrix(int width, int height) {

			return glm::perspective(fov, GLfloat(width) / height, clipNear,farZ);	
		};

		//setters
		inline void SetPosition(glm::vec3 pos) { cameraPos = pos; }
		inline void SetFront(glm::vec3 front) { cameraFront = front; }
		inline void SetSpeed(float speed) { cameraSpeed = speed; }

		//getters
		inline glm::vec3 GetPos() { return cameraPos; }
		inline glm::vec3 GetFront() { return cameraFront; }
		inline glm::vec3 GetUp() { return cameraUp; }
		inline float GetSpeed() { return cameraSpeed; }

	private:

		float fov = 90.f;
		float clipNear = 0.1f;
		float farZ = 500.f;

		glm::vec3 cameraPos = glm::vec3(0.0f, -5.0f, 25.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		float cameraSpeed = 0.2f;

	};
}





#endif // !CAMERA_HEADER

