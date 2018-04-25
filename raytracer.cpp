#include "precomp.h"

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
  screen_(nullptr),
  scene(nullptr)
{

}

//------------------------------------------------------------------------------------------------------
Raytracer::~Raytracer()
{

}

//------------------------------------------------------------------------------------------------------
void Raytracer::Init(Surface* screen)
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
  Mesh::screen = screen;
  // initialize scene
  (scene = new Scene())->root = new SGNode();
  screen_ = screen;
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
  Sphere mySphere(vec3(0.0f, 0.0f, 10.0f), 1.0f);
  
  zdepth = 15.0f;
  d = 2.0f;
  E = camera.GetPosition();
  V = camera.GetForward().normalized();
  C = E + d * V;

  vec3 topleft = (camera.transform * vec4(-1.0f, -1.0f, 0.0f, 0.0f)).xyz;
  vec3 topright = (camera.transform * vec4(1.0f, -1.0f, 0.0f, 0.0f)).xyz;
  vec3 bottomleft = (camera.transform * vec4(-1.0f, 1.0f, 0.0f, 0.0f)).xyz;

  p0 = C + topleft;
  p1 = C + topright;
  p2 = C + bottomleft;

  float wr = 1.0f / SCRWIDTH;
  float hr = 1.0f / SCRHEIGHT;

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
      D = normalize(sp - E);

      Ray someRay = Ray(E, D, zdepth);

      mySphere.Intersect(someRay);

      if (someRay.t != zdepth)
      {
        float percentage = 1 - (someRay.t / zdepth);

        unsigned int color = percentage * 255;
        color <<= 8;

        screen_->Plot(u, v, 0xFFFFFF);
      }
    }
  }
}

//------------------------------------------------------------------------------------------------------
Sphere::Sphere(const vec3& pos, float radius) :
  pos(pos),
  r(radius),
  r2(radius * radius)
{

}

//------------------------------------------------------------------------------------------------------
Sphere::~Sphere()
{

}

//------------------------------------------------------------------------------------------------------
void Sphere::Intersect(Ray& ray)
{
  vec3 c = pos - ray.O;
  float t = dot(c, ray.D);
  vec3 q = c - t * ray.D;
  float p2 = dot(q, q);
  
  if (p2 > r2)
  {
    return;
  }
  
  t -= sqrt(r2 - p2);
  
  if (t < ray.t && t > 0)
  {
    ray.t = t;
  }
}
