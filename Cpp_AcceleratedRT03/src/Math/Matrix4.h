#ifndef MATRIX4_H
#define MATRIX4_H

#include <cmath>
#include <string>

#include "../Utilities/Utility.h"

// A Vector4 type will probably need to be made to communicate with this class,
// so transformations can be done.

class Matrix4
{
private:
	// Composed of column vectors.
	double x[4];
	double y[4];
	double z[4];
	double w[4];
public:
	Matrix4()
	{
		this->x[0] = 0.0;
		this->x[1] = 0.0;
		this->x[2] = 0.0;
		this->x[3] = 0.0;

		this->y[0] = 0.0;
		this->y[1] = 0.0;
		this->y[2] = 0.0;
		this->y[3] = 0.0;

		this->z[0] = 0.0;
		this->z[1] = 0.0;
		this->z[2] = 0.0;
		this->z[3] = 0.0;

		this->w[0] = 0.0;
		this->w[1] = 0.0;
		this->w[2] = 0.0;
		this->w[3] = 0.0;
	}

	static Matrix4 identity()
	{
		Matrix4 m = Matrix4();
		m.x[0] = 1.0;
		m.y[1] = 1.0;
		m.z[2] = 1.0;
		m.w[3] = 1.0;
		return m;
	}

	static Matrix4 translation(double x, double y, double z)
	{
		Matrix4 m = Matrix4::identity();
		m.x[3] = x;
		m.y[3] = y;
		m.z[3] = z;
		return m;
	}

	static Matrix4 scale(double x, double y, double z)
	{
		Matrix4 m = Matrix4::identity();
		m.x[0] = x;
		m.y[1] = y;
		m.z[2] = z;
		return m;
	}

	static Matrix4 xRotation(double radians)
	{
		Matrix4 m = Matrix4::identity();
		double sAngle = std::sin(radians);
		double cAngle = std::cos(radians);
		m.y[1] = cAngle;
		m.y[2] = sAngle;
		m.z[1] = -sAngle;
		m.z[2] = cAngle;
		return m;
	}

	static Matrix4 yRotation(double radians)
	{
		Matrix4 m = Matrix4::identity();
		double sAngle = std::sin(radians);
		double cAngle = std::cos(radians);
		m.x[0] = cAngle;
		m.x[2] = sAngle;
		m.z[0] = -sAngle;
		m.z[2] = cAngle;
		return m;
	}

	static Matrix4 zRotation(double radians)
	{
		Matrix4 m = Matrix4::identity();
		double sAngle = std::sin(radians);
		double cAngle = std::cos(radians);
		m.x[0] = cAngle;
		m.x[1] = sAngle;
		m.y[0] = -sAngle;
		m.y[1] = cAngle;
		return m;
	}

	static Matrix4 projection(double near, double far, double width, double height)
	{
		Matrix4 m = Matrix4::identity();
		m.x[0] = (2.0 * near) / width;
		m.y[1] = (2.0 * near) / height;
		m.z[2] = -(far + near) / (far - near);
		m.z[3] = -1.0;
		m.w[2] = ((-2.0 * far) * near) / (far - near);
		m.w[3] = 0.0;
		return m;
	}

	static Matrix4 perspective(double fovY, double aspect, double near, double far)
	{
		double height = (2.0 * near) * std::tan(((0.5 * Utility::PI) * fovY) / 180.0);
		double width = height * aspect;
		return Matrix4::projection(near, far, width, height);
	}

