#pragma once

struct Sphere;
struct Plane;
struct Triangle;
struct AABB;

#define EPSILON (0.0001f)

enum class PrimitiveType
{
  kAABB,
  kPlane,
  kSphere,
  kTriangle
};

class Ray
{
public:
  Ray(const XMVECTOR& origin, const XMVECTOR& direction, float distance);
  Ray(const XMFLOAT4& origin, const XMFLOAT4& direction, float distance);

  void Intersect(Sphere& sphere);
  void Intersect(Plane& plane);
  void Intersect(Triangle& triangle);
  void Intersect(AABB& aabb);
  
  union
  {
    XMVECTOR O;
    XMFLOAT4 Of;
  };

  union
  {
    XMVECTOR D;
    XMFLOAT4 Df;
  };

  float t;
  void* primitive;
  int primitive_type;
};