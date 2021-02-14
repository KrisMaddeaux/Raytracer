#pragma once

#include "HitObjects.h"

class Material
{
public:
	virtual bool Scatter(Ray inRay, HitRecord hitRecord, Ray& rScatteredRay) = 0;
	Vec3f m_diffuseColour;
};

class LambertianDiffuse : public Material
{
public:
	LambertianDiffuse(Vec3f diffuse)
	{
		m_diffuseColour = diffuse;
	}

	virtual bool Scatter(Ray inRay, HitRecord hitRecord, Ray& rScatteredRay)
	{
		Vec3f target = hitRecord.m_intersectPoint + hitRecord.m_normal + GetRandomUnitVecInSphere();
		rScatteredRay = Ray(hitRecord.m_intersectPoint, target - hitRecord.m_intersectPoint);
		return true;
	}
};

class Metal : public Material
{
public:
	Metal(Vec3f diffuse, float fuzzyness)
		:m_fuzzyness(fuzzyness)
	{
		m_diffuseColour = diffuse;
	}

	virtual bool Scatter(Ray inRay, HitRecord hitRecord, Ray& rScatteredRay)
	{
		Vec3f reflected = Reflect(inRay.GetDirection().normalize(), hitRecord.m_normal);
		rScatteredRay = Ray(hitRecord.m_intersectPoint, reflected + GetRandomUnitVecInSphere() * m_fuzzyness);
		return (rScatteredRay.GetDirection().dot(hitRecord.m_normal) > 0);
	}

private:
	float m_fuzzyness;
};

class Emmisive : public Material
{
public:
	Emmisive(Vec3f diffuse)
	{
		m_diffuseColour = diffuse;
	}

	virtual bool Scatter(Ray inRay, HitRecord hitRecord, Ray& rScatteredRay)
	{
		// For debugging lights
		/*
		LightSphere* pLightSphere = static_cast<LightSphere*>(hitRecord.m_pHitObject);
		float hitDistance = (hitRecord.m_intersectPoint - hitRecord.m_objectPosition).magnitude();
		if (hitDistance > pLightSphere->m_radius)
		{
			float distanceToSphere = pLightSphere->m_lightRadius - pLightSphere->m_radius;
			Vec3f directionToSphere = (hitRecord.m_objectPosition - hitRecord.m_intersectPoint).normalize();
			Vec3f sphereIntersectPoint = directionToSphere * distanceToSphere;
			float attenuationDistance = (sphereIntersectPoint - hitRecord.m_intersectPoint).magnitude();
			float attenuation = 1.0f / (1.0f + attenuationDistance * attenuationDistance);
			if (attenuation < 0.0f)
			{
				attenuation = 0.0f;
			}
			else if (attenuation > 1.0f)
			{
				attenuation = 1.0f;
			}
			//rAttenuation = m_albedo * attenuation;
			rAttenuation = m_albedo;
			rScatteredRay = Ray(hitRecord.m_intersectPoint, hitRecord.m_intersectPoint + inRay.GetDirection().normalize());
		}
		else
		*/
		{
			rScatteredRay = Ray(hitRecord.m_intersectPoint, hitRecord.m_objectPosition);
		}
		return true;
	}
};

