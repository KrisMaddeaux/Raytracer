#include <iostream>
#include <fstream>
#include <vector>
#include "HitObjects.h"

std::vector<HitObject*> g_hitObjectsList;

Vec3f GetRaytracedColor(Ray r)
{
	for (HitObject * pHitObject : g_hitObjectsList)
	{
		Vec3f intersectPoint;
		if(pHitObject->HasHit(r, intersectPoint))
		{
			Vec3f normal = (intersectPoint - pHitObject->m_position).normalize();
			return Vec3f(normal.x + 1.0f, normal.y + 1.0f, normal.z + 1.0f) * 0.5f;
		}
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
	myfile << "P3\n" << nx << " " << ny << "\n255\n";

	Vec3f lowerLeftCorner(-2.0f, -1.0f, -1.0f);
	Vec3f horizontal(4.0f, 0.0f, 0.0f);
	Vec3f vertical(0.0f, 2.0f, 0.0f);
	Vec3f origin(0.0f, 0.0f, 0.0f);

	g_hitObjectsList.push_back(new Sphere(Vec3f(0.0f, 0.0f, -1.0f), 0.5f));
	g_hitObjectsList.push_back(new Sphere(Vec3f(0.0f, -100.5f, -1.0f), 100.0f));

	for (int i = ny; 0 <= i; i--)
	{
		for (int j = 0; j < nx; j++)
		{
			float u = float(j) / float(nx);
			float v = float(i) / float(ny);
			Ray r(origin, (lowerLeftCorner + horizontal * u + vertical * v ) - origin);
			Vec3f col = GetRaytracedColor(r);
			int ir = int(255.99 * col.r);
			int ig = int(255.99 * col.g);
			int ib = int(255.99 * col.b);
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

