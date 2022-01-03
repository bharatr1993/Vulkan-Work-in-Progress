#version 400
#extension GL_ARB_separate_shader_objects  : enable
#extension GL_ARB_shading_language_420pack : enable

// non-opaque must be in a uniform block:

layout( std140, set = 4, binding = 0 ) uniform lightBuf
{
	float uKa;
	float uKd;
	float uKs;
	float uShininess;
	vec4  uLightPos;
	vec4  uLightSpecularColor;
	vec4  uEyePos;
} Light;

layout( std140, set = 8, binding = 0 ) uniform hBuf
{
	mat4 uModelMatrix;
	mat4 uViewMatrix;
	mat4 uProjectionMatrix;
	mat4 uNormalMatrix[2];
	int direction; //0 is right facing, 1 is left facing
} Matricestl;

layout( location = 0 ) in vec3 aVertex;
layout( location = 1 ) in vec3 aNormal;
layout( location = 2 ) in vec3 aColor;
layout( location = 3 ) in vec2 aTexCoord;
//layout( location = 4 ) in vec3 aInstanceColor;


layout ( location = 0 ) out vec3 vColor;
layout ( location = 1 ) out vec2 vTexCoord;
layout ( location = 2 ) out vec3 vN;
layout ( location = 3 ) out vec3 vL;
layout ( location = 4 ) out vec3 vE;
layout ( location = 5 ) flat out int dir;


void
main( ) 
{

	vColor  = aColor;
	vTexCoord = aTexCoord;

	vN = normalize( mat3( Matricestl.uNormalMatrix[0] ) * aNormal );
																// surface normal vector
	mat4  P = Matricestl.uProjectionMatrix;
	mat4  M = Matricestl.uModelMatrix;
	mat4  V = Matricestl.uViewMatrix;
	mat4 VM = V * M;
	mat4 PVM = P * VM;
	
	vec4 ECposition = M * vec4( aVertex, 1. );
	vec4 lightPos = vec4( Light.uLightPos.xyz, 1. );        // light source in fixed location
																// because not transformed
	vL = normalize( lightPos.xyz  -  ECposition.xyz );      // vector from the point
																// to the light

	vec4 eyePos = Light.uEyePos;
	vE = normalize( eyePos.xyz -  ECposition.xyz );          // vector from the point
																 // to the eye
	dir=Matricestl.direction;
	gl_Position = PVM * vec4( aVertex, 1. + 2.5 );


}
