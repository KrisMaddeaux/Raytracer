#pragma once

#include "Ray.h"

class Camera
{
public:
	Camera(Vec3f lookfrom, Vec3f lookat, Vec3f vup, float vfov, float aspect) // vfov is top to bottom in degrees
	{
		float theta = vfov * PI / 180.0f;
		float halfHeight = tanf(theta / 2.0f);
		float halfWidth = aspect * halfHeight;

		m_origin = lookfrom;

		Vec3f u, v, w;
		w = (lookfrom - lookat).normalize();
		u = vup.cross(w).normalize();
		v = w.cross(u);

		m_lowerLeftCorner = lookfrom - u * halfWidth - v * halfHeight - w;
		m_horizontal = u * halfWidth * 2;
		m_vertical = v * halfHeight * 2;
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

