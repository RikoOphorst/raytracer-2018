#include "precomp.h"

#include "primitive.h"
#include "color.h"
#include "material.h"
#include "ray.h"
#include "light.h"

#include <algorithm>

#include <DirectXMath.h>

#define MAX_RAY_DEPTH 32

float* Raytracer::zbuffer = nullptr;
vec4 Raytracer::frustum[5];

#undef max
#undef min

//------------------------------------------------------------------------------------------------------
Raytracer::Raytracer() :
  screen(nullptr),
  scene(nullptr)
{

}

//------------------------------------------------------------------------------------------------------
Raytracer::~Raytracer()
{

}

//------------------------------------------------------------------------------------------------------
void Raytracer::Init(Surface* scr)
{
  // setup outline tables & zbuffer
  Mesh::xleft = new float[SCRHEIGHT], Mesh::xright = new float[SCRHEIGHT];
  Mesh::uleft = new float[SCRHEIGHT], Mesh::uright = new float[SCRHEIGHT];
  Mesh::vleft = new float[SCRHEIGHT], Mesh::vright = new float[SCRHEIGHT];
  Mesh::zleft = new float[SCRHEIGHT], Mesh::zright = new float[SCRHEIGHT];
  for (int y = 0; y < SCRHEIGHT; y++) Mesh::xleft[y] = SCRWIDTH - 1, Mesh::xright[y] = 0;
  zbuffer = new float[SCRWIDTH * SCRHEIGHT];
  // calculate view frustum planes
  float C = -1.0f, x1 = 0.5f, x2 = SCRWIDTH - 1.5f, y1 = 0.5f, y2 = SCRHEIGHT - 1.5f;
  vec3 p0 = { 0, 0, 0 };
  vec3 p1 = { ((x1 - SCRWIDTH / 2) * C) / SCRWIDTH, ((y1 - SCRHEIGHT / 2) * C) / SCRWIDTH, 1.0f };
  vec3 p2 = { ((x2 - SCRWIDTH / 2) * C) / SCRWIDTH, ((y1 - SCRHEIGHT / 2) * C) / SCRWIDTH, 1.0f };
  vec3 p3 = { ((x2 - SCRWIDTH / 2) * C) / SCRWIDTH, ((y2 - SCRHEIGHT / 2) * C) / SCRWIDTH, 1.0f };
  vec3 p4 = { ((x1 - SCRWIDTH / 2) * C) / SCRWIDTH, ((y2 - SCRHEIGHT / 2) * C) / SCRWIDTH, 1.0f };
  frustum[0] = { 0, 0, -1, 0.2f };
  frustum[1] = vec4(normalize(cross(p1 - p0, p4 - p1)), 0); // left plane
  frustum[2] = vec4(normalize(cross(p2 - p0, p1 - p2)), 0); // top plane
  frustum[3] = vec4(normalize(cross(p3 - p0, p2 - p3)), 0); // right plane
  frustum[4] = vec4(normalize(cross(p4 - p0, p3 - p4)), 0); // bottom plane
                                                            // store screen pointer
  Mesh::screen = scr;
  // initialize scene
  (scene = new Scene())->root = new SGNode();
  screen = scr;
  
  max_point_lights = 20;
  num_point_lights = 0;
  point_lights = new PointLight[max_point_lights];

  max_planes = 50;
  num_planes = 0;
  planes = new Plane[max_planes];

  max_spheres = 1000;
  num_spheres = 0;
  spheres = new Sphere[max_spheres];

  max_triangles = 10000000;
  num_triangles = 0;
  triangles = new Triangle[max_triangles];

  max_aabbs = 1000;
  num_aabbs = 0;
  aabbs = new AABB[max_aabbs];
}

//------------------------------------------------------------------------------------------------------
void Raytracer::FindNearest(Ray& ray)
{

}

//------------------------------------------------------------------------------------------------------
bool Raytracer::IsOccluded(Ray& ray)
{
  return false;
}

