#include "precomp.h"
#include "material.h"

//------------------------------------------------------------------------------------------------------
Material::Material() :
  is_mirror(false),
  diffuse(0xFFFFFFFF)
{

}

//------------------------------------------------------------------------------------------------------
Material::Material(bool is_mirror) :
  is_mirror(is_mirror),
  diffuse(0xFFFFFFFF)
{

}

//------------------------------------------------------------------------------------------------------
Material::Material(Color diffuse) :
  is_mirror(false),
  diffuse(diffuse)
{

}

//------------------------------------------------------------------------------------------------------
Material::Material(bool is_mirror, const Color& diffuse) :
  is_mirror(is_mirror),
  diffuse(diffuse)
{

}
