#pragma once

#include "raytracer.h"

#undef min
#undef max

struct Triangle
{
  Triangle() : material(0), p0f(0.0f, 0.0f, 0.0f, 1.0f), p1f(0.0f, 1.0f, 0.0f, 1.0f), p2f(1.0f, 0.0f, 0.0f, 1.0f), nf(0.0f, 0.0f, 0.0f, 0.0f) {}

  Triangle(int material, const XMVECTOR& point0, const XMVECTOR& point1, const XMVECTOR& point2, const XMVECTOR& normal) :
    material(material),
    p0v(point0),
    p1v(point1),
    p2v(point2),
    nv(normal)
  {
    
  }

  Triangle(int material, const XMFLOAT4& point0, const XMFLOAT4& point1, const XMFLOAT4& point2, const XMFLOAT4& normal) :
    material(material),
    p0f(point0),
    p1f(point1),
    p2f(point2),
    nf(normal)
  {

  }

  union
  {
    XMVECTOR p0v;
    XMFLOAT4 p0f;
  };

  union
  {
    XMVECTOR p1v;
    XMFLOAT4 p1f;
  };

  union
  {
    XMVECTOR p2v;
    XMFLOAT4 p2f;
  };

  union
  {
    XMVECTOR nv;
    XMFLOAT4 nf;
  };

  int material;
};

struct Sphere
{
public:
  Sphere() : material(0), sphere(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f) {}

  Sphere(int material, const XMFLOAT3& pos, float radius) :
    material(material),
    sphere(pos, radius)
  {
    
  }

  XMVECTOR NormalAt(const XMVECTOR& point)
  {
    return XMVector3Normalize(point - XMVectorSet(sphere.Center.x, sphere.Center.y, sphere.Center.z, 0.0f));
  }

  BoundingSphere sphere;
  int material;
};

struct Plane
{
  Plane() : material(0), pf(0.0f, 1.0f, 0.0f, 0.0f) {}

  Plane(int material, const XMVECTOR& plane) :
    material(material),
    p(plane)
  {

  }

  Plane(int material, const XMFLOAT4& plane) :
    material(material),
    pf(plane)
  {

  }

  union
  {
    XMVECTOR p; // Plane vector. XYZ = direction. W = offset along that direction. Origin of plane is therefore Ox = X*W | Oy = Y*W | Oz = Z*W
    XMFLOAT4 pf; // Float representation of plane
  };

  int material;
};

struct AABB
{
  AABB() : material(0), box(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)) {}

  AABB(int material, const XMVECTOR& center, const XMVECTOR& extents) :
    material(material)
  {
    XMStoreFloat3(&box.Center, center);
    XMStoreFloat3(&box.Extents, extents);
  }

  AABB(int material, const XMFLOAT3& center, const XMFLOAT3& extents) :
    material(material),
    box(center, extents)
  {
    
  }

  int material;
  BoundingBox box;
};