//------------------------------------------------------------------------------------------------------
void Tmpl8::Raytracer::LoadNode(SGNode* node, const mat4& transform)
{
  mat4 transformMatrix = transform * node->localTransform;

  if (node->GetType() == SGNode::SG_MESH)
  {
    Mesh& mesh = *reinterpret_cast<Mesh*>(node);

    for (int i = 0; i < mesh.tris; i++)
    {
      //Triangle* triangle = new Triangle(
      //  0,
      //  (transformMatrix * vec4(mesh.pos[mesh.tri[i] + 0], 1.0f)).xyz,
      //  (transformMatrix * vec4(mesh.pos[mesh.tri[i] + 1], 1.0f)).xyz,
      //  (transformMatrix * vec4(mesh.pos[mesh.tri[i] + 2], 1.0f)).xyz,
      //  mesh.norm[mesh.tri[i]]
      //);
      //
      //primitives.push_back(triangle);
    }
  }

  for (int i = 0; i < node->child.size(); i++)
  {
    LoadNode(node->child[i], transform * node->localTransform);
  }
}

//------------------------------------------------------------------------------------------------------
void Raytracer::Render(Camera& camera)
{
  zdepth = 150000.0f;
  d = 2.0f;

  XMVECTOR camera_position = XMVectorSet(camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z, 1.0f);

  vec3 p0t = camera.GetPosition() + (camera.transform * vec4(-SCRASPECT,  1, -d, 0)).xyz;
  vec3 p1t = camera.GetPosition() + (camera.transform * vec4( SCRASPECT,  1, -d, 0)).xyz;
  vec3 p2t = camera.GetPosition() + (camera.transform * vec4(-SCRASPECT, -1, -d, 0)).xyz;

  p0 = XMVectorSet(p0t.x, p0t.y, p0t.z, 1.0f);
  p1 = XMVectorSet(p1t.x, p1t.y, p1t.z, 1.0f);
  p2 = XMVectorSet(p2t.x, p2t.y, p2t.z, 1.0f);

  float wr = 1.0f / SCRWIDTH;
  float hr = 1.0f / SCRHEIGHT;

  vec3 camPos = camera.GetPosition();

  Ray ray(camera_position, XMVectorZero(), zdepth);
  Color color;

  ray_counter = 0;

  for (int u = 0; u < SCRWIDTH; u++)
  {
    for (int v = 0; v < SCRHEIGHT; v++)
    {
      XMVECTOR sp; // screen position

      float uu, vv;
      uu = u * wr;
      vv = v * hr;

      sp = p0 + XMVectorScale((p1 - p0), uu) + XMVectorScale((p2 - p0), vv);

      ray.D = XMVectorSetW(XMVector3Normalize(sp - ray.O), 0.0f);
      ray.t = zdepth;
      ray.primitive = nullptr;

      Trace(ray, color);
      screen->Plot(u, v, color.color_byte);
    }
  }

  char num_rays[256];
  sprintf_s(num_rays, 256, "Number of rays: %i", ray_counter);
  screen->Print(num_rays, 0, 30, Color::kRed.color_byte);
}

//------------------------------------------------------------------------------------------------------
void Tmpl8::Raytracer::Trace(Ray& ray, Color& out_color, int ray_depth)
{
  ray_counter++;

  XMVECTOR point, normal;
  int material;

  if (IntersectScene(ray, point, normal, material) && ray_depth < MAX_RAY_DEPTH)
  {
    if (materials[material]->is_mirror)
    {
      Ray secondary_ray(point, XMVector3Normalize(XMVector3Reflect(ray.D, normal)), zdepth);

      Color reflection_color;

      Trace(secondary_ray, reflection_color, ray_depth + 1);

      out_color = reflection_color * materials[material]->diffuse;
    }
    else
    {
      DirectIllumination(
        point,
        normal,
        out_color
      );

      out_color = out_color * materials[material]->diffuse;
    }
  }
  else
  {
    out_color = Color::kEigengrau;
  }
}

