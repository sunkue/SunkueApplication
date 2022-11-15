#pragma once

#include "../Base"
#include "Transform.hpp"
#include "Shader.h"
#include "GuiManager.h"



class Actor : public TransformComponnent, public GuiObject
{
private:
	Shader& shader;
	GLuint vao{};
	size_t vertexSize{};
	GLfloat pointSize{};
	GLenum drawMode{ GL_POINTS };// GL_TRIANGLES
public:
	Actor() :shader{ Shader::Basic() } {}

	virtual void Draw() {
		rotate(Eigen::Quaterniond(Eigen::AngleAxisd((0.01), Eigen::Vector3d(0, 1, 0).normalized())));
		shader.Use();
		shader.Set("u_model_mat", modelMatrix());
		glBindVertexArray(vao);
		// glDrawArrays(GL_TRIANGLES, 0, vertexSize);
		glDrawArrays(drawMode, 0, vertexSize);
		glPointSize(pointSize);
	}
	
	virtual void InitVao() {
		glDeleteBuffers(1, &vao);
		glGenVertexArrays(1, &vao);
		GLuint vbo;
		glGenBuffers(1, &vbo);

		std::vector<Eigen::Vector3f> points
		{
			{
				Eigen::Vector3f{ 1.0, -1, -1},
				Eigen::Vector3f{ 1, -1, 1 },
				Eigen::Vector3f{ -1, -1, 1 },
				Eigen::Vector3f{ -1, -1, -1 },
				Eigen::Vector3f{ 1, 1, -1 },
				Eigen::Vector3f{ 1, 1, 1 },
				Eigen::Vector3f{ -1, 1, 1 },
				Eigen::Vector3f{ -1, 1, -1 }
			}
		};
		std::vector<Eigen::Vector3f> normals; normals.assign(points.size(), { 1,0,0 });
		std::vector<Eigen::Vector3f> colors
		{
			{
				Eigen::Vector3f{ 1, 0, 0},
				Eigen::Vector3f{ 1, 0, 1 },
				Eigen::Vector3f{ 0, 0, 1 },
				Eigen::Vector3f{ 0, 0, 0 },
				Eigen::Vector3f{ 1, 1, 0 },
				Eigen::Vector3f{ 1, 1, 1 },
				Eigen::Vector3f{ 0, 1, 1 },
				Eigen::Vector3f{ 0, 1, 0 }
			}
		};
		vertexSize = points.size();
		const auto sizeofVec3 = sizeof(Eigen::Vector3f);
		const auto pointsSize = points.size();
		const auto attributeSize = pointsSize;
		const auto bufferSize = attributeSize * sizeofVec3;
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, bufferSize * 3, nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, points.data());
		glBufferSubData(GL_ARRAY_BUFFER, bufferSize, bufferSize, normals.data());
		glBufferSubData(GL_ARRAY_BUFFER, bufferSize * 2, bufferSize, colors.data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeofVec3, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeofVec3, reinterpret_cast<GLvoid*>(bufferSize));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeofVec3, reinterpret_cast<GLvoid*>(bufferSize * 2));
		glBindVertexArray(0);
	}
};

