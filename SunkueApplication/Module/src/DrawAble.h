#include "../Base"
#include "Transform.hpp"
#include "Shader.h"
#include "GuiManager.h"

struct Bound {
	SunkueMakeGetSet(Eigen::Array3f, minimum);
	SunkueMakeGetSet(Eigen::Array3f, maximum);
public:
	Eigen::Array3f size() const { return _maximum - _minimum; }
	Eigen::Array3f center() const { return (_minimum + _maximum) / 2; }
	template<class gentype>
	void set(const std::vector<Eigen::Vector3<gentype>>& points) {
		_minimum = { std::numeric_limits<float>::max() };
		_maximum = { std::numeric_limits<float>::min() };
		for (auto& p : points) {
			_minimum.x() = std::min(_minimum.x(), static_cast<float>(p.x()));
			_minimum.y() = std::min(_minimum.y(), static_cast<float>(p.y()));
			_minimum.z() = std::min(_minimum.z(), static_cast<float>(p.z()));
			_maximum.x() = std::max(_maximum.x(), static_cast<float>(p.x()));
			_maximum.y() = std::max(_maximum.y(), static_cast<float>(p.y()));
			_maximum.z() = std::max(_maximum.z(), static_cast<float>(p.z()));
		}
	}
};

class DrawAble : public TransformComponnent
{
protected:
	GLuint vao{};
	GLuint vbo{};
	size_t vertexNum{};
	GLenum drawMode{ GL_POINTS };// GL_TRIANGLES
	float shininess{ 128 };
	SunkueMakeGetSet(Bound, bound);
public:
	Shader& shader;
	DrawAble(Shader& shader) :shader{ shader } {}
	virtual void Draw() {};
	template<class genType>
	void Init(const std::vector<Eigen::Vector3<genType>>& points, const std::vector<Eigen::Vector3<genType>>& normals, const std::vector<Eigen::Vector3<genType>>& colors);
	template<class genType>
	void Init(const std::vector<Eigen::Vector3<genType>>& points, const std::vector<Eigen::Vector3<genType>>& normals, const std::vector<genType>& scalars);
};