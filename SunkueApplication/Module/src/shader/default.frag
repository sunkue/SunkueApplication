#version 450


in VS_OUT
{
	vec3 world_pos;
	vec3 normal;
	vec3 color;
} fs_in;


out vec4 o_color;

void main()
{
	o_color = vec4(fs_in.color, 1);
}

