#pragma once

template <class genType>
inline genType radians(genType degrees)
{
	static_assert(std::numeric_limits<genType>::is_iec559, "'radians' only accept floating-point input");
	return degrees * static_cast<genType>(0.01745329251994329576923690768489);
}

template <class genType>
Eigen::Matrix4<genType> lookAt(const Eigen::Vector3<genType>& eye, const Eigen::Vector3<genType>& center, const Eigen::Vector3<genType>& up) {
	auto f = (center - eye).normalized();
	auto u = up.normalized();
	auto s = f.cross(u).normalized();
	u = s.cross(f);
	Eigen::Matrix4<genType> mat; mat.setZero();
	mat(0, 0) = s.x();
	mat(0, 1) = s.y();
	mat(0, 2) = s.z();
	mat(0, 3) = -s.dot(eye);
	mat(1, 0) = u.x();
	mat(1, 1) = u.y();
	mat(1, 2) = u.z();
	mat(1, 3) = -u.dot(eye);
	mat(2, 0) = -f.x();
	mat(2, 1) = -f.y();
	mat(2, 2) = -f.z();
	mat(2, 3) = f.dot(eye);
	mat.row(3) << 0, 0, 0, 1;
	return mat;
}
/*
template <class genType>
Eigen::Matrix4<genType> lookAt(const Eigen::Vector3<genType>& eye, const  Eigen::Vector3<genType>& center, const  Eigen::Vector3<genType>& up)
{
	Eigen::Vector3<genType> const f((center - eye).normalized());
	Eigen::Vector3<genType> const s(f.cross(up).normalized());
	Eigen::Vector3<genType> const u(s.cross(f));
	Eigen::Matrix3<genType> R;
	R.col(0) = s;
	R.col(1) = u;
	R.col(2) = -f;
	Eigen::Matrix4<genType> Result;
	Result.topLeftCorner<3, 3>() = R;
	Result(0, 3) = -(s.dot(eye));
	Result(1, 3) = -(u.dot(eye));
	Result(2, 3) = (f.dot(eye));
	Result(3, 3) = 1.0f;
	return Result;
}
*/


template <class genType>
Eigen::Matrix4<genType> perspective(const genType fovY, const genType aspect, const genType n, const genType f)
{
	genType theta = fovY * static_cast<genType>(0.5);
	genType range = f - n;
	genType invtan = static_cast<genType>(1) / std::tan(theta);
	Eigen::Matrix4<genType> projectionMatrix; projectionMatrix.setZero();
	projectionMatrix(0, 0) = invtan / aspect;
	projectionMatrix(1, 1) = invtan;
	projectionMatrix(2, 2) = -(n + f) / range;
	projectionMatrix(3, 2) = static_cast<genType>(-1);
	projectionMatrix(2, 3) = static_cast<genType>(-2) * n * f / range;
	return projectionMatrix;
}

#include <vector>
#include <algorithm>
#include <numeric>

// mean , stdDev
inline std::pair<double, double> StandardDeviation(const std::vector<double>& src) {
	const double mean = std::accumulate(src.begin(), src.end(), 0.) / src.size();
	double devSum = std::inner_product(src.begin(), src.end(), src.begin(), 0.,
		[](const double sum, const double dev) {return sum + dev; },
		[mean](const double a, const double b) {return (a - mean) * (a - mean); }
	);
	double stdDev = std::sqrt(devSum / (src.size() - 1));
	return std::make_pair(mean, stdDev);
}