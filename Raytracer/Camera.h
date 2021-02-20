#pragma once

#include "Ray.h"

static Vec3f RandomInUnitDisk()
{
	Vec3f p;
	do
	{
		p = Vec3f(GetRandomNum(), GetRandomNum(), 0.0f) * 2.0f - Vec3f(1.0f, 1.0f, 0.0f);
	} while (p.dot(p) >= 1.0f);
	return p;
}

class Camera
{
public:
	Camera() {}

	void Setup(Vec3f lookfrom, Vec3f lookat, Vec3f vup, float vfov, float aspect, float aperture, float focusDistance) // vfov is top to bottom in degrees
	{
		m_lensRadius = aperture / 2.0f;
		float theta = vfov * PI / 180.0f;
		float halfHeight = tanf(theta / 2.0f);
		float halfWidth = aspect * halfHeight;

		m_origin = lookfrom;

		m_w = (lookfrom - lookat).normalize();
		m_u = vup.cross(m_w).normalize();
		m_v = m_w.cross(m_u);

		m_lowerLeftCorner = lookfrom - m_u * halfWidth * focusDistance - m_v * halfHeight * focusDistance - m_w * focusDistance;
		m_horizontal = m_u * halfWidth * 2 * focusDistance;
		m_vertical = m_v * halfHeight * 2 * focusDistance;
	}

	Ray CastRay(float u, float v)
	{
		Vec3f rd =  RandomInUnitDisk() * m_lensRadius;
		Vec3f offset = m_u * rd.x + m_v * rd.y;
		return Ray(m_origin + offset, m_lowerLeftCorner + m_horizontal * u + m_vertical * v - m_origin - offset);
	}

	Vec3f GetPosition()
	{
		return m_origin;
	}

private:
	Vec3f m_lowerLeftCorner;
	Vec3f m_horizontal;
	Vec3f m_vertical;
	Vec3f m_origin;
	Vec3f m_u, m_v, m_w;
	float m_lensRadius;
};

