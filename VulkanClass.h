#pragma once
#ifndef VULKANCLASS_H
#define VULKANCLASS_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <stdarg.h>
#include <string.h>
#include <string>
#include <stdbool.h>
#include <assert.h>
#include <signal.h>
#include <chrono> 
#include "GameEngineHeader.h"
#include <thread>

#ifndef STRUCTURES_H
#define STRUCTURES_H
#include "Structures.h"
#include <vector>
#endif // !STRUCTURES_H

#ifndef VULKAN
#define VULKAN
#include "vulkan.h"
#include "vk_sdk_platform.h"
#endif // !VULKAN

#define GLFW_INCLUDE_VULKAN
#include "glfw3.h"

#define IN
#define OUT
#define INOUT

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "SampleVertexData.cpp"

#define M_PI  3.14159265f
#define DEBUGFILE		"VulkanDebug.txt"
#define nullptr			(void *)NULL
#define MILLION			1000000L
#define BILLION			1000000000L
#define TEXTURE_COUNT		1
#define APP_SHORT_NAME		"Cube Sample"
#define APP_LONG_NAME		"Vulkan Cube Sample Program"

#define SECONDS_PER_CYCLE	3.f
#define FRAME_LAG		2
#define SWAPCHAINIMAGECOUNT	2

#define NUM_INSTANCES		1

const float ANGFACT = { M_PI / 180.f };
const float SCLFACT = { 0.005f };


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT = { 4 };
const int MIDDLE = { 2 };
const int RIGHT = { 1 };

#define PALLOCATOR		(VkAllocationCallbacks *)nullptr


// report on a result return:

#define REPORT(s)		{ PrintVkError( result, s );  fflush(FpDebug); }
#define HERE_I_AM(s)		if( Verbose )  { fprintf( FpDebug, "\n***** %s *****\n", s );  fflush(FpDebug); }


// graphics parameters:

const double FOV = glm::radians(60.);	// field-of-view angle
const float EYEDIST = 3.;			// eye distance
const float OMEGA = 2. * M_PI;		// angular velocity, radians/sec

#define SPIRV_MAGIC		0x07230203
// if you do an od -x, the magic number looks like this:
// 0000000 0203 0723 . . .

#define NUM_QUEUES_WANTED	1

#define ARRAY_SIZE(a)		(sizeof(a) / sizeof(a[0]))

// these are here for convenience and readability:
#define VK_FORMAT_VEC4		VK_FORMAT_R32G32B32A32_SFLOAT
#define VK_FORMAT_XYZW		VK_FORMAT_R32G32B32A32_SFLOAT
#define VK_FORMAT_VEC3		VK_FORMAT_R32G32B32_SFLOAT
#define VK_FORMAT_STP		VK_FORMAT_R32G32B32_SFLOAT
#define VK_FORMAT_XYZ		VK_FORMAT_R32G32B32_SFLOAT
#define VK_FORMAT_VEC2		VK_FORMAT_R32G32_SFLOAT
#define VK_FORMAT_ST		VK_FORMAT_R32G32_SFLOAT
#define VK_FORMAT_XY		VK_FORMAT_R32G32_SFLOAT
#define VK_FORMAT_FLOAT		VK_FORMAT_R32_SFLOAT
#define VK_FORMAT_S		VK_FORMAT_R32_SFLOAT
#define VK_FORMAT_X		VK_FORMAT_R32_SFLOAT

// my own error codes:
#define VK_FAILURE		(VkResult)( -2000000000 )
#define VK_SHOULD_EXIT		(VkResult)( -2000000001 )
#define VK_ERROR_SOMETHING_ELSE	(VkResult)( -2000000002 )



// ***********************************************
// MY HELPER TYPEDEFS AND STRUCTS FOR VULKAN WORK:
// ***********************************************

typedef VkBuffer		VkDataBuffer;
typedef VkDevice		VkLogicalDevice;
typedef VkDeviceCreateInfo	VkLogicalDeviceCreateInfo;
#define vkCreateLogicalDevice	vkCreateDevice

