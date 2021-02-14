#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

#include "HitObjects.h"
#include "Camera.h"

std::vector<HitObject*> g_hitObjectsList;

// Returns a random number between 0 - 1
const float GetRandomNum()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

bool HasHit(Ray r, HitRecord& rHitRecord)
{
	bool hasHit = false;
	float currentClosestHitDistance = INT_MAX;
	for (HitObject* pHitObject : g_hitObjectsList)
	{
		Vec3f intersectPoint;
		if (pHitObject->HasHit(r, currentClosestHitDistance, intersectPoint) /*&& intersectPoint.magnitude() < closestHitDistance*/)
		{
			rHitRecord.m_hitObjectPosition = pHitObject->m_position;
			rHitRecord.m_rayIntersectPoint = intersectPoint;
			//currentClosestHitDistance = intersectPoint.magnitude();
			hasHit = true;
		}
	}
	return hasHit;
}

Vec3f GetRandomUnitVecInSphere()
{
	Vec3f p;
	do 
	{
		p = Vec3f(GetRandomNum(), GetRandomNum(), GetRandomNum()) * 2.0f - Vec3f(1, 1, 1);
	} while (p.x * p.x + p.y * p.y + p.z * p.z >= 1.0f);
	return p;
}

Vec3f GetRaytracedColor(Ray r)
{
	HitRecord hitRecord;
	if (HasHit(r, hitRecord))
	{
		Vec3f normal = (hitRecord.m_rayIntersectPoint - hitRecord.m_hitObjectPosition).normalize();
		Vec3f target = hitRecord.m_rayIntersectPoint + normal + GetRandomUnitVecInSphere();
		return GetRaytracedColor(Ray(hitRecord.m_rayIntersectPoint, target - hitRecord.m_rayIntersectPoint)) * 0.5f;
	}

	Vec3f dir = r.GetDirection().normalize();
	float t = 0.5 * (dir.y + 1.0);
	return LERP(Vec3f(1.0f, 1.0f, 1.0f), Vec3f(0.5f, 0.7f, 1.0f), t);
}

int main()
{
	std::ofstream myfile;
	myfile.open("RaytracedOutput.ppm");
	myfile.clear();

	const int outputImageWidth = 200;
	const int outputImageHeight = 100;
	const int antialisingSamples = 100;
	myfile << "P3\n" << outputImageWidth << " " << outputImageHeight << "\n255\n";

	g_hitObjectsList.push_back(new Sphere(Vec3f(0.0f, 0.0f, -1.0f), 0.5f));
	g_hitObjectsList.push_back(new Sphere(Vec3f(0.0f, -100.5f, -1.0f), 100.0f));

	Camera camera;
	srand(time(0));

	for (int i = outputImageHeight; 0 <= i; i--)
	{
		for (int j = 0; j < outputImageWidth; j++)
		{
			Vec3f col(0.0f, 0.0f, 0.0f);
			for (int k = 0; k < antialisingSamples; k++)
			{
				const float randomU = GetRandomNum();
				const float randomV = GetRandomNum();
				float u = static_cast<float>(j + randomU) / static_cast<float>(outputImageWidth + randomU);
				float v = static_cast<float>(i + randomV) / static_cast<float>(outputImageHeight + randomV);

				Ray r = camera.CastRay(u, v);
				col += GetRaytracedColor(r);
			}

			col.r /= static_cast<float>(antialisingSamples);
			col.g /= static_cast<float>(antialisingSamples);
			col.b /= static_cast<float>(antialisingSamples);
			col = Vec3f(sqrtf(col.r), sqrtf(col.g), sqrtf(col.b));

			int ir = static_cast<int>(255.99 * col.r);
			int ig = static_cast<int>(255.99 * col.g);
			int ib = static_cast<int>(255.99 * col.b);
			myfile << ir << " " << ig << " " << ib << "\n";
		}
	}

	myfile.close();
	for (HitObject* pHitObject : g_hitObjectsList)
	{
		delete pHitObject;
	}
	return 0;
}

