#pragma once

#define COLOR_CHANNEL_ALPHA 3
#define COLOR_CHANNEL_RED 2
#define COLOR_CHANNEL_GREEN 1
#define COLOR_CHANNEL_BLUE 0

struct Color
{
  static const Color kRed;
  static const Color kGreen;
  static const Color kGrassGreen;
  static const Color kLime;
  static const Color kBlue;
  static const Color kSkyBlue;
  static const Color kWhite;
  static const Color kHalfDutchWhite;
  static const Color kBlack;
  static const Color kYellow;
  static const Color kOrange;
  static const Color kPurple;
  static const Color kCyan;
  static const Color kMagenta;
  static const Color kBrown;
  static const Color kGrey;
  static const Color kGray;
  static const Color kCornFlower;
  static const Color kEigengrau;
  
  static const Color kWhiteTransparent;
  static const Color kBlackTransparent;

  union 
  {
    unsigned int color_byte;
    unsigned char color_channels[4];
  };

  Color();
  Color(unsigned int byte_color);
  Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
  Color(float r, float g, float b, float a);
  Color(const vec3& c, bool is_normalized = true);
  Color(const vec4& c, bool is_normalized = true);

  Color& operator+=(const Color& rhs);
  friend Color operator+(Color lhs, const Color& rhs);
  Color& operator-=(const Color& rhs);
  friend Color operator-(Color lhs, const Color& rhs);
  Color& operator*=(const Color& rhs);
  friend Color operator*(Color lhs, const Color& rhs);
  Color& operator/=(const Color& rhs);
  friend Color operator/(Color lhs, const Color& rhs);

  void SetAlpha(float a);
  void SetAlphaNormalized(float a);
  void SetAlpha(unsigned char a);
  void SetAlpha(int a);
  void SetAlpha(unsigned int a);

  unsigned char GetAlpha(float a);
  float GetAlphaNormalized(unsigned char a);

  void SetRed(float a);
  void SetRedNormalized(float a);
  void SetRed(unsigned char a);
  void SetRed(int a);
  void SetRed(unsigned int a);

  unsigned char GetRed(float a);
  float GetRedNormalized(unsigned char a);

  void SetGreen(float a);
  void SetGreenNormalized(float a);
  void SetGreen(unsigned char a);
  void SetGreen(int a);
  void SetGreen(unsigned int a);

  unsigned char GetGreen(float a);
  float GetGreenNormalized(unsigned char a);

  void SetBlue(float a);
  void SetBlueNormalized(float a);
  void SetBlue(unsigned char a);
  void SetBlue(int a);
  void SetBlue(unsigned int a);

  unsigned char GetBlue(float a);
  float GetBlueNormalized(unsigned char a);
};