#version 460

layout (location = 0) out vec4 FragColor;

uniform struct LightInfo 
{
  vec4 Position;
  vec3 Intensity;
} Light;

uniform struct MaterialInfo 
{
  vec3 Ks;
  vec3 Ka;
  vec3 Kd;
  float Shininess;
} Material;

uniform sampler2DShadow ShadowMap;

in vec3 Position;
in vec3 Normal;
in vec4 ShadowCoord;

uniform int Pass;

vec3 blinnphongModelDiffAndSpec()
{
	vec3 n = Normal;
	vec3 s = normalize(vec3(Light.Position) - Position);
	vec3 v = normalize(-Position.xyz);
	vec3 r = reflect(-s, n);

	float sDotN = max(dot(s, n), 0.0);
	vec3 diffuse = Light.Intensity * Material.Kd * sDotN;

	vec3 spec = vec3(0.0f);

	if (sDotN > 0.0)
	{
		spec = Light.Intensity * Material.Ks * pow(max(dot(r, v), 0.0), Material.Shininess);
	}

	return diffuse + spec;
}

vec3 blinnphongModelDiffAndSpecNORMAL()
{
	vec3 n = Normal;
	vec3 s = normalize(vec3(Light.Position) - Position);
	vec3 v = normalize(-Position.xyz);
	vec3 r = reflect(-s, n);

	float sDotN = max(dot(s, n), 0.0);
	vec3 diffuse = Light.Intensity * Material.Kd * sDotN;

	vec3 spec = vec3(0.0f);

	if (sDotN > 0.0)
	{
		spec = Light.Intensity * Material.Ks * pow(max(dot(r, v), 0.0), Material.Shininess);
	}

	return diffuse + spec;
}

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

subroutine(RenderPassType) void shaderWithShadows()
{
	vec3 ambient = Light.Intensity * Material.Kd;

	vec3 diffAndSpec = blinnphongModelDiffAndSpec();

//	if (Pass == 1)
//	{
//		diffAndSpec = phongModelDiffAndSpec();
//	}
//	else
//	{
//		diffAndSpec = phongModelDiffAndSpecNORMAL();
//	}


	float shadow = 1.0;
	if (ShadowCoord.z >= 0)
	{
		shadow = textureProj(ShadowMap, ShadowCoord);
	}

	FragColor = vec4(diffAndSpec * shadow + ambient, 1.0);
}

subroutine (RenderPassType) void recordDepth()
{
	//Nothing is Required Here.
}


void main()
{
	RenderPass();
}

