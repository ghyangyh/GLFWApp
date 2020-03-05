#include "transformation_3d.h"

#include <cmath>
using namespace std;

float degree_to_radians(float degrees) {
	return degrees * 0.01745329251994329576923690768489f;
}

float normalize_angle_degrees(float& angle) {
	while (angle < 0.f)
		angle += 360.f;
	while (angle > 360.f)
		angle -= 360.f;
}

Eigen::Matrix4f rotate_x(float radians) {
	Eigen::Matrix4f mat;
	mat << 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(radians), -sin(radians), 0.f,
		0.0f, sin(radians), cos(radians), 0.f,
		0.f, 0.f, 0.f, 1.f;
	return mat;
}

Eigen::Matrix4f rotate_y(float radians) {
	Eigen::Matrix4f mat;
	mat << cos(radians), 0.f, sin(radians), 0.f,
		0.f, 1.0f, 0.f, 0.f,
		-sin(radians), 0.0f, cos(radians), 0.f,
		0.f, 0.f, 0.f, 1.f;
	return mat;
}

Eigen::Matrix4f rotate_z(float radians) {
	Eigen::Matrix4f mat;
	mat << cos(radians), -sin(radians), 0.f, 0.f,
		sin(radians), cos(radians), 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f;
	return mat;
}

Eigen::Matrix4f scale(float xscale, float yscale, float zscale) {
	Eigen::Matrix4f mat;
	mat << xscale, 0.f, 0.f, 0.f,
		0.f, yscale, 0.f, 0.f,
		0.f, 0.f, zscale, 0.f,
		0.f, 0.f, 0.f, 1.f;
	return mat;
}

Eigen::Matrix4f translate(float xoffset, float yoffset, float zoffset) {
	Eigen::Matrix4f mat;
	mat << 1.0f, 0.0f, 0.0f, xoffset,
		0.0f, 1.0f, 0.0f, yoffset,
		0.0f, 0.0f, 1.0f, zoffset,
		0.0f, 0.0f, 0.0f, 1.0f;
	return mat;
}

Eigen::Matrix4f translate(const Eigen::Vector3f& aOffsetVec) {
	Eigen::Matrix4f mat;
	mat << 1.0f, 0.0f, 0.0f, aOffsetVec(0),
		0.0f, 1.0f, 0.0f, aOffsetVec(1),
		0.0f, 0.0f, 1.0f, aOffsetVec(2),
		0.0f, 0.0f, 0.0f, 1.0f;
	return mat;
}

Eigen::Matrix4f view_transform(const Eigen::Vector3f& aCameraPos,
	const Eigen::Vector3f& aCameraLookAtPos,
	const Eigen::Vector3f& aCameraUpDir) {
	// compute the forward direction (negative z)
	Eigen::Vector3f camera_forward = aCameraLookAtPos - aCameraPos;
	camera_forward.normalize();

	// compute the right direction (positive x)
	// the right direction is a cross product of the forward direction and the up direction
	Eigen::Vector3f camera_right = camera_forward.cross(aCameraUpDir);
	camera_right.normalize();

	// recompute the up direction (positive y)
	// the up direction is a cross product of the right and forward direction
	Eigen::Vector3f camera_up = camera_right.cross(camera_forward);
	camera_up.normalize();

	// construct the final view transformation matrix
	Eigen::Matrix4f mat = Eigen::Matrix4f::Zero();
	mat.block(0, 0, 1, 3) = camera_right.transpose();	 // positive x
	mat.block(1, 0, 1, 3) = camera_up.transpose();		 // positive y
	mat.block(2, 0, 1, 3) = -camera_forward.transpose(); // positive z
	/*mat(0, 0) = camera_right(0);
	mat(0, 1) = camera_right(1);
	mat(0, 2) = camera_right(2);

	mat(1, 0) = camera_up(0);
	mat(1, 1) = camera_up(1);
	mat(1, 2) = camera_up(2);

	mat(2, 0) = -camera_forward(0);
	mat(2, 1) = -camera_forward(1);
	mat(2, 2) = -camera_forward(2);*/

	mat(3, 3) = 1.0f;

	Eigen::Matrix4f view_mat = mat * translate(-aCameraPos);
	return view_mat;
}

/*Compute the OpenGL orthographic projection matrix.
* left, right: the left and right clipping plane in the x direction
* bottom, top: the bottom and top clipping plane in the y direction
* near: the distance to the near clipping plane in the z direction
* far: the distance to the far clipping plane in the z direction
* NOTE: near and far can be both positive or both negative, in the later
* case, the scene is behind the camera.
*/
Eigen::Matrix4f ortho3d(float left, float right,
	float bottom, float top,
	float near, float far) {
	near = -near;
	far = -far;
	Eigen::Matrix4f mat;
	mat << 2.f / (right - left), 0.f, 0.f, -(right + left) / (right - left),
		0.f, 2.f / (top - bottom), 0.f, -(top + bottom) / (top - bottom),
		0.f, 0.f, 2.f / (far - near), -(near + far) / (far - near),
		0.f, 0.f, 0.f, 1.f;
	return mat;
}

/*
* Compute the OpenGL perspective matrix.
* fovy: the vertical field of view (in degrees)
* aspect: the aspect ratio, namely width / height
* near: the distance to the near clipping plane, a positive number
* far : the distance to the far clipping plane, a positive number
*/
Eigen::Matrix4f perspective(float fovy, float aspect, float near, float far) {
	float half_fovy_radians = degree_to_radians(fovy * 0.5f);
	float c = 1.f / tan(half_fovy_radians);
	Eigen::Matrix4f mat;
	mat << c / aspect, 0.f, 0.f, 0.f,
		0.f, c, 0.f, 0.f,
		0.f, 0.f, (near + far) / (near - far), 2 * far*near / (near - far),
		0.f, 0.f, -1.f, 0.f;
	return mat;
}
