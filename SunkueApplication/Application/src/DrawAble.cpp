#include "DrawAble.h"

template<>
void DrawAble::Init(const std::vector<Eigen::Vector3d>& points, const std::vector<Eigen::Vector3d>& normals, const std::vector<Eigen::Vector3d>& colors, const std::vector<unsigned int>& indices)
{
	_bound.set(points);
	glDeleteVertexArrays(1, &vao);
	glGenVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glGenBuffers(1, &vbo);
	vertexNum = points.size();
	const auto sizeofVec3 = sizeof(Eigen::Vector3d);
	const auto pointsSize = points.size();
	const auto attributeSize = pointsSize;
	const auto bufferSize = attributeSize * sizeofVec3;
	glBindVertexArray(vao);

	if (!indices.empty()) {
		indexNum = indices.size();
		glDeleteBuffers(1, &ebo);
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
	}

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

template<>
void DrawAble::Init(const std::vector<Eigen::Vector3d>& points, const std::vector<Eigen::Vector3d>& normals, const std::vector<double>& scalars, const std::vector<unsigned int>& indices)
{
	_bound.set(points);
	glDeleteVertexArrays(1, &vao);
	glGenVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glGenBuffers(1, &vbo);
	vertexNum = points.size();
	const auto sizeofVec3 = sizeof(Eigen::Vector3d);
	const auto sizeofdouble = sizeof(double);
	const auto pointsSize = points.size();
	const auto attributeSize = pointsSize;
	const auto bufferSize3 = attributeSize * sizeofVec3;
	const auto bufferSize1 = attributeSize * sizeofdouble;
	glBindVertexArray(vao);

	if (!indices.empty()) {
		indexNum = indices.size();
		glDeleteBuffers(1, &ebo);
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, bufferSize3 * 2 + bufferSize1, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize3, points.data());
	glBufferSubData(GL_ARRAY_BUFFER, bufferSize3, bufferSize3, normals.data());
	glBufferSubData(GL_ARRAY_BUFFER, bufferSize3 * 2, bufferSize1, scalars.data());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeofVec3, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, sizeofVec3, reinterpret_cast<GLvoid*>(bufferSize3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_DOUBLE, GL_FALSE, sizeofdouble, reinterpret_cast<GLvoid*>(bufferSize3 * 2));
	glBindVertexArray(0);
}

