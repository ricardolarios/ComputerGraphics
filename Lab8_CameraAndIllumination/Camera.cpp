#define _USE_MATH_DEFINES

#include "Camera.h"
#include <math.h>

Camera::Camera() : position_(0.0, 0.0, 0.0), lookAt_(0.0, 0.0, 0.0), up_(0.0, 1.0, 0.0), current_angle_(0)
{
	projection_.setToIdentity();
}

Camera::~Camera()
{}

void Camera::setPerspective(float fov, float aspect, float near, float far)
{
	projection_.setToIdentity();
	projection_.perspective(fov, aspect, near, far);
}

void Camera::setPosition(const QVector3D& position)
{
	position_ = position;
}

QVector3D Camera::position() const
{
	return position_;
}

void Camera::translateCamera(const QVector3D& delta)
{
	// TODO:  Implement camera translation
	position_ += delta;
}

void Camera::setGazeVector(const QVector3D& gaze)
{
	lookAt_ = gaze + position_;
	lookAt_.normalize();
}

QVector3D Camera::gazeVector() const
{
	QVector3D gaze = lookAt_ - position_;
	gaze.normalize();
	return gaze;
}

QVector3D Camera::lookAt() const
{
	return lookAt_;
}

QVector3D Camera::upVector() const
{
	return up_;
}

void Camera::setLookAt(const QVector3D& lookAt)
{
	lookAt_ = lookAt;
}

void Camera::translateLookAt(const QVector3D& delta)
{
	lookAt_ += delta;
}

QMatrix4x4 Camera::getViewMatrix() const
{
	QMatrix4x4 ret;
	ret.setToIdentity();
	ret.lookAt(position_, lookAt_, up_);
	return ret;
}

QMatrix4x4 Camera::getProjectionMatrix() const
{
	return projection_;
}

void Camera::rotateAroundLookAt(double angle)
{
	QVector3D gaze = this->lookAt_ - this->position_;
	double radius = QVector2D(gaze.x(), gaze.z()).length();

	current_angle_ += angle;
	//double new_x = position_.x() + sin(current_angle_);
	//double new_z = position_.z() + (angle > 0 ? cos(current_angle_) : -cos(current_angle_));
	qDebug() << "current radius: " << radius;
	position_ = QVector3D(2 * cos(current_angle_), position_.y(), 2 * sin(current_angle_));
	//qDebug() << "x: " << position_.x() << " and z: " << position_.z();
	//qDebug() << "sin angle: " << sin(current_angle_) << " and cos angle: " << cos(current_angle_);
	qDebug() << "X after: " << position_.x() << " and z: " << position_.z();
}