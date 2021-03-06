#pragma once
#include <iostream>
#include <algorithm>

const float G = -9.81f; //gravity
const float PI = 3.141592654f;
const float DegreesToRadians = PI / 180.0f;
const float RadiansToDegrees = 180.0f / PI;

//3D vectors
class Vec3f
{
public:
	Vec3f();
	Vec3f(float x, float y, float z);

	Vec3f operator+ (const Vec3f &other);
	Vec3f operator- (const Vec3f &other);
	Vec3f operator* (const Vec3f &other);
	Vec3f operator* (float scale);
	/*Vec3f operator/ (Vec3f &other);*/

	Vec3f operator+= (const Vec3f &other);
	//Vec3f operator-= (Vec3f &other);
	Vec3f operator*= (const Vec3f &other);
	Vec3f operator*= (float scale);
	bool operator == (const Vec3f &other);
	bool operator != (const Vec3f &other);
	bool operator <= (const Vec3f &other);

	float magnitude();
	Vec3f normalize();
	Vec3f square();
	float dot(const Vec3f &other);	//dot product
	Vec3f cross(const Vec3f &other);	//cross product
	Vec3f dirVec(const Vec3f &other);		//direction of a vector between two points

	Vec3f negate(); //finds the inverse of the vector

	union
	{
		struct { float x, y, z; };
		struct { float r, g, b; };
		float v[3];
	};

//private:
//	float v[3];
};

std::ostream &operator<<(std::ostream &os, const Vec3f &v);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//4D vectors
class Vec4f
{
public:
	Vec4f();
	Vec4f(float x, float y, float z, float a);
	Vec4f(Vec3f vec, float a);

	Vec4f operator+ (const Vec4f &other);
	Vec4f operator- (const Vec4f &other);
	Vec4f operator* (const Vec4f &other);
	Vec4f operator* (float scale);

	Vec4f operator+= (const Vec4f &other);
	//Vec3f operator-= (Vec3f &other);
	Vec4f operator*= (const Vec4f &other);
	Vec4f operator*= (float scale);
	bool operator == (const Vec4f &other);
	bool operator != (const Vec4f &other);

	float magnitude();
	Vec4f normalize();
	Vec4f square();
	float dot(const Vec4f &other);	//dot product
	Vec4f cross(const Vec4f &other);	//cross product
	Vec4f dirVec(const Vec4f &other);		//direction of a vector between two points

	Vec4f negate(); //finds the inverse of the vector

	Vec3f returnAsVec3f(); 

	union
	{
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
		float v[4];
	};

//private:
//	float v[4];
};

std::ostream &operator<<(std::ostream &os, const Vec4f &v);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//3D matrices
class Mat3f
{
public:
	Mat3f();
	Mat3f(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);
	Mat3f(Vec3f Vrow1, Vec3f Vrow2, Vec3f Vrow3);
	//~Mat3f();

	Mat3f operator+ (const Mat3f &other);
	Mat3f operator* (const Mat3f &other);
	Mat3f operator* (float scale);
	Vec3f operator* (const Vec3f &other);
	
	//Mat3f operator+= (Mat3f &other);
	//Mat3f operator*= (Mat3f &other);
	//Mat3f operator*= (float scale);
	//Vec3f operator*= (Vec3f &other);

	Mat3f square();
	Mat3f transpose(); 

	//union; this means that all of these variables are accessing the same memory location and i can determine how i want my data to be represented
	union
	{
		float m[9];
		struct
		{
			Vec3f row1;
			Vec3f row2;
			Vec3f row3;
		};
	};
};

//ostream& operator<<(ostream& os, const Mat3f& Mat3f);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//4D matrices
class Mat4f
{
public:
	Mat4f();
	Mat4f(float matrix[16]);
	Mat4f(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);
	Mat4f(Vec4f Vrow1, Vec4f Vrow2, Vec4f Vrow3, Vec4f Vrow4);
	//~Mat3f();

	Mat4f operator+ (const Mat4f &other);
	Mat4f operator* (const Mat4f &other);
	Mat4f operator* (const Mat4f &other) const;
	Mat4f operator* (const float scale);
	Vec4f operator* (const Vec4f &other);
	
	//Mat4f operator+= (Mat4f &other);
	//Mat4f operator*= (Mat4f &other);
	//Mat4f operator*= (float scale);
	//Vec3f operator*= (Vec3f &other);

	Mat4f square();
	Mat4f transpose(); 

