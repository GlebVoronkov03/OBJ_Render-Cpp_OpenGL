#version 440

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
}

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec3 lightPos0;
uniform vec3 cameraPos;

void main()
{
	//Ambient light
	vec3 ambientLight = vec3(0.1f, 0.1f, 0.1f);

	//Diffuse light
	vec3 posToLightDirVec = normalize(vs_position - lightPos0);
	vec3 diffuseColor = vec3(1.f, 1.f, 1.f);
	float diffuse = clamp(dot(posToLightDirVec, vs_normal), 0, 1);
	vec3 diffuseFinal = diffuseColor * diffuse;
	
	//Specular light
	vec3 lightToPosDirVec = normalize(lightPos0 - vs_position);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(vs_position - cameraPos);
	//reflection strength adjustment
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 5000);
	vec3 specularFinal = vec3(1.f, 1.f, 1.f) * specularConstant;

	//Attenuation

	//Final light
	//texture mix
	fs_color = texture(texture0, vs_texcoord) 
	* (vec4(ambientLight, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}
