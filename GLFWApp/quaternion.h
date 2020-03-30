#ifndef _QUATERNION_H
#define _QUATERNION_H

#include "Eigen/Dense"

class Quaternion {
public:
	Quaternion();
	/* Construct a quaternion with the given four components
	*/
	Quaternion(float w, float x, float y, float z);
	/* Construct a unit quaternion with the given rotation angle and rotation direction
	*/
	Quaternion(float aRotRadians, Eigen::Vector3f& aRotDir);
	Quaternion(const Quaternion& aCopy);
	Quaternion& operator=(const Quaternion& aRhs);
public:
	Eigen::Matrix4f toMatrix() const;
	inline float norm() const {
		return m_quat_norm;
	}
public:
	/* The spherical linear interpolation between two quaternions.
	*/
	static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);
	friend Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
private:
	// a quaternion is represented by a 4d vector
	// m_quat(0) = cos(theta/2)
	// m_quat(1) = sin(theta/2)*x
	// m_quat(2) = sin(theta/2)*y
	// m_quat(3) = sin(theta/2)*z
	// the 3d vector (x, y, z) is the unit rotation direction
	// theata is the rotationf angle in radians
	Eigen::Vector4f m_quat; 
	float m_quat_norm;
};



#endif
