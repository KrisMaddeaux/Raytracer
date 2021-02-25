#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <thread> 

#include "Materials.h"
#include "Camera.h"

#define PROCESSOR_NUM 8

std::vector<Vec3f> g_outputPixels[PROCESSOR_NUM];

std::vector<HitObject*> g_hitObjectsList;
std::vector<HitObject*> g_lightObjectsList;
Camera g_camera;

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

Vec3f CalcLighting(HitObject* pLightObject, HitRecord hitRecord, float distanceToLight)
{
	Vec3f lightColour = Vec3f(0.0f, 0.0f, 0.0f);
	Vec3f lightDir = (pLightObject->m_position - hitRecord.m_intersectPoint).normalize();
	Vec3f attenuatedLightColour = LERP(pLightObject->m_pMaterial->m_diffuseColour, Vec3f(0.0f, 0.0f, 0.0f), distanceToLight / static_cast<LightSphere*>(pLightObject)->m_lightRadius);

	// Diffuse light
	{
		float diffuseLightIntensity = hitRecord.m_normal.dot(lightDir);
		diffuseLightIntensity = std::max(0.0f, diffuseLightIntensity);
		lightColour += attenuatedLightColour * diffuseLightIntensity * static_cast<LightSphere*>(pLightObject)->m_lightIntensity;
	}

	// Specular light
	{
		Vec3f v = (g_camera.GetPosition() - hitRecord.m_intersectPoint).normalize();
		Vec3f h = (lightDir + v).normalize();
		float specularLightIntensity = powf(hitRecord.m_normal.dot(h), hitRecord.m_pMaterial->m_shininess);
		specularLightIntensity = std::max(0.0f, specularLightIntensity);
		lightColour += attenuatedLightColour * specularLightIntensity * static_cast<LightSphere*>(pLightObject)->m_lightIntensity;
	}

	return lightColour;
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
			if (hitRecord.m_pMaterial->m_materialType == MaterialType::enEmmisive)
			{
				lightColour = hitRecord.m_pMaterial->m_diffuseColour;
				return lightColour + hitRecord.m_pMaterial->m_diffuseColour;
			}
			else
			{
				float shadowMultiply = 1.0f;
				for (HitObject* pLightObject : g_lightObjectsList)
				{
					float distanceToLight = (hitRecord.m_intersectPoint - pLightObject->m_position).magnitude();
					if (distanceToLight <= static_cast<LightSphere*>(pLightObject)->m_lightRadius)
					{
						Ray shadowRay = Ray(hitRecord.m_intersectPoint, pLightObject->m_position - hitRecord.m_intersectPoint);
						HitRecord shadowHitRecord;
						if (HasHit(shadowRay, 0.001f, distanceToLight, shadowHitRecord))
						{
							float shadowDistanceToLight = (shadowHitRecord.m_intersectPoint - pLightObject->m_position).magnitude();
							if (shadowDistanceToLight <= static_cast<LightSphere*>(pLightObject)->m_radius)
							{
								lightColour += CalcLighting(pLightObject, hitRecord, distanceToLight);
							}
							else
							{
								float softShadowMultiply = 0.0f;
								const int numOfSoftShadowSamples = 100;
								int numOfSamplesToAverage = 0;
								for (int s = 0; s < numOfSoftShadowSamples; s++)
								{
									Vec3f randomLightPos = GetRandomUnitVecInSphere() * static_cast<LightSphere*>(pLightObject)->m_radius + pLightObject->m_position;
									Ray softShadowRay = Ray(hitRecord.m_intersectPoint, randomLightPos - hitRecord.m_intersectPoint);
									float softShadowRayMaxHitDistance = (randomLightPos - hitRecord.m_intersectPoint).magnitude();
									HitRecord softShadowHitRecord;
									if (HasHit(softShadowRay, 0.001f, softShadowRayMaxHitDistance, softShadowHitRecord))
									{
										float shadowDistanceToLight = (softShadowHitRecord.m_intersectPoint - randomLightPos).magnitude();
										if (shadowDistanceToLight <= static_cast<LightSphere*>(pLightObject)->m_radius)
										{
											lightColour += CalcLighting(pLightObject, hitRecord, distanceToLight);
											softShadowMultiply += 1.0f;
											numOfSamplesToAverage++;
										}
										else
										{
											softShadowMultiply += 0.4f;
											numOfSamplesToAverage++;
										}
									}
								}

								lightColour.r /= numOfSamplesToAverage;
								lightColour.g /= numOfSamplesToAverage;
								lightColour.b /= numOfSamplesToAverage;

								softShadowMultiply /= numOfSamplesToAverage;

								shadowMultiply *= softShadowMultiply;
							}
						}
					}
				}
				return (lightColour + hitRecord.m_pMaterial->m_diffuseColour * GetRaytracedColor(scattered, depth + 1)) * shadowMultiply;
			}
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

void CreateImageSection(int bottomLeftPixelX, int bottomLeftPixelY, int sectionWidth, int sectionHeight, int finalWidth, int finalHeight, int sectionNumber)
{
	const int antialisingSamples = 100;

	for (int i = sectionHeight + bottomLeftPixelY; bottomLeftPixelY <= i; i--)
	{
		for (int j = bottomLeftPixelX; j < sectionWidth + bottomLeftPixelX; j++)
		{
			Vec3f col(0.0f, 0.0f, 0.0f);
			for (int k = 0; k < antialisingSamples; k++)
			{
				const float randomU = GetRandomNum();
				const float randomV = GetRandomNum();
				float u = static_cast<float>(j + randomU) / static_cast<float>(finalWidth + randomU);
				float v = static_cast<float>(i + randomV) / static_cast<float>(finalHeight + randomV);

				Ray r = g_camera.CastRay(u, v);
				col += GetRaytracedColor(r, 0);
			}

			col.r /= static_cast<float>(antialisingSamples);
			col.g /= static_cast<float>(antialisingSamples);
			col.b /= static_cast<float>(antialisingSamples);

			col = ACESFilmToneMapper(col);
			int ir = static_cast<int>(255.99 * col.r);
			int ig = static_cast<int>(255.99 * col.g);
			int ib = static_cast<int>(255.99 * col.b);
			g_outputPixels[sectionNumber].push_back(Vec3f(ir, ig, ib));
		}
	}
}

void CreateFinalImage(int sectionWidth, int sectionHeight, int finalWidth, int finalHeight)
{
	std::ofstream myfile;
	myfile.open("RaytracedOutput.ppm");
	myfile.clear();

	myfile << "P3\n" << finalWidth << " " << finalHeight << "\n255\n";

	int sectionNumber = 0;
	int sectionPixelNumber = 0;
	int pixelX = 0;
	int pixelY = sectionHeight;
	int sectionRow = 0;
	while (true)
	{
		if (pixelX == sectionWidth || (pixelX % sectionWidth == 0 && pixelX >= sectionWidth))
		{
			sectionNumber++;
			sectionPixelNumber = sectionWidth * (sectionHeight - pixelY);
		}

		if (pixelX >= finalWidth)
		{
			pixelX = 0;
			pixelY--;

			if (pixelY < 0)
			{
				pixelY = sectionHeight;
				sectionRow++;
			}

			sectionPixelNumber = sectionWidth * (sectionHeight - pixelY);

			if (sectionRow == 1)
			{
				sectionNumber = 4;
			}
			else if (sectionRow > 1)
			{
				// Done!
				break;
			}
			else
			{
				sectionNumber = 0;
			}
		}

		if (sectionNumber < 8)
		{
			int r = static_cast<int>(g_outputPixels[sectionNumber][sectionPixelNumber].r);
			int g = static_cast<int>(g_outputPixels[sectionNumber][sectionPixelNumber].g);
			int b = static_cast<int>(g_outputPixels[sectionNumber][sectionPixelNumber].b);
			myfile << r << " " << g << " " << b << "\n";
		}
		else
		{
			// Done!
			break;
		}

		pixelX++;
		sectionPixelNumber++;
	}
	myfile.close();
}

void MakeScene()
{
	g_hitObjectsList.push_back(new Sphere(Vec3f(0.0f, -1000.0f, 0.0f), 1000.0f, new LambertianDiffuse(Vec3f(0.5f, 0.5f, 0.5f))));
	g_hitObjectsList.push_back(new Sphere(Vec3f(-4.0f, 1.0f, 0.0f), 1.0f, new Metal(Vec3f(0.7f, 0.6f, 0.5f), 0.0f)));
	g_hitObjectsList.push_back(new Sphere(Vec3f(4.0f, 1.0f, 0.0f), 1.0f, new Metal(Vec3f(0.7f, 0.6f, 0.5f), 0.0f)));

	LightSphere* pLightObject0 = new LightSphere(Vec3f(0.0f, 1.65f, 0.0f), 0.5f, 30.0f, 0.2f, new Emmisive(Vec3f(0.969f, 0.906f, 0.039f)));
	g_hitObjectsList.push_back(pLightObject0);
	g_lightObjectsList.push_back(pLightObject0);

	for (int a = -11; a < 8; a++)
	{
		for (int b = -8; b < 8; b++)
		{
			float chooseMaterial = GetRandomNum();
			Vec3f center(a + 0.9f * GetRandomNum(), 0.2f, b + 0.9f * GetRandomNum());
			if (chooseMaterial <= 0.75f)
			{
				g_hitObjectsList.push_back(new Sphere(center, 0.2f, new LambertianDiffuse(Vec3f(GetRandomNum() * GetRandomNum(), GetRandomNum() * GetRandomNum(), GetRandomNum() * GetRandomNum()))));
			}
			else
			{
				g_hitObjectsList.push_back(new Sphere(center, 0.2f, new Metal(Vec3f(0.5f * (1.0f + GetRandomNum()), 0.5f * (1.0f + GetRandomNum()), 0.5f * (1.0f + GetRandomNum())), 0.5f * GetRandomNum())));
			}
		}
	}
}

int main()
{
	const int outputImageWidth = 200;
	const int outputImageHeight = 100;

	MakeScene();

	Vec3f lookfrom(13.0f, 2.0f, 3.0f);
	Vec3f lookat(0.0f, 0.0f, 0.0f);
	float distanceToFocus = 10.0f;
	float aperture = 0.1f;
	g_camera.Setup(lookfrom, lookat, Vec3f(0.0f, 1.0f, 0.0f), 20.0f, static_cast<float>(outputImageWidth) / static_cast<float>(outputImageHeight), aperture, distanceToFocus);

	srand(time(0));

	int imageSectionWidth = outputImageWidth / 4;
	int imageSectionHeight = outputImageHeight / 2;

	std::vector<std::thread*> threads;

	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			int bottomLeftPixelX = imageSectionWidth * x;
			int bottomLeftPixelY = imageSectionHeight - (imageSectionHeight * y);
			std::thread* pThread = new std::thread(CreateImageSection, bottomLeftPixelX, bottomLeftPixelY, imageSectionWidth, imageSectionHeight, outputImageWidth, outputImageHeight, x + (4 * y));
			threads.push_back(pThread);
		}
	}

	for (std::thread* pThread : threads)
	{
		pThread->join();
	}

	for (std::thread* pThread : threads)
	{
		delete pThread;
	}

	for (HitObject* pHitObject : g_hitObjectsList)
	{
		delete pHitObject;
	}

	CreateFinalImage(imageSectionWidth, imageSectionHeight, outputImageWidth, outputImageHeight);

	return 0;
}

