#pragma once
#include <corecrt_math.h>

struct GCColor {

	int r;
	int g;
	int b;
	int a;

	// Default Constructor
	GCColor();

	/// <summary>
	///  Constructor using RGBA parameters
	/// </summary>
	/// <param name="r">Red element of the color</param>
	/// <param name="g">Green element of the color</param>
	/// <param name="b">Blue element of the color</param>
	/// <param name="a">Alpha element of the color</param>
	GCColor(int r, int g, int b, int a);

	/// <summary>
	///  Constructor using hexadecimal code parameters
	/// </summary>
	/// <param name="hexaCode">Hexadecimal color code</param>
	GCColor(const char* hexaCode);

	/// <summary>
	///  Constructor using HSLA parameters
	/// </summary>
	/// <param name="hue">Hue element of the color</param>
	/// <param name="saturation">Saturation element of the color</param>
	/// <param name="light">light element of the color</param>
	/// <param name="alpha">Alpha element of the color </param>
	GCColor(float hue, float saturation, float light, float alpha);

	//Destructor
	~GCColor();

private:
	float HSLToRGB(float p, float q, float t);
};