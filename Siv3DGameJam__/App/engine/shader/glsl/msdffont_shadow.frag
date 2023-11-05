//	Copyright (c) 2008-2022 Ryo Suzuki.
//	Copyright (c) 2016-2022 OpenSiv3D Project.
//	Licensed under the MIT License.

# version 410

//
//	Textures
//
uniform sampler2D Texture0;

//
//	PSInput
//
layout(location = 0) in vec4 Color;
layout(location = 1) in vec2 UV;

//
//	PSOutput
//
layout(location = 0) out vec4 FragColor;

//
//	Constant Buffer
//
layout(std140) uniform PSConstants2D
{
	vec4 g_colorAdd;
	vec4 g_sdfParam;
	vec4 g_sdfOutlineColor;
	vec4 g_sdfShadowColor;
	vec4 g_internal;
};

//
//	Functions
//
double median(double r, double g, double b)
{
	return max(min(r, g), min(max(r, g), b));
}

void main()
{
	vec2 size = textureSize(Texture0, 0);
	const double pxRange = 4.0;
	vec2 msdfUnit = (pxRange / size);

	vec3 s = texture(Texture0, UV).rgb;
	double d = median(s.r, s.g, s.b);

	double td = (d - 0.5);
	double textAlpha = clamp(td * dot(msdfUnit, 0.5 / fwidth(UV)) + 0.5, 0.0, 1.0);

	vec2 shadowOffset = (g_sdfParam.zw / size);
	vec3 s2 = texture(Texture0, UV - shadowOffset).rgb;
	double d2 = median(s2.r, s2.g, s2.b);

	double sd = (d2 - 0.5);
	double shadowAlpha = clamp(sd * dot(msdfUnit, 0.5 / fwidth(UV)) + 0.5, 0.0, 1.0);
	double sBase = shadowAlpha * (1.0 - textAlpha);

	vec4 color;
	if (textAlpha == 0.0)
	{
		color.rgb = g_sdfShadowColor.rgb;
	}
	else
	{
		color.rgb = mix(Color.rgb, g_sdfShadowColor.rgb, sBase);
	}
	color.a = (sBase * g_sdfShadowColor.a) + (textAlpha * Color.a);

	FragColor = (color + g_colorAdd);
}
