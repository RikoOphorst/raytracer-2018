#include "precomp.h" // include (only) this in every .cpp file

Raytracer raytracer;
Camera camera;
vec3 position;

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	// initialize raytracer
	raytracer.Init( screen );
	// setup camera (note: in ogl/glm, z for 'far' is -inf)
	position = vec3( 0, 0, 8 );
	camera.SetPosition( position );
	camera.LookAt( vec3( 0, 0, 0 ) );
	// initialize scene
	//raytracer.scene->Add( "assets/unity_full/unityScene.obj" );
}

// -----------------------------------------------------------
// Input handling
// -----------------------------------------------------------
void Game::HandleInput( float dt )
{
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
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------
void Game::Tick( float deltaTime )
{
	HandleInput( 1 );
	screen->Clear( 0 );
	raytracer.Render( camera );
}