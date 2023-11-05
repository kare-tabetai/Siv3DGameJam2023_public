//	Copyright (c) 2008-2022 Ryo Suzuki.
//	Copyright (c) 2016-2022 OpenSiv3D Project.
//	Licensed under the MIT License.

# version 410

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
void main()
{
	double t = mod(UV.x, 2.0);
	vec2 tex = UV;
	tex.x = abs(1 - t) * 2.0;
	vec4 color = Color;

	double dist = dot(tex, tex) * 0.5;
	double delta = fwidth(dist);
	double alpha = smoothstep(0.5 - delta, 0.5, dist);
	color.a *= 1.0 - alpha;

	FragColor = (color + g_colorAdd);
}
