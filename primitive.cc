#include "precomp.h"

#include "primitive.h"

#include <cmath>

//------------------------------------------------------------------------------------------------------
Primitive::Primitive(int material) :
  mat(material)
{

}

//------------------------------------------------------------------------------------------------------
Triangle::Triangle(int material) :
  Primitive(material),
  p0(0.0f, 0.0f, 0.0f),
  p1(1.0f, 0.0f, 0.0f),
  p2(0.0f, 1.0f, 0.0f),
  N(0.0f, 0.0f, 1.0f)
{
  Init();
}

//------------------------------------------------------------------------------------------------------
Triangle::Triangle(int material, const Tmpl8::vec3& point0, const Tmpl8::vec3& point1, const Tmpl8::vec3& point2, const Tmpl8::vec3& normal) :
  Primitive(material),
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
void Triangle::Intersect(const Tmpl8::Ray& ray, bool& out_hit, float& out_t, Tmpl8::vec3& out_I, Tmpl8::vec3& out_N)
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
  if (v < 0.0 || u + v > 1.0)
  {
    out_hit = false;
    return;
  }

  // At this stage we can compute t to find out where the intersection point is on the line.
  float t = f * dot(edge2, q);
  if (t > EPSILON) // ray intersection
  {
    out_N = N;
    out_I = ray.O + ray.D * (t - 0.001f);
    out_t = t;
    out_hit = true;
  }
  else
  {
    out_hit = false;
  }
}



//------------------------------------------------------------------------------------------------------
Sphere::Sphere(int material) :
  Primitive(material)
{

}

//------------------------------------------------------------------------------------------------------
Sphere::Sphere(int material, const Tmpl8::vec3& pos, float radius) :
  Primitive(material),
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
void Sphere::Intersect(const Tmpl8::Ray& ray, bool& out_hit, float& out_t, Tmpl8::vec3& out_I, Tmpl8::vec3& out_N)
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
    out_I = ray.O + ray.D * (out_t);
    out_N = (out_I - c);
    out_N.normalize();
    return;
  }

  out_hit = false;
}

//------------------------------------------------------------------------------------------------------
AABB::AABB(int material) :
  Primitive(material),
  minp(0.0f, 0.0f, 0.0f),
  maxp(1.0f, 1.0f, 1.0f)
{
  Init();
}

//------------------------------------------------------------------------------------------------------
AABB::AABB(int material, const Tmpl8::vec3& min, const Tmpl8::vec3& max) :
  Primitive(material),
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
void AABB::Intersect(const Tmpl8::Ray& ray, bool& out_hit, float& out_t, Tmpl8::vec3& out_I, Tmpl8::vec3& out_N)
{

}

//------------------------------------------------------------------------------------------------------
Plane::Plane(int material) :
  Primitive(material),
  N(0.0f, 1.0f, 0.0f)
{
  c = Tmpl8::vec3(0.0f, 0.0f, 0.0f);
}

//------------------------------------------------------------------------------------------------------
Plane::Plane(int material, const Tmpl8::vec3& origin, const Tmpl8::vec3& normal) :
  Primitive(material),
  N(normal)
{
  c = origin;
  Init();
}

//------------------------------------------------------------------------------------------------------
void Plane::Init()
{
  N.normalize();
}

//------------------------------------------------------------------------------------------------------
void Plane::Init(const Tmpl8::vec3& origin, const Tmpl8::vec3& normal)
{
  c = origin;
  N = normal;
  Init();
}

//------------------------------------------------------------------------------------------------------
void Plane::Intersect(const Tmpl8::Ray& ray, bool& out_hit, float& out_t, Tmpl8::vec3& out_I, Tmpl8::vec3& out_N)
{
  //float denom = dot(N, ray.D);
  //if (abs(denom) > 1e-6)
  //{
  //  vec3 d = c - ray.O;
  //  float t = dot(d, N) / denom;
  //  out_hit = (t < ray.t && t >= 0.0f);
  // 
  //  if (out_hit)
  //  {
  //    out_I = ray.O + ray.D * (t - 0.001f);
  //    out_N = N;
  //    out_t = t;
  //  }
  //  
  //  return;
  //}
  //
  //out_hit = false;

  //float denom = normal.dot(ray.direction);
  float denom = dot(N, ray.D);
  if (abs(denom) > 0.0001f) // your favorite epsilon
  {
    float t = dot((c - ray.O),N) / denom;
    if (t >= 0.0001f)
    {
      out_I = ray.O + ray.D * t;
      out_N = N;
      out_t = t;
      out_hit = true;
      return;
    }
  }

  out_hit = false;
}
