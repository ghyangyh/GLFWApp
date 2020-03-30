#include "quaternion.h"

#include <cmath>
using namespace std;

Quaternion::Quaternion() : m_quat(0.f, 0.f, 0.f,0.f), m_quat_norm(0.f) {

}

Quaternion::Quaternion(float w, float x, float y, float z) : m_quat(w, x, y, z),
m_quat_norm(m_quat.norm()) {

}

Quaternion::Quaternion(float aRotRadians, Eigen::Vector3f& aRotDir){
	aRotDir.normalized();
	m_quat(0) = cos(aRotRadians * 0.5f);
	m_quat.tail(3) = sin(aRotRadians * 0.5f) * aRotDir;
	m_quat_norm = 1.f;
}

Quaternion::Quaternion(const Quaternion& aCopy) : m_quat(aCopy.m_quat),
m_quat_norm(aCopy.m_quat_norm){

}

Quaternion& Quaternion::operator=(const Quaternion& aRhs) {
	if (this != &aRhs) {
		m_quat = aRhs.m_quat;
		m_quat_norm = aRhs.m_quat_norm;
	}
	return (*this);
}

Eigen::Matrix4f Quaternion::toMatrix() const {
	Eigen::Matrix4f mat;
	float s = 2.f / (m_quat_norm * m_quat_norm);
	float qx2 = m_quat(1) * m_quat(1);
	float qy2 = m_quat(2) * m_quat(2);
	float qz2 = m_quat(3) * m_quat(3);
	float qxqy = m_quat(1) * m_quat(2);
	float qxqz = m_quat(1) * m_quat(3);
	float qyqz = m_quat(2) * m_quat(3);
	float qwqx = m_quat(0) * m_quat(1);
	float qwqy = m_quat(0) * m_quat(2);
	float qwqz = m_quat(0) * m_quat(3);
	mat << 1 - s * (qy2 + qz2), s* (qxqy - qwqz), s* (qxqz + qwqy), 0,
		s* (qxqy + qwqz), 1 - s * (qx2 + qz2), s* (qyqz - qwqx), 0,
		s* (qxqz - qwqy), s* (qyqz + qwqx), 1 - s * (qx2 + qy2), 0,
		0, 0, 0, 1;
	return mat;
}

Quaternion Quaternion::slerp(const Quaternion& q1, const Quaternion& q2, float t) {
	float dot_result = q1.m_quat.dot(q2.m_quat);
	// if q1 and q2 are equal, return any of them
	if (dot_result >= 1.f)
		return q1;
	Quaternion q_1(q1);
	if (dot_result < 0.f) // if q1 and q2 in oppsite direction
		q_1.m_quat = -q1.m_quat;
	float sin_omega = sqrt(1.f - dot_result * dot_result);
	Quaternion quat_result;
	if (fabs(sin_omega) < 0.0001f) { // sin_omega approximates to 0, then do linear interpolation
		quat_result.m_quat = (1.f - t) * q_1.m_quat + t * q2.m_quat;
		quat_result.m_quat_norm = quat_result.m_quat.norm();
	}
	else {
		float omega = acos(dot_result);
		float a = (sin((1.f - t) * omega)) / sin_omega;
		float b = sin(t * omega) / sin_omega;
		quat_result.m_quat = a * q_1.m_quat + b * q2.m_quat;
		quat_result.m_quat_norm = quat_result.m_quat.norm();
	}
	return quat_result;
}

Quaternion operator*(const Quaternion& q1, const Quaternion& q2) {
	Quaternion result;
	result.m_quat(0) = q1.m_quat(0) * q2.m_quat(0) -
		q1.m_quat(1) * q2.m_quat(1) -
		q1.m_quat(2) * q2.m_quat(2) -
		q1.m_quat(3) * q2.m_quat(3);
	result.m_quat(1) = q1.m_quat(2) * q2.m_quat(3) -
		q1.m_quat(3) * q2.m_quat(2) +
		q1.m_quat(1) * q2.m_quat(0) + 
		q1.m_quat(0) * q2.m_quat(1);
	result.m_quat(2) = q1.m_quat(3) * q2.m_quat(1) -
		q1.m_quat(1) * q2.m_quat(3) +
		q1.m_quat(2) * q2.m_quat(0) +
		q1.m_quat(0) * q2.m_quat(2);
	result.m_quat(3) = q1.m_quat(1) * q2.m_quat(2) -
		q1.m_quat(2) * q2.m_quat(1) +
		q1.m_quat(3) * q2.m_quat(0) +
		q1.m_quat(0) * q2.m_quat(3);
	result.m_quat_norm = result.m_quat.norm();
	return result;
}