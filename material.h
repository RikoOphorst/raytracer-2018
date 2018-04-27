#pragma once

#include "color.h"

struct Material
{
  Material();
  Material(bool is_mirror);
  Material(Color diffuse);
  Material(bool is_mirror, const Color& diffuse);

  bool is_mirror;
  Color diffuse;
};