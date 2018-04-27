#include "precomp.h"
#include "color.h"

Color const Color::kRed = Color(255u, 0u, 0u, 255u);
Color const Color::kGreen = Color(136u, 255u, 0u, 255u);
Color const Color::kGrassGreen = Color(61u, 74u, 48u, 255u);
Color const Color::kLime = Color(123u, 203u, 79u, 255u);
Color const Color::kBlue = Color(34u, 34u, 255u, 255u);
Color const Color::kSkyBlue = Color(169u, 198u, 242u, 255u);
Color const Color::kWhite = Color(255u, 255u, 255u, 255u);
Color const Color::kHalfDutchWhite = Color(255u, 244u, 214u, 255u);
Color const Color::kBlack = Color(0u, 0u, 0u, 255u);
Color const Color::kYellow = Color(255u, 238u, 0u, 255u);
Color const Color::kOrange = Color(255u, 165u, 0u, 255u);
Color const Color::kPurple = Color(160u, 32u, 240u, 255u);
Color const Color::kCyan = Color(0u, 255u, 255u, 255u);
Color const Color::kMagenta = Color(255u, 0u, 255u, 255u);
Color const Color::kBrown = Color(123u, 74u, 18u, 255u);
Color const Color::kGrey = Color(128u, 128u, 128u, 255u);
Color const Color::kGray = Color::kGrey;
Color const Color::kCornFlower = Color(100u, 149u, 237u, 255u);
Color const Color::kEigengrau = Color(22u, 22u, 29u, 255u);
Color const Color::kWhiteTransparent = Color(255u, 255u, 255u, 0u);
Color const Color::kBlackTransparent = Color(0u, 0u, 0u, 0u);

//------------------------------------------------------------------------------------------------------
Color::Color() :
  color_byte(0x000000FF)
{

}

//------------------------------------------------------------------------------------------------------
Color::Color(unsigned int color_byte) :
  color_byte(color_byte)
{

}

//------------------------------------------------------------------------------------------------------
Color::Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
  color_channels[COLOR_CHANNEL_RED]   = static_cast<unsigned char>(r);
  color_channels[COLOR_CHANNEL_GREEN] = static_cast<unsigned char>(g);
  color_channels[COLOR_CHANNEL_BLUE]  = static_cast<unsigned char>(b);
  color_channels[COLOR_CHANNEL_ALPHA] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
Color::Color(float r, float g, float b, float a)
{
  if (r > 1.0f || g > 1.0f || b > 1.0f || a > 1.0f)
  {
    color_channels[COLOR_CHANNEL_RED]   = static_cast<unsigned char>(r / 255.0f);
    color_channels[COLOR_CHANNEL_GREEN] = static_cast<unsigned char>(g / 255.0f);
    color_channels[COLOR_CHANNEL_BLUE]  = static_cast<unsigned char>(b / 255.0f);
    color_channels[COLOR_CHANNEL_ALPHA] = static_cast<unsigned char>(a / 255.0f);
  }
  else
  {
    color_channels[COLOR_CHANNEL_RED]   = static_cast<unsigned char>(r);
    color_channels[COLOR_CHANNEL_GREEN] = static_cast<unsigned char>(g);
    color_channels[COLOR_CHANNEL_BLUE]  = static_cast<unsigned char>(b);
    color_channels[COLOR_CHANNEL_ALPHA] = static_cast<unsigned char>(a);
  }
}

//------------------------------------------------------------------------------------------------------
Color::Color(const vec3& c, bool is_normalized)
{
  if (!is_normalized)
  {
    color_channels[COLOR_CHANNEL_RED]   = static_cast<unsigned char>(c.x);
    color_channels[COLOR_CHANNEL_GREEN] = static_cast<unsigned char>(c.y);
    color_channels[COLOR_CHANNEL_BLUE]  = static_cast<unsigned char>(c.z);
    color_channels[COLOR_CHANNEL_ALPHA] = static_cast<unsigned char>(255);
  }
  else
  {
    color_channels[COLOR_CHANNEL_RED]   = static_cast<unsigned char>(c.x * 255.0f);
    color_channels[COLOR_CHANNEL_GREEN] = static_cast<unsigned char>(c.y * 255.0f);
    color_channels[COLOR_CHANNEL_BLUE]  = static_cast<unsigned char>(c.z * 255.0f);
    color_channels[COLOR_CHANNEL_ALPHA] = static_cast<unsigned char>(255);
  }
}

