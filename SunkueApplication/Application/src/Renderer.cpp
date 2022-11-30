#include "Renderer.h"

/*
void Renderer::ResetCamera()
{
	Eigen::Array3f minimum; minimum.setConstant(std::numeric_limits<float>::max());
	Eigen::Array3f maximum; maximum.setConstant(std::numeric_limits<float>::min());
	for (auto& item : _drawAbleItems) {
		auto m = item.get().modelMatrix();
		auto& min = m * item.get().bound().minimum();
		auto& max = m * item.get().bound().maximum();
		minimum.x() = std::min(minimum.x(), min.x());
		minimum.y() = std::min(minimum.y(), min.y());
		minimum.z() = std::min(minimum.z(), min.z());
		maximum.x() = std::max(maximum.x(), max.x());
		maximum.y() = std::max(maximum.y(), max.y());
		maximum.z() = std::max(maximum.z(), max.z());
	}
	Eigen::Array3f size = (maximum - minimum).abs();
	Eigen::Array3f center = (maximum + minimum) / 2.f;
	auto cameraFront(Eigen::Vector3f::UnitZ());
	auto cameraRight(Eigen::Vector3f::UnitX());
	float objectSize{0};
	Eigen::Array3f temp; temp.setConstant(1); temp -= static_cast<Eigen::Array3f>(cameraFront);
	for (const auto& v : (size * temp)) objectSize += v * v;
	objectSize = std::sqrt(objectSize / 2.f);
	float dist = (objectSize / std::tan(radians(_mainCamera.fovy()) / 2.f));
	_mainCamera.fovy() = 45.f;
	_mainCamera.position() = center + static_cast<Eigen::Array3f>(cameraFront) * dist;
	_mainCamera.target(center);
}
*/
void Renderer::ResetCamera()
{
	Eigen::Array3f minimum; minimum.setConstant(std::numeric_limits<float>::max());
	Eigen::Array3f maximum; maximum.setConstant(std::numeric_limits<float>::min());
	for (auto& item : _drawAbleItems) {
		auto m = item.get().transform().modelMatrix();
		auto& min = m * item.get().bound().minimum();
		auto& max = m * item.get().bound().maximum();
		minimum.x() = std::min(minimum.x(), min.x());
		minimum.y() = std::min(minimum.y(), min.y());
		minimum.z() = std::min(minimum.z(), min.z());
		maximum.x() = std::max(maximum.x(), max.x());
		maximum.y() = std::max(maximum.y(), max.y());
		maximum.z() = std::max(maximum.z(), max.z());
	}
	Eigen::Array3f size = (maximum - minimum).abs();
	Eigen::Array3f center = (maximum + minimum) / 2.f;
	auto minimumIdx = std::distance(size.begin(), std::min_element(size.begin(), size.end()));
	auto maximumIdx = std::distance(size.begin(), std::max_element(size.begin(), size.end()));
	if (minimumIdx == maximumIdx)maximumIdx += 1;
	Eigen::Vector3f cameraFront; cameraFront.setZero(); cameraFront[minimumIdx] = 1.f;
	Eigen::Vector3f cameraRight; cameraRight.setZero(); cameraRight[maximumIdx] = 1.f;
	float objectSize{ 0 };
	Eigen::Array3f temp; temp.setConstant(1); temp -= static_cast<Eigen::Array3f>(cameraFront);
	for (const auto& v : (size* temp)) objectSize += v * v;
	objectSize = std::sqrt(objectSize / 2.f);
	float dist = (objectSize / std::tan(radians(_mainCamera.fovy()) / 2.f));
	_mainCamera.fovy() = 45.f;
	_mainCamera.position() = center + static_cast<Eigen::Array3f>(cameraFront) * dist;
	_mainCamera.target(center);
}
