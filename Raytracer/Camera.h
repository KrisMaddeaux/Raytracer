#pragma once

#include "Ray.h"

class Camera
{
public:
	Camera()
	{
		m_lowerLeftCorner = Vec3f(-2.0f, -1.0f, -1.0f);
		m_horizontal = Vec3f(4.0f, 0.0f, 0.0f);
		m_vertical = Vec3f(0.0f, 2.0f, 0.0f);
		m_origin = Vec3f(0.0f, 0.0f, 0.0f);
	}

	Ray CastRay(float u, float v)
	{
		return Ray(m_origin, (m_lowerLeftCorner + m_horizontal * u + m_vertical * v) - m_origin);
	}

private:
	Vec3f m_lowerLeftCorner;
	Vec3f m_horizontal;
	Vec3f m_vertical;
	Vec3f m_origin;
};

