#include "DrawAble.h"
template<>
void DrawAble::Init(const std::vector<Eigen::Vector3f>& points, const std::vector<Eigen::Vector3f>& normals, const std::vector<Eigen::Vector3f>& colors)
{
	_bound.set(points);
	glDeleteBuffers(1, &vao);
	glGenVertexArrays(1, &vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	vertexNum = points.size();
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

template<>
void DrawAble::Init(const std::vector<Eigen::Vector3d>& points, const std::vector<Eigen::Vector3d>& normals, const std::vector<Eigen::Vector3d>& colors)
{
	_bound.set(points);
	glDeleteBuffers(1, &vao);
	glGenVertexArrays(1, &vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	vertexNum = points.size();
	const auto sizeofVec3 = sizeof(Eigen::Vector3d);
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
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeofVec3, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, sizeofVec3, reinterpret_cast<GLvoid*>(bufferSize));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_DOUBLE, GL_FALSE, sizeofVec3, reinterpret_cast<GLvoid*>(bufferSize * 2));
	glBindVertexArray(0);
}