#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;


//out variables
out vec3 Normal;
out vec2 TexCoord;
out vec3 LightDir;
out vec3 ViewDir;

//declare your uniform variables here

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

uniform struct LightInfo 
{
  vec4 Position;		//Light position in eye coords.
  vec3 La;				//Ambient light intensity
  vec3 L;				//Diffuse and specular light intensity
} Light;


//Sets the normal, tangent and binormal for the diffuse 
//map texture. We also set the light direction and View
//direction here instead of in the fragment shader and we
//pass them through.
void main()
{



Normal = normalize( NormalMatrix * VertexNormal);

vec3 tang = normalize(NormalMatrix * vec3(VertexTangent));


vec3 binormal = normalize( cross( Normal, tang ) ) * VertexTangent.w;

mat3 toObjectLocal = mat3(
 tang.x, binormal.x, Normal.x,
 tang.y, binormal.y, Normal.y,
tang.z, binormal.z, Normal.z ) ;

vec3 pos = vec3( ModelViewMatrix * vec4(VertexPosition,1.0) );

LightDir = toObjectLocal * (Light.Position.xyz - pos);
ViewDir = toObjectLocal * normalize(-pos);

TexCoord = VertexTexCoord;

//Position = ( ModelViewMatrix * vec4(VertexPosition,1.0) ).xyz;

gl_Position = MVP * vec4(VertexPosition,1.0);

}

