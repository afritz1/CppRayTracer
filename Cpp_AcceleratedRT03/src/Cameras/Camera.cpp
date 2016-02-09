#include <iostream>

#include "Camera.h"
#include "../Math/Quaternion.h"

const double Camera::DEFAULT_ZOOM = 1.25;
const double Camera::DEFAULT_MOVE_SPEED = 0.040;
const double Camera::DEFAULT_TURN_SPEED = 1.20;
const double Camera::DEFAULT_GRAB_DISTANCE = 2.00;
const double Camera::DEFAULT_HOLD_DISTANCE = 2.50;
const double Camera::DEFAULT_HOLD_INCREMENT = 0.10;
const Vector3 Camera::GLOBAL_UP = Vector3(0.0, 1.0, 0.0);

Camera::Camera(const Vector3 &eye, const Vector3 &forward, const Vector3 &right,
	const Vector3 &up, double zoom, double moveSpeed, double turnSpeed, 
	double holdDistance)
{
	this->eye = eye;
	this->forward = forward;
	this->right = right;
	this->up = up;
	this->zoom = zoom;
	this->moveSpeed = moveSpeed;
	this->turnSpeed = turnSpeed;
	this->holdDistance = holdDistance;
}

Camera Camera::defaultCamera(double worldSize, double aspect)
{
	return Camera::lookAt(
		Vector3(worldSize * 0.75, 1.0, -worldSize * 0.75),
		Vector3(0.0, 1.0, 0.0),
		Camera::GLOBAL_UP,
		aspect,
		Camera::DEFAULT_ZOOM,
		Camera::DEFAULT_MOVE_SPEED,
		Camera::DEFAULT_TURN_SPEED,
		Camera::DEFAULT_HOLD_DISTANCE);
}

Camera Camera::lookAt(const Vector3 &eye, const Vector3 &focus, const Vector3 &up,
	double aspect, double zoom, double moveSpeed, double turnSpeed, 
	double holdDistance)
{
	Vector3 newEye = eye;
	Vector3 newForward = (focus - eye).normalized().scaledBy(zoom);
	Vector3 newRight = newForward.cross(up).normalized().scaledBy(aspect);
	Vector3 newUp = newRight.cross(newForward).normalized();
	return Camera(newEye, newForward, newRight, newUp, zoom, moveSpeed, turnSpeed, 
		holdDistance);
}

const Vector3 &Camera::getEye() const
{
	return this->eye;
}

const Vector3 &Camera::getForward() const
{
	return this->forward;
}

const Vector3 &Camera::getRight() const
{
	return this->right;
}

const Vector3 &Camera::getUp() const
{
	return this->up;
}

double Camera::getZoom() const
{
	return this->zoom;
}

double Camera::getMoveSpeed() const
{
	return this->moveSpeed;
}

double Camera::getTurnSpeed() const
{
	return this->turnSpeed;
}

double Camera::getGrabDistance() const
{
	return Camera::DEFAULT_GRAB_DISTANCE;
}

double Camera::getHoldDistance() const
{
	return this->holdDistance;
}

void Camera::setZoom(double zoom)
{
	this->zoom = zoom;
	this->forward = this->forward.normalized().scaledBy(this->zoom);
}

void Camera::setAspectRatio(double aspect)
{
	this->right = this->right.normalized().scaledBy(aspect);
}

void Camera::setHoldDistance(double distance)
{
	this->holdDistance = (distance < Utility::EPSILON) ? Utility::EPSILON : distance;
}

void Camera::moveBy(const Vector3 &dv)
{
	if (std::isinf(dv.length()) || std::isnan(dv.length()))
	{
		return;
	}

	this->eye = this->eye + dv.scaledBy(this->moveSpeed);
}

void Camera::pitch(double radians)
{
	Quaternion q = Quaternion::fromAxisAngle(this->right, radians / this->zoom) *
		Quaternion(this->forward, 0.0);

	this->forward = Vector3(q.getX(), q.getY(), q.getZ()).normalized().scaledBy(this->zoom);
	Camera newCamera = Camera::lookAt(this->eye, this->eye + this->forward,
		Camera::GLOBAL_UP, this->right.length(), this->zoom, this->moveSpeed,
		this->turnSpeed, this->holdDistance);
	this->up = newCamera.up;
}

void Camera::yaw(double radians)
{
	Quaternion q = Quaternion::fromAxisAngle(Camera::GLOBAL_UP, radians / this->zoom) *
		Quaternion(this->forward, 0.0);

	this->forward = Vector3(q.getX(), q.getY(), q.getZ()).normalized().scaledBy(this->zoom);
	Camera newCamera = Camera::lookAt(this->eye, this->eye + this->forward,
		Camera::GLOBAL_UP, this->right.length(), this->zoom, this->moveSpeed,
		this->turnSpeed, this->holdDistance);
	this->up = newCamera.up;
	this->right = newCamera.right;
}

void Camera::rotate(int dx, int dy)
{
	const double DEG_TO_RAD = static_cast<double>(Utility::PI / 180.0);

	double lookRightRads = (this->turnSpeed * static_cast<double>(dx)) * DEG_TO_RAD;
	double lookUpRads = (this->turnSpeed * static_cast<double>(dy)) * DEG_TO_RAD;

	const double currentDec = std::acos(this->forward.normalized().getY());
	const double requestedDec = currentDec - lookUpRads;

	const double MIN_UP_TILT_DEG = 0.10;
	const double zenithMaxDec = MIN_UP_TILT_DEG * DEG_TO_RAD;
	const double zenithMinDec = (180.0 - MIN_UP_TILT_DEG) * DEG_TO_RAD;

	lookUpRads = (requestedDec > zenithMinDec) ? (currentDec - zenithMinDec) :
		((requestedDec < zenithMaxDec) ? (currentDec - zenithMaxDec) : lookUpRads);

	this->pitch(lookUpRads);
	this->yaw(-lookRightRads);
}

void Camera::calculateImageRays(std::vector<Vector3> &imageDirections, int width,
	int height) const
{
	const double widthRecip = 1.0 / static_cast<double>(width);
	const double heightRecip = 1.0 / static_cast<double>(height);

	const Camera camera = *this;
	const Vector3 eye = camera.eye;
	const Vector3 forward = camera.forward;
	const Vector3 right = camera.right;
	const Vector3 up = camera.up;

	const Vector3 topLeft = eye + forward + up - right;
	const Vector3 imagePointStep = right.scaledBy(widthRecip * 2.0);

#pragma omp parallel for
	for (int y = 0; y < height; y++)
	{
		double yy = heightRecip * static_cast<double>(y);
		Vector3 direction = (topLeft - up.scaledBy(2.0 * yy)) - eye;

		int index = y * width;
		for (int x = 0; x < width; x++, index++)
		{
			double len = direction.lengthSquared();
			double halfLength = len * 0.5;
			llong i = *reinterpret_cast<llong*>(&len);
			i = 0x5fe6eb50c7b537a9 - (i >> 1);
			len = *reinterpret_cast<double*>(&i);
			len = len * (1.5 - (halfLength * len * len));
			len = len * (1.5 - (halfLength * len * len));
			len = len * (1.5 - (halfLength * len * len));

			imageDirections[index] = direction.scaledBy(len);
			direction = direction + imagePointStep;
		}
	}
}