#ifndef _TRANSFORMATION_3D_H
#define _TRANSFORMATION_3D_H

/* 
* Author: Yinhui Yang
* Date: 2020-01-29
*/


#include <Eigen/Dense>


float degree_to_radians(float degree);

/*Normalize the angle in degrees within range [0, 360]
*/
void normalize_angle_degrees(float& angle);

/* Compute the 3d roatation matrix around the x-axis.
* radians: the rotation angle in radians
* Returns a 4X4 matrix in column major.
*/
Eigen::Matrix4f rotate_x(float radians);


/* Compute the 3d roatation matrix around the y-axis.
* radians: the rotation angle in radians
* Returns a 4X4 matrix in column major.
*/
Eigen::Matrix4f rotate_y(float radians);

/* Compute the 3d roatation matrix around the x-axis.
* radians: the rotation angle in radians
* Returns a 4X4 matrix in column major.
*/
Eigen::Matrix4f rotate_z(float radians);

/* Compute the 3d scale matrix.
* xscale: scale along the x axis
* yscale: scale along the y axis
* zscale: scale along the z axis
* Returns a 4X4 matrix in column major.
*/
Eigen::Matrix4f scale(float xscale, float yscale, float zscale);

/* Compute the 3d translation matrix.
* xoffset: offset along the x axis
* yoffset: offset along the y axis
* zoffset: offset along the z axis
* Returns a 4X4 matrix in column major.
*/
Eigen::Matrix4f translate(float xoffset, float yoffset, float zoffset);


/* Compute the 3d translation matrix.
* aOffsetVec: a 3d vector storing the offsets along the x, y and z axis respectively 
* Returns a 4X4 matrix in column major.
*/
Eigen::Matrix4f translate(const Eigen::Vector3f& aOffsetVec);


/* Compute the 3d viewing transfofmation matrix.
* aCameraPos: the camera position in the world coordinates
* aCameraLookAtPos: the camera look at position in the world coordinates
* aCameraUpDir: the camera's up direction
* Returns a 4X4 matrix in column major.
*/
Eigen::Matrix4f view_transform(
	const Eigen::Vector3f& aCameraPos,
	const Eigen::Vector3f& aCameraLookAtPos,
	const Eigen::Vector3f& aCameraUpDir
);

/*Compute the OpenGL orthographic projection matrix.
* left, right: the left and right clipping plane in the x direction
* bottom, top: the bottom and top clipping plane in the y direction
* near: the distance to the near clipping plane in the z direction
* far: the distance to the far clipping plane in the z direction
* NOTE: near and far can be both positive or both negative, in the later
* case, the scene is behind the camera.
* Returns a 4 by 4 orthographic projection matrix in column major.
*/
Eigen::Matrix4f ortho3d(float left, float right,
	float bottom, float top,
	float near, float far);

/*
* Compute the OpenGL perspective matrix.
* fovy: the vertical field of view (in degrees)
* aspect: the aspect ratio, namely width / height
* near: the distance to the near clipping plane, a positive number
* far : the distance to the far clipping plane, a positive number
* Returns a 4 by 4 perspective projection matrix in column major.
*/
Eigen::Matrix4f perspective(float fovy, float aspect, float near, float far);



#endif