	//union; this means that all of these variables are accessing the same memory location and i can determine how i want my data to be represented
	union
	{
		float m[16];
		float a[4][4];
		struct
		{
			Vec4f row1;
			Vec4f row2;
			Vec4f row3;
			Vec4f row4;
		};
	};
};

//ostream& operator<<(ostream& os, const Mat4f& Mat4f);

//------------------------------------------------------------------------------------------------------------------------------------------------>

//creates a 4D translation matrix
inline Mat4f Translate(float x, float y, float z)
{
	return Mat4f(Vec4f(1.0f, 0.0f, 0.0f, x), 
				 Vec4f(0.0f, 1.0f, 0.0f, y), 
				 Vec4f(0.0f, 0.0f, 1.0f, z), 
				 Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
}

inline Mat4f Translate(const Vec3f &v)
{
	return Translate(v.x, v.y, v.z);
}

inline Mat4f Translate(const Vec4f &v)
{
	return Translate(v.x, v.y, v.z);
}

//------------------------------------------------------------------------------------------------------------------------------------------------>

//creates a 4D scaling matrix
inline Mat4f Scale(float x, float y, float z)
{
	return Mat4f(Vec4f(x, 0.0f, 0.0f, 0.0f), 
				 Vec4f(0.0f, y, 0.0f, 0.0f), 
				 Vec4f(0.0f, 0.0f, z, 0.0f), 
				 Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
}

inline Mat4f Scale(const Vec3f &v)
{
	return Scale(v.x, v.y, v.z);
}

inline Mat4f Scale(const Vec4f &v)
{
	return Scale(v.x, v.y, v.z);
}
//------------------------------------------------------------------------------------------------------------------------------------------------>
//return an identiy matrix
inline Mat4f Identity()
{
	return Mat4f(1.0f, 0.0f, 0.0f, 0.0f,
				 0.0f, 1.0f, 0.0f, 0.0f,
				 0.0f, 0.0f, 1.0f, 0.0f,
				 0.0f, 0.0f, .0f, 1.0f); 
}

//------------------------------------------------------------------------------------------------------------------------------------------------>
////creates a 4D rotation matrix on the x-axis
inline Mat4f RotateX(const float angle)
{
	float radians = DegreesToRadians * angle;
	return Mat4f(Vec4f(1.0f, 0.0f, 0.0f, 0.0f), 
				 Vec4f(0.0f, cosf(radians),	-sinf(radians), 0.0f), 
				 Vec4f(0.0f, sinf(radians), cosf(radians), 0.0f), 
				 Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
}

////creates a 4D rotation matrix on the y-axis
inline Mat4f RotateY(const float angle)
{
	float radians = DegreesToRadians * angle;
	return Mat4f(Vec4f(cosf(radians), 0.0f, sinf(radians), 0.0f), 
		         Vec4f(0.0f, 1.0f, 0.0f, 0.0f), 
				 Vec4f(-sinf(radians), 0.0f, cosf(radians), 0.0f), 
				 Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
}

////creates a 4D rotation matrix on the z-axis
inline Mat4f RotateZ(const float angle)
{
	float radians = DegreesToRadians * angle;
	return Mat4f(Vec4f(cosf(radians), -sinf(radians), 0.0f, 0.0f), 
				 Vec4f(sinf(radians), cos(radians), 0.0f, 0.0f), 
				 Vec4f(0.0f, 0.0f, 1.0f, 0.0f), 
				 Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
}

inline Mat4f Rotate(const float angleX, const float angleY, const float angleZ) {
	return RotateZ(angleZ) * RotateY(angleY) * RotateX(angleX);
}

//------------------------------------------------------------------------------------------------------------------------------------------------>
// homogeneous transform
inline Mat4f makeTransform(const Mat3f &r, const Vec3f &t)
{
	return Mat4f(Vec4f(r.row1.x, r.row1.y, r.row1.z, t.x),
				 Vec4f(r.row2.x, r.row2.y, r.row2.z, t.y),
				 Vec4f(r.row3.x, r.row3.y, r.row3.z, t.z),
				 Vec4f(0.0f,	  0.0f,		 0.0f,		1.0f));
}

inline Mat4f makeTransform(const Mat4f &r, const Vec3f &t)
{
	return Mat4f(Vec4f(r.row1.x, r.row1.y, r.row1.z, t.x),
				 Vec4f(r.row2.x, r.row2.y, r.row2.z, t.y),
				 Vec4f(r.row3.x, r.row3.y, r.row3.z, t.z),
				 Vec4f(0.0f,	  0.0f,		 0.0f,		1.0f));
}

// homogeneous transform transform with scale
inline Mat4f makeTransform(const Mat4f &r, const Mat4f &s, const Vec3f &t)
{
	return makeTransform( r * s, t );
}

//creates a inverse matrix of a transformation matrix that does not have a scale factor applied to it
inline Mat4f transformInverseNoScale(const Mat4f &m)
{
	//extract rotation component of matrix and transpose it
	Mat3f r = Mat3f(Vec3f(m.row1.x, m.row1.y, m.row1.z), 
					Vec3f(m.row2.x, m.row2.y, m.row2.z), 
					Vec3f(m.row3.x, m.row3.y, m.row3.z)).transpose();
	
	//extract translation component of matrix and negate it
	Vec3f t = Vec3f(m.row1.a, m.row2.a, m.row3.a);

	return makeTransform(r, (r * t) * -1.0f ); 
}

//------------------------------------------------------------------------------------------------------------------------------------------------>

//creates a 4D perspective matrix
/*
fovy: is the field of view angle on the yz-plane ranging from 0 to 180 degrees
aspect: is the aspect ratio (window width/ window height)
zNear and zFar: is the distance from the near clipping plane to the far clipping plane
which starts from the viewport and travels down the z-axis. 
*/
inline Mat4f Perspective(float fovyDegrees, float aspectRatio, float zNear, float zFar)
{
	/*float top = tanf((fovyDegrees * DegreesToRadians) * 0.5f) * zNear;
	float right = top * aspectRatio;

	return Mat4f(Vec4f(zNear/right, 0.0f, 0.0f, 0.0f), 
				Vec4f(0.0f, zNear/top, 0.0f, 0.0f), 
				Vec4f(0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), (-2.0f * zFar * zNear) / (zFar - zNear)), 
				Vec4f(0.0f, 0.0f, -1.0f, 0.0f));*/

	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(DegreesToRadians * (fovyDegrees / 2.0f));

	Mat4f m;

	m.a[0][0] = 1.0f / (tanHalfFOV * aspectRatio);
	m.a[0][1] = 0.0f;
	m.a[0][2] = 0.0f;
	m.a[0][3] = 0.0f;
	
	m.a[1][0] = 0.0f;
	m.a[1][1] = 1.0f / tanHalfFOV;
	m.a[1][2] = 0.0f;
	m.a[1][3] = 0.0f;
	  
	m.a[2][0] = 0.0f;
	m.a[2][1] = 0.0f;
	m.a[2][2] = (-zNear - zFar) / zRange;
	m.a[2][3] = 2.0f * zFar * zNear / zRange;
	  
	m.a[3][0] = 0.0f;
	m.a[3][1] = 0.0f;
	m.a[3][2] = 1.0f;
	m.a[3][3] = 0.0f;

	return m; 
}

inline Mat4f Orthographic(float width, float height, float zNear, float zFar)
{
	return Mat4f(Vec4f(2.0f * zNear / (width), 0.0f, 0.0f, 0.0f),
				 Vec4f(0.0f, 2.0f * zNear / (height), 0.0f, 0.0f),
				 Vec4f(0.0f, 0.0f, -2.0f / (zFar - zNear), -(zFar + zNear) / (zFar - zNear)),
				 Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
}
/*
left and right specify the x-coordinate clipping planes, bottom and top specify the y-coordinate clipping planes, 
and near and far specify the distance to the z-coordinate clipping planes. 
Together these coordinates provide a box shaped viewing volume. 
*/

inline Mat4f Orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	/*
	return Mat4f(Vec4f(2.0f * zNear / (right - left), 0.0f, 0.0f, -(right + left) / (right - left)),
				 Vec4f(0.0f, 2.0f * zNear / (top - bottom), 0.0f, -(top + bottom) / (top - bottom)),
				 Vec4f(0.0f, 0.0f, -2.0f / (zFar - zNear), -(zFar + zNear) / (zFar - zNear)),
				 Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
				 */
	return Mat4f(Vec4f(2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left)),
				 Vec4f(0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom)),
				 Vec4f(0.0f, 0.0f, -2.0f / (zFar - zNear), -(zFar + zNear) / (zFar - zNear)),
				 Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
}

// compute a transform (obj-to-world) using a 'look-at' utility
// this can be used for anything, not just a camera!
inline Mat4f LookAt(Vec3f eye, Vec3f at, Vec3f yAxis)
{
	//	
	//	YOU SHOULD REMEMBER THIS FROM LAST TERM: 
	//	"FRENET FRAME"
	//
	//				c1 = j = Up
	//						o
	//						|	
	//						|  . (Dir)
	//						| .
	//						|.
	//	c3 = E = position	X----------o  c0 = i = Right
	//					   /
	//					  /
	//					 o
	//	c2 = k = -(Direction)
	//	

	//frenet frame
	/*
	Vec3f dir = (eye - at).normalize();		
	Vec3f right = (yAxis.cross(dir)).normalize();
	Vec3f up = (dir.cross(right)).normalize();

	return Mat4f(right.x, up.x, dir.x, eye.x,
				 right.y, up.y, dir.y, eye.y,
				 right.z, up.z, dir.z, eye.z,
				 0.0f,	   0.0f,  0.0f,	  1.0f);
				 */
	Vec3f dir = (at - eye).normalize();		
	Vec3f right = (dir.cross(yAxis)).normalize();
	Vec3f up = (right.cross(dir)).normalize();

	/*
	return Translate(eye * -1) * Mat4f(right.x, up.x, -dir.x, -right.dot(eye),
								       right.y, up.y, -dir.y, -up.dot(eye),
				                       right.z, up.z, -dir.z, dir.dot(eye),
									   0.0f,	0.0f, 0.0f,  1.0f);
									   */
	Mat3f r = Mat3f(right.x, right.y, right.z, 
					up.x,    up.y,    up.z,   
					-dir.x, -dir.y,   -dir.z);

	Vec3f t = (r * eye) * -1.0f; 

	return Mat4f(right.x, right.y, right.z, t.x,
				 up.x,    up.y,    up.z,    t.y,
				 -dir.x, -dir.y,   -dir.z,  t.z,
				 0.0f,	0.0f,    0.0f,      1.0f);
}

// compute a transform (obj-to-world) using a 'look-at' utility
// this can be used for anything, not just a camera!
inline Mat4f ObjectLookAt(Vec3f eye, Vec3f at, Vec3f yAxis)
{
	//	
	//	YOU SHOULD REMEMBER THIS FROM LAST TERM: 
	//	"FRENET FRAME"
	//
	//				c1 = j = Up
	//						o
	//						|	
	//						|  . (Dir)
	//						| .
	//						|.
	//	c3 = E = position	X----------o  c0 = i = Right
	//					   /
	//					  /
	//					 o
	//	c2 = k = -(Direction)
	//	

	//frenet frame
	Vec3f dir = (eye - at).normalize();		
	Vec3f right = (yAxis.cross(dir)).normalize();
	Vec3f up = (dir.cross(right)).normalize();

	return Mat4f(right.x, up.x, dir.x, eye.x,
				 right.y, up.y, dir.y, eye.y,
				 right.z, up.z, dir.z, eye.z,
				 0.0f,	   0.0f,  0.0f,	  1.0f);
				
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//for animation
//LERP
template <typename lerpClass>
inline lerpClass LERP(lerpClass v0, lerpClass v1, float t)
{
	/*if (v0 == v1)
	{
		return v0;
	}*/

	return (v0*(1.0f-t)) + (v1*t);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Returns a random number between 0 - 1
static const float GetRandomNum()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

static float Clamp(float n, float lower, float upper)
{
	return std::max(lower, std::min(n, upper));
}

static Vec3f GetRandomUnitVecInSphere()
{
	Vec3f p;
	do
	{
		p = Vec3f(GetRandomNum(), GetRandomNum(), GetRandomNum()) * 2.0f - Vec3f(1, 1, 1);
	} while (p.x * p.x + p.y * p.y + p.z * p.z >= 1.0f);
	return p;
}

static Vec3f Reflect(Vec3f v, Vec3f n)
{
	return v - (n * v.dot(n) * 2);
}

static Vec3f ACESFilmToneMapper(Vec3f col)
{
	float A = 2.51f;
	float B = 0.03f;
	float C = 2.43f;
	float D = 0.59f;
	float E = 0.14f;

	float r = (col.r * (A * col.r + B)) / (col.r * (C * col.r + D) + E);
	float g = (col.g * (A * col.g + B)) / (col.g * (C * col.g + D) + E);
	float b = (col.b * (A * col.b + B)) / (col.b * (C * col.b + D) + E);

	return Vec3f(Clamp(r, 0.0f, 1.0f), Clamp(g, 0.0f, 1.0f), Clamp(b, 0.0f, 1.0f));
}

