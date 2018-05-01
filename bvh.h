#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;

struct Sphere;

enum class SplitAxis
{
  kX,
  kY,
  kZ
};

struct BVHNode
{
  BoundingBox bounds;
  int left_first; // Either stores the index of the left child BVHNode - or - the index of the first primitive contained.
  int count;      // If count == 0 then left_first should be interpreted as a leaf node and count means nothing then 
                  // - or -
                  // if count in non-zero, then left_first is interpreted as the first index of the primitive contained by this leaf node && count is the number of prims after the first primitive

  void Subdivide();
  void CalculateBounds();
  void Partition();
  SplitAxis BestSplitAxis();
};

class BVH
{
public:
  BVH();
  ~BVH();

  void ConstructBVH(Sphere* spheres, int num_spheres);

  BVHNode* nodes;
  int used_nodes;
  int max_nodes;

  BVHNode* root;

  static BVH* instance;
};