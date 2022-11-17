#include "Renderer.h"

void Renderer::ResetCamera()
{
	Eigen::Array3f minimum; minimum = { std::numeric_limits<float>::max() };
	Eigen::Array3f maximum; maximum = { std::numeric_limits<float>::min() };
	for (auto& item : _drawAbleItems) {
		auto& min = item.get().bound().minimum();
		auto& max = item.get().bound().maximum();
		minimum.x() = std::min(minimum.x(), min.x());
		minimum.y() = std::min(minimum.y(), min.y());
		minimum.z() = std::min(minimum.z(), min.z());
		maximum.x() = std::max(maximum.x(), max.x());
		maximum.y() = std::max(maximum.y(), max.y());
		maximum.z() = std::max(maximum.z(), max.z());
	}
	Eigen::Array3f size = maximum - minimum;
	Eigen::Array3f center = (maximum + minimum) / 2.f;
	auto minimumIdx = std::distance(size.begin(), std::min_element(size.begin(), size.end()));
	auto maximumIdx = std::distance(size.begin(), std::max_element(size.begin(), size.end()));
	if (minimumIdx == maximumIdx)maximumIdx += 1;
	Eigen::Vector3f cameraFront{}; cameraFront[minimumIdx] = 1.f;
	Eigen::Vector3f cameraRight{}; cameraRight[maximumIdx] = 1.f;
	float objectSize{};
	Eigen::Array3f temp{ 1.f }; temp -= static_cast<Eigen::Array3f>(cameraFront);
	for (const auto& v : (size* temp)) {
		objectSize += v * v;
	}
	objectSize = std::sqrt(objectSize / 2.f);
	float dist = (objectSize / std::tan(radians(_mainCamera.fovy()) / 2.f));
	_mainCamera.position() = center + static_cast<Eigen::Array3f>(cameraFront) * dist;
	_mainCamera.target() = center;
	_mainCamera.up() = cameraRight.cross(cameraFront).normalized();
}