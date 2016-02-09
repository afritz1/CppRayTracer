#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include "../Math/Vector3.h"

class Camera
{
private:
	Vector3 eye, forward, right, up;
	double zoom, moveSpeed, turnSpeed, holdDistance;

	Camera();

	void pitch(double radians);
	void yaw(double radians);
public:
	static const double DEFAULT_ZOOM;
	static const double DEFAULT_MOVE_SPEED;
	static const double DEFAULT_TURN_SPEED;
	static const double DEFAULT_GRAB_DISTANCE;
	static const double DEFAULT_HOLD_DISTANCE;
	static const double DEFAULT_HOLD_INCREMENT;
	static const Vector3 GLOBAL_UP;

	Camera(const Vector3 &eye, const Vector3 &forward, const Vector3 &right,
		const Vector3 &up, double zoom, double moveSpeed, double turnSpeed, double holdDistance);

	static Camera defaultCamera(double worldSize, double aspect);
	static Camera lookAt(const Vector3 &eye, const Vector3 &focus, const Vector3 &up,
		double aspect, double zoom, double moveSpeed, double turnSpeed, double holdDistance);

	const Vector3 &getEye() const;
	const Vector3 &getForward() const;
	const Vector3 &getRight() const;
	const Vector3 &getUp() const;
	double getZoom() const;
	double getMoveSpeed() const;
	double getTurnSpeed() const;
	double getGrabDistance() const;
	double getHoldDistance() const;
	void setZoom(double zoom);
	void setAspectRatio(double aspect);
	void setHoldDistance(double distance);
	void moveBy(const Vector3 &dv);
	void rotate(int dx, int dy);
	void calculateImageRays(std::vector<Vector3> &imageDirections, int width, int height) const;
};

#endif