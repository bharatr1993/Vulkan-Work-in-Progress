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
layout( set = 9, binding = 0 ) uniform sampler2D uSamplernorm;
layout( set = 10, binding = 0 ) uniform sampler2D uSamplerflip;

layout ( location = 0 ) in vec3 vColor;
layout ( location = 1 ) in vec2 vTexCoord;
layout ( location = 5 ) flat in int dir;

layout ( location = 0 ) out vec4 fFragColor;

void
main( )
{
	vec3 rgb;
	
	if(dir==0)
		rgb = texture( uSamplernorm, vTexCoord ).rgb;
	else
		rgb = texture( uSamplerflip, vTexCoord ).rgb;
	
	if(vec3(rgb)==vec3(0.,0.,0.))
		fFragColor=vec4(rgb,0.);
	else
		fFragColor = vec4( rgb, 1. );
}

