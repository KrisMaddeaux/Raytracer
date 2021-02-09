#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

#include "HitObjects.h"
#include "Camera.h"

std::vector<HitObject*> g_hitObjectsList;

bool HasHit(Ray r, HitRecord& rHitRecord)
{
	bool hasHit = false;
	float closestHitDistance = INT_MAX;
	for (HitObject* pHitObject : g_hitObjectsList)
	{
		Vec3f intersectPoint;
		if (pHitObject->HasHit(r, intersectPoint) && intersectPoint.magnitude() < closestHitDistance)
		{
			rHitRecord.m_hitObjectPosition = pHitObject->m_position;
			rHitRecord.m_rayIntersectPoint = intersectPoint;
			closestHitDistance = intersectPoint.magnitude();
			hasHit = true;
		}
	}
	return hasHit;
}

Vec3f GetRaytracedColor(Ray r)
{
	HitRecord hitRecord;
	if (HasHit(r, hitRecord))
	{
		Vec3f normal = (hitRecord.m_rayIntersectPoint - hitRecord.m_hitObjectPosition).normalize();
		return Vec3f(normal.x + 1.0f, normal.y + 1.0f, normal.z + 1.0f) * 0.5f;
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

	int nx = 200;
	int ny = 100;
	int ns = 100;
	myfile << "P3\n" << nx << " " << ny << "\n255\n";

	g_hitObjectsList.push_back(new Sphere(Vec3f(0.0f, 0.0f, -1.0f), 0.5f));
	g_hitObjectsList.push_back(new Sphere(Vec3f(0.0f, -100.5f, -1.0f), 100.0f));

	Camera camera;
	srand(time(0));

	for (int i = ny; 0 <= i; i--)
	{
		for (int j = 0; j < nx; j++)
		{
			Vec3f col(0.0f, 0.0f, 0.0f);
			for (int k = 0; k < ns; k++)
			{
				float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				float u = static_cast<float>(j + random) / static_cast<float>(nx);
				float v = static_cast<float>(i + random) / static_cast<float>(ny);

				Ray r = camera.CastRay(u, v);
				col += GetRaytracedColor(r);
			}

			col.r /= static_cast<float>(ns);
			col.g /= static_cast<float>(ns);
			col.b /= static_cast<float>(ns);

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

