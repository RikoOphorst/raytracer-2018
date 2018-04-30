#pragma once

struct BVHNode
{
  BoundingBox box;
};

class BVH
{
public:
  BVH();
  ~BVH();

  void ConstructBVH();
};