//Global Variables

bool				NeedToExit;			// true means the program should exit
int				NumRenders;			// how many times the render loop has been called
bool				Paused;				// true means don't animate
float				Scale;				// scaling factor
double				Time;
bool				Verbose;			// true = write messages into a file
int				Xmouse, Ymouse;			// mouse values
float				Xrot, Yrot;			// rotation angles in degrees
bool				UseIndexBuffer;			// true = use both vertex and index buffer, false = just use vertex buffer
bool				UseLighting;			// true = use lighting for display
bool				UseRotate;			// true = rotate-animate, false = use mouse for interaction
int				Mode;				// 0 = use colors, 1 = use textures, ...
FILE*           FpDebug;			// where to send debugging messages
int				ActiveButton;			// current button that is down
double           Mosx, Mosy, offsetX, offsetY;
std::string     Keypress="";
double fElapsedTime=0;
bool sjaudio=false;

class Vulkan: public GameEngine {
private:
	//Vulkan Variables
	VkCommandBuffer			CommandBuffers[2];			// 2, because of double-buffering
	VkPipeline			ComputePipeline;
	VkPipelineCache			ComputePipelineCache;
	VkPipelineLayout		ComputePipelineLayout;
	VkDataBuffer 			DataBuffer;
	VkImage				DepthStencilImage;
	VkImageView			DepthStencilImageView;
	VkDescriptorPool		DescriptorPool;
	VkDescriptorSetLayout		DescriptorSetLayouts[21];
	VkDescriptorSet			DescriptorSets[21];
	VkDebugReportCallbackEXT	ErrorCallback = VK_NULL_HANDLE;
	VkEvent				Event;
	VkFence				Fence;
	VkFramebuffer			Framebuffers[2];
	VkCommandPool			GraphicsCommandPool;
	VkPipeline			GraphicsPipeline[12];
	VkPipelineCache			GraphicsPipelineCache;
	VkPipelineLayout		GraphicsPipelineLayout;

public:
	uint32_t			Height;
private:

	VkInstance			Instance;
	VkExtensionProperties* InstanceExtensions;
	VkLayerProperties* InstanceLayers;
	VkLogicalDevice			LogicalDevice;
public:
	GLFWwindow* MainWindow;
private:
	VkPhysicalDevice		PhysicalDevice;
	VkPhysicalDeviceProperties	PhysicalDeviceProperties;
	uint32_t			PhysicalDeviceCount;
	VkPhysicalDeviceFeatures	PhysicalDeviceFeatures;
	VkImage* PresentImages;
	VkImageView* PresentImageViews;	// the swap chain image views
	VkQueue				Queue;
	VkRect2D			RenderArea;
	VkRenderPass			RenderPass;
	VkSemaphore			SemaphoreImageAvailable;
	VkSemaphore			SemaphoreRenderFinished;
	VkShaderModule			ShaderModuleFragment[13];
	VkShaderModule			ShaderModuleVertex[13];
	VkBuffer			StagingBuffer;
	VkDeviceMemory			StagingBufferMemory;
	VkSurfaceKHR			Surface;
	VkSwapchainKHR			SwapChain;
	VkCommandBuffer			TextureCommandBuffer;	// used for transfering texture from staging buffer to actual texture buffer
	VkImage				TextureImage;
	VkDeviceMemory			TextureImageMemory;
	VkCommandPool			TransferCommandPool;
	VkDebugReportCallbackEXT	WarningCallback;
public:
	uint32_t			Width;
private:
	//Function Varaiables
	struct lightBuf			Light;				// cpu struct to hold light information
	struct matBuf			Matricestl,Matricesbl, Matricestr, Matricesbr, Matricesbulpos, Matricesbulposflip;			// cpu struct to hold matrix information
	struct hbuf				HeroPos;			// cpu struct to hold matrix information
	struct miscBuf			Misc;				// cpu struct to hold miscellaneous information information
	struct misclocbuf		MiscLocation;

