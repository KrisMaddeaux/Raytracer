#pragma once

#include"MathClass.h"

class Ray
{
public:
	Ray() {}

	Ray(Vec3f origin, Vec3f direction)
	{
		m_origin = origin;
		m_direction = direction;
	}

	Vec3f GetOrigin()
	{
		return m_origin;
	}

	Vec3f GetDirection()
	{
		return m_direction;
	}

	Vec3f GetPointAtParameter(float t)
	{
		return m_origin + (m_direction * t);
	}

private:
	Vec3f m_origin;
	Vec3f m_direction;
};

