#pragma once

#include "Ray.h"

class Material;
class HitObject;

struct HitRecord
{
	Vec3f m_objectPosition;
	Vec3f m_intersectPoint;
	Vec3f m_normal;
	Material* m_pMaterial;
	HitObject* m_pHitObject;
};

// An object that can be hit by a ray
class HitObject
{
public:
	~HitObject()
	{
		delete m_pMaterial;
	}

	virtual bool HasHit(Ray r, float minHitDistance, float& rMaxHitDistance, HitRecord& rHitRecord) = 0;
	Vec3f m_position;
	Material* m_pMaterial;
};

class Sphere : public HitObject
{
public:
	Sphere(Vec3f center, float radius, Material* material)
	{
		m_position = center;
		m_radius = radius;
		m_pMaterial = material;
	}

	virtual bool HasHit(Ray r, float minHitDistance, float& rMaxHitDistance, HitRecord& rHitRecord)
	{
		bool hasHitSphere = false;

		Vec3f oc = r.GetOrigin() - m_position;
		float a = r.GetDirection().dot(r.GetDirection());
		float b = oc.dot(r.GetDirection());
		float c = oc.dot(oc) - m_radius * m_radius;
		float discriminant = b * b - a * c;
		if (discriminant > 0)
		{
			float t = (-b - sqrtf(b * b - a * c)) / a;
			if (t < rMaxHitDistance && t > minHitDistance)
			{
				rMaxHitDistance = t;
				rHitRecord.m_intersectPoint = r.GetPointAtParameter(t);
				rHitRecord.m_objectPosition = m_position;
				rHitRecord.m_normal = (rHitRecord.m_intersectPoint - rHitRecord.m_objectPosition).normalize();
				rHitRecord.m_pMaterial = m_pMaterial;
				rHitRecord.m_pHitObject = this;
				hasHitSphere = true;
			}

			t = (-b + sqrtf(b * b - a * c)) / a;
			if (t < rMaxHitDistance && t > minHitDistance)
			{
				rMaxHitDistance = t;
				rHitRecord.m_intersectPoint = r.GetPointAtParameter(t);
				rHitRecord.m_objectPosition = m_position;
				rHitRecord.m_normal = (rHitRecord.m_intersectPoint - rHitRecord.m_objectPosition).normalize();
				rHitRecord.m_pMaterial = m_pMaterial;
				rHitRecord.m_pHitObject = this;
				hasHitSphere = true;
			}
		}

		
		/*
		Vec3f centerPos = m_position - r.GetOrigin();
		Vec3f rayDir = r.GetDirection().normalize();
		float proj = rayDir.dot(centerPos);
		if (proj > 0.0f)
		{
			Vec3f projPos = rayDir * proj;
			float distanceToSphere = (projPos - centerPos).magnitude();
			if (distanceToSphere <= m_radius)
			{
				float offset = m_radius - distanceToSphere;
				Vec3f intersectPoint = projPos - (rayDir * offset);
				float intersectDistance = (intersectPoint - r.GetOrigin()).magnitude();
				if (intersectDistance < rMaxHitDistance && intersectDistance > minHitDistance)
				{
					rMaxHitDistance = intersectDistance;
					rHitRecord.m_intersectPoint = intersectPoint;
					rHitRecord.m_objectPosition = m_position;
					hasHitSphere = true;
				}
			}
		}
		*/

		return hasHitSphere;
	}

	float m_radius;
};

class LightSphere : public Sphere
{
public:
	LightSphere(Vec3f center, float radius, float lightRadius, Material* material)
		:Sphere(center, radius, material)
		,m_isLightHit(false)
	{
		m_lightRadius = (lightRadius > radius) ? lightRadius : radius;
	}

	virtual bool HasHit(Ray r, float minHitDistance, float& rMaxHitDistance, HitRecord& rHitRecord)
	{
		bool hasHitSphere = false;

		if (!Sphere::HasHit(r, minHitDistance, rMaxHitDistance, rHitRecord))
		{
			// For debugging lights
			/*
			Vec3f oc = r.GetOrigin() - m_position;
			float a = r.GetDirection().dot(r.GetDirection());
			float b = oc.dot(r.GetDirection());
			float c = oc.dot(oc) - m_lightRadius * m_lightRadius;
			float discriminant = b * b - a * c;
			if (discriminant > 0)
			{
				float t = (-b - sqrtf(b * b - a * c)) / a;
				if (t < rMaxHitDistance && t > minHitDistance)
				{
					rMaxHitDistance = t;
					rHitRecord.m_intersectPoint = r.GetPointAtParameter(t);
					rHitRecord.m_objectPosition = m_position;
					rHitRecord.m_normal = (rHitRecord.m_intersectPoint - rHitRecord.m_objectPosition).normalize();
					rHitRecord.m_pMaterial = m_pMaterial;
					rHitRecord.m_pHitObject = this;
					hasHitSphere = true;
					m_isLightHit = true;
				}

				t = (-b + sqrtf(b * b - a * c)) / a;
				if (t < rMaxHitDistance && t > minHitDistance)
				{
					rMaxHitDistance = t;
					rHitRecord.m_intersectPoint = r.GetPointAtParameter(t);
					rHitRecord.m_objectPosition = m_position;
					rHitRecord.m_normal = (rHitRecord.m_intersectPoint - rHitRecord.m_objectPosition).normalize();
					rHitRecord.m_pMaterial = m_pMaterial;
					rHitRecord.m_pHitObject = this;
					hasHitSphere = true;
					m_isLightHit = true;
				}
			}
			*/
		}
		else
		{
			hasHitSphere = true;
		}

		return hasHitSphere;
	}

	float m_lightRadius;
	bool m_isLightHit;
};

