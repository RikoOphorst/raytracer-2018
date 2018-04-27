#include "precomp.h"

#include "primitive.h"
#include "color.h"
#include "material.h"

float* Raytracer::zbuffer = nullptr;
vec4 Raytracer::frustum[5];

bool IntersectSphere(const vec3& rayO, const vec3& rayD)
{
  vec3 sphereOrigin(0, 0, 10);
  float sphereR2 = 0.25f;

  vec3 c = sphereOrigin - rayO;
  float t = dot(c, rayD);
  vec3 q = c - t * rayD;
  float p2 = dot(q, q);
  if (p2 > sphereR2) return false;
  t -= sqrt(sphereR2 - p2);
  return t < 0;
}

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

  materials.push_back(new Material(false, Color::kBlue));

  primitives.push_back(new Sphere(0, vec3(0.0f, 1.0f, -50.0f), 1.0f));
  primitives.push_back(new Plane(0, vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
  primitives.push_back(new Triangle(0, vec3(10.0f, 20.0f, -50.0f), vec3(10.0f, 0.0f, -50.0f), vec3(20.0f, 0.0f, -50.0f), vec3(0.0f, 0.0f, 1.0f)));
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
void Raytracer::Render(Camera& camera)
{
  zdepth = 1500.0f;
  d = 5.0f;

  p0 = camera.GetPosition() + (camera.transform * vec4(-SCRASPECT,  1, -d, 0)).xyz;
  p1 = camera.GetPosition() + (camera.transform * vec4( SCRASPECT,  1, -d, 0)).xyz;
  p2 = camera.GetPosition() + (camera.transform * vec4(-SCRASPECT, -1, -d, 0)).xyz;

  float wr = 1.0f / SCRWIDTH;
  float hr = 1.0f / SCRHEIGHT;

  Ray ray(camera.GetPosition(), p0, zdepth);
  Color color;

  for (int u = 0; u < SCRWIDTH; u++)
  {
    for (int v = 0; v < SCRHEIGHT; v++)
    {
      vec3 sp; // screen position

      float uu, vv;
      uu = u * wr;
      vv = v * hr;

      sp = p0 + (uu * (p1 - p0)) + (vv * (p2 - p0));

      vec3 D; // ray direction
      D = normalize(sp - camera.GetPosition());

      ray.O = camera.GetPosition();
      ray.D = normalize(sp - camera.GetPosition());
      ray.t = zdepth;

      Trace(ray, color);
      screen->Plot(u, v, color.color_byte);
    }
  }
}

//------------------------------------------------------------------------------------------------------
void Tmpl8::Raytracer::Trace(const Ray& ray, Color& out_color)
{
  int i;
  int num_primitives = primitives.size();
  bool hit;
  float t;
  vec3 I;
  vec3 N;
  
  for (i = 0; i < num_primitives; ++i)
  {
    hit = false;
    t = zdepth;
    primitives[i]->Intersect(ray, hit, t, I, N);

    if (hit)
    {
      DirectIllumination(I, N, out_color);

      out_color = out_color * materials[primitives[i]->mat]->diffuse;

      break;
    }
    else
    {
      out_color = Color::kBlack;
    }
  }
}

//------------------------------------------------------------------------------------------------------
void Tmpl8::Raytracer::DirectIllumination(const vec3& I, const vec3& N, Color& out_color)
{
  vec3 L = vec3(0.0f, 5.0f, -40.0f) - I;
  float dist = L.length();
  L.normalize();

  if (!IsVisible(I, L, dist))
  {
    out_color = Color::kBlack;
  }
  else
  {
    out_color = Color(vec3(1.0f, 1.0f, 1.0f) * dot(N, L), true);
  }
}

//------------------------------------------------------------------------------------------------------
bool Tmpl8::Raytracer::IsVisible(const vec3& from, const vec3& direction, float t)
{
  Ray ray(from, direction, 1000.0f);

  int i;
  int num_primitives = primitives.size();
  bool hit;
  vec3 I0;
  vec3 N;
  float t0;

  for (i = 0; i < num_primitives; ++i)
  {
    hit = false;
    primitives[i]->Intersect(ray, hit, t0, I0, N);
    
    if (hit)
    {
      return false;
    }
  }

  return true;
}
