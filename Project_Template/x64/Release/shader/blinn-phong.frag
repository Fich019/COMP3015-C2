#version 460

//in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec3 LightDir;
in vec3 ViewDir;


//declare your uniform variables here
layout( location = 0 ) out vec4 FragColor;
layout(binding = 0) uniform sampler2D Tex1;
layout(binding = 1) uniform sampler2D NormalMapTex;

uniform int pass;

uniform struct LightInfo 
{
  vec4 Position;		//Light position in eye coords.
  vec3 La;				//Ambient light intensity
  vec3 L;				//Diffuse and specular light intensity
} Lights[3];

//material information struct
uniform struct MaterialInfo 
{
  vec3 Ka;				//Ambient reflectivity
  vec3 Kd;				//Diffuse reflectivity
  vec3 Ks;				//Specular reflectivity
  float Shininess;		//Specular shininess factor
} Material;


//blinn phong function for when the object has a texture applied to it. This will
//then apply blinn phong ontop of the texture.
vec3 blinnPhongTex( vec3 LightDir, vec3 Normal , int light) 
{
	vec3 texColor = texture(Tex1, TexCoord).rgb;


	//calculate ambient here
	vec3 ambient = Lights[light].La * texColor;


	//calculate diffuse here
	 vec3 s = normalize(vec3(Lights[light].Position) - LightDir);
	 float sDotN = max( dot(s,Normal), 0.0 );
	 vec3 diffuse = texColor * sDotN;


	//calculates the specular
	vec3 spec = vec3(0.0);
	if( sDotN > 0.0 )
	{
	//vec3 v = normalize(-position.xyz);
	vec3 h = normalize( LightDir + s );
	spec = Material.Ks * pow( max( dot(h,Normal), 0.0 ), Material.Shininess );
	}

	return ambient + Lights[light].L * (diffuse + spec);
}


//This blinn phong function is for objects that don't have textures applied to 
//them. This stops textures being applied when they shouldn't be. The difference
//in this function compared to the other is that we don't take into account the
//texture colour and instead use the material.
vec3 blinnPhong( vec3 LightDir, vec3 Normal , int light) 
{
	//calculate ambient here
	vec3 ambient = Lights[light].La * Material.Ka;

	//calculate diffuse here
	 vec3 s = normalize(vec3(Lights[light].Position) - LightDir);
	 float sDotN = max( dot(s,Normal), 0.0 );
	 vec3 diffuse = Material.Kd * sDotN;

	//calculate specular here
	 vec3 spec = vec3(0.0);
	if( sDotN > 0.0 )
	{
	//vec3 v = normalize(-LightDir.xyz);
	vec3 h = normalize( LightDir + s );
	spec = Material.Ks * pow( max( dot(h,Normal), 0.0 ), Material.Shininess );
	}

	 return ambient + Lights[light].L * (diffuse + spec);
}

//In main, I make use of the pass uniform declared at the start of the fragment shader. I have 
//two versions of pass, one for value of 1 and another for a value of 2. Each value coincidences
//with a version of the blinn phong function, one for objects with textures, one without.
void main()
{
	vec3 norm = texture(NormalMapTex, TexCoord).xyz;
	norm.xy = 2.0 * norm.xy - 1.0;


	for( int i = 0; i < 3; i++ )
		if (pass == 1) {
			FragColor += vec4(blinnPhongTex(LightDir, normalize(Normal),i) , 1);
		}
		else if(pass == 2) {
			FragColor += vec4(blinnPhong(LightDir, normalize(Normal),i) , 1);
		}



}
