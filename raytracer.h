#pragma once

struct Plane;
struct Sphere;
struct AABB;
struct Triangle;
struct Color;
struct Material;
struct PointLight;
class Ray;

namespace Tmpl8
{
  class Raytracer
  {
  public:
    Raytracer();
    ~Raytracer();

    void Init(Surface* screen);
    void FindNearest(Ray& ray);
    bool IsOccluded(Ray& ray);
    void Render(Camera& camera);

    void LoadNode(SGNode* node, const mat4& transform);

    void Trace(Ray& ray, Color& out_color, int ray_depth = 0);
    bool IntersectScene(Ray& ray, XMVECTOR& out_point, XMVECTOR& out_normal, int& out_material);
    void DirectIllumination(const XMVECTOR& I, const XMVECTOR& N, Color& out_color);
    bool IsVisible(const XMVECTOR& origin, const XMVECTOR& direction, float t);
    int AddPointLight(const XMFLOAT3& pos, const Color& color);

    Scene* scene;
    static float* zbuffer;
    static vec4 frustum[5];

    float d; // distance of the screen plane from the E point
    XMVECTOR p0; // top left of screen
    XMVECTOR p1; // top right of screen
    XMVECTOR p2; // bottom left of screen

    float zdepth; // zdepth

    Surface* screen;

    std::vector<Material*> materials;

    Plane* planes;
    int max_planes, num_planes;

    Sphere* spheres;
    int max_spheres, num_spheres;

    AABB* aabbs;
    int max_aabbs, num_aabbs;

    Triangle* triangles;
    int max_triangles, num_triangles;

    PointLight* point_lights;
    int num_point_lights, max_point_lights;

    int ray_counter = 0;

    static Raytracer* instance;
  };
};