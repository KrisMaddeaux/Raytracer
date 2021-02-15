#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

#include "Materials.h"
#include "Camera.h"

std::vector<HitObject*> g_hitObjectsList;
std::vector<HitObject*> g_lightObjectsList;

bool HasHit(Ray r, float minHitDistance, float maxHitDistance, HitRecord& rHitRecord)
{
	bool hasHit = false;
	float closestHitDistance = maxHitDistance;
	for (HitObject* pHitObject : g_hitObjectsList)
	{
		if (pHitObject->HasHit(r, minHitDistance, closestHitDistance, rHitRecord))
		{
			hasHit = true;
		}
	}
	return hasHit;
}

Vec3f GetRaytracedColor(Ray r, int depth)
{
	HitRecord hitRecord;
	if (HasHit(r, 0.001f, INT_MAX, hitRecord))
	{
		Ray scattered;
		if (depth < 50 && hitRecord.m_pMaterial->Scatter(r, hitRecord, scattered))
		{
			Vec3f lightColour = Vec3f(0.0f, 0.0f, 0.0f);
			for (HitObject* pLightObject : g_lightObjectsList)
			{
				float distanceToLight = (hitRecord.m_intersectPoint - pLightObject->m_position).magnitude();
				if (distanceToLight <= static_cast<LightSphere*>(pLightObject)->m_lightRadius)
				{
					//float lightAttenuation = 1.0f / (0.5f + distanceToLight * distanceToLight);
					//light += pLightObject->m_pMaterial->m_albedo * lightAttenuation;
					lightColour += LERP(pLightObject->m_pMaterial->m_diffuseColour, Vec3f(0.0f, 0.0f, 0.0f), distanceToLight / static_cast<LightSphere*>(pLightObject)->m_lightRadius);
				}
			}

			return lightColour + hitRecord.m_pMaterial->m_diffuseColour * GetRaytracedColor(scattered, depth + 1);
		}
		else
		{
			return Vec3f(0.0f, 0.0f, 0.0f);
		}
	}

	Vec3f dir = r.GetDirection().normalize();
	float t = 0.5 * (dir.y + 1.0);
	return LERP(Vec3f(1.0f, 1.0f, 1.0f), Vec3f(0.5f, 0.7f, 1.0f), t);
}

void MakeScene()
{
	g_hitObjectsList.push_back(new Sphere(Vec3f(0.0f, -1000.0f, 0.0f), 1000.0f, new LambertianDiffuse(Vec3f(0.5f, 0.5f, 0.5f))));
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float chooseMaterial = GetRandomNum();
			Vec3f center(a + 0.9f * GetRandomNum(), 0.2f, b + 0.9f * GetRandomNum());
			if (chooseMaterial <= 0.5f)
			{
				g_hitObjectsList.push_back(new Sphere(center, 0.2f, new LambertianDiffuse(Vec3f(GetRandomNum() * GetRandomNum(), GetRandomNum() * GetRandomNum(), GetRandomNum() * GetRandomNum()))));
			}
			else
			{
				g_hitObjectsList.push_back(new Sphere(center, 0.2f, new Metal(Vec3f(0.5f * (1.0f + GetRandomNum()), 0.5f * (1.0f + GetRandomNum()), 0.5f * (1.0f + GetRandomNum())), 0.5f * GetRandomNum())));
			}
		}
	}

	LightSphere* pLightObject = new LightSphere(Vec3f(0.0f, 1.0f, 0.0f), 0.5f, 2.5f, new Emmisive(Vec3f(0.969f, 0.906f, 0.039f)));
	g_hitObjectsList.push_back(pLightObject);
	g_hitObjectsList.push_back(new Sphere(Vec3f(-4.0f, 1.0f, 0.0f), 1.0f, new Metal(Vec3f(0.7f, 0.6f, 0.5f), 0.0f)));
	g_hitObjectsList.push_back(new Sphere(Vec3f(4.0f, 1.0f, 0.0f), 1.0f, new Metal(Vec3f(0.7f, 0.6f, 0.5f), 0.0f)));

	g_lightObjectsList.push_back(pLightObject);
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

	MakeScene();

	Vec3f lookfrom(13.0f, 2.0f, 3.0f);
	Vec3f lookat(0.0f, 0.0f, 0.0f);
	float distanceToFocus = 10.0f;
	float aperture = 0.1f;
	Camera camera(lookfrom, lookat, Vec3f(0.0f, 1.0f, 0.0f), 20.0f, static_cast<float>(outputImageWidth) / static_cast<float>(outputImageHeight), aperture, distanceToFocus);

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
				col += GetRaytracedColor(r, 0);
			}

			col.r /= static_cast<float>(antialisingSamples);
			col.g /= static_cast<float>(antialisingSamples);
			col.b /= static_cast<float>(antialisingSamples);
			//col = Vec3f(sqrtf(col.r), sqrtf(col.g), sqrtf(col.b));

			col = ACESFilmToneMapper(col);
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

