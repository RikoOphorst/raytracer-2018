#pragma once

#include "color.h"

struct PointLight
{
  union
  {
    XMVECTOR p;
    XMFLOAT3 pf;
  };

  Color color;
};