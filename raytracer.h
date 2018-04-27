#pragma once

class Primitive;
struct Color;
struct Material;

namespace Tmpl8
{
  // -----------------------------------------------------------
  // Raytracer struct
  // generic ray
  // -----------------------------------------------------------
  class Ray
  {
  public:
    // constructor / destructor
    Ray(vec3 origin, vec3 direction, float distance) : O(origin), D(direction), t(distance) {}
    // methods
    void Intersect(Mesh* mesh, int triIdx);

    // data members
    vec3 O, D;
    float t;
  };

  class Raytracer
  {
  public:
    Raytracer();
    ~Raytracer();

    void Init(Surface* screen);
    void FindNearest(Ray& ray);
    bool IsOccluded(Ray& ray);
    void Render(Camera& camera);

    void Trace(const Ray& ray, Color& out_color);
    void DirectIllumination(const vec3& I, const vec3& N, Color& out_color);
    bool IsVisible(const vec3& from, const vec3& to, float t);

    Scene* scene;
    static float* zbuffer;
    static vec4 frustum[5];

    float d; // distance of the screen plane from the E point
    vec3 p0; // top left of screen
    vec3 p1; // top right of screen
    vec3 p2; // bottom left of screen

    float zdepth; // zdepth

    Surface* screen;

    std::vector<Material*> materials;
    std::vector<Primitive*> primitives;
  };
};