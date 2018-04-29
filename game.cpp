#include "precomp.h" // include (only) this in every .cpp file

#include <chrono>

#include "color.h"

#include "primitive.h"
#include "material.h"
#include "timer.h"

Raytracer raytracer;
Rasterizer rasterizer;
Camera camera;
vec3 position;
bool space_pressed;
bool space_released;
bool space;
bool frame_rendered;

Timer frame_timer;
Timer render_timer;

std::chrono::time_point<std::chrono::high_resolution_clock> prev_frame;

enum class RenderState
{
  kRasterized,
  kRaytraced
};

RenderState render_state;

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
  space_pressed = false;
  render_state = RenderState::kRaytraced;
  frame_rendered = false;

	// initialize raytracer
	raytracer.Init(screen);
  rasterizer.Init(screen);

	position = vec3( 0, 0, 8 );
	camera.SetPosition( position );
	camera.LookAt( vec3( 0, 0, 0 ) );

  if (false)
  {
    SGNode* unity_scene = rasterizer.scene->LoadOBJ("assets/unity_full/unityScene.obj", 1.0f);
    raytracer.materials.push_back(new Material(false, Color::kBlue));
    raytracer.LoadNode(unity_scene->child[4], mat4::identity());
    rasterizer.scene->root->Add(unity_scene);
  }
  else
  {
    raytracer.materials.push_back(new Material(false, Color::kBlue));   // 0

    float boxSize = 10.0f;

    for (int x = 0; x < 10; x++)
    {
      for (int y = 0; y < 10; y++)
      {
        raytracer.primitives.push_back(new Sphere(0, XMFLOAT3(x, 0.0f, y), 0.5f));
      }
    }
  }
}

// -----------------------------------------------------------
// Input handling
// -----------------------------------------------------------
void Game::HandleInput( float dt )
{
  float move_speed = 0.01f;
  float rotation_speed = 5.0f;

  if (GetAsyncKeyState(VK_SHIFT))
  {
    move_speed *= 5.0f;
    rotation_speed *= 2.5f;
  }

	if (GetAsyncKeyState( 'W' )) position += camera.GetForward() * dt * 0.4f;
	if (GetAsyncKeyState( 'S' )) position -= camera.GetForward() * dt * 0.4f;
	if (GetAsyncKeyState( 'A' )) position -= camera.GetRight() * dt * 0.4f;
	if (GetAsyncKeyState( 'D' )) position += camera.GetRight() * dt * 0.4f;
	if (GetAsyncKeyState( 'R' )) position += camera.GetUp() * dt * 0.4f;
	if (GetAsyncKeyState( 'F' )) position -= camera.GetUp() * dt * 0.4f;
	camera.SetPosition( position );
	if (GetAsyncKeyState( VK_LEFT )) camera.LookAt( camera.GetPosition() + camera.GetForward() - dt * 0.02f * camera.GetRight() );
	if (GetAsyncKeyState( VK_RIGHT )) camera.LookAt( camera.GetPosition() + camera.GetForward() + dt * 0.02f * camera.GetRight() );
	if (GetAsyncKeyState( VK_UP )) camera.LookAt( camera.GetPosition() + camera.GetForward() + dt * 0.02f * camera.GetUp() );
	if (GetAsyncKeyState( VK_DOWN )) camera.LookAt( camera.GetPosition() + camera.GetForward() - dt * 0.02f * camera.GetUp() );

  if (GetAsyncKeyState(VK_SPACE))
  {
    if (space_released)
    {
      space = true;
    }
    else
    {
      space = false;
    }

    space_pressed = true;
    space_released = false;
  }
  else
  {
    space_pressed = false;
    space_released = true;
    space = false;
  }

  if (space)
  {
    if (render_state == RenderState::kRasterized)
    {
      render_state = RenderState::kRaytraced;
      frame_rendered = false;
    }
    else
    {
      render_state = RenderState::kRasterized;
    }
  }
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------
void Game::Tick( float deltaTime )
{
  frame_timer.Reset();
  Sleep(50);

  double dt = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - prev_frame).count());
  HandleInput(1);

  if (render_state == RenderState::kRasterized)
  {
    screen->Clear(Color::kSkyBlue.color_byte);

    char perf_text[256];
    render_timer.Reset();
    rasterizer.Render(camera);
    
    sprintf_s(perf_text, 200, "Render time: %f ms", static_cast<double>(render_timer.ElapsedInNanoseconds()) / 1000000.0);
    screen->Print(perf_text, 0, 5, Color::kWhite.color_byte);
  }
  else
  {
    if (!frame_rendered || true)
    {
      screen->Clear(Color::kSkyBlue.color_byte);
      
      char perf_text[256];
      render_timer.Reset();
      raytracer.Render(camera);

      sprintf_s(perf_text, 200, "Render time: %f ms", static_cast<double>(render_timer.ElapsedInNanoseconds()) / 1000000.0);
      screen->Print(perf_text, 0, 5, Color::kWhite.color_byte);
      
      frame_rendered = true;
    }
  }
}