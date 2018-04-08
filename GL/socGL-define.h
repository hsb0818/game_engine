#pragma once

struct SOC_Color3f
{
	float a;
	float r;
	float g;
	float b;

	SOC_Color3f(float _a = 0.f, float _r = 0.f, 
		float _g = 0.f, float _b = 0.f)
		: a(_a)
		, r(_r)
		, g(_g)
		, b(_b)
	{}
};

struct SOC_Point2f
{
	float x;
	float y;

	SOC_Point2f(float _x = 0.f, float _y = 0.f)
		: x(_x)
		, y(_y)
	{}
};