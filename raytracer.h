#pragma once

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

    Scene* scene;
    static float* zbuffer;
    static vec4 frustum[5];

    vec3 E; // eye position
    vec3 V; // view direction
    vec3 C; // center of screen;
    float d; // distance of the screen plane from the E point
    vec3 p0; // top left of screen
    vec3 p1; // top right of screen
    vec3 p2; // bottom left of screen

    float zdepth; // zdepth

  private:
    Surface* screen_;
  };

  class Sphere
  {
  public:
    Sphere(const vec3& pos, float radius);
    ~Sphere();

    void Intersect(Ray& ray);

  private:
    vec3 pos;
    float r;
    float r2;
  };

};