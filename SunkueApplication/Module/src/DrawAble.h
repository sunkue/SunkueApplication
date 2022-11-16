#include "../Base"
#include "Transform.hpp"
#include "Shader.h"
#include "GuiManager.h"

class DrawAble
{
protected:
	GLuint vao{};
	size_t vertexNum{};
	GLenum drawMode{ GL_POINTS };// GL_TRIANGLES
	float shininess{ 64 };
public:
	Shader& shader;
	DrawAble(Shader& shader) :shader{ shader } {}
	virtual void Draw() {};
	void Init(const std::vector<Eigen::Vector3f>& points, const std::vector<Eigen::Vector3f>& normals, const std::vector<Eigen::Vector3f>& colors);
};