#pragma once

#include "raytracer.h"

class Primitive
{
public:
  Primitive(int material);

  virtual void Intersect(const Tmpl8::Ray&, bool& out_hit, float& out_t, Tmpl8::vec3& out_I, Tmpl8::vec3& out_N) = 0;

public:
  Tmpl8::vec3 c; // Centroid of the primitive
  int mat; // MaterialRasterizer of this primitive
};

class Triangle : public Primitive
{
public:
  Triangle(int material);
  Triangle(int material, const Tmpl8::vec3& point0, const Tmpl8::vec3& point1, const Tmpl8::vec3& point2, const Tmpl8::vec3& normal);
  
  void Init();
  void Init(const Tmpl8::vec3& point0, const Tmpl8::vec3& point1, const Tmpl8::vec3& point2, const Tmpl8::vec3& normal);

  virtual void Intersect(const Tmpl8::Ray& ray, bool& out_hit, float& out_t, Tmpl8::vec3& out_I, Tmpl8::vec3& out_N) override;
  
public:
  Tmpl8::vec3 p0; // Point 0 of the triangle
  Tmpl8::vec3 p1; // Point 1 of the triangle
  Tmpl8::vec3 p2; // Point 2 of the triangle
  Tmpl8::vec3 N; // Normal of the triangle
};

class Sphere : public Primitive
{
public:
  Sphere(int material);
  Sphere(int material, const Tmpl8::vec3& pos, float radius);

  void Init();
  void Init(const Tmpl8::vec3& pos, float radius);

  virtual void Intersect(const Tmpl8::Ray& ray, bool& out_hit, float& out_t, Tmpl8::vec3& out_I, Tmpl8::vec3& out_N) override;

public:
  float r; // Radius of the sphere
  float r2; // Radisu of the sphere squared
};

class AABB : public Primitive
{
public:
  AABB(int material);
  AABB(int material, const Tmpl8::vec3& minp, const Tmpl8::vec3& maxp);

  void Init();
  void Init(const Tmpl8::vec3& minp, const Tmpl8::vec3& maxp);

  virtual void Intersect(const Tmpl8::Ray& ray, bool& out_hit, float& out_t, Tmpl8::vec3& out_I, Tmpl8::vec3& out_N) override;

public:
  Tmpl8::vec3 minp; // min point of the AABB
  Tmpl8::vec3 maxp; // max point of the AABB
};

class Plane : public Primitive
{
public:
  Plane(int material);
  Plane(int material, const Tmpl8::vec3& origin, const Tmpl8::vec3& normal);

  void Init();
  void Init(const Tmpl8::vec3& origin, const Tmpl8::vec3& normal);

  virtual void Intersect(const Tmpl8::Ray& ray, bool& out_hit, float& out_t, Tmpl8::vec3& out_I, Tmpl8::vec3& out_N) override;

public:
  Tmpl8::vec3 N; // normal of the plane
};