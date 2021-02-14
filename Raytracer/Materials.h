#pragma once

#include "HitObjects.h"

class Material
{
public:
	virtual bool Scatter(Ray inRay, HitRecord hitRecord, Vec3f& rAttenuation, Ray& rScatteredRay) = 0;
};

class LambertianDiffuse : public Material
{
public:
	LambertianDiffuse(Vec3f albedo)
		:m_albedo(albedo)
	{}

	virtual bool Scatter(Ray inRay, HitRecord hitRecord, Vec3f& rAttenuation, Ray& rScatteredRay)
	{
		Vec3f target = hitRecord.m_intersectPoint + hitRecord.m_normal + GetRandomUnitVecInSphere();
		rScatteredRay = Ray(hitRecord.m_intersectPoint, target - hitRecord.m_intersectPoint);
		rAttenuation = m_albedo;
		return true;
	}

private:
	Vec3f m_albedo;
};

class Metal : public Material
{
public:
	Metal(Vec3f albedo, float fuzzyness)
		:m_albedo(albedo)
		, m_fuzzyness(fuzzyness)
	{}

	virtual bool Scatter(Ray inRay, HitRecord hitRecord, Vec3f& rAttenuation, Ray& rScatteredRay)
	{
		Vec3f reflected = Reflect(inRay.GetDirection().normalize(), hitRecord.m_normal);
		rScatteredRay = Ray(hitRecord.m_intersectPoint, reflected + GetRandomUnitVecInSphere() * m_fuzzyness);
		rAttenuation = m_albedo;
		return (rScatteredRay.GetDirection().dot(hitRecord.m_normal) > 0);
	}

private:
	Vec3f m_albedo;
	float m_fuzzyness;
};

