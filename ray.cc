#include "precomp.h"
#include "ray.h"

#include "primitive.h"

//------------------------------------------------------------------------------------------------------
Ray::Ray(const Tmpl8::vec3& origin, const Tmpl8::vec3& direction, float distance) :
  O(origin),
  D(direction),
  t(distance),
  primitive(nullptr)
{

}

//------------------------------------------------------------------------------------------------------
void Ray::Intersect(Tmpl8::Mesh* mesh, int triIdx)
{

}

//------------------------------------------------------------------------------------------------------
void Ray::Intersect(Sphere* sphere)
{
  Tmpl8::vec3 c = sphere->center - O;
  float dist = dot(c, D);
  Tmpl8::vec3 q = c - dist * D;
  float p2 = dot(q, q);

  if (p2 > sphere->radius2)
  {
    return;
  }

  dist -= sqrt(sphere->radius2 - p2);

  if (dist < t && dist > 0)
  {
    primitive = sphere;
    t = dist;
    return;
  }
}

//------------------------------------------------------------------------------------------------------
void Ray::Intersect(Plane* plane)
{
  float denom = dot(plane->normal, D);
  if (abs(denom) > EPSILON)
  {
    float dist = dot((plane->center - O), plane->normal) / denom;
    if (dist < t && dist >= EPSILON)
    {
      primitive = plane;
      t = dist;
    }
  }
}

//------------------------------------------------------------------------------------------------------
void Ray::Intersect(Triangle* triangle)
{
  Tmpl8::vec3 edge1, edge2, h, s, q;
  float a, f, u, v;
  edge1 = triangle->p1 - triangle->p0;
  edge2 = triangle->p2 - triangle->p0;
  h = cross(D, edge2);
  a = dot(edge1, h);

  if (a > -EPSILON && a < EPSILON)
  {
    return;
  }

  f = 1 / a;
  s = O - triangle->p0;
  u = f * (dot(s, h));
  if (u < 0.0f || u > 1.0f)
  {
    return;
  }

  q = cross(s, edge1);
  v = f * dot(D, q);
  if (v < 0.0f || u + v > 1.0f)
  {
    return;
  }

  // At this stage we can compute t to find out where the intersection point is on the line.
  float dist = f * dot(edge2, q);
  if (dist > EPSILON && dist < t) // ray intersection
  {
    primitive = triangle;
    t = dist;
  }
}

//------------------------------------------------------------------------------------------------------
void Ray::Intersect(AABB* aabb)
{

}
