#version 450

in VS_OUT
{
	vec3 worldPos;
	vec3 normal;
	vec3 color;
} fs_in;

out vec4 o_color;

flat in vec2 center;
flat in float radiusPixels;

void main()
{
	vec2 coord = (gl_FragCoord.xy - center) / radiusPixels;
    if (length(coord) > 1.0)
        discard;

	o_color = vec4(fs_in.color, 1);
}

