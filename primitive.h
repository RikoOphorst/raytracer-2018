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
  Triangle(int material, const XMVECTOR& point0, const XMVECTOR& point1, const XMVECTOR& point2, const XMVECTOR& normal) :
    Primitive(PrimitiveType::kTriangle, material),
    p0v(point0),
    p1v(point1),
    p2v(point2),
    nv(normal)
  {

  }

  Triangle(int material, const XMFLOAT4& point0, const XMFLOAT4& point1, const XMFLOAT4& point2, const XMFLOAT4& normal) :
    Primitive(PrimitiveType::kTriangle, material),
    p0f(point0),
    p1f(point1),
    p2f(point2),
    nf(normal)
  {

  }

  union
  {
    XMVECTOR p0v; // Point 0 of the triangle
    XMFLOAT4 p0f; // Point 0 of the triangle
  };

  union
  {
    XMVECTOR p1v; // Point 1 of the triangle
    XMFLOAT4 p1f; // Point 1 of the triangle
  };

  union
  {
    XMVECTOR p2v; // Point 2 of the triangle
    XMFLOAT4 p2f; // Point 2 of the triangle
  };

  union
  {
    XMVECTOR nv; // Normal of the triangle
    XMFLOAT4 nf; // Normal of the triangle
  };
};

struct Sphere : public Primitive
{
public:
  Sphere(int material, const XMFLOAT3& pos, float radius) :
    Primitive(PrimitiveType::kSphere, material),
    sphere(pos, radius)
  {
    
  }

  XMVECTOR NormalAt(const XMVECTOR& point)
  {
    return XMVectorSetW(XMVector3Normalize(point - XMVectorSet(sphere.Center.x, sphere.Center.y, sphere.Center.z, 1.0f)), 0.0f);
  }

  BoundingSphere sphere;
};

struct Plane : public Primitive
{
  Plane(int material, const XMVECTOR& plane) :
    Primitive(PrimitiveType::kPlane, material),
    p(plane)
  {

  }

  Plane(int material, const XMFLOAT4& plane) :
    Primitive(PrimitiveType::kPlane, material),
    pf(plane)
  {

  }

  union
  {
    XMVECTOR p; // center / origin of the plane
    XMFLOAT4 pf; // center / origin of the plane
  };
};

struct AABB : public Primitive
{
  AABB(int material, const XMVECTOR& center, const XMVECTOR& extents) :
    Primitive(PrimitiveType::kAABB, material)
  {
    XMStoreFloat3(&box.Center, center);
    XMStoreFloat3(&box.Extents, extents);
  }

  AABB(int material, const XMFLOAT3& center, const XMFLOAT3& extents) :
    Primitive(PrimitiveType::kAABB, material),
    box(center, extents)
  {
    
  }

  BoundingBox box;
};