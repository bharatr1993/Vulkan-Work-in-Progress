#pragma once

#ifndef VULKAN
#define VULKAN
#include "vulkan.h"
#include "vk_sdk_platform.h"
typedef VkBuffer		VkDataBuffer;
typedef VkDevice		VkLogicalDevice;
typedef VkDeviceCreateInfo	VkLogicalDeviceCreateInfo;

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#endif

#include <string>

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

struct Object{
	float x, y;
	float angle;
};

struct BulletObj {
	float x, y;
	int poscount;
	float angle;
};

struct matBuf
{
	glm::mat4 uModelMatrix[100];
	glm::mat4 uViewMatrix;
	glm::mat4 uProjectionMatrix;
	glm::mat4 uNormalMatrix[2];
	int count = 0;
};

struct hbuf
{
	glm::mat4 uModelMatrix;
	glm::mat4 uViewMatrix;
	glm::mat4 uProjectionMatrix;
	glm::mat4 uNormalMatrix[2];
	int direction=0; //0 is right facing, 1 is left facing
};

struct misclocbuf
{
	glm::mat4 uModelMatrix[10];
	glm::mat4 uViewMatrix;
	glm::mat4 uProjectionMatrix;
	glm::mat4 uNormalMatrix[2];
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

struct errorcode
{
	VkResult	resultCode;
	std::string	meaning;
}
ErrorCodes[] =
{
	{ VK_NOT_READY,				"Not Ready"			},
	{ VK_TIMEOUT,				"Timeout"			},
	{ VK_EVENT_SET,				"Event Set"			},
	{ VK_EVENT_RESET,			"Event Reset"			},
	{ VK_INCOMPLETE,			"Incomplete"			},
	{ VK_ERROR_OUT_OF_HOST_MEMORY,		"Out of Host Memory"		},
	{ VK_ERROR_OUT_OF_DEVICE_MEMORY,	"Out of Device Memory"		},
	{ VK_ERROR_INITIALIZATION_FAILED,	"Initialization Failed"		},
	{ VK_ERROR_DEVICE_LOST,			"Device Lost"			},
	{ VK_ERROR_MEMORY_MAP_FAILED,		"Memory Map Failed"		},
	{ VK_ERROR_LAYER_NOT_PRESENT,		"Layer Not Present"		},
	{ VK_ERROR_EXTENSION_NOT_PRESENT,	"Extension Not Present"		},
	{ VK_ERROR_FEATURE_NOT_PRESENT,		"Feature Not Present"		},
	{ VK_ERROR_INCOMPATIBLE_DRIVER,		"Incompatible Driver"		},
	{ VK_ERROR_TOO_MANY_OBJECTS,		"Too Many Objects"		},
	{ VK_ERROR_FORMAT_NOT_SUPPORTED,	"Format Not Supported"		},
	{ VK_ERROR_FRAGMENTED_POOL,		"Fragmented Pool"		},
	{ VK_ERROR_SURFACE_LOST_KHR,		"Surface Lost"			},
	{ VK_ERROR_NATIVE_WINDOW_IN_USE_KHR,	"Native Window in Use"		},
	{ VK_SUBOPTIMAL_KHR,			"Suboptimal"			},
	{ VK_ERROR_OUT_OF_DATE_KHR,		"Error Out of Date"		},
	{ VK_ERROR_INCOMPATIBLE_DISPLAY_KHR,	"Incompatible Display"		},
	{ VK_ERROR_VALIDATION_FAILED_EXT,	"Valuidation Failed"		},
	{ VK_ERROR_INVALID_SHADER_NV,		"Invalid Shader"		},
	{ VK_ERROR_OUT_OF_POOL_MEMORY_KHR,	"Out of Pool Memory"		},
	{ VK_ERROR_INVALID_EXTERNAL_HANDLE,	"Invalid External Handle"	},
};
