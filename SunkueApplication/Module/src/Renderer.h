#pragma once

#include "Shader.h"
#include "Actor.h"

class Camera
{
	Eigen::Vector3f position;
	Eigen::Vector3f target;
	Eigen::Vector3f up;
public:
	Eigen::Matrix4f viewMatrix;
	Eigen::Matrix4f projectionMatrix;
public:
	void lookAt(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up)
	{
		Eigen::Matrix3f R;
		R.col(2) = (position - target).normalized();
		R.col(0) = up.cross(R.col(2)).normalized();
		R.col(1) = R.col(2).cross(R.col(0));
		viewMatrix.topLeftCorner<3, 3>() = R.transpose();
		viewMatrix.topRightCorner<3, 1>() = -R.transpose() * position;
		viewMatrix(3, 3) = 1.0f;
	}

	void setPerspective(float fovY, float aspect, float n, float f)
	{
		float theta = fovY * 0.5;
		float range = f - n;
		float invtan = 1. / tan(theta);
		projectionMatrix(0, 0) = invtan / aspect;
		projectionMatrix(1, 1) = invtan;
		projectionMatrix(2, 2) = -(n + f) / range;
		projectionMatrix(3, 2) = -1;
		projectionMatrix(2, 3) = -2 * n * f / range;
		projectionMatrix(3, 3) = 0;
	}
};

class Renderer
{
public:
	Renderer(GLFWwindow* w) : window{ w } { ScreenQuad(); }
private:
	using ActorsList = std::list<std::reference_wrapper<Actor>>;
	ActorsList Actors;
	Camera mainCamera;
	GLFWwindow* window;
public:
	void Render() {
		Clear();
		Draw();
	}
private:
	void RegisterActor(Actor& actor) {
		Actors.push_back({ actor });
	};
	void UnRegisterActor(const Actor& actor) {
		Actors.remove_if([&](const auto& a) { return &a.get() == &actor; });
	};

	void Clear()
	{
		glClearColor(0.2f, 0, 0.3f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Draw()
	{

		for (auto& a : Actors) {
			a.get().f(); 
			// use shader
			// update uniforms
			// bind vao
			// draw call with mode
		}

		glBindVertexArray(QuadVao_);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	GLuint QuadVao_;
	void ScreenQuad()
	{
		glGenVertexArrays(1, &QuadVao_);
		GLuint vbo;
		glGenBuffers(1, &vbo);

		struct QUAD
		{
			Eigen::Vector2f pos;
			Eigen::Vector2f tex;
		};

		QUAD quadv[] =
		{
			{ {-0.2,-1},{0,0} }
			,{{ 1, 1},{1,0.4} }
			,{{-0.2, 1},{0,1} }
			,{{-1,-1},{0,0} }
			,{{ 1,-1},{1,0} }
			,{{ 1, 1},{1,1} }
		};

		glBindVertexArray(QuadVao_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadv), quadv, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QUAD), (const GLvoid*)offsetof(QUAD, pos));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QUAD), (const GLvoid*)offsetof(QUAD, tex));

		glBindVertexArray(0);
	}

};

