#pragma once

#include "raytracer.h"

#undef min
#undef max

enum class PrimitiveType
{
  kTriangle,
  kSphere,
  kPlane,
  kAABB
};

struct Primitive
{
  Primitive(PrimitiveType type, int material) :
    type(type),
    mat(material)
  {

  }

  PrimitiveType type;
  int mat;
};

struct Triangle : public Primitive
{
  Triangle(int material, const Tmpl8::vec3& point0, const Tmpl8::vec3& point1, const Tmpl8::vec3& point2) :
    Primitive(PrimitiveType::kTriangle, material),
    p0(point0),
    p1(point1),
    p2(point2)
  {
    center = Tmpl8::vec3(
      (p0.x + p1.x + p2.x) / 3.0f,
      (p0.y + p1.y + p2.y) / 3.0f,
      (p0.z + p1.z + p2.z) / 3.0f
    );

    normal = normalize(cross(p1 - p0, p2 - p0));
  }

  const Tmpl8::vec3& NormalAt(const vec3& point)
  {
    return normal;
  }

  Tmpl8::vec3 center;
  Tmpl8::vec3 p0; // Point 0 of the triangle
  Tmpl8::vec3 p1; // Point 1 of the triangle
  Tmpl8::vec3 p2; // Point 2 of the triangle
  Tmpl8::vec3 normal; // Normal of the triangle
};

struct Sphere : public Primitive
{
public:
  Sphere(int material, const Tmpl8::vec3& pos, float radius) :
    Primitive(PrimitiveType::kSphere, material),
    center(pos),
    radius(radius),
    radius2(radius * radius)
  {

  }

  vec3 NormalAt(const vec3& point)
  {
    return normalize(point - center);
  }

  Tmpl8::vec3 center; // Centroid of the sphere
  float radius; // Radius of the sphere
  float radius2; // Radius of the sphere squared
};

struct Plane : public Primitive
{
  Plane(int material, const Tmpl8::vec3& origin, const Tmpl8::vec3& normal) :
    Primitive(PrimitiveType::kPlane, material),
    center(origin),
    normal(normalize(normal))
  {

  }

  const vec3& NormalAt(const Tmpl8::vec3& point)
  {
    return normal;
  }

  Tmpl8::vec3 center; // center / origin of the plane
  Tmpl8::vec3 normal; // normal of the plane
};

struct AABB : public Primitive
{
  AABB(int material, const Tmpl8::vec3& min, const Tmpl8::vec3& max) :
    Primitive(PrimitiveType::kAABB, material),
    min(min),
    max(max)
  {
    center = (min + max) * 0.5f;
  }

  Tmpl8::vec3 center; // center of the AABB
  Tmpl8::vec3 min; // min point of the AABB
  Tmpl8::vec3 max; // max point of the AABB
};