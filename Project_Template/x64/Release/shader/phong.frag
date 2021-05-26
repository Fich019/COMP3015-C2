#version 460

//in variable that receives the diffuse calculation from the vertex shader
in vec3 position;
in vec3 normal;
//out variable, this typical for all fragment shaders
layout (location = 0) out vec4 FragColor;
 
 //light information struct
uniform struct LightInfo 
{
  vec4 Position;		//Light position in eye coords.
  vec3 La;				//Ambient light intensity
  vec3 L;				//Diffuse and specular light intensity
} Lights;

//material information struct
uniform struct MaterialInfo 
{
  vec3 Ka;				//Ambient reflectivity
  vec3 Kd;				//Diffuse reflectivity
  vec3 Ks;				//Specular reflectivity
  float Shininess;		//Specular shininess factor
} Material;



 vec3 phongModel(vec3 pos, vec3 n)
 {
	//calculate ambient here
	vec3 ambient = Lights.La * Material.Ka;

	//calculate diffuse here
	 vec3 s = normalize(vec3(Lights.Position) - pos);
	 float sDotN = max( dot(s,n), 0.0 );
	 vec3 diffuse = Material.Kd * sDotN;

	//calculate specular here
	 vec3 spec = vec3(0.0);
	if( sDotN > 0.0 )
	{
	 vec3 v = normalize(-position.xyz);
	 vec3 r = reflect( -s, n );
	 spec = Material.Ks * pow( max( dot(r,v), 0.0 ),
	 Material.Shininess );
	}

	 return ambient + Lights.L * (diffuse + spec);
 }
void main()
{
    //we pass LightInyensity to outr FragColor, notice the difference between vector types
    // vec3 and vec4 and how we solved the problem



	FragColor = vec4(phongModel(position, normal), 1.0);
}