//------------------------------------------------------------------------------------------------------
Color::Color(const vec4& c, bool is_normalized)
{
  if (!is_normalized)
  {
    color_channels[COLOR_CHANNEL_RED]   = static_cast<unsigned char>(c.x);
    color_channels[COLOR_CHANNEL_GREEN] = static_cast<unsigned char>(c.y);
    color_channels[COLOR_CHANNEL_BLUE]  = static_cast<unsigned char>(c.z);
    color_channels[COLOR_CHANNEL_ALPHA] = static_cast<unsigned char>(c.w);
  }
  else
  {
    color_channels[COLOR_CHANNEL_RED]   = static_cast<unsigned char>(c.x * 255.0f);
    color_channels[COLOR_CHANNEL_GREEN] = static_cast<unsigned char>(c.y * 255.0f);
    color_channels[COLOR_CHANNEL_BLUE]  = static_cast<unsigned char>(c.z * 255.0f);
    color_channels[COLOR_CHANNEL_ALPHA] = static_cast<unsigned char>(c.w * 255.0f);
  }
}

//------------------------------------------------------------------------------------------------------
Color& Color::operator+=(const Color& rhs)
{
  color_channels[COLOR_CHANNEL_ALPHA] = color_channels[COLOR_CHANNEL_ALPHA] + rhs.color_channels[COLOR_CHANNEL_ALPHA];
  color_channels[COLOR_CHANNEL_RED]   = color_channels[COLOR_CHANNEL_RED]   + rhs.color_channels[COLOR_CHANNEL_RED];
  color_channels[COLOR_CHANNEL_GREEN] = color_channels[COLOR_CHANNEL_GREEN] + rhs.color_channels[COLOR_CHANNEL_GREEN];
  color_channels[COLOR_CHANNEL_BLUE]  = color_channels[COLOR_CHANNEL_BLUE]  + rhs.color_channels[COLOR_CHANNEL_BLUE];
  return *this;
}

//------------------------------------------------------------------------------------------------------
Color& Color::operator-=(const Color& rhs)
{
  color_channels[COLOR_CHANNEL_ALPHA] = color_channels[COLOR_CHANNEL_ALPHA] - rhs.color_channels[COLOR_CHANNEL_ALPHA];
  color_channels[COLOR_CHANNEL_RED]   = color_channels[COLOR_CHANNEL_RED]   - rhs.color_channels[COLOR_CHANNEL_RED];
  color_channels[COLOR_CHANNEL_GREEN] = color_channels[COLOR_CHANNEL_GREEN] - rhs.color_channels[COLOR_CHANNEL_GREEN];
  color_channels[COLOR_CHANNEL_BLUE]  = color_channels[COLOR_CHANNEL_BLUE]  - rhs.color_channels[COLOR_CHANNEL_BLUE];
  return *this;
}

//------------------------------------------------------------------------------------------------------
Color& Color::operator*=(const Color& rhs)
{
  color_channels[COLOR_CHANNEL_ALPHA] = color_channels[COLOR_CHANNEL_ALPHA] * rhs.color_channels[COLOR_CHANNEL_ALPHA];
  color_channels[COLOR_CHANNEL_RED]   = color_channels[COLOR_CHANNEL_RED]   * rhs.color_channels[COLOR_CHANNEL_RED];
  color_channels[COLOR_CHANNEL_GREEN] = color_channels[COLOR_CHANNEL_GREEN] * rhs.color_channels[COLOR_CHANNEL_GREEN];
  color_channels[COLOR_CHANNEL_BLUE]  = color_channels[COLOR_CHANNEL_BLUE]  * rhs.color_channels[COLOR_CHANNEL_BLUE];
  return *this;
}

//------------------------------------------------------------------------------------------------------
Color& Color::operator/=(const Color& rhs)
{
  color_channels[COLOR_CHANNEL_ALPHA] = color_channels[COLOR_CHANNEL_ALPHA] / rhs.color_channels[COLOR_CHANNEL_ALPHA];
  color_channels[COLOR_CHANNEL_RED]   = color_channels[COLOR_CHANNEL_RED]   / rhs.color_channels[COLOR_CHANNEL_RED];
  color_channels[COLOR_CHANNEL_GREEN] = color_channels[COLOR_CHANNEL_GREEN] / rhs.color_channels[COLOR_CHANNEL_GREEN];
  color_channels[COLOR_CHANNEL_BLUE]  = color_channels[COLOR_CHANNEL_BLUE]  / rhs.color_channels[COLOR_CHANNEL_BLUE];
  return *this;
}

