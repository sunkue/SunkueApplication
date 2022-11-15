#pragma once

template <class genType>
inline genType radians(genType degrees)
{
	static_assert(std::numeric_limits<genType>::is_iec559, "'radians' only accept floating-point input");
	return degrees * static_cast<genType>(0.01745329251994329576923690768489);
}

template <class genType>
Eigen::Matrix4<genType> lookAt(const Eigen::Vector3<genType>& position, const Eigen::Vector3<genType>& target, const Eigen::Vector3<genType>& up)
{
	Eigen::Matrix3<genType> R;
	R.col(2) = (position - target).normalized();
	R.col(0) = up.cross(R.col(2)).normalized();
	R.col(1) = R.col(2).cross(R.col(0));
	Eigen::Matrix4<genType> viewMatrix{};
	viewMatrix.topLeftCorner<3, 3>() = R.transpose();
	viewMatrix.topRightCorner<3, 1>() = -R.transpose() * position;
	viewMatrix(3, 3) = 1.0f;
	return viewMatrix;
}


template <class genType>
Eigen::Matrix4<genType> perspective(genType fovY, genType aspect, genType n, genType f)
{
	genType theta = fovY * 0.5;
	genType range = f - n;
	genType invtan = 1. / tan(theta);
	Eigen::Matrix4<genType> projectionMatrix{};
	projectionMatrix(0, 0) = invtan / aspect;
	projectionMatrix(1, 1) = invtan;
	projectionMatrix(2, 2) = -(n + f) / range;
	projectionMatrix(3, 2) = -1;
	projectionMatrix(2, 3) = -2 * n * f / range;
	projectionMatrix(3, 3) = 0;
	return projectionMatrix;
}