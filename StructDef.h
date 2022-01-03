#ifndef STRUCTDEF_H
#define STRUCTDEF_H

#ifndef VULKAN_H
#define VULKAN_H
#include "vulkan.h"
#include "vk_sdk_platform.h"
#endif // !VULKAN_H

#ifndef VECT2_HPP
#define VECT2_HPP
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#endif // !VECT2_HPP



// ***********************************************
// MY HELPER TYPEDEFS AND STRUCTS FOR VULKAN WORK:
// ***********************************************

typedef VkBuffer		VkDataBuffer;
typedef VkDevice		VkLogicalDevice;
typedef VkDeviceCreateInfo	VkLogicalDeviceCreateInfo;
#define vkCreateLogicalDevice	vkCreateDevice


// holds all the information about a data buffer so it can be encapsulated in one variable:

typedef struct MyBuffer
{
	VkDataBuffer		buffer;
	VkDeviceMemory		vdm;
	VkDeviceSize		size;
} MyBuffer;


typedef struct MyTexture
{
	uint32_t			width;
	uint32_t			height;
	unsigned char* pixels;
	VkImage				texImage;
	VkImageView			texImageView;
	VkSampler			texSampler;
	VkDeviceMemory			vdm;
} MyTexture;



// bmp file headers:
struct bmfh
{
	short bfType;
	int bfSize;
	short bfReserved1;
	short bfReserved2;
	int bfOffBits;
} FileHeader;


struct bmih
{
	int biSize;
	int biWidth;
	int biHeight;
	short biPlanes;
	short biBitCount;
	int biCompression;
	int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	int biClrUsed;
	int biClrImportant;
} InfoHeader;




// *****************************
// STRUCTS FOR THIS APPLICATION:
// *****************************

// uniform variable block:

struct matBuf
{
	glm::mat4 uModelMatrix;
	glm::mat4 uViewMatrix;
	glm::mat4 uProjectionMatrix;
	int createcount;
	float angle;
	float mousposx;
	float mousposy;
	double uTime;
};


// uniform variable block:

struct lightBuf
{
	float uKa;
	float uKd;
	float uKs;
	float uShininess;
	glm::vec4 uLightPos;
	glm::vec4 uLightSpecularColor;
	glm::vec4 uEyePos;
};


// uniform variable block:

struct miscBuf
{
	float uTime;
	int   uMode;
	int   uLighting;
};


// an array of this struct will hold all vertex information:

struct vertex
{
	glm::vec3	position;
	glm::vec3	normal;
	glm::vec3	color;
	glm::vec2	texCoord;
};


#endif // !STRUCTDEF_H

