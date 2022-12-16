#version 450

struct LightBasic
{
	vec3 color; 
	vec3 power; // 	vec3 ambient; vec3 diffuse; vec3 specular;
};

struct DirectionalLight
{
	LightBasic light;
	vec3 direction;  
};

layout(std140) uniform LIGHT
{
	DirectionalLight u_sun;
};

layout(std140) uniform CAMERA
{
	vec3 u_camera_pos;
};

uniform float u_shininess = 0;

in VS_OUT
{
	vec3 worldPos;
	vec3 normal;
	vec4 color;
} fs_in;

out vec4 o_color;

vec3 caculateLight(DirectionalLight light, vec3 viewDir, vec3 normal, vec3 albedo, float shininess)
{
	vec3 lightColor = light.light.color.xyz;
	vec3 ambientColor = lightColor * light.light.power.x;
	vec3 diffuseColor = lightColor * light.light.power.y;
	vec3 specularColor = vec3(1) * light.light.power.z;

	vec3 ambient = ambientColor * albedo;
	
	vec3 lightDir = normalize(-light.direction.xyz);
	
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = diffuseColor * (diff * albedo);
	
	vec3 halfway_dir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfway_dir), 0), shininess);
	vec3 specular = specularColor * spec;
	return ambient + diffuse + specular;
}

void main()
{
	o_color = vec4(caculateLight(u_sun, u_camera_pos - fs_in.worldPos, fs_in.normal, fs_in.color.rgb, u_shininess), fs_in.color.a);
}

