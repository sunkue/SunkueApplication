#pragma once

#include "../Base"
#include "Transform.hpp"
#include "ClippedWindow.hpp"

class Frame
{
public:
	Frame(const Eigen::Vector3f& pos = Eigen::Vector3f::Zero(),
		const Eigen::Quaternionf& o = Eigen::Quaternionf::Identity())
		: _orientation(o), _position(pos)
	{}
	Frame lerp(float alpha, const Frame& other) const
	{
		return Frame((1.f - alpha) * _position + alpha * other._position,
			_orientation.slerp(alpha, other._orientation));
	}
private:
	SunkueMakeGetSet(Eigen::Quaternionf, orientation);
	SunkueMakeGetSet(Eigen::Vector3f, position);
};

// vp matrix = viewport + sight
class Camera : public ClippedWindow, public GuiObject
{
	SunkueMakeVar(Eigen::Vector3f, target) { Eigen::Vector3f::Zero() };
	SunkueMakeGet(Eigen::Vector3f, target);
	SunkueMakeGetSet(float, fovy) { 45.0 };
	SunkueMakeGetSet(float, n) { 0.1 };
	SunkueMakeGetSet(float, f) { std::numeric_limits<float>::max() };
private:
	SunkueMakeVar(Frame, frame);
	SunkueMakeGet(Frame, frame);
	SunkueMakeVar(Eigen::Affine3f, viewMatrix) { Eigen::Affine3f ::Identity()} ;
	SunkueMakeGet(Eigen::Affine3f, viewMatrix);
	SunkueMakeVar(Eigen::Matrix4f, projectionMatrix) { Eigen::Matrix4f::Identity() };
	SunkueMakeGet(Eigen::Matrix4f, projectionMatrix);
private: 
	void updateViewMatrix() { 
//		Eigen::Quaternionf q = orientation().conjugate();
//		_viewMatrix.linear() = q.toRotationMatrix();
//		_viewMatrix.translation() = -(_viewMatrix.linear() * position());
		_viewMatrix = lookAt(frame().position(), target(), up()); 
	}
	void updateProjectionMatrix(GLFWwindow* window){ _projectionMatrix = perspective(radians(fovy()), aspect(window), n(), f()); }
public:
	void Update(GLFWwindow* window) { updateViewMatrix(); updateProjectionMatrix(window); }
	virtual void DrawGui() {
		ImGui::Begin("Main Camera");
		ImGui::DragFloat("fovy", &_fovy, 1, 10, 180, "%.1f", 1);
		ImGui::DragFloat3("position", _frame.position().data(), 1, -5000, 5000, "%.1f", 1);
		ImGui::DragFloat3("target", _target.data(), 1, -5000, 5000, "%.1f", 1);
		ClippedWindow::DrawChildGui("ViewPort");
		ImGui::End();
	}
public:
	Eigen::Vector3f& position() { return _frame.position(); }
	const Eigen::Vector3f& position() const { return frame().position(); }
	Eigen::Quaternionf& orientation() { return _frame.orientation(); }
	const Eigen::Quaternionf& orientation() const { return frame().orientation(); }
public:
	const Eigen::Vector3f direction() const { return -(orientation() * Eigen::Vector3f::UnitZ()); }
	const Eigen::Vector3f up() const { return orientation() * Eigen::Vector3f::UnitY(); }
	const Eigen::Vector3f right() const { return orientation() * Eigen::Vector3f::UnitX(); }
	void direction(const Eigen::Vector3f& newDirection)
	{
		_frame.orientation() *= Eigen::Quaternionf::FromTwoVectors(direction().normalized(), newDirection.normalized());
	}
	void target(const Eigen::Vector3f& target)
	{
		_target = target;
		if (!_target.isApprox(position())) direction(_target - position());
	}
public:
	void zoom(float d) {
		float dist = (position() - target()).norm();
		if (dist > d) position() = position() + direction() * d;
	}
	void rotateAroundTarget(const Eigen::Quaternionf& q)
	{
		updateViewMatrix();
		Eigen::Vector3f t = _viewMatrix * target();

		_viewMatrix = Eigen::Translation3f(t)
			* q
			* Eigen::Translation3f(-t)
			* _viewMatrix;

		Eigen::Quaternionf qa(_viewMatrix.linear());
		qa = qa.conjugate();
		orientation() = qa;
		position() = -(qa * _viewMatrix.translation());
	}
	void localRotate(const Eigen::Quaternionf& q) {
		orientation() = orientation() * q;
		float dist = (position() - target()).norm();
		target(position() + dist * direction());
	}
	void localTranslate(const Eigen::Vector3f& t)
	{
		Eigen::Vector3f trans = orientation() * t;
		position() = position() + trans;
		target(target() + trans);
	}
};


/*
void Camera::updateViewMatrix(void) const
{
	if (!mViewIsUptodate)
	{
		Quaternionf q = orientation().conjugate();
		mViewMatrix.linear() = q.toRotationMatrix();
		mViewMatrix.translation() = -(mViewMatrix.linear() * position());

		mViewIsUptodate = true;
	}
}

Vector3f Camera::unProject(const Vector2f& uv, float depth) const
{
	Matrix4f inv = mViewMatrix.inverse().matrix();
	return unProject(uv, depth, inv);
}

Vector3f Camera::unProject(const Vector2f& uv, float depth, const Matrix4f& invModelview) const
{
	Vector3f a(2. * uv.x() / float(mVpWidth) - 1., 2. * uv.y() / float(mVpHeight) - 1., 1.);
	a.x() *= depth / mProjectionMatrix(0, 0);
	a.y() *= depth / mProjectionMatrix(1, 1);
	a.z() = -depth;
	// FIXME /\/|
	Vector4f b = invModelview * Vector4f(a.x(), a.y(), a.z(), 1.);
	return Vector3f(b.x(), b.y(), b.z());
}
*/