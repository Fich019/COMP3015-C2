#version 460

//layout (location = 0) in vec3 VertexPosition; 
//layout (location = 1) in vec3 VertexNormal; 
//
////out vec3 Color;
//out vec3 LightIntensity;
//
//uniform vec4 LightPosition;
//uniform vec3 Kd;
//uniform vec3 Ld;
//uniform mat4 ModelViewMatrix;
//uniform mat3 NormalMatrix;
//uniform mat4 MVP;
//
//
//void main()
//{
//    vec3 n = normalize(NormalMatrix * VertexNormal);
//    vec4 p = vec4(VertexPosition, 1.0f) * ModelViewMatrix;
//    vec3 s = normalize(Light.Position - p);
//
//    LightIntensity = Kd * Ld * max(dot(s,n), 0.0);
//
//    //Color = VertexColor;
//    gl_Position = vec4(VertexPosition,1.0) * MVP;
//}

//in variables, this are in model coordinates
layout (location = 0) in vec3 VertexPosition; 
layout (location = 1) in vec3 VertexNormal; 

//out vector needed for the fragment shader
//out vec3 LightIntensity;
flat out vec3 LightIntensity;
 
 //light information struct
uniform struct LightInfo 
{
  vec4 Position; // Light position in eye coords.
  vec3 Ld;       // Diffuse light intensity
} Light;

//material information struct
uniform struct MaterialInfo 
{
  vec3 Kd;      // Diffuse reflectivity
} Material;

//uniforms for matrices required in the shader
uniform mat4 ModelViewMatrix;   //model view matrix
uniform mat3 NormalMatrix;		//normal matrix
uniform mat4 MVP;				//model view projection matrix
 



 //get position and normal to camera space
void getCamSpaceValues ( out vec3 norm, out vec3 position )
{

 norm = normalize( NormalMatrix * VertexNormal);
 position = (ModelViewMatrix * vec4(VertexPosition,1.0)).xyz;

}


void main() 
{ 
  //transfrom normal from model coordinates to view coordinates
  //vec3 n = normalize( NormalMatrix * VertexNormal);

  //transform vertex position from model coordinates to view coordinates
  //vec4 pos = ModelViewMatrix * vec4(VertexPosition,1.0);

  vec3 camNorm, camPosition;
 getCamSpaceValues(camNorm, camPosition);

  //calculate light direction, notice the light is already in the view coordinates 
  vec3 s = normalize(vec3(Light.Position) - camPosition);

  //calculate dot product for vector s and n using max. Read about max in glsl documentation, if not clear talk to me
  float sDotN = max( dot(s,camNorm), 0.0 );

  //difuse formula for light calculations
  vec3 diffuse = Light.Ld * Material.Kd * sDotN;

  //pass the colour to LightIntensity which will transfer it to the fragment shader
  LightIntensity = diffuse;

  //turns any vertex position into model view projection in preparations to 
  //graphics pipeline processes before fragment shader (clipping)
  gl_Position = MVP * vec4(VertexPosition,1.0); 

}
