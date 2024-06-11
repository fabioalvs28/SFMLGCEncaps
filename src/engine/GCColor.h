#pragma once

struct GCColor {

	int r;
	int g;
	int b;
	int a;

	GCColor();
	GCColor(int r, int g, int b, int a);
	GCColor(const char* hexaCode);
	GCColor(float degree, float saturationPercent, float lightPercent, float alpha);
	~GCColor();

	float HSLToRGB(float p, float q, float t);
};