//------------------------------------------------------------------------------------------------------
Color operator+(Color lhs, const Color& rhs)
{
  Color r;
  r.color_channels[COLOR_CHANNEL_ALPHA] = lhs.color_channels[COLOR_CHANNEL_ALPHA] + rhs.color_channels[COLOR_CHANNEL_ALPHA];
  r.color_channels[COLOR_CHANNEL_RED]   = lhs.color_channels[COLOR_CHANNEL_RED]   + rhs.color_channels[COLOR_CHANNEL_RED];
  r.color_channels[COLOR_CHANNEL_GREEN] = lhs.color_channels[COLOR_CHANNEL_GREEN] + rhs.color_channels[COLOR_CHANNEL_GREEN];
  r.color_channels[COLOR_CHANNEL_BLUE]  = lhs.color_channels[COLOR_CHANNEL_BLUE]  + rhs.color_channels[COLOR_CHANNEL_BLUE];
  return r;
}

//------------------------------------------------------------------------------------------------------
Color operator-(Color lhs, const Color& rhs)
{
  Color r;
  r.color_channels[COLOR_CHANNEL_ALPHA] = lhs.color_channels[COLOR_CHANNEL_ALPHA] - rhs.color_channels[COLOR_CHANNEL_ALPHA];
  r.color_channels[COLOR_CHANNEL_RED]   = lhs.color_channels[COLOR_CHANNEL_RED]   - rhs.color_channels[COLOR_CHANNEL_RED];
  r.color_channels[COLOR_CHANNEL_GREEN] = lhs.color_channels[COLOR_CHANNEL_GREEN] - rhs.color_channels[COLOR_CHANNEL_GREEN];
  r.color_channels[COLOR_CHANNEL_BLUE]  = lhs.color_channels[COLOR_CHANNEL_BLUE]  - rhs.color_channels[COLOR_CHANNEL_BLUE];
  return r;
}

//------------------------------------------------------------------------------------------------------
Color operator*(Color lhs, const Color& rhs)
{
  Color r;

  r.color_channels[COLOR_CHANNEL_ALPHA] = static_cast<unsigned char>(((static_cast<float>(lhs.color_channels[COLOR_CHANNEL_ALPHA])  / 255.0f) * (static_cast<float>(rhs.color_channels[COLOR_CHANNEL_ALPHA])  / 255.0f)) * 255.0f);
  r.color_channels[COLOR_CHANNEL_RED]   = static_cast<unsigned char>(((static_cast<float>(lhs.color_channels[COLOR_CHANNEL_RED])    / 255.0f) * (static_cast<float>(rhs.color_channels[COLOR_CHANNEL_RED])    / 255.0f)) * 255.0f);
  r.color_channels[COLOR_CHANNEL_GREEN] = static_cast<unsigned char>(((static_cast<float>(lhs.color_channels[COLOR_CHANNEL_GREEN])  / 255.0f) * (static_cast<float>(rhs.color_channels[COLOR_CHANNEL_GREEN])  / 255.0f)) * 255.0f);
  r.color_channels[COLOR_CHANNEL_BLUE]  = static_cast<unsigned char>(((static_cast<float>(lhs.color_channels[COLOR_CHANNEL_BLUE])   / 255.0f) * (static_cast<float>(rhs.color_channels[COLOR_CHANNEL_BLUE])   / 255.0f)) * 255.0f);

  return r;
}

//------------------------------------------------------------------------------------------------------
Color operator/(Color lhs, const Color& rhs)
{
  Color r;
  r.color_channels[COLOR_CHANNEL_ALPHA] = lhs.color_channels[COLOR_CHANNEL_ALPHA] / rhs.color_channels[COLOR_CHANNEL_ALPHA];
  r.color_channels[COLOR_CHANNEL_RED]   = lhs.color_channels[COLOR_CHANNEL_RED]   / rhs.color_channels[COLOR_CHANNEL_RED];
  r.color_channels[COLOR_CHANNEL_GREEN] = lhs.color_channels[COLOR_CHANNEL_GREEN] / rhs.color_channels[COLOR_CHANNEL_GREEN];
  r.color_channels[COLOR_CHANNEL_BLUE]  = lhs.color_channels[COLOR_CHANNEL_BLUE]  / rhs.color_channels[COLOR_CHANNEL_BLUE];
  return r;
}

