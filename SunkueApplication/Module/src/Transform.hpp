#pragma once

#include "../Base"

class TransformComponnent {
public:
	Eigen::Translation3d t;
	Eigen::Quaterniond r;
	Eigen::AlignedScaling3d s;
public:
	TransformComponnent() { reset(); }

	void reset() {
		t = t.Identity();
		r.setIdentity();
		s.setIdentity();
	}

	void translate(const Eigen::Translation3d& move) {
		t = t * move;
	}
	void rotate(const Eigen::Quaterniond& move) {
		r = r * move;
	}
	void scale(const double& move) {
		s = s + Eigen::AlignedScaling3d(move, move, move);
	}
	void scale(const Eigen::AlignedScaling3d& move) {
		s = s + move;
	}
	Eigen::Affine3d modelMatrix() {
		return t * r * s;
	}
};