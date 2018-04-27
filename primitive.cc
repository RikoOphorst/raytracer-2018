#include "precomp.h"

#include "primitive.h"

#include <cmath>

//------------------------------------------------------------------------------------------------------
Triangle::Triangle() :
  p0(0.0f, 0.0f, 0.0f),
  p1(1.0f, 0.0f, 0.0f),
  p2(0.0f, 1.0f, 0.0f),
  N(0.0f, 0.0f, 1.0f)
{
  Init();
}

//------------------------------------------------------------------------------------------------------
Triangle::Triangle(const Tmpl8::vec3& point0, const Tmpl8::vec3& point1, const Tmpl8::vec3& point2, const Tmpl8::vec3& normal) :
  p0(point0),
  p1(point1),
  p2(point2),
  N(normal)
{
  Init();
}

//------------------------------------------------------------------------------------------------------
void Triangle::Init()
{
  c = Tmpl8::vec3(
    (p0.x + p1.x + p2.x) / 3.0f,
    (p0.y + p1.y + p2.y) / 3.0f,
    (p0.z + p1.z + p2.z) / 3.0f
  );
}

//------------------------------------------------------------------------------------------------------
void Triangle::Init(const Tmpl8::vec3& point0, const Tmpl8::vec3& point1, const Tmpl8::vec3& point2, const Tmpl8::vec3& normal)
{
  p0 = point0;
  p1 = point1;
  p2 = point2;
  Init();
}

//------------------------------------------------------------------------------------------------------
void Triangle::Intersect(const Tmpl8::Ray& ray, bool& out_hit, float& out_t)
{
  const float EPSILON = 0.0000001f;

  Tmpl8::vec3 edge1, edge2, h, s, q;
  float a, f, u, v;
  edge1 = p1 - p0;
  edge2 = p2 - p0;
  h = cross(ray.D, edge2);
  a = dot(edge1, h);
  if (a > -EPSILON && a < EPSILON)
  {
    out_hit = false;
    return;
  }

  f = 1 / a;
  s = ray.O - p0;
  u = f * (dot(s, h));
  if (u < 0.0 || u > 1.0)
  {
    out_hit = false;
    return;
  }

  q = cross(s, edge1);
  v = f * dot(ray.D, q);
  if (v < 0.0f || u + v > 1.0f)
  {
    out_hit = false;
    return;
  }

  // At this stage we can compute t to find out where the intersection point is on the line.
  float t = f * dot(edge2, q);
  if (t > EPSILON) // ray intersection
  {
    //outIntersectionPoint = rayOrigin + rayVector * t;
    out_t = t;
    out_hit = true;
  }
  else
  {
    out_hit = false;
  }
}



//------------------------------------------------------------------------------------------------------
Sphere::Sphere()
{

}

//------------------------------------------------------------------------------------------------------
Sphere::Sphere(const Tmpl8::vec3& pos, float radius) :
  r(radius),
  r2(radius * radius)
{
  c = pos;
}

//------------------------------------------------------------------------------------------------------
void Sphere::Init()
{
  //
}

//------------------------------------------------------------------------------------------------------
void Sphere::Init(const Tmpl8::vec3& pos, float radius)
{
  c = pos;
  r = radius;
  r2 = r * r;
}

//------------------------------------------------------------------------------------------------------
void Sphere::Intersect(const Tmpl8::Ray& ray, bool& out_hit, float& out_t)
{
  Tmpl8::vec3 C = c - ray.O;
  float t = dot(C, ray.D);
  Tmpl8::vec3 q = C - t * ray.D;
  float p2 = dot(q, q);

  if (p2 > r2)
  {
    out_hit = false;
    return;
  }

  t -= sqrt(r2 - p2);

  if (t < ray.t && t > 0)
  {
    out_hit = true;
    out_t = t;
    return;
  }

  out_hit = false;
}

//------------------------------------------------------------------------------------------------------
AABB::AABB() :
  minp(0.0f, 0.0f, 0.0f),
  maxp(1.0f, 1.0f, 1.0f)
{
  Init();
}

//------------------------------------------------------------------------------------------------------
AABB::AABB(const Tmpl8::vec3& min, const Tmpl8::vec3& max) :
  minp(minp),
  maxp(maxp)
{
  Init();
}

//------------------------------------------------------------------------------------------------------
void AABB::Init()
{
  c = (minp + maxp) * 0.5f;
}

//------------------------------------------------------------------------------------------------------
void AABB::Init(const Tmpl8::vec3& mina, const Tmpl8::vec3& maxa)
{
  minp = mina;
  maxp = maxa;
  Init();
}

//------------------------------------------------------------------------------------------------------
void AABB::Intersect(const Tmpl8::Ray& ray, bool& out_hit, float& out_t)
{

}

//------------------------------------------------------------------------------------------------------
Plane::Plane() :
  N(0.0f, 1.0f, 0.0f)
{
  c = Tmpl8::vec3(0.0f, 0.0f, 0.0f);
}

//------------------------------------------------------------------------------------------------------
Plane::Plane(const Tmpl8::vec3& origin, const Tmpl8::vec3& normal) :
  N(normal)
{
  c = origin;
}

//------------------------------------------------------------------------------------------------------
void Plane::Init()
{
  
}

//------------------------------------------------------------------------------------------------------
void Plane::Init(const Tmpl8::vec3& origin, const Tmpl8::vec3& normal)
{
  c = origin;
  N = normal;
}

//------------------------------------------------------------------------------------------------------
void Plane::Intersect(const Tmpl8::Ray& ray, bool& out_hit, float& out_t)
{
  //out.normal = normal;

  float denom = dot(N, ray.D);
  if (abs(denom) > 1e-6)
  {
    vec3 d = c - ray.O;
    float t = dot(d, N) / denom;
    //out.point = ray.O + (t - 0.0001f) * ray.D;
    out_t = t;
    out_hit = (t < ray.t && t >= 0.0f);
    return;
  }

  out_hit = false;
}