	MyBuffer			MyLightUniformBuffer;
	MyTexture			Background;			// the cute puppy texture struct
	MyTexture			Bug,BugF;			// the not so cute dragon ball struct
	MyTexture			Hero,HeroF;			// Hero Texture
	MyTexture			Bullet, BulletF;	// Bullet Texture
	MyTexture			Siren, Fan;			// Misc obj Texture
	MyTexture			StartScreen,StartButton, Gameoverscreen;		// Hero Texture
	MyBuffer			MyMatrixUniformBuffertopleft, MyMatrixUniformBufferbottomleft, MyMatrixUniformBuffertopright, MyMatrixUniformBufferbottomright, MyMatrixUniformBufferBulPos, MyMatrixUniformBufferBulPosFlip;
	MyBuffer			MyVertexDataBuffer;
	MyBuffer			MyJustIndexDataBuffer;
	MyBuffer			MyJustVertexDataBuffer;
	MyBuffer            HeroPosUniformBuffer;
	MyBuffer            MiscLocUniformBuffer;

	std::vector<Object> scobj;
	Object object;
	float withinWindowCursorPosX, withinWindowCursorPosY;

public:
	VkResult			DestroyAllVulkan();

	//VkBool32			ErrorCallback( VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t, const char *, const char *, void * );

	int				FindMemoryThatIsDeviceLocal(uint32_t);
	int				FindMemoryThatIsHostVisible(uint32_t);
	int				FindMemoryByFlagAndType(VkMemoryPropertyFlagBits, uint32_t);

	int				FindQueueFamilyThatDoesGraphics();
	int				FindQueueFamilyThatDoesCompute();
	int				FindQueueFamilyThatDoesTransfer();

	void				InitGraphics();

	VkResult			Init01Instance();

	VkResult			Init03PhysicalDeviceAndGetQueueFamilyProperties();

	VkResult			Init04LogicalDeviceAndQueue();

	VkResult			Init05DataBuffer(VkDeviceSize, VkBufferUsageFlags, OUT MyBuffer*);
	VkResult			Init05UniformBuffer(VkDeviceSize, OUT MyBuffer*);
	VkResult			Init05MyIndexDataBuffer(VkDeviceSize, OUT MyBuffer*);
	VkResult			Init05MyVertexDataBuffer(VkDeviceSize, OUT MyBuffer*);
	VkResult			Fill05DataBuffer(IN MyBuffer, IN void*);

	VkResult			Init06CommandPools();
	VkResult			Init06CommandBuffers();

	VkResult			Init07TextureSampler(OUT MyTexture*);
	VkResult			Init07TextureBuffer(INOUT MyTexture*);

	VkResult			Init07TextureBufferAndFillFromBmpFile(IN std::string, OUT MyTexture*);

	VkResult			Init08Swapchain();

	VkResult			Init09DepthStencilImage();

	VkResult			Init10RenderPasses();

	VkResult			Init11Framebuffers();

	VkResult			Init12SpirvShader(std::string, OUT VkShaderModule*);

	VkResult			Init13DescriptorSetPool();
	VkResult			Init13DescriptorSetLayouts();
	VkResult			Init13DescriptorSets();

	VkResult			Init14GraphicsPipelineLayout();
	VkResult			Init14GraphicsVertexFragmentPipeline(VkShaderModule[], VkShaderModule[], VkPrimitiveTopology, OUT VkPipeline[]);
	VkResult			Init14ComputePipeline(VkShaderModule, OUT VkPipeline*);
	VkResult            InitFence();
	void                PrintVkError(VkResult result, std::string prefix);
	VkResult            InitSemaphore();

	VkResult			RenderScene();
	void				UpdateScene();
	//VkBool32			WarningCallback( VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t, const char *, const char *, void * );
	void				Reset();
	
	void                InitGLFW();
	void                InitGLFWSurface();

	void                Finish();

	void                CursorPosNorm();
};


#endif // !VULKANCLASS_H
