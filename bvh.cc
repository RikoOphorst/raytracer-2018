#include "precomp.h"

#include "bvh.h"
#include "raytracer.h"
#include "primitive.h"

#include <limits>

#undef min
#undef max

BVH* BVH::instance = nullptr;

//------------------------------------------------------------------------------------------------------
BVH::BVH()
{
  instance = this;
}

//------------------------------------------------------------------------------------------------------
BVH::~BVH()
{

}

//------------------------------------------------------------------------------------------------------
void BVH::ConstructBVH(Sphere* spheres, int num_spheres)
{
  max_nodes = num_spheres * 2 - 1;
  used_nodes = 0;

  nodes = new BVHNode[num_spheres * 2 - 1];

  root = &nodes[0];
  used_nodes = 2;

  root->left_first = 0;
  root->count = num_spheres;
  root->CalculateBounds();
  root->Subdivide();
}

//------------------------------------------------------------------------------------------------------
void BVHNode::Subdivide()
{
  if (count < 3)
    return;

  left_first = BVH::instance->used_nodes;
  BVH::instance->used_nodes = BVH::instance->used_nodes + 2;

  Partition();
  BVH::instance->nodes[left_first + 0].Subdivide();
  BVH::instance->nodes[left_first + 1].Subdivide();
}

//------------------------------------------------------------------------------------------------------
void BVHNode::CalculateBounds()
{
  XMFLOAT3 min(FLT_MAX, FLT_MAX, FLT_MAX);
  XMFLOAT3 max(FLT_MIN, FLT_MIN, FLT_MIN);

  Raytracer& rt = *Raytracer::instance;

  for (int i = 0; i < count; i++)
  {
    if (rt.spheres[left_first + i].sphere.Center.x - rt.spheres[left_first + i].sphere.Radius < min.x)
      min.x = rt.spheres[left_first + i].sphere.Center.x - rt.spheres[left_first + i].sphere.Radius;

    if (rt.spheres[left_first + i].sphere.Center.y - rt.spheres[left_first + i].sphere.Radius < min.y)
      min.y = rt.spheres[left_first + i].sphere.Center.y - rt.spheres[left_first + i].sphere.Radius;

    if (rt.spheres[left_first + i].sphere.Center.z - rt.spheres[left_first + i].sphere.Radius < min.z)
      min.z = rt.spheres[left_first + i].sphere.Center.z - rt.spheres[left_first + i].sphere.Radius;

    if (rt.spheres[left_first + i].sphere.Center.x + rt.spheres[left_first + i].sphere.Radius > max.x)
      max.x = rt.spheres[left_first + i].sphere.Center.x + rt.spheres[left_first + i].sphere.Radius;

    if (rt.spheres[left_first + i].sphere.Center.y + rt.spheres[left_first + i].sphere.Radius > max.y)
      max.y = rt.spheres[left_first + i].sphere.Center.y + rt.spheres[left_first + i].sphere.Radius;

    if (rt.spheres[left_first + i].sphere.Center.z + rt.spheres[left_first + i].sphere.Radius > max.z)
      max.z = rt.spheres[left_first + i].sphere.Center.z + rt.spheres[left_first + i].sphere.Radius;
  }

  XMVECTOR minv = XMVectorSet(min.x, min.y, min.z, 0.0f);
  XMVECTOR maxv = XMVectorSet(max.x, max.y, max.z, 0.0f);
  XMVECTOR centerv = maxv + minv * 0.5f;

  XMStoreFloat3(&bounds.Center, centerv);

  bounds.Extents = XMFLOAT3(
    max.x - min.x,
    max.y - min.y,
    max.z - min.z
  );
}

//------------------------------------------------------------------------------------------------------
void BVHNode::Partition()
{
  //XMFLOAT3 center_left = XMFLOAT3(
  //  bounds.Center.x + bounds.Extents.x * 0.5f,
  //  bounds.Center.y + bounds.Extents.y * 0.5f,
  //  bounds.Center.z + bounds.Extents.z * 0.5f
  //);
  //
  //XMFLOAT3 center_right = XMFLOAT3(
  //  bounds.Center.x - bounds.Extents.x * 0.5f,
  //  bounds.Center.y - bounds.Extents.y * 0.5f,
  //  bounds.Center.z - bounds.Extents.z * 0.5f
  //);

  SplitAxis axis = BestSplitAxis();


}

//------------------------------------------------------------------------------------------------------
SplitAxis BVHNode::BestSplitAxis()
{
  if (bounds.Extents.x < bounds.Extents.y && bounds.Extents.x < bounds.Extents.z)
  {
    // Split ALONG x axis
    return SplitAxis::kX;
  }
  else if (bounds.Extents.y < bounds.Extents.x && bounds.Extents.y < bounds.Extents.z)
  {
    // Split ALONG y axis
    return SplitAxis::kY;
  }
  else if (bounds.Extents.z < bounds.Extents.x && bounds.Extents.z < bounds.Extents.y)
  {
    // Split ALONG z axis
    return SplitAxis::kZ;
  }
  else
  {
    if (bounds.Extents.x == bounds.Extents.y)
    {
      // Can split along X or Y axis.
      // Choose X axis (arbitrary choice).
      return SplitAxis::kX;
    }
    else if (bounds.Extents.x == bounds.Extents.z)
    {
      // Can split along X or Z axis.
      // Choose X axis (arbitrary choice).
      return SplitAxis::kX;
    }
    else if (bounds.Extents.y == bounds.Extents.x)
    {
      // Can split along Y or X axis.
      // Choose Y axis (arbitrary choice).
      return SplitAxis::kY;
    }
    else if (bounds.Extents.y == bounds.Extents.z)
    {
      // Can split along Y or Z axis.
      // Choose Y axis (arbitrary choice).
      return SplitAxis::kY;
    }
  }
}
