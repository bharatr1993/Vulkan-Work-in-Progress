#version 400
#extension GL_ARB_separate_shader_objects  : enable
#extension GL_ARB_shading_language_420pack : enable


layout( std140, set = 4, binding = 0 ) uniform lightBuf
{
	float uKa;
	float uKd;
	float uKs;
	float uShininess;
	vec4 uLightPos;
	vec4 uLightSpecularColor;
	vec4 uEyePos;
} Light;

// opaque must be outside of a uniform block:
// also, can't specify packing
layout( set = 6, binding = 0 ) uniform sampler2D uSampler;

layout ( location = 0 ) in vec3 vColor;
layout ( location = 1 ) in vec2 vTexCoord;
layout ( location = 2 ) in vec3 vN;
layout ( location = 3 ) in vec3 vL;
layout ( location = 4 ) in vec3 vE;

layout ( location = 0 ) out vec4 fFragColor;

void
main( )
{
	vec3 rgb;
	
	rgb = texture( uSampler, vTexCoord ).rgb;
	
	fFragColor = vec4( rgb, 1. );
	
}
