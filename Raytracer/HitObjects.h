#pragma once

#include "Ray.h"

struct HitRecord
{
	Vec3f m_hitObjectPosition;
	Vec3f m_rayIntersectPoint;
};

// An object that can be hit by a ray
class HitObject
{
public:
	virtual bool HasHit(Ray r, Vec3f& rIntersectPoint) = 0;
	Vec3f m_position;
};

class Sphere : public HitObject
{
public:
	Sphere(Vec3f center, float radius)
	{
		m_position = center;
		m_radius = radius;
	}

	virtual bool HasHit(Ray r, Vec3f& rIntersectPoint)
	{
		bool hasHitSphere = false;

		/*Vec3f oc = r.GetOrigin() - m_position;
		float a = r.GetDirection().dot(r.GetDirection());
		float b = oc.dot(r.GetDirection());
		float c = oc.dot(oc) - m_radius * m_radius;
		float discriminant = b * b - a * c;
		if (discriminant > 0)
		{
			float t = (-b - sqrtf(b * b - a * c)) / a;
			if (t < INT_MAX && t > 0.0f)
			{
				rIntersectPoint = r.GetPointAtParameter(t);
				hasHitSphere = true;
			}

			t = (-b + sqrtf(b * b - a * c)) / a;
			if (t < INT_MAX && t > 0.0f)
			{
				rIntersectPoint = r.GetPointAtParameter(t);
				hasHitSphere = true;
			}
		}*/

		
		Vec3f centerPos = m_position - r.GetOrigin();
		Vec3f rayDir = r.GetDirection().normalize();
		float proj = rayDir.dot(centerPos);
		if (proj >= 0.0f)
		{
			Vec3f projPos = rayDir * proj;
			float distanceToSphere = (projPos - centerPos).magnitude();
			if (distanceToSphere <= m_radius)
			{
				float offset = m_radius - distanceToSphere;
				rIntersectPoint = projPos - (rayDir * offset);
				hasHitSphere = true;
			}
		}


		return hasHitSphere;
	}

private:
	float m_radius;
};