//------------------------------------------------------------------------------------------------------
void Color::SetAlpha(float a)
{
  color_channels[COLOR_CHANNEL_ALPHA] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
void Color::SetAlphaNormalized(float a)
{
  color_channels[COLOR_CHANNEL_ALPHA] = static_cast<unsigned char>(a * 255.0f);
}

//------------------------------------------------------------------------------------------------------
void Color::SetAlpha(unsigned char a)
{
  color_channels[COLOR_CHANNEL_ALPHA] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
void Color::SetAlpha(int a)
{
  color_channels[COLOR_CHANNEL_ALPHA] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
void Color::SetAlpha(unsigned int a)
{
  color_channels[COLOR_CHANNEL_ALPHA] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
unsigned char Color::GetAlpha(float a)
{
  return color_channels[COLOR_CHANNEL_ALPHA];
}

//------------------------------------------------------------------------------------------------------
float Color::GetAlphaNormalized(unsigned char a)
{
  return static_cast<float>(color_channels[COLOR_CHANNEL_ALPHA]) / 255.0f;
}

//------------------------------------------------------------------------------------------------------
void Color::SetRed(float a)
{
  color_channels[COLOR_CHANNEL_RED] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
void Color::SetRedNormalized(float a)
{
  color_channels[COLOR_CHANNEL_RED] = static_cast<unsigned char>(a * 255.0f);
}

//------------------------------------------------------------------------------------------------------
void Color::SetRed(unsigned char a)
{
  color_channels[COLOR_CHANNEL_RED] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
void Color::SetRed(int a)
{
  color_channels[COLOR_CHANNEL_RED] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
void Color::SetRed(unsigned int a)
{
  color_channels[COLOR_CHANNEL_RED] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
unsigned char Color::GetRed(float a)
{
  return color_channels[COLOR_CHANNEL_RED];
}

//------------------------------------------------------------------------------------------------------
float Color::GetRedNormalized(unsigned char a)
{
  return static_cast<float>(color_channels[COLOR_CHANNEL_RED]) / 255.0f;
}

//------------------------------------------------------------------------------------------------------
void Color::SetGreen(float a)
{
  color_channels[COLOR_CHANNEL_GREEN] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
void Color::SetGreenNormalized(float a)
{
  color_channels[COLOR_CHANNEL_GREEN] = static_cast<unsigned char>(a * 255.0f);
}

//------------------------------------------------------------------------------------------------------
void Color::SetGreen(unsigned char a)
{
  color_channels[COLOR_CHANNEL_GREEN] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
void Color::SetGreen(int a)
{
  color_channels[COLOR_CHANNEL_GREEN] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
void Color::SetGreen(unsigned int a)
{
  color_channels[COLOR_CHANNEL_GREEN] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
unsigned char Color::GetGreen(float a)
{
  return color_channels[COLOR_CHANNEL_GREEN];
}

//------------------------------------------------------------------------------------------------------
float Color::GetGreenNormalized(unsigned char a)
{
  return static_cast<float>(color_channels[COLOR_CHANNEL_GREEN]) / 255.0f;
}

//------------------------------------------------------------------------------------------------------
void Color::SetBlue(float a)
{
  color_channels[COLOR_CHANNEL_BLUE] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
void Color::SetBlueNormalized(float a)
{
  color_channels[COLOR_CHANNEL_BLUE] = static_cast<unsigned char>(a * 255.0f);
}

//------------------------------------------------------------------------------------------------------
void Color::SetBlue(unsigned char a)
{
  color_channels[COLOR_CHANNEL_BLUE] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
void Color::SetBlue(int a)
{
  color_channels[COLOR_CHANNEL_BLUE] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
void Color::SetBlue(unsigned int a)
{
  color_channels[COLOR_CHANNEL_BLUE] = static_cast<unsigned char>(a);
}

//------------------------------------------------------------------------------------------------------
unsigned char Color::GetBlue(float a)
{
  return color_channels[COLOR_CHANNEL_BLUE];
}

//------------------------------------------------------------------------------------------------------
float Color::GetBlueNormalized(unsigned char a)
{
  return static_cast<float>(color_channels[COLOR_CHANNEL_BLUE]) / 255.0f;
}