//------------------------------------------------------------------------------------------------------
bool Tmpl8::Raytracer::IntersectScene(Ray& ray, XMVECTOR& out_point, XMVECTOR& out_normal, int& out_material)
{
  int i = 0;

  for (i = 0; i < num_triangles; ++i)
  {
    ray.Intersect(triangles[i]);
  }

  for (i = 0; i < num_aabbs; ++i)
  {
    ray.Intersect(aabbs[i]);
  }

  for (i = 0; i < num_planes; ++i)
  {
    ray.Intersect(planes[i]);
  }

  for (i = 0; i < num_spheres; ++i)
  {
    ray.Intersect(spheres[i]);
  }

  if (ray.primitive != nullptr)
  {
    out_point = ray.O + DirectX::XMVectorScale(XMVector3Normalize(ray.D), ray.t - 0.001f);

    switch ((PrimitiveType)ray.primitive_type)
    {
    case PrimitiveType::kTriangle:
      out_material = reinterpret_cast<Triangle*>(ray.primitive)->material;
      out_normal = reinterpret_cast<Triangle*>(ray.primitive)->nv;
      break;
    case PrimitiveType::kAABB:
      out_material = reinterpret_cast<AABB*>(ray.primitive)->material;
      //out_normal = reinterpret_cast<AABB*>(ray.primitive)->;
      break;
    case PrimitiveType::kPlane:
      out_material = reinterpret_cast<Plane*>(ray.primitive)->material;
      out_normal = XMVectorSetW(reinterpret_cast<Plane*>(ray.primitive)->p, 0.0f);
      break;
    case PrimitiveType::kSphere:
      out_material = reinterpret_cast<Sphere*>(ray.primitive)->material;
      out_normal = reinterpret_cast<Sphere*>(ray.primitive)->NormalAt(out_point);
      break;
    }

    return true;
  }

  return false;
}

//------------------------------------------------------------------------------------------------------
void Tmpl8::Raytracer::DirectIllumination(const XMVECTOR& point, const XMVECTOR& normal, Color& out_color)
{
  for (int i = 0; i < num_point_lights; i++)
  {
    XMVECTOR L = point_lights[i].p - point;
    XMVECTOR dist = XMVector3Length(L);

    L = XMVector3Normalize(L);

    if (!IsVisible(point, L, XMVectorGetX(dist)))
    {
      out_color = Color::kBlack;
    }
    else
    {
      XMVECTOR NdotL = XMVector3Dot(normal, L);
      if (XMVectorGetX(NdotL) <= 0.0f)
      {
        out_color = Color::kBlack;
      }
      else
      {
        out_color = Color(
          vec3(1.0f, 1.0f, 1.0f) *
          std::max(XMVectorGetX(NdotL), 0.0f)
          , true
        );
      }
    }
  }
}

//------------------------------------------------------------------------------------------------------
bool Tmpl8::Raytracer::IsVisible(const XMVECTOR& origin, const XMVECTOR& direction, float t)
{
  Ray visibility_ray(origin, direction, t);

  int i = 0;

  for (i = 0; i < num_triangles; ++i)
  {
    visibility_ray.Intersect(triangles[i]);

    if (visibility_ray.t < t)
      return false;
  }

  for (i = 0; i < num_aabbs; ++i)
  {
    visibility_ray.Intersect(aabbs[i]);

    if (visibility_ray.t < t)
      return false;
  }

  for (i = 0; i < num_planes; ++i)
  {
    visibility_ray.Intersect(planes[i]);

    if (visibility_ray.t < t)
      return false;
  }

  for (i = 0; i < num_spheres; ++i)
  {
    visibility_ray.Intersect(spheres[i]);

    if (visibility_ray.t < t)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------------------------------
int Tmpl8::Raytracer::AddPointLight(const XMFLOAT3& pos, const Color& color)
{
  point_lights[num_point_lights].pf = pos;
  point_lights[num_point_lights].color = color;
  
  return num_point_lights++;
}
