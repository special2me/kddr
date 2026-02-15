#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define DEFAULT_POS glm::vec3(0.f, 0.f, 0.f)
#define DEFAULT_UP glm::vec3(0.f, 1.f, 0.f)
#define DEFAULT_YAW -90.f
#define DEFAULT_PITCH 0.f
#define DEFAULT_FRONT glm::vec3(0.f, 0.f, -1.f)
#define DEFAULT_SPEED 10.f
#define DEFAULT_SENSITIVITY 0.1f
#define DEFAULT_ZOOM 45.f

#define Z_NEAR 0.1f
#define Z_FAR 100.f

class Camera {
	public:
		enum Movement {
			NONE, FORWARD, BACKWARDS, LEFT, RIGHT
		};

		Camera(glm::vec3 pos = DEFAULT_POS, glm::vec3 up = DEFAULT_UP, float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH) {
			m_Front = DEFAULT_FRONT;
			m_Speed = DEFAULT_SPEED;
			m_Sensitivity = DEFAULT_SENSITIVITY;
			m_Zoom = DEFAULT_ZOOM;

			m_Pos = pos;
			m_WorldUp = up;
			m_Yaw = yaw;
			m_Pitch = pitch;

			updateCameraVectors();
		}

		glm::mat4 getProjectionMatrix(int screenWidth, int screenHeight) {
			return glm::perspective(glm::radians(m_Zoom), (float)screenWidth / (float)screenHeight, Z_NEAR, Z_FAR);
		}

		glm::mat4 getViewMatrix() {
			return glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
		}

		void updatePosition(Movement direction, float deltaTime);
		void updateRotation(float xoff, float yoff);

	private:
		glm::vec3 m_Pos, m_Up;
		glm::vec3 m_Front, m_Right;
		glm::vec3 m_WorldUp;

		GLfloat m_Yaw, m_Pitch;
		GLfloat m_Speed, m_Sensitivity;
		GLfloat m_Zoom;

		void updateCameraVectors();
};

