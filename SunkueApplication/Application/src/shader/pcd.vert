#version 450

uniform mat4 u_model_mat;

layout(std140) uniform V_MAT
{
	mat4 u_view_mat;
};

layout(std140) uniform P_MAT
{
	mat4 u_proj_mat;
};

layout(std140) uniform RESOLUTION
{
	ivec4 u_viewport;
};

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_color;

uniform float u_radius;
uniform bool u_show_normal_color = false;

out VS_OUT
{
	vec3 worldPos;
	vec3 normal;
	vec4 color;
} vs_out;

flat out vec2 center;
flat out float radiusPixels;

void main()
{
	vs_out.worldPos = vec3(u_model_mat * vec4(a_position, 1));
	vs_out.normal = normalize(mat3(transpose(inverse(u_model_mat))) * a_normal);
	vs_out.color = vec4(a_color, 1);
	vs_out.color.rgb = (1. - float(u_show_normal_color)) * vs_out.color.rgb + float(u_show_normal_color) * vs_out.normal;
	gl_Position = vec4(u_proj_mat * u_view_mat * vec4(vs_out.worldPos, 1));
	center = u_viewport.xy + (0.5 * gl_Position.xy/gl_Position.w + 0.5) * u_viewport.zw;
    gl_PointSize =  max(1, u_viewport.w * u_proj_mat[1][1] * u_radius / gl_Position.w);
    radiusPixels = gl_PointSize / 2.0;
}
