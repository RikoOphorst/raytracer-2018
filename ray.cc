#include "precomp.h"
#include "ray.h"

#include "primitive.h"

//------------------------------------------------------------------------------------------------------
Ray::Ray(const XMVECTOR& origin, const XMVECTOR& direction, float distance) :
  O(origin),
  D(direction),
  t(distance),
  primitive(nullptr)
{

}

//------------------------------------------------------------------------------------------------------
Ray::Ray(const XMFLOAT4& origin, const XMFLOAT4& direction, float distance) :
  Of(origin),
  Df(direction),
  t(distance),
  primitive(nullptr)
{

}

//------------------------------------------------------------------------------------------------------
void Ray::Intersect(Sphere& sphere)
{
  float dist = t;
  if (sphere.sphere.Intersects(O, D, dist) && dist < t)
  {
    primitive = &sphere;
    t = dist;
  }
}

//------------------------------------------------------------------------------------------------------
void Ray::Intersect(Plane& plane)
{
  XMVECTOR planeNormal = XMVectorSet(plane.pf.x, plane.pf.y, plane.pf.z, 0.0f);

  XMVECTOR denom = XMVector3Dot(planeNormal, D);
  if (abs(XMVectorGetX(denom)) > EPSILON)
  {
    XMVECTOR center = XMVectorSet(plane.pf.x, plane.pf.y, plane.pf.z, 1.0f) * plane.pf.w;

    XMVECTOR dist = XMVector3Dot((center - O), planeNormal) / denom;
    
    if (XMVectorGetX(dist) < t && XMVectorGetX(dist) >= EPSILON)
    {
      primitive = &plane;
      t = XMVectorGetX(dist);
    }
  }
}

//------------------------------------------------------------------------------------------------------
void Ray::Intersect(Triangle& triangle)
{
  float dist = t;
  if (TriangleTests::Intersects(O, D, triangle.p0v, triangle.p1v, triangle.p2v, dist) && dist < t)
  {
    primitive = &triangle;
    t = dist;
  }
}

//------------------------------------------------------------------------------------------------------
void Ray::Intersect(AABB& aabb)
{
  float dist = t;
  if (aabb.box.Intersects(O, D, dist) && dist < t)
  {
    primitive = &aabb;
    t = dist;
  }
}