	Matrix4 operator *(const Matrix4 &m) const
	{
		Matrix4 p = Matrix4();

		// Column X
		p.x[0] =
			(this->x[0] * m.x[0]) +
			(this->y[0] * m.x[1]) +
			(this->z[0] * m.x[2]) +
			(this->w[0] * m.x[3]);
		p.x[1] =
			(this->x[1] * m.x[0]) +
			(this->y[1] * m.x[1]) +
			(this->z[1] * m.x[2]) +
			(this->w[1] * m.x[3]);
		p.x[2] =
			(this->x[2] * m.x[0]) +
			(this->y[2] * m.x[1]) +
			(this->z[2] * m.x[2]) +
			(this->w[2] * m.x[3]);
		p.x[3] =
			(this->x[3] * m.x[0]) +
			(this->y[3] * m.x[1]) +
			(this->z[3] * m.x[2]) +
			(this->w[3] * m.x[3]);

		// Column Y
		p.y[0] =
			(this->x[0] * m.y[0]) +
			(this->y[0] * m.y[1]) +
			(this->z[0] * m.y[2]) +
			(this->w[0] * m.y[3]);
		p.y[1] =
			(this->x[1] * m.y[0]) +
			(this->y[1] * m.y[1]) +
			(this->z[1] * m.y[2]) +
			(this->w[1] * m.y[3]);
		p.y[2] =
			(this->x[2] * m.y[0]) +
			(this->y[2] * m.y[1]) +
			(this->z[2] * m.y[2]) +
			(this->w[2] * m.y[3]);
		p.y[3] =
			(this->x[3] * m.y[0]) +
			(this->y[3] * m.y[1]) +
			(this->z[3] * m.y[2]) +
			(this->w[3] * m.y[3]);

		// Column Z
		p.z[0] =
			(this->x[0] * m.z[0]) +
			(this->y[0] * m.z[1]) +
			(this->z[0] * m.z[2]) +
			(this->w[0] * m.z[3]);
		p.z[1] =
			(this->x[1] * m.z[0]) +
			(this->y[1] * m.z[1]) +
			(this->z[1] * m.z[2]) +
			(this->w[1] * m.z[3]);
		p.z[2] =
			(this->x[2] * m.z[0]) +
			(this->y[2] * m.z[1]) +
			(this->z[2] * m.z[2]) +
			(this->w[2] * m.z[3]);
		p.z[3] =
			(this->x[3] * m.z[0]) +
			(this->y[3] * m.z[1]) +
			(this->z[3] * m.z[2]) +
			(this->w[3] * m.z[3]);

		// Column W
		p.w[0] =
			(this->x[0] * m.w[0]) +
			(this->y[0] * m.w[1]) +
			(this->z[0] * m.w[2]) +
			(this->w[0] * m.w[3]);
		p.w[1] =
			(this->x[1] * m.w[0]) +
			(this->y[1] * m.w[1]) +
			(this->z[1] * m.w[2]) +
			(this->w[1] * m.w[3]);
		p.w[2] =
			(this->x[2] * m.w[0]) +
			(this->y[2] * m.w[1]) +
			(this->z[2] * m.w[2]) +
			(this->w[2] * m.w[3]);
		p.w[3] =
			(this->x[3] * m.w[0]) +
			(this->y[3] * m.w[1]) +
			(this->z[3] * m.w[2]) +
			(this->w[3] * m.w[3]);

		return p;
	}
	
	std::string toString() const
	{
		return std::string("[") +
			std::to_string(this->x[0]) + std::string(", ") +
			std::to_string(this->y[0]) + std::string(", ") +
			std::to_string(this->z[0]) + std::string(", ") +
			std::to_string(this->w[0]) + std::string(",\n ") +

			std::to_string(this->x[1]) + std::string(", ") +
			std::to_string(this->y[1]) + std::string(", ") +
			std::to_string(this->z[1]) + std::string(", ") +
			std::to_string(this->w[1]) + std::string(",\n ") +

			std::to_string(this->x[2]) + std::string(", ") +
			std::to_string(this->y[2]) + std::string(", ") +
			std::to_string(this->z[2]) + std::string(", ") +
			std::to_string(this->w[2]) + std::string(",\n ") +

			std::to_string(this->x[3]) + std::string(", ") +
			std::to_string(this->y[3]) + std::string(", ") +
			std::to_string(this->z[3]) + std::string(", ") +
			std::to_string(this->w[3]) + std::string("]");
	}
};

#endif