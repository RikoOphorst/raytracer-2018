#pragma once

struct Sphere;
struct Plane;
struct Triangle;
struct AABB;
struct Primitive;

#define EPSILON (0.0001f)

class Ray
{
public:
  Ray(const Tmpl8::vec3& origin, const Tmpl8::vec3& direction, float distance);

  void Intersect(Tmpl8::Mesh* mesh, int triIdx);
  void Intersect(Sphere* sphere);
  void Intersect(Plane* plane);
  void Intersect(Triangle* triangle);
  void Intersect(AABB* aabb);
  
  Tmpl8::vec3 O, D;
  float t;
  Primitive* primitive;
};