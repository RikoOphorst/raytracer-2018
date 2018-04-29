#pragma once

struct Primitive;
struct Color;
struct Material;
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
    void IntersectScene(Ray& ray, XMVECTOR& out_point, XMVECTOR& out_normal, int& out_material);
    void DirectIllumination(const XMVECTOR& I, const XMVECTOR& N, Color& out_color);
    bool IsVisible(const XMVECTOR& origin, const XMVECTOR& direction, float t);

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
    std::vector<Primitive*> primitives;

    int ray_counter = 0;
  };
};