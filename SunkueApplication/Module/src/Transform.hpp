#pragma once

#include "../Base"

class TransformComponnent {
protected:
//	Affine3f
	Eigen::Translation3f t;
	Eigen::Quaternionf r;
	Eigen::AlignedScaling3f s;
public:
	TransformComponnent() { reset(); }

	void reset() {
		t = t.Identity();
		r.setIdentity();
		s.setIdentity();
	}

	void translate(const Eigen::Translation3f& move) {
		t = t * move;
	}
	void rotate(const Eigen::Quaternionf& move) {
		r = r * move;
	}
	void scale(const float& move) {
		s = s + Eigen::AlignedScaling3f(move, move, move);
	}
	void scale(const Eigen::AlignedScaling3f& move) {
		s = s + move;
	}
	Eigen::Affine3f modelMatrix() {
		return t * r * s;
	}
};