#version 450

uniform dmat4 u_model_mat;

layout(std140) uniform VP_MAT
{
	dmat4 u_vp_mat;
};

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_color;

out VS_OUT
{
	vec3 world_pos;
	vec3 normal;
	vec3 color;
} vs_out;

void main()
{
	vs_out.world_pos = vec3(u_model_mat * vec4(a_position, 1.0));
	vs_out.normal = normalize(mat3(transpose(inverse(u_model_mat))) * a_normal);
	vs_out.color = vec3(a_color);
	gl_Position = vec4(u_vp_mat * vec4(vs_out.world_pos, 1));
}
