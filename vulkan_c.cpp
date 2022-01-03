/*

This is code is not fully mine!!!

A huge shout out of thanks to Mike Bailey of CS Oregon State University for teaching me and providing a code framework. I was his SIGGRAPH Student

However, the same has been extensively modified by me to make a "GAME"!!!

*/

#include "VulkanClass.h"
//#include <iostream>

Vulkan vulkan;

#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"vulkan-1.lib")

void taskaudiobckg(bool cont)
{
	GameEngine::Sounds_BCKG("Bug.ogg");
}

int
ReadInt(FILE* fp)
{
	unsigned char b3, b2, b1, b0;
	b0 = fgetc(fp);
	b1 = fgetc(fp);
	b2 = fgetc(fp);
	b3 = fgetc(fp);
	return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
}


short
ReadShort(FILE* fp)
{
	unsigned char b1, b0;
	b0 = fgetc(fp);
	b1 = fgetc(fp);
	return (b1 << 8) | b0;
}

void GLFWErrorCallback(int error, const char* description)
{
	//Do Nothing
}

void GLFWKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case 'i':
		case 'I':
			UseIndexBuffer = !UseIndexBuffer;
			break;

		case 'l':
		case 'L':
			UseLighting = !UseLighting;
			break;

		case 'm':
		case 'M':
			Mode++;
			if (Mode >= 2)
				Mode = 0;
			break;

		case 'p':
		case 'P':
			Paused = !Paused;
			break;

		case 'q':
		case 'Q':
		case GLFW_KEY_ESCAPE:
			NeedToExit = true;
			break;

		case 'r':
		case 'R':
			UseRotate = !UseRotate;
			break;

		case 'v':
		case 'V':
			Verbose = !Verbose;
			break;

		default:
			fprintf(FpDebug, "Unknown key hit: 0x%04x = '%c'\n", key, key);
			fprintf(stderr, "Unknown key hit: 0x%04x = '%c'\n", key, key);
			fflush(FpDebug);
		}
	}
	*/
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case 'w':
		case 'W':
			Keypress = "W";
			break;

		case 'a':
		case 'A':
			Keypress = "A";
			break;

		case 's':
		case 'S':
			Keypress = "S";
			break;

		case 'd':
		case 'D':
			Keypress = "D";
			break;

		case 'q':
		case 'Q':
			Keypress = "P";
			break;

		case 'r':
		case 'R':
			Keypress = "R";
			break;

		case 'v':
		case 'V':
			Keypress = "V";
			break;

		case GLFW_KEY_SPACE:
			Keypress = "sp";
			break;

		default:
			fprintf(FpDebug, "Unknown key hit: 0x%04x = '%c'\n", key, key);
			fprintf(stderr, "Unknown key hit: 0x%04x = '%c'\n", key, key);
			fflush(FpDebug);

			Keypress = "";
		}
	}

	else if (action == GLFW_RELEASE)
		Keypress = "";
}

void GLFWMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (Verbose)		fprintf(FpDebug, "Mouse button = %d; Action = %d\n", button, action);

	int b = 0;		// LEFT, MIDDLE, or RIGHT

	// get the proper button bit mask:

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		b = LEFT;		break;

	case GLFW_MOUSE_BUTTON_MIDDLE:
		b = MIDDLE;		break;

	case GLFW_MOUSE_BUTTON_RIGHT:
		b = RIGHT;		break;

	default:
		b = 0;
		fprintf(FpDebug, "Unknown mouse button: %d\n", button);
	}


	// button down sets the bit, up clears the bit:

	if (action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		Xmouse = (int)xpos;
		Ymouse = (int)ypos;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}

void GLFWMouseMotion(GLFWwindow* window, double xpos, double ypos)
{
	glfwGetCursorPos(window, &Mosx, &Mosy);

	int dx = (int)xpos - Xmouse;		// change in mouse coords
	int dy = (int)ypos - Ymouse;

	if ((ActiveButton & LEFT) != 0)
	{
		Xrot += (ANGFACT * dy);
		Yrot += (ANGFACT * dx);
	}


	if ((ActiveButton & MIDDLE) != 0)
	{
		Scale += SCLFACT * (float)(dx - dy);

		// keep object from turning inside-out or disappearing:

		if (Scale < MINSCALE)
			Scale = MINSCALE;
	}

	Xmouse = (int)xpos;			// new current position
	Ymouse = (int)ypos;
}

void Vulkan::InitGraphics()
{
	VkResult result = VK_SUCCESS;

	Vulkan::InitGLFW();

	Vulkan::Init01Instance();

	Vulkan::InitGLFWSurface();

	Vulkan::Init03PhysicalDeviceAndGetQueueFamilyProperties();

	Vulkan::Init04LogicalDeviceAndQueue();

	Vulkan::Init05UniformBuffer(sizeof(Matricestl), &MyMatrixUniformBuffertopleft);
	Vulkan::Fill05DataBuffer(MyMatrixUniformBuffertopleft, (void*)&Matricestl);

	Vulkan::Init05UniformBuffer(sizeof(Matricesbl), &MyMatrixUniformBufferbottomleft);
	Vulkan::Fill05DataBuffer(MyMatrixUniformBufferbottomleft, (void*)&Matricesbl);

	Vulkan::Init05UniformBuffer(sizeof(Matricestr), &MyMatrixUniformBuffertopright);
	Vulkan::Fill05DataBuffer(MyMatrixUniformBuffertopright, (void*)&Matricestr);

	Vulkan::Init05UniformBuffer(sizeof(Matricesbr), &MyMatrixUniformBufferbottomright);
	Vulkan::Fill05DataBuffer(MyMatrixUniformBufferbottomright, (void*)&Matricesbr);

	Vulkan::Init05UniformBuffer(sizeof(Light), &MyLightUniformBuffer);
	Vulkan::Fill05DataBuffer(MyLightUniformBuffer, (void*)&Light);

	Vulkan::Init05UniformBuffer(sizeof(Matricesbulposflip), &MyMatrixUniformBufferBulPosFlip);
	Vulkan::Fill05DataBuffer(MyMatrixUniformBufferBulPosFlip, (void*)&Matricesbulposflip);

	Vulkan::Init05UniformBuffer(sizeof(MiscLocation), &MiscLocUniformBuffer);
	Vulkan::Fill05DataBuffer(MiscLocUniformBuffer, (void*)&MiscLocation);

	Vulkan::Init05UniformBuffer(sizeof(Matricesbulpos), &MyMatrixUniformBufferBulPos);
	Vulkan::Fill05DataBuffer(MyMatrixUniformBufferBulPos , (void*)&Matricesbulpos);

	Vulkan::Init05UniformBuffer(sizeof(HeroPos), &HeroPosUniformBuffer);
	Vulkan::Fill05DataBuffer(HeroPosUniformBuffer, (void*)&HeroPos);

	Vulkan::Init05MyVertexDataBuffer(sizeof(VertexData), &MyVertexDataBuffer);
	Vulkan::Fill05DataBuffer(MyVertexDataBuffer, (void*)VertexData);

	Vulkan::Init05MyVertexDataBuffer(sizeof(JustVertexData), &MyJustVertexDataBuffer);
	Vulkan::Fill05DataBuffer(MyJustVertexDataBuffer, (void*)JustVertexData);

	Vulkan::Init05MyIndexDataBuffer(sizeof(JustIndexData), &MyJustIndexDataBuffer);
	Vulkan::Fill05DataBuffer(MyJustIndexDataBuffer, (void*)JustIndexData);

	Vulkan::Init06CommandPools();
	Vulkan::Init06CommandBuffers();

	Vulkan::Init07TextureSampler(&Background);
	Vulkan::Init07TextureBufferAndFillFromBmpFile("Bug_GJ_BG.bmp", &Background);

	Vulkan::Init07TextureSampler(&Bug);
	Vulkan::Init07TextureBufferAndFillFromBmpFile("Bug_GJ_Bug.bmp", &Bug);

	Vulkan::Init07TextureSampler(&BugF);
	Vulkan::Init07TextureBufferAndFillFromBmpFile("Bug_GJ_BugFlip.bmp", &BugF);

	Vulkan::Init07TextureSampler(&Hero);
	Vulkan::Init07TextureBufferAndFillFromBmpFile("Bug_GJ_CharFlip.bmp", &Hero);

	Vulkan::Init07TextureSampler(&HeroF);
	Vulkan::Init07TextureBufferAndFillFromBmpFile("Bug_GJ_Char.bmp", &HeroF);

	Vulkan::Init07TextureSampler(&Bullet);
	Vulkan::Init07TextureBufferAndFillFromBmpFile("Bug_GJ_ProjectileFlip.bmp", &Bullet); //Change image, Left to right

	Vulkan::Init07TextureSampler(&BulletF);
	Vulkan::Init07TextureBufferAndFillFromBmpFile("Bug_GJ_Projectile.bmp", &BulletF); //Right to left

	Vulkan::Init07TextureSampler(&Siren);
	Vulkan::Init07TextureBufferAndFillFromBmpFile("Bug_GJ_Siren.bmp", &Siren);

	Vulkan::Init07TextureSampler(&Fan);
	Vulkan::Init07TextureBufferAndFillFromBmpFile("Bug_GJ_GPU_Fan.bmp", &Fan);

	Vulkan::Init07TextureSampler(&StartScreen);
	Vulkan::Init07TextureBufferAndFillFromBmpFile("Bug_GJ_Char.bmp", &StartScreen); //Change!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	Vulkan::Init07TextureSampler(&StartButton);
	Vulkan::Init07TextureBufferAndFillFromBmpFile("Bug_GJ_CharFlip.bmp", &StartButton); //Change!!!!!!!!!!!!!!!!!!!!!!!!!!!

	Vulkan::Init07TextureSampler(&Gameoverscreen);
	Vulkan::Init07TextureBufferAndFillFromBmpFile("Bug_GJ_Char.bmp", &Gameoverscreen); //Change!!!!!!!!!!!!!!!!!!!!!!!!!!!

	Vulkan::Init08Swapchain();

	Vulkan::Init09DepthStencilImage();

	Vulkan::Init10RenderPasses();

	Vulkan::Init11Framebuffers();

	Vulkan::Init12SpirvShader("myverttl.spv", &ShaderModuleVertex[0]);
	Vulkan::Init12SpirvShader("myfragtl.spv", &ShaderModuleFragment[0]);
	Vulkan::Init12SpirvShader("myvert1.spv", &ShaderModuleVertex[1]);
	Vulkan::Init12SpirvShader("myfrag1.spv", &ShaderModuleFragment[1]);
	Vulkan::Init12SpirvShader("myvertbl.spv", &ShaderModuleVertex[2]);
	Vulkan::Init12SpirvShader("myfragbl.spv", &ShaderModuleFragment[2]);
	Vulkan::Init12SpirvShader("myverttr.spv", &ShaderModuleVertex[3]);
	Vulkan::Init12SpirvShader("myfragtr.spv", &ShaderModuleFragment[3]);
	Vulkan::Init12SpirvShader("myvertbr.spv", &ShaderModuleVertex[4]);
	Vulkan::Init12SpirvShader("myfragbr.spv", &ShaderModuleFragment[4]);
	Vulkan::Init12SpirvShader("myverth.spv", &ShaderModuleVertex[5]);
	Vulkan::Init12SpirvShader("myfragh.spv", &ShaderModuleFragment[5]);
	Vulkan::Init12SpirvShader("myvertsir.spv", &ShaderModuleVertex[6]);
	Vulkan::Init12SpirvShader("myfragsir.spv", &ShaderModuleFragment[6]);
	Vulkan::Init12SpirvShader("myvertfan.spv", &ShaderModuleVertex[7]);
	Vulkan::Init12SpirvShader("myfragfan.spv", &ShaderModuleFragment[7]);
	Vulkan::Init12SpirvShader("myvertbul.spv", &ShaderModuleVertex[8]);
	Vulkan::Init12SpirvShader("myfragbul.spv", &ShaderModuleFragment[8]);
	Vulkan::Init12SpirvShader("myvertstarts.spv", &ShaderModuleVertex[9]);
	Vulkan::Init12SpirvShader("myfragstarts.spv", &ShaderModuleFragment[9]);
	Vulkan::Init12SpirvShader("myvertstartbut.spv", &ShaderModuleVertex[10]);
	Vulkan::Init12SpirvShader("myfragstartbut.spv", &ShaderModuleFragment[10]);
	Vulkan::Init12SpirvShader("myvertgovers.spv", &ShaderModuleVertex[11]);
	Vulkan::Init12SpirvShader("myfraggovers.spv", &ShaderModuleFragment[11]);
	//Vulkan::Init12SpirvShader("myvertbulf.spv", &ShaderModuleVertex[12]); Uncomment Later!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//Vulkan::Init12SpirvShader("myfragbulf.spv", &ShaderModuleFragment[12]); Uncomment Later!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	Vulkan::Init13DescriptorSetPool();
	Vulkan::Init13DescriptorSetLayouts();
	Vulkan::Init13DescriptorSets();

	Vulkan::Init14GraphicsVertexFragmentPipeline(ShaderModuleVertex, ShaderModuleFragment, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, GraphicsPipeline);
}

VkResult Vulkan::Init01Instance()
{
	VkResult result = VK_SUCCESS;

	VkApplicationInfo vai;
	vai.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vai.pNext = nullptr;
	vai.pApplicationName = "Vulkan Sample";
	vai.applicationVersion = 100;
	vai.pEngineName = "";
	vai.engineVersion = 1;
	vai.apiVersion = VK_MAKE_VERSION(1, 0, 0);


	// figure out what instance layers are wanted and available:

	std::vector<char*> instanceLayersWantedAndAvailable;

	{
		// these are the instance layers we would like to have:

		const char* instanceLayersWanted[] =
		{
			//"VK_LAYER_LUNARG_api_dump",			// turn this on if want to see each function call and its arguments (very slow!)
			"VK_LAYER_LUNARG_core_validation",
			"VK_LAYER_LUNARG_object_tracker",
			"VK_LAYER_LUNARG_parameter_validation",
			"VK_LAYER_NV_optimus"
		};
		uint32_t numLayersWanted = sizeof(instanceLayersWanted) / sizeof(char*);

		fprintf(FpDebug, "\n%d Instance Layers originally wanted:\n", numLayersWanted);
		for (unsigned int i = 0; i < numLayersWanted; i++)
		{
			fprintf(FpDebug, "\t%s\n", instanceLayersWanted[i]);
		}

		// see what layers are actually available:

		uint32_t numLayersAvailable;
		vkEnumerateInstanceLayerProperties(&numLayersAvailable, (VkLayerProperties*)nullptr);
		InstanceLayers = new VkLayerProperties[numLayersAvailable];
		result = vkEnumerateInstanceLayerProperties(&numLayersAvailable, InstanceLayers);
		REPORT("vkEnumerateInstanceLayerProperties");
		if (result != VK_SUCCESS)
		{
			return result;
		}

		fprintf(FpDebug, "\n%d Instance Layers actually available:\n", numLayersAvailable);
		for (unsigned int i = 0; i < numLayersAvailable; i++)
		{
			fprintf(FpDebug, "0x%08x  %2d  '%s'  '%s'\n",
				InstanceLayers[i].specVersion,
				InstanceLayers[i].implementationVersion,
				InstanceLayers[i].layerName,
				InstanceLayers[i].description);
		}

		instanceLayersWantedAndAvailable.clear();
		for (uint32_t wanted = 0; wanted < numLayersWanted; wanted++)
		{
			for (uint32_t available = 0; available < numLayersAvailable; available++)
			{
				if (strcmp(instanceLayersWanted[wanted], InstanceLayers[available].layerName) == 0)
				{
					instanceLayersWantedAndAvailable.push_back(InstanceLayers[available].layerName);
					break;
				}
			}
		}

		fprintf(FpDebug, "\nWill now ask for %d Instance Layers:\n", instanceLayersWantedAndAvailable.size());
		for (uint32_t i = 0; i < instanceLayersWantedAndAvailable.size(); i++)
		{
			fprintf(FpDebug, "\t%s\n", instanceLayersWantedAndAvailable[i]);
		}
	}


	std::vector<char*> extensionsWantedAndAvailable;

	{
		// figure out what instance extensions are wanted and available:

		const char* instanceExtensionsWanted[] =
		{
			"VK_KHR_surface",
#ifdef _WIN32
			"VK_KHR_win32_surface",
#endif
			"VK_EXT_debug_report",
		};
		uint32_t numExtensionsWanted = sizeof(instanceExtensionsWanted) / sizeof(char*);

		fprintf(FpDebug, "\n%d Instance Extensions originally wanted:\n", numExtensionsWanted);
		for (unsigned int i = 0; i < numExtensionsWanted; i++)
		{
			fprintf(FpDebug, "\t%s\n", instanceExtensionsWanted[i]);
		}


		// see what extensions are actually available:

		uint32_t numExtensionsAvailable;
		vkEnumerateInstanceExtensionProperties((char*)nullptr, &numExtensionsAvailable, (VkExtensionProperties*)nullptr);
		InstanceExtensions = new VkExtensionProperties[numExtensionsAvailable];
		result = vkEnumerateInstanceExtensionProperties((char*)nullptr, &numExtensionsAvailable, InstanceExtensions);
		REPORT("vkEnumerateInstanceExtensionProperties");
		if (result != VK_SUCCESS)
		{
			return result;
		}

		fprintf(FpDebug, "\n%d Instance Extensions actually available:\n", numExtensionsAvailable);
		for (unsigned int i = 0; i < numExtensionsAvailable; i++)
		{
			fprintf(FpDebug, "0x%08x  '%s'\n",
				InstanceExtensions[i].specVersion,
				InstanceExtensions[i].extensionName);
		}

		// look for extensions both on the wanted list and the available list:

		extensionsWantedAndAvailable.clear();
		for (uint32_t wanted = 0; wanted < numExtensionsWanted; wanted++)
		{
			for (uint32_t available = 0; available < numExtensionsAvailable; available++)
			{
				if (strcmp(instanceExtensionsWanted[wanted], InstanceExtensions[available].extensionName) == 0)
				{
					extensionsWantedAndAvailable.push_back(InstanceExtensions[available].extensionName);
					break;
				}
			}
		}

		fprintf(FpDebug, "\nWill now ask for %d Instance Extensions\n", extensionsWantedAndAvailable.size());
		for (uint32_t i = 0; i < extensionsWantedAndAvailable.size(); i++)
		{
			fprintf(FpDebug, "\t%s\n", extensionsWantedAndAvailable[i]);
		}
	}


	// create the instance, asking for the layers and extensions:

	VkInstanceCreateInfo vici;
	vici.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vici.pNext = nullptr;
	vici.flags = 0;
	vici.pApplicationInfo = &vai;
	vici.enabledLayerCount = instanceLayersWantedAndAvailable.size();
	vici.ppEnabledLayerNames = instanceLayersWantedAndAvailable.data();
	vici.enabledExtensionCount = extensionsWantedAndAvailable.size();
	vici.ppEnabledExtensionNames = extensionsWantedAndAvailable.data();


	result = vkCreateInstance(IN & vici, PALLOCATOR, OUT & Instance);
	REPORT("vkCreateInstance");
	return result;
}

VkResult Vulkan::Init03PhysicalDeviceAndGetQueueFamilyProperties()
{
	VkResult result = VK_SUCCESS;

	result = vkEnumeratePhysicalDevices(Instance, OUT & PhysicalDeviceCount, (VkPhysicalDevice*)nullptr);
	REPORT("vkEnumeratePhysicalDevices - 1");
	if (result != VK_SUCCESS || PhysicalDeviceCount <= 0)
	{
		fprintf(FpDebug, "Could not count the physical devices\n");
		return VK_SHOULD_EXIT;
	}

	fprintf(FpDebug, "\n%d physical devices found.\n", PhysicalDeviceCount);

	VkPhysicalDevice* physicalDevices = new VkPhysicalDevice[PhysicalDeviceCount];
	result = vkEnumeratePhysicalDevices(Instance, OUT & PhysicalDeviceCount, OUT physicalDevices);
	REPORT("vkEnumeratePhysicalDevices - 2");
	if (result != VK_SUCCESS)
	{
		fprintf(FpDebug, "Could not enumerate the %d physical devices\n", PhysicalDeviceCount);
		return VK_SHOULD_EXIT;
	}

	int discreteSelect = -1;
	int integratedSelect = -1;
	for (unsigned int i = 0; i < PhysicalDeviceCount; i++)
	{
		VkPhysicalDeviceProperties vpdp;
		vkGetPhysicalDeviceProperties(IN physicalDevices[i], OUT & vpdp);
		if (result != VK_SUCCESS)
		{
			fprintf(FpDebug, "Could not get the physical device properties of device %d\n", i);
			return VK_SHOULD_EXIT;
		}

		fprintf(FpDebug, " \n\nDevice %2d:\n", i);
		fprintf(FpDebug, "\tAPI version: %d\n", vpdp.apiVersion);
		fprintf(FpDebug, "\tDriver version: %d\n", vpdp.apiVersion);
		fprintf(FpDebug, "\tVendor ID: 0x%04x\n", vpdp.vendorID);
		fprintf(FpDebug, "\tDevice ID: 0x%04x\n", vpdp.deviceID);
		fprintf(FpDebug, "\tPhysical Device Type: %d =", vpdp.deviceType);
		if (vpdp.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)	fprintf(FpDebug, " (Discrete GPU)\n");
		if (vpdp.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)	fprintf(FpDebug, " (Integrated GPU)\n");
		if (vpdp.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)	fprintf(FpDebug, " (Virtual GPU)\n");
		if (vpdp.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)		fprintf(FpDebug, " (CPU)\n");
		fprintf(FpDebug, "\tDevice Name: %s\n", vpdp.deviceName);
		fprintf(FpDebug, "\tPipeline Cache Size: %d\n", vpdp.pipelineCacheUUID[0]);
		//fprintf( FpDebug, "?", vpdp.limits );
		//fprintf( FpDebug, "?", vpdp.sparseProperties );

		// need some logical here to decide which physical device to select:

		if (vpdp.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			discreteSelect = i;

		if (vpdp.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
			integratedSelect = i;
	}

	int which = -1;
	if (discreteSelect >= 0)
	{
		which = discreteSelect;
		PhysicalDevice = physicalDevices[which];
	}
	else if (integratedSelect >= 0)
	{
		which = integratedSelect;
		PhysicalDevice = physicalDevices[which];
	}
	else
	{
		fprintf(FpDebug, "Could not select a Physical Device\n");
		return VK_SHOULD_EXIT;
	}

	delete[] physicalDevices;

	vkGetPhysicalDeviceProperties(PhysicalDevice, OUT & PhysicalDeviceProperties);
	fprintf(FpDebug, "Device #%d selected ('%s')\n", which, PhysicalDeviceProperties.deviceName);

	vkGetPhysicalDeviceFeatures(IN PhysicalDevice, OUT & PhysicalDeviceFeatures);

	fprintf(FpDebug, "\nPhysical Device Features:\n");
	fprintf(FpDebug, "geometryShader = %2d\n", PhysicalDeviceFeatures.geometryShader);
	fprintf(FpDebug, "tessellationShader = %2d\n", PhysicalDeviceFeatures.tessellationShader);
	fprintf(FpDebug, "multiDrawIndirect = %2d\n", PhysicalDeviceFeatures.multiDrawIndirect);
	fprintf(FpDebug, "wideLines = %2d\n", PhysicalDeviceFeatures.wideLines);
	fprintf(FpDebug, "largePoints = %2d\n", PhysicalDeviceFeatures.largePoints);
	fprintf(FpDebug, "multiViewport = %2d\n", PhysicalDeviceFeatures.multiViewport);
	fprintf(FpDebug, "occlusionQueryPrecise = %2d\n", PhysicalDeviceFeatures.occlusionQueryPrecise);
	fprintf(FpDebug, "pipelineStatisticsQuery = %2d\n", PhysicalDeviceFeatures.pipelineStatisticsQuery);
	fprintf(FpDebug, "shaderFloat64 = %2d\n", PhysicalDeviceFeatures.shaderFloat64);
	fprintf(FpDebug, "shaderInt64 = %2d\n", PhysicalDeviceFeatures.shaderInt64);
	fprintf(FpDebug, "shaderInt16 = %2d\n", PhysicalDeviceFeatures.shaderInt16);

	VkFormatProperties					vfp;

		fprintf(FpDebug, "\nImage Formats Checked:\n");
	vkGetPhysicalDeviceFormatProperties(PhysicalDevice, IN VK_FORMAT_R32G32B32A32_SFLOAT, &vfp);
	fprintf(FpDebug, "Format VK_FORMAT_R32G32B32A32_SFLOAT: 0x%08x 0x%08x  0x%08x\n",
		vfp.linearTilingFeatures, vfp.optimalTilingFeatures, vfp.bufferFeatures);
	vkGetPhysicalDeviceFormatProperties(PhysicalDevice, IN VK_FORMAT_R8G8B8A8_UNORM, &vfp);
	fprintf(FpDebug, "Format VK_FORMAT_R8G8B8A8_UNORM: 0x%08x 0x%08x  0x%08x\n",
		vfp.linearTilingFeatures, vfp.optimalTilingFeatures, vfp.bufferFeatures);
	vkGetPhysicalDeviceFormatProperties(PhysicalDevice, IN VK_FORMAT_B8G8R8A8_UNORM, &vfp);
	fprintf(FpDebug, "Format VK_FORMAT_B8G8R8A8_UNORM: 0x%08x 0x%08x  0x%08x\n",
		vfp.linearTilingFeatures, vfp.optimalTilingFeatures, vfp.bufferFeatures);
	vkGetPhysicalDeviceFormatProperties(PhysicalDevice, IN VK_FORMAT_B8G8R8A8_SRGB, &vfp);
	fprintf(FpDebug, "Format VK_FORMAT_B8G8R8A8_SRGB: 0x%08x 0x%08x  0x%08x\n",
		vfp.linearTilingFeatures, vfp.optimalTilingFeatures, vfp.bufferFeatures);

	VkPhysicalDeviceMemoryProperties			vpdmp;
	vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, OUT & vpdmp);

	fprintf(FpDebug, "\n%d Memory Types:\n", vpdmp.memoryTypeCount);
	for (unsigned int i = 0; i < vpdmp.memoryTypeCount; i++)
	{
		VkMemoryType vmt = vpdmp.memoryTypes[i];
		VkMemoryPropertyFlags vmpf = vmt.propertyFlags;
		fprintf(FpDebug, "Memory %2d: ", i);
		if ((vmpf & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0)	fprintf(FpDebug, " DeviceLocal");
		if ((vmpf & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0)	fprintf(FpDebug, " HostVisible");
		if ((vmpf & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0)	fprintf(FpDebug, " HostCoherent");
		if ((vmpf & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) != 0)	fprintf(FpDebug, " HostCached");
		if ((vmpf & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) != 0)	fprintf(FpDebug, " LazilyAllocated");
		fprintf(FpDebug, "\n");
	}

	fprintf(FpDebug, "\n%d Memory Heaps:\n", vpdmp.memoryHeapCount);
	for (unsigned int i = 0; i < vpdmp.memoryHeapCount; i++)
	{
		fprintf(FpDebug, "Heap %d: ", i);
		VkMemoryHeap vmh = vpdmp.memoryHeaps[i];
		fprintf(FpDebug, " size = 0x%08lx", (unsigned long int)vmh.size);
		if ((vmh.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) != 0)	fprintf(FpDebug, " DeviceLocal");
		if ((vmh.flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT) != 0)	fprintf(FpDebug, " MultiInstance");
		fprintf(FpDebug, "\n");
	}

	uint32_t count = -1;
	vkGetPhysicalDeviceQueueFamilyProperties(IN PhysicalDevice, &count, OUT(VkQueueFamilyProperties*)nullptr);
	fprintf(FpDebug, "\nFound %d Queue Families:\n", count);

	VkQueueFamilyProperties* vqfp = new VkQueueFamilyProperties[count];
	vkGetPhysicalDeviceQueueFamilyProperties(IN PhysicalDevice, &count, OUT vqfp);
	for (unsigned int i = 0; i < count; i++)
	{
		fprintf(FpDebug, "\t%d: Queue Family Count = %2d  ;   ", i, vqfp[i].queueCount);
		if ((vqfp[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)	fprintf(FpDebug, " Graphics");
		if ((vqfp[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0)	fprintf(FpDebug, " Compute");
		if ((vqfp[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0)	fprintf(FpDebug, " Transfer");
		fprintf(FpDebug, "\n");
	}

	delete[] vqfp;

	return result;
}

VkResult Vulkan::Init04LogicalDeviceAndQueue()
{
	VkResult result = VK_SUCCESS;

	float 	queuePriorities[NUM_QUEUES_WANTED] =
	{
		1.
	};

	VkDeviceQueueCreateInfo				vdqci[NUM_QUEUES_WANTED];
	vdqci[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	vdqci[0].pNext = nullptr;
	vdqci[0].flags = 0;
	vdqci[0].queueFamilyIndex = FindQueueFamilyThatDoesGraphics();
	vdqci[0].queueCount = 1;		// how many queues to create
	vdqci[0].pQueuePriorities = queuePriorities;	// array of queue priorities [0.,1.]


	const char* myDeviceLayers[] =
	{
		"VK_LAYER_LUNARG_object_tracker",
		"VK_LAYER_LUNARG_parameter_validation",
	};

	const char* myDeviceExtensions[] =
	{
		"VK_KHR_swapchain",
	};


	// see what device layers are available:

	uint32_t layerCount;
	vkEnumerateDeviceLayerProperties(PhysicalDevice, &layerCount, (VkLayerProperties*)nullptr);
	VkLayerProperties* deviceLayers = new VkLayerProperties[layerCount];
	result = vkEnumerateDeviceLayerProperties(PhysicalDevice, &layerCount, deviceLayers);
	REPORT("vkEnumerateDeviceLayerProperties");
	if (result != VK_SUCCESS)
	{
		return result;
	}

	fprintf(FpDebug, "\n%d physical device layers enumerated:\n", layerCount);
	for (unsigned int i = 0; i < layerCount; i++)
	{
		fprintf(FpDebug, "0x%08x  %2d  '%s'  '%s'\n",
			deviceLayers[i].specVersion,
			deviceLayers[i].implementationVersion,
			deviceLayers[i].layerName,
			deviceLayers[i].description);


		// see what device extensions are available:

		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(PhysicalDevice, deviceLayers[i].layerName, &extensionCount, (VkExtensionProperties*)nullptr);
		VkExtensionProperties* deviceExtensions = new VkExtensionProperties[extensionCount];
		result = vkEnumerateDeviceExtensionProperties(PhysicalDevice, deviceLayers[i].layerName, &extensionCount, deviceExtensions);
		REPORT("vkEnumerateDeviceExtensionProperties");
		if (result != VK_SUCCESS)
		{
			return result;
		}

		fprintf(FpDebug, "\t%d device extensions enumerated for '%s':\n", extensionCount, deviceLayers[i].layerName);
		for (unsigned int ii = 0; ii < extensionCount; ii++)
		{
			fprintf(FpDebug, "\t0x%08x  '%s'\n",
				deviceExtensions[ii].specVersion,
				deviceExtensions[ii].extensionName);
		}
		fprintf(FpDebug, "\n");
	}

	delete[] deviceLayers;


	VkDeviceCreateInfo   vdci;
	vdci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	vdci.pNext = nullptr;
	vdci.flags = 0;
	vdci.queueCreateInfoCount = NUM_QUEUES_WANTED;		// # of device queues, each of which can create multiple queues
	vdci.pQueueCreateInfos = IN & vdqci[0];			// array of VkDeviceQueueCreateInfo's

	vdci.enabledLayerCount = sizeof(myDeviceLayers) / sizeof(char*);
	//vdci.enabledLayerCount = 0;
	vdci.ppEnabledLayerNames = myDeviceLayers;

	vdci.enabledExtensionCount = sizeof(myDeviceExtensions) / sizeof(char*);
	vdci.ppEnabledExtensionNames = myDeviceExtensions;

	vdci.pEnabledFeatures = IN & PhysicalDeviceFeatures;	// already created

	result = vkCreateLogicalDevice(PhysicalDevice, IN & vdci, PALLOCATOR, OUT & LogicalDevice);
	REPORT("vkCreateLogicalDevice");


	// get the queue for this logical device:

	vkGetDeviceQueue(LogicalDevice, 0, 0, OUT & Queue);
	// queueFamilyIndex, queueIndex
	return result;
}

VkResult Vulkan::Init05DataBuffer(VkDeviceSize size, VkBufferUsageFlags usage, OUT MyBuffer* pMyBuffer)
{
	VkResult result = VK_SUCCESS;

	VkBufferCreateInfo  vbci;
	vbci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vbci.pNext = nullptr;
	vbci.flags = 0;
	vbci.size = size;
	vbci.usage = usage;
#ifdef CHOICES
	VK_USAGE_TRANSFER_SRC_BIT
		VK_USAGE_TRANSFER_DST_BIT
		VK_USAGE_UNIFORM_TEXEL_BUFFER_BIT
		VK_USAGE_STORAGE_TEXEL_BUFFER_BIT
		VK_USAGE_UNIFORM_BUFFER_BIT
		VK_USAGE_STORAGE_BUFFER_BIT
		VK_USAGE_INDEX_BUFFER_BIT
		VK_USAGE_VERTEX_BUFFER_BIT
		VK_USAGE_INDIRECT_BUFFER_BIT
#endif
		vbci.queueFamilyIndexCount = 0;
	vbci.pQueueFamilyIndices = (const uint32_t*)nullptr;
	vbci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;	// can only use CONCURRENT if .queueFamilyIndexCount > 0
#ifdef CHOICES
	VK_SHARING_MODE_EXCLUSIVE
		VK_SHARING_MODE_CONCURRENT
#endif

		pMyBuffer->size = size;
	result = vkCreateBuffer(LogicalDevice, IN & vbci, PALLOCATOR, OUT & pMyBuffer->buffer);
	REPORT("vkCreateBuffer");

	VkMemoryRequirements			vmr;
	vkGetBufferMemoryRequirements(LogicalDevice, IN pMyBuffer->buffer, OUT & vmr);		// fills vmr
	if (Verbose)
	{
		fprintf(FpDebug, "Buffer vmr.size = %lld\n", vmr.size);
		fprintf(FpDebug, "Buffer vmr.alignment = %lld\n", vmr.alignment);
		fprintf(FpDebug, "Buffer vmr.memoryTypeBits = 0x%08x\n", vmr.memoryTypeBits);
		fflush(FpDebug);
	}

	VkMemoryAllocateInfo			vmai;
	vmai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vmai.pNext = nullptr;
	vmai.allocationSize = vmr.size;
	vmai.memoryTypeIndex = FindMemoryThatIsHostVisible(vmr.memoryTypeBits);

	VkDeviceMemory				vdm;
	result = vkAllocateMemory(LogicalDevice, IN & vmai, PALLOCATOR, OUT & vdm);
	REPORT("vkAllocateMemory");
	pMyBuffer->vdm = vdm;

	result = vkBindBufferMemory(LogicalDevice, pMyBuffer->buffer, IN vdm, 0);		// 0 is the offset
	REPORT("vkBindBufferMemory");

	return result;
}

VkResult Vulkan::Init05MyIndexDataBuffer(IN VkDeviceSize size, OUT MyBuffer* pMyBuffer)
{
	VkResult result = Vulkan::Init05DataBuffer(size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, pMyBuffer);          // fills pMyBuffer
	REPORT("Init05MyIndexDataBufferBuffer");
	return result;
}

VkResult Vulkan::Init05MyVertexDataBuffer(IN VkDeviceSize size, OUT MyBuffer* pMyBuffer)
{
	VkResult result = Vulkan::Init05DataBuffer(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, pMyBuffer);		// fills pMyBuffer
	REPORT("InitDataBuffer");
	return result;
}

VkResult Vulkan::Init05UniformBuffer(VkDeviceSize size, MyBuffer* pMyBuffer)
{
	VkResult result = Vulkan::Init05DataBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, OUT pMyBuffer);	// fills pMyBuffer
	return result;
}

VkResult Vulkan::Fill05DataBuffer(IN MyBuffer myBuffer, IN void* data)
{
	// the size of the data had better match the size that was used to Init the buffer!

	void* pGpuMemory;
	vkMapMemory(LogicalDevice, IN myBuffer.vdm, 0, VK_WHOLE_SIZE, 0, &pGpuMemory);	// 0 and 0 are offset and flags
	memcpy(pGpuMemory, data, (size_t)myBuffer.size);
	vkUnmapMemory(LogicalDevice, IN myBuffer.vdm);
	return VK_SUCCESS;
}

VkResult Vulkan::Init07TextureSampler(MyTexture* pMyTexture)
{
	VkResult result = VK_SUCCESS;

	VkSamplerCreateInfo					vsci;
	vsci.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	vsci.pNext = nullptr;
	vsci.flags = 0;
	vsci.magFilter = VK_FILTER_LINEAR;
	vsci.minFilter = VK_FILTER_LINEAR;
	vsci.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	vsci.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	vsci.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	vsci.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
#ifdef CHOICES
	VK_SAMPLER_ADDRESS_MODE_REPEAT
		VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT
		VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
		VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER
		VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
#endif
		vsci.mipLodBias = 0.;
	vsci.anisotropyEnable = VK_FALSE;
	vsci.maxAnisotropy = 1.;
	vsci.compareEnable = VK_FALSE;
	vsci.compareOp = VK_COMPARE_OP_NEVER;
#ifdef CHOICES
	VK_COMPARE_OP_NEVER
		VK_COMPARE_OP_LESS
		VK_COMPARE_OP_EQUAL
		VK_COMPARE_OP_LESS_OR_EQUAL
		VK_COMPARE_OP_GREATER
		VK_COMPARE_OP_NOT_EQUAL
		VK_COMPARE_OP_GREATER_OR_EQUAL
		VK_COMPARE_OP_ALWAYS
#endif
		vsci.minLod = 0.;
	vsci.maxLod = 0.;
	vsci.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
#ifdef CHOICES
	VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK
		VK_BORDER_COLOR_INT_TRANSPARENT_BLACK
		VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK
		VK_BORDER_COLOR_INT_OPAQUE_BLACK
		VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE
		VK_BORDER_COLOR_INT_OPAQUE_WHITE
#endif
		vsci.unnormalizedCoordinates = VK_FALSE;	// VK_TRUE means we are use raw texels as the index
								// VK_FALSE means we are uing the usual 0. - 1.

	result = vkCreateSampler(LogicalDevice, IN & vsci, PALLOCATOR, OUT & pMyTexture->texSampler);
	REPORT("vkCreateSampler");
	return result;
}

VkResult Vulkan::Init07TextureBuffer(INOUT MyTexture* pMyTexture)
{
	VkResult result = VK_SUCCESS;

	uint32_t texWidth = pMyTexture->width;
	uint32_t texHeight = pMyTexture->height;
	unsigned char* texture = pMyTexture->pixels;
	VkDeviceSize textureSize = texWidth * texHeight * 4;		// rgba, 1 byte each

	VkImage  stagingImage;
	VkImage  textureImage;


	// *******************************************************************************
	// this first {...} is to create the staging image:
	// *******************************************************************************
	{
		VkImageCreateInfo			vici;
		vici.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		vici.pNext = nullptr;
		vici.flags = 0;
#ifdef CHOICES
		VK_IMAGE_CREATE_SPARSE_BINDING_BIT
			VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT
			VK_IMAGE_CREATE_SPARSE_ALIASED_BIT
			VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT
			VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT
			VK_IMAGE_CREATE_BIND_SFR_BIT_KHX
			VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT_KHR
#endif
			vici.imageType = VK_IMAGE_TYPE_2D;
		//vici.format = VK_FORMAT_R8G8B8A8_UNORM;
		vici.format = VK_FORMAT_B8G8R8A8_SRGB;
		vici.extent.width = texWidth;
		vici.extent.height = texHeight;
		vici.extent.depth = 1;
		vici.mipLevels = 1;
		vici.arrayLayers = 1;
		vici.samples = VK_SAMPLE_COUNT_1_BIT;
		vici.tiling = VK_IMAGE_TILING_LINEAR;
#ifdef CHOICES
		VK_IMAGE_TILING_OPTIMAL
			VK_IMAGE_TILING_LINEAR
#endif
			vici.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
#ifdef CHOICES
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT
			VK_IMAGE_USAGE_TRANSFER_DST_BIT
			VK_IMAGE_USAGE_SAMPLED_BIT
			VK_IMAGE_USAGE_STORAGE_BIT
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
			VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT
			VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
#endif
			vici.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vici.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
#ifdef CHOICES
		VK_IMAGE_LAYOUT_UNDEFINED
			VK_IMAGE_LAYOUT_PREINITIALIZED
#endif
			vici.queueFamilyIndexCount = 0;
		vici.pQueueFamilyIndices = (const uint32_t*)nullptr;

		result = vkCreateImage(LogicalDevice, IN & vici, PALLOCATOR, OUT & stagingImage);	// allocated, but not filled
		REPORT("vkCreateImage");

		VkMemoryRequirements			vmr;
		vkGetImageMemoryRequirements(LogicalDevice, IN stagingImage, OUT & vmr);

		if (Verbose)
		{
			fprintf(FpDebug, "Image vmr.size = %lld\n", vmr.size);
			fprintf(FpDebug, "Image vmr.alignment = %lld\n", vmr.alignment);
			fprintf(FpDebug, "Image vmr.memoryTypeBits = 0x%08x\n", vmr.memoryTypeBits);
			fflush(FpDebug);
		}

		VkMemoryAllocateInfo			vmai;
		vmai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		vmai.pNext = nullptr;
		vmai.allocationSize = vmr.size;
		vmai.memoryTypeIndex = FindMemoryThatIsHostVisible(vmr.memoryTypeBits);	// because we want to mmap it

		VkDeviceMemory				vdm;
		result = vkAllocateMemory(LogicalDevice, IN & vmai, PALLOCATOR, OUT & vdm);
		REPORT("vkAllocateMemory");
		pMyTexture->vdm = vdm;

		result = vkBindImageMemory(LogicalDevice, IN stagingImage, IN vdm, 0);	// 0 = offset
		REPORT("vkBindImageMemory");


		// we have now created the staging image -- fill it with the pixel data:

		VkImageSubresource			vis;
		vis.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		vis.mipLevel = 0;
		vis.arrayLayer = 0;

		VkSubresourceLayout			vsl;
		vkGetImageSubresourceLayout(LogicalDevice, stagingImage, IN & vis, OUT & vsl);

		if (Verbose)
		{
			fprintf(FpDebug, "Subresource Layout:\n");
			fprintf(FpDebug, "\toffset = %lld\n", vsl.offset);
			fprintf(FpDebug, "\tsize = %lld\n", vsl.size);
			fprintf(FpDebug, "\trowPitch = %lld\n", vsl.rowPitch);
			fprintf(FpDebug, "\tarrayPitch = %lld\n", vsl.arrayPitch);
			fprintf(FpDebug, "\tdepthPitch = %lld\n", vsl.depthPitch);
			fflush(FpDebug);
		}

		void* gpuMemory;
		vkMapMemory(LogicalDevice, vdm, 0, VK_WHOLE_SIZE, 0, OUT & gpuMemory);
		// 0 and 0 = offset and memory map flags

		if (vsl.rowPitch == 4 * texWidth)
		{
			memcpy(gpuMemory, (void*)texture, (size_t)textureSize);
		}
		else
		{
			unsigned char* gpuBytes = (unsigned char*)gpuMemory;
			for (unsigned int y = 0; y < texHeight; y++)
			{
				memcpy(&gpuBytes[y * vsl.rowPitch], &texture[4 * y * texWidth], (size_t)(4 * texWidth));
			}
		}

		vkUnmapMemory(LogicalDevice, vdm);

	}
	// *******************************************************************************


	// *******************************************************************************
	// this second {...} is to create the actual texture image:
	// *******************************************************************************
	{
		VkImageCreateInfo			vici;
		vici.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		vici.pNext = nullptr;
		vici.flags = 0;
		vici.imageType = VK_IMAGE_TYPE_2D;
		vici.format = VK_FORMAT_R8G8B8A8_SRGB;
		vici.extent.width = texWidth;
		vici.extent.height = texHeight;
		vici.extent.depth = 1;
		vici.mipLevels = 1;
		vici.arrayLayers = 1;
		vici.samples = VK_SAMPLE_COUNT_1_BIT;
		vici.tiling = VK_IMAGE_TILING_OPTIMAL;
		vici.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		// because we are transfering into it and will eventual sample from it
		vici.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vici.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
		vici.queueFamilyIndexCount = 0;
		vici.pQueueFamilyIndices = (const uint32_t*)nullptr;

		result = vkCreateImage(LogicalDevice, IN & vici, PALLOCATOR, OUT & textureImage);	// allocated, but not filled
		REPORT("vkCreateImage");

		VkMemoryRequirements			vmr;
		vkGetImageMemoryRequirements(LogicalDevice, IN textureImage, OUT& vmr);

		if (Verbose)
		{
			fprintf(FpDebug, "Texture vmr.size = %lld\n", vmr.size);
			fprintf(FpDebug, "Texture vmr.alignment = %lld\n", vmr.alignment);
			fprintf(FpDebug, "Texture vmr.memoryTypeBits = 0x%08x\n", vmr.memoryTypeBits);
			fflush(FpDebug);
		}

		VkMemoryAllocateInfo			vmai;
		vmai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		vmai.pNext = nullptr;
		vmai.allocationSize = vmr.size;
		vmai.memoryTypeIndex = FindMemoryThatIsDeviceLocal(vmr.memoryTypeBits);  // because we want to sample from it

		VkDeviceMemory				vdm;
		result = vkAllocateMemory(LogicalDevice, IN & vmai, PALLOCATOR, OUT & vdm);
		REPORT("vkAllocateMemory");

		result = vkBindImageMemory(LogicalDevice, IN textureImage, IN vdm, 0);	// 0 = offset
		REPORT("vkBindImageMemory");
	}
	// *******************************************************************************


	// copy pixels from the staging image to the texture:

	VkCommandBufferBeginInfo		vcbbi;
	vcbbi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	vcbbi.pNext = nullptr;
	vcbbi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vcbbi.pInheritanceInfo = (VkCommandBufferInheritanceInfo*)nullptr;

	result = vkBeginCommandBuffer(TextureCommandBuffer, IN & vcbbi);
	REPORT("Init07TextureBuffer -- vkBeginCommandBuffer");


	// *******************************************************************************
	// transition the staging buffer layout:
	// *******************************************************************************
	{
		VkImageSubresourceRange			visr;
		visr.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		visr.baseMipLevel = 0;
		visr.levelCount = 1;
		visr.baseArrayLayer = 0;
		visr.layerCount = 1;

		VkImageMemoryBarrier			vimb;
		vimb.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		vimb.pNext = nullptr;
		vimb.oldLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
		vimb.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		vimb.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		vimb.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		vimb.image = stagingImage;
		vimb.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
		vimb.dstAccessMask = 0;
		vimb.subresourceRange = visr;

		vkCmdPipelineBarrier(TextureCommandBuffer,
			VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_HOST_BIT, 0,
			0, (VkMemoryBarrier*)nullptr,
			0, (VkBufferMemoryBarrier*)nullptr,
			1, IN & vimb);
	}
	// *******************************************************************************



	// *******************************************************************************
	// transition the texture buffer layout:
	// *******************************************************************************
	{
		VkImageSubresourceRange			visr;
		visr.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		visr.baseMipLevel = 0;
		visr.levelCount = 1;
		visr.baseArrayLayer = 0;
		visr.layerCount = 1;

		VkImageMemoryBarrier			vimb;
		vimb.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		vimb.pNext = nullptr;
		vimb.oldLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
		vimb.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		vimb.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		vimb.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		vimb.image = textureImage;
		vimb.srcAccessMask = 0;
		vimb.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		vimb.subresourceRange = visr;

		vkCmdPipelineBarrier(TextureCommandBuffer,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			0, (VkMemoryBarrier*)nullptr,
			0, (VkBufferMemoryBarrier*)nullptr,
			1, IN & vimb);

		// now do the final image transfer:

		VkImageSubresourceLayers		visl;
		visl.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		visl.baseArrayLayer = 0;
		visl.mipLevel = 0;
		visl.layerCount = 1;

		VkOffset3D				vo3;
		vo3.x = 0;
		vo3.y = 0;
		vo3.z = 0;

		VkExtent3D				ve3;
		ve3.width = texWidth;
		ve3.height = texHeight;
		ve3.depth = 1;

		VkImageCopy				vic;
		vic.srcSubresource = visl;
		vic.srcOffset = vo3;
		vic.dstSubresource = visl;
		vic.dstOffset = vo3;
		vic.extent = ve3;

		vkCmdCopyImage(TextureCommandBuffer,
			stagingImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, IN & vic);
	}
	// *******************************************************************************


	// *******************************************************************************
	// transition the texture buffer layout a second time:
	// *******************************************************************************
	{
		VkImageSubresourceRange			visr;
		visr.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		visr.baseMipLevel = 0;
		visr.levelCount = 1;
		visr.baseArrayLayer = 0;
		visr.layerCount = 1;

		VkImageMemoryBarrier			vimb;
		vimb.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		vimb.pNext = nullptr;
		vimb.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		vimb.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		vimb.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		vimb.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		vimb.image = textureImage;
		//vimb.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		vimb.srcAccessMask = 0;
		//vimb.dstAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
		vimb.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		vimb.subresourceRange = visr;

		vkCmdPipelineBarrier(TextureCommandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, (VkMemoryBarrier*)nullptr,
			0, (VkBufferMemoryBarrier*)nullptr,
			1, IN & vimb);
	}
	// *******************************************************************************

	result = vkEndCommandBuffer(TextureCommandBuffer);
	REPORT("Init07TextureBuffer -- vkEndCommandBuffer");

	VkSubmitInfo				vsi;
	vsi.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	vsi.pNext = nullptr;
	vsi.commandBufferCount = 1;
	vsi.pCommandBuffers = &TextureCommandBuffer;
	vsi.waitSemaphoreCount = 0;
	vsi.pWaitSemaphores = (VkSemaphore*)nullptr;
	vsi.signalSemaphoreCount = 0;
	vsi.pSignalSemaphores = (VkSemaphore*)nullptr;
	vsi.pWaitDstStageMask = (VkPipelineStageFlags*)nullptr;

	result = vkQueueSubmit(Queue, 1, IN & vsi, VK_NULL_HANDLE);
	if (Verbose)		REPORT("vkQueueSubmit");

	result = vkQueueWaitIdle(Queue);
	if (Verbose)		REPORT("vkQueueWaitIdle");


	// create an image view for the texture image:

	VkImageSubresourceRange			visr;
	visr.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	visr.baseMipLevel = 0;
	visr.levelCount = 1;
	visr.baseArrayLayer = 0;
	visr.layerCount = 1;

	VkImageViewCreateInfo			vivci;
	vivci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	vivci.pNext = nullptr;
	vivci.flags = 0;
	vivci.image = textureImage;
	vivci.viewType = VK_IMAGE_VIEW_TYPE_2D;
	vivci.format = VK_FORMAT_R8G8B8A8_SRGB;
	vivci.components.r = VK_COMPONENT_SWIZZLE_R;
	vivci.components.g = VK_COMPONENT_SWIZZLE_G;
	vivci.components.b = VK_COMPONENT_SWIZZLE_B;
	vivci.components.a = VK_COMPONENT_SWIZZLE_A;
	vivci.subresourceRange = visr;

	result = vkCreateImageView(LogicalDevice, IN & vivci, PALLOCATOR, OUT & pMyTexture->texImageView);
	REPORT("vkCreateImageView");

	vkDestroyImage(LogicalDevice, stagingImage, PALLOCATOR);

	return result;
}

VkResult Vulkan::Init07TextureBufferAndFillFromBmpFile(IN std::string filename, OUT MyTexture* pMyTexture)
{
	VkResult result = VK_SUCCESS;

	const int birgb = { 0 };

	FILE* fp;
#ifdef _WIN32
	errno_t err = fopen_s(&fp, filename.c_str(), "rb");
	if (err != 0)
	{
		fprintf(stderr, "Cannot open BMP file '%s'\n", filename.c_str());
		return VK_FAILURE;
	}
#else
	fp = fopen(filename.c_str(), "rb");
	if (fp == NULL)
	{
		fprintf(stderr, "Cannot open BMP file '%s'\n", filename.c_str());
		return VK_FAILURE;
	}
#endif

	FileHeader.bfType = ReadShort(fp);

	// if bfType is not 0x4d42, the file is not a bmp:

	if (FileHeader.bfType != 0x4d42)
	{
		fprintf(FpDebug, "Wrong type of file: 0x%0x\n", FileHeader.bfType);
		fclose(fp);
		return VK_FAILURE;
	}


	FileHeader.bfSize = ReadInt(fp);
	FileHeader.bfReserved1 = ReadShort(fp);
	FileHeader.bfReserved2 = ReadShort(fp);
	FileHeader.bfOffBits = ReadInt(fp);

	InfoHeader.biSize = ReadInt(fp);
	InfoHeader.biWidth = ReadInt(fp);
	InfoHeader.biHeight = ReadInt(fp);

	uint32_t texWidth = InfoHeader.biWidth;
	uint32_t texHeight = InfoHeader.biHeight;

	InfoHeader.biPlanes = ReadShort(fp);
	InfoHeader.biBitCount = ReadShort(fp);
	InfoHeader.biCompression = ReadInt(fp);
	InfoHeader.biSizeImage = ReadInt(fp);
	InfoHeader.biXPelsPerMeter = ReadInt(fp);
	InfoHeader.biYPelsPerMeter = ReadInt(fp);
	InfoHeader.biClrUsed = ReadInt(fp);
	InfoHeader.biClrImportant = ReadInt(fp);

	fprintf(FpDebug, "Image size found: %d x %d\n", texWidth, texHeight);

	unsigned char* texture = new unsigned char[4 * texWidth * texHeight];

	// extra padding bytes:

	int numExtra = 4 * (((3 * InfoHeader.biWidth) + 3) / 4) - 3 * InfoHeader.biWidth;

	// we do not support compression:

	if (InfoHeader.biCompression != birgb)
	{
		fprintf(FpDebug, "Wrong type of image compression: %d\n", InfoHeader.biCompression);
		fclose(fp);
		return VK_FAILURE;
	}

	rewind(fp);
	fseek(fp, 14 + 40, SEEK_SET);

	if (InfoHeader.biBitCount == 24)
	{
		unsigned char* tp = texture;
		for (unsigned int t = 0; t < texHeight; t++)
		{
			for (unsigned int s = 0; s < texWidth; s++, tp += 4)
			{
				*(tp + 3) = 255;			// a
				*(tp + 2) = fgetc(fp);		// b
				*(tp + 1) = fgetc(fp);		// g
				*(tp + 0) = fgetc(fp);		// r
			}

			for (int e = 0; e < numExtra; e++)
			{
				fgetc(fp);
			}
		}
	}
	fclose(fp);

	pMyTexture->width = texWidth;
	pMyTexture->height = texHeight;
	pMyTexture->pixels = texture;

	result = Init07TextureBuffer(INOUT pMyTexture);
	REPORT("Init07TextureBuffer");

	return result;
}

VkResult Vulkan::Init08Swapchain()
{
	VkResult result = VK_SUCCESS;

	VkSurfaceCapabilitiesKHR		vsc;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice, Surface, OUT & vsc);

	VkExtent2D surfaceRes = vsc.currentExtent;
	fprintf(FpDebug, "\nvkGetPhysicalDeviceSurfaceCapabilitiesKHR:\n");
	fprintf(FpDebug, "\tminImageCount = %d ; maxImageCount = %d\n", vsc.minImageCount, vsc.maxImageCount);
	fprintf(FpDebug, "\tcurrentExtent = %d x %d\n", vsc.currentExtent.width, vsc.currentExtent.height);
	fprintf(FpDebug, "\tminImageExtent = %d x %d\n", vsc.minImageExtent.width, vsc.minImageExtent.height);
	fprintf(FpDebug, "\tmaxImageExtent = %d x %d\n", vsc.maxImageExtent.width, vsc.maxImageExtent.height);
	fprintf(FpDebug, "\tmaxImageArrayLayers = %d\n", vsc.maxImageArrayLayers);
	fprintf(FpDebug, "\tsupportedTransforms = 0x%04x\n", vsc.supportedTransforms);
	fprintf(FpDebug, "\tcurrentTransform = 0x%04x\n", vsc.currentTransform);
	fprintf(FpDebug, "\tsupportedCompositeAlpha = 0x%04x\n", vsc.supportedCompositeAlpha);
	fprintf(FpDebug, "\tsupportedUsageFlags = 0x%04x\n", vsc.supportedUsageFlags);

	VkBool32  supported;
	result = vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, FindQueueFamilyThatDoesGraphics(), Surface, &supported);
	REPORT("vkGetPhysicalDeviceSurfaceSupportKHR");
	if (supported == VK_TRUE)
	{
		fprintf(FpDebug, "** This Surface is supported by the Graphics Queue **\n");
	}
	else
	{
		fprintf(FpDebug, "** This Surface is not supported by the Graphics Queue **\n");
	}


	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &formatCount, (VkSurfaceFormatKHR*) nullptr);
	VkSurfaceFormatKHR* surfaceFormats = new VkSurfaceFormatKHR[formatCount];
	vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &formatCount, surfaceFormats);

		fprintf(FpDebug, "\nFound %d Surface Formats:\n", formatCount);
	for (uint32_t i = 0; i < formatCount; i++)
	{
		fprintf(FpDebug, "%3d:     %4d     %12d", i, surfaceFormats[i].format, surfaceFormats[i].colorSpace); \
			if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)			fprintf(FpDebug, "\tVK_COLOR_SPACE_SRGB_NONLINEAR_KHR\n");
		if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT)		fprintf(FpDebug, "\tVK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT\n");
		if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT)		fprintf(FpDebug, "\tVK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT\n");
		if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_DCI_P3_LINEAR_EXT)			fprintf(FpDebug, "\tVK_COLOR_SPACE_DCI_P3_LINEAR_EXT\n");
		if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT)		fprintf(FpDebug, "\tVK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT\n");
		if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_BT709_LINEAR_EXT)			fprintf(FpDebug, "\tVK_COLOR_SPACE_BT709_LINEAR_EXT\n");
		if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_BT709_NONLINEAR_EXT)			fprintf(FpDebug, "\tVK_COLOR_SPACE_BT709_NONLINEAR_EXT\n");
		if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_BT2020_LINEAR_EXT)			fprintf(FpDebug, "\tVK_COLOR_SPACE_BT2020_LINEAR_EXT\n");
		if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_HDR10_ST2084_EXT)			fprintf(FpDebug, "\tVK_COLOR_SPACE_HDR10_ST2084_EXT\n");
		if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_DOLBYVISION_EXT)			fprintf(FpDebug, "\tVK_COLOR_SPACE_DOLBYVISION_EXT\n");
		if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_HDR10_HLG_EXT)			fprintf(FpDebug, "\tVK_COLOR_SPACE_HDR10_HLG_EXT\n");
		if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT)			fprintf(FpDebug, "\tVK_COLOR_SPACE_ADOBERGB_LINEAR_EXT\n");
		if (surfaceFormats[i].colorSpace == VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT)		fprintf(FpDebug, "\tVK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT\n");

	}
	delete[] surfaceFormats;


	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &presentModeCount, (VkPresentModeKHR*) nullptr);
	VkPresentModeKHR* presentModes = new VkPresentModeKHR[presentModeCount];
	vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &presentModeCount, presentModes);
#ifdef PRESENT_MODE_CHOICES
	VK_PRESENT_MODE_IMMEDIATE_KHR = 0,
		VK_PRESENT_MODE_MAILBOX_KHR = 1, < ---- - the 1080tis can handle this
		VK_PRESENT_MODE_FIFO_KHR = 2, < ---- - the 1080tis can handle this
		VK_PRESENT_MODE_FIFO_RELAXED_KHR = 3, < ---- - the 1080tis can handle this
		VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR = 1000111000,
		VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR = 1000111001,
#endif
		fprintf(FpDebug, "\nFound %d Present Modes:\n", presentModeCount);
	for (uint32_t i = 0; i < presentModeCount; i++)
	{
		fprintf(FpDebug, "%3d:     %4d", i, presentModes[i]);
		if (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)			fprintf(FpDebug, "\tVK_PRESENT_MODE_IMMEDIATE_KHR\n");
		if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)			fprintf(FpDebug, "\tVK_PRESENT_MODE_MAILBOX_KHR\n");
		if (presentModes[i] == VK_PRESENT_MODE_FIFO_KHR)			fprintf(FpDebug, "\tVK_PRESENT_MODE_FIFO_KHR\n");
		if (presentModes[i] == VK_PRESENT_MODE_FIFO_RELAXED_KHR)		fprintf(FpDebug, "\tVK_PRESENT_MODE_FIFO_RELAXED_KHR\n");
		if (presentModes[i] == VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR)	fprintf(FpDebug, "\tVK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR\n");
		if (presentModes[i] == VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR)	fprintf(FpDebug, "\tVK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR\n");

	}
	fprintf(stderr, "\n");


	// find the present mode we should use:

	VkPresentModeKHR acceptablePresentModes[] =
	{
			VK_PRESENT_MODE_MAILBOX_KHR,
			VK_PRESENT_MODE_FIFO_KHR,
			VK_PRESENT_MODE_FIFO_RELAXED_KHR,
			VK_PRESENT_MODE_IMMEDIATE_KHR,
	};

	VkPresentModeKHR thePresentMode = (VkPresentModeKHR)VK_NULL_HANDLE;
	for (VkPresentModeKHR apm : acceptablePresentModes)
	{
		for (uint32_t i = 0; i < presentModeCount; i++)
		{
			if (apm == presentModes[i])
			{
				thePresentMode = apm;
				break;
			}
		}
		if (thePresentMode != (VkPresentModeKHR)VK_NULL_HANDLE)
			break;
	}

	if (thePresentMode == (VkPresentModeKHR)VK_NULL_HANDLE)
	{
		fprintf(FpDebug, "Couldn't find an acceptable Present Mode!\n");
	}
	else
	{
		fprintf(FpDebug, "The Present Mode to use = %d\n", thePresentMode);
	}

	delete[] presentModes;

	VkSwapchainCreateInfoKHR		vscci;
	vscci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	vscci.pNext = nullptr;
	vscci.flags = 0;
	vscci.surface = Surface;
	vscci.minImageCount = 2;			// quad buffering
	//vscci.imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
	vscci.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
	vscci.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	vscci.imageExtent.width = surfaceRes.width;
	vscci.imageExtent.height = surfaceRes.height;
	vscci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	vscci.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	vscci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	vscci.imageArrayLayers = 1;
	vscci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vscci.queueFamilyIndexCount = 0;
	vscci.pQueueFamilyIndices = (const uint32_t*)nullptr;
	//vscci.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
	vscci.presentMode = thePresentMode;
	vscci.oldSwapchain = VK_NULL_HANDLE;	// we're not replacing an old swapchain
	vscci.clipped = VK_TRUE;

	result = vkCreateSwapchainKHR(LogicalDevice, IN & vscci, PALLOCATOR, OUT & SwapChain);
	REPORT("vkCreateSwapchainKHR");


	uint32_t imageCount;
	result = vkGetSwapchainImagesKHR(LogicalDevice, IN SwapChain, OUT & imageCount, (VkImage*)nullptr);	// 0 
	REPORT("vkGetSwapchainImagesKHR - 0");
	if (imageCount != 2)
	{
		fprintf(FpDebug, "imageCount return from vkGetSwapchainImages = %d; should have been 2\n", imageCount);
		return result;
	}

	PresentImages = new VkImage[imageCount];
	result = vkGetSwapchainImagesKHR(LogicalDevice, SwapChain, OUT & imageCount, PresentImages);	// 0 
	REPORT("vkGetSwapchainImagesKHR - 1");


	// present views for the double-buffering:

	PresentImageViews = new VkImageView[imageCount];	// better be 2
	for (unsigned int i = 0; i < imageCount; i++)
	{
		VkImageViewCreateInfo		vivci;
		vivci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		vivci.pNext = nullptr;
		vivci.flags = 0;
		vivci.viewType = VK_IMAGE_VIEW_TYPE_2D;
		//vivci.format = VK_FORMAT_B8G8R8A8_UNORM;
		vivci.format = VK_FORMAT_B8G8R8A8_SRGB;
		vivci.components.r = VK_COMPONENT_SWIZZLE_R;
		vivci.components.g = VK_COMPONENT_SWIZZLE_G;
		vivci.components.b = VK_COMPONENT_SWIZZLE_B;
		vivci.components.a = VK_COMPONENT_SWIZZLE_A;
		vivci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		vivci.subresourceRange.baseMipLevel = 0;
		vivci.subresourceRange.levelCount = 1;
		vivci.subresourceRange.baseArrayLayer = 0;
		vivci.subresourceRange.layerCount = 1;
		vivci.image = PresentImages[i];

		result = vkCreateImageView(LogicalDevice, IN & vivci, PALLOCATOR, OUT & PresentImageViews[i]);
		REPORT("vkCreateImageView");
	}

	return result;
}

VkResult Vulkan::Init09DepthStencilImage()
{
	VkResult result = VK_SUCCESS;

	VkExtent3D ve3d = { Width, Height, 1 };

	VkImageCreateInfo			vici;
	vici.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	vici.pNext = nullptr;
	vici.flags = 0;
	vici.imageType = VK_IMAGE_TYPE_2D;
	vici.format = VK_FORMAT_D32_SFLOAT_S8_UINT;
	vici.extent = ve3d;
	vici.mipLevels = 1;
	vici.arrayLayers = 1;
	vici.samples = VK_SAMPLE_COUNT_1_BIT;
	vici.tiling = VK_IMAGE_TILING_OPTIMAL;
	vici.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	vici.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vici.queueFamilyIndexCount = 0;
	vici.pQueueFamilyIndices = (const uint32_t*)nullptr;
	vici.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	result = vkCreateImage(LogicalDevice, IN & vici, PALLOCATOR, &DepthStencilImage);
	REPORT("vkCreateImage");

	VkMemoryRequirements			vmr;
	vkGetImageMemoryRequirements(LogicalDevice, IN DepthStencilImage, OUT & vmr);

	VkMemoryAllocateInfo			vmai;
	vmai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vmai.pNext = nullptr;
	vmai.allocationSize = vmr.size;
	vmai.memoryTypeIndex = FindMemoryThatIsDeviceLocal(vmr.memoryTypeBits);

	VkDeviceMemory imageMemory;
	result = vkAllocateMemory(LogicalDevice, IN & vmai, PALLOCATOR, OUT & imageMemory);
	REPORT("vkAllocateMemory");

	result = vkBindImageMemory(LogicalDevice, DepthStencilImage, imageMemory, 0);	// 0 is the offset
	REPORT("vkBindImageMemory");

	VkImageViewCreateInfo			vivci;
	vivci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	vivci.pNext = nullptr;
	vivci.flags = 0;
	vivci.image = DepthStencilImage;
	vivci.viewType = VK_IMAGE_VIEW_TYPE_2D;
	vivci.format = vici.format;
	vivci.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	vivci.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	vivci.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	vivci.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	vivci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	vivci.subresourceRange.baseMipLevel = 0;
	vivci.subresourceRange.levelCount = 1;
	vivci.subresourceRange.baseArrayLayer = 0;
	vivci.subresourceRange.layerCount = 1;

	result = vkCreateImageView(LogicalDevice, IN & vivci, PALLOCATOR, OUT & DepthStencilImageView);
	REPORT("vkCreateImageView");
	return result;
}

VkResult Vulkan::Init10RenderPasses()
{
	VkResult result = VK_SUCCESS;

	// need 2 - one for the color and one for the depth/stencil
	VkAttachmentDescription				vad[2];
	//vad[0].format = VK_FORMAT_B8G8R8A8_UNORM;
	vad[0].format = VK_FORMAT_B8G8R8A8_SRGB;
	vad[0].samples = VK_SAMPLE_COUNT_1_BIT;
	vad[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	vad[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	vad[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	vad[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	vad[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	vad[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	vad[0].flags = 0;
	//vad[0].flags = VK_ATTACHMENT_DESCRIPTION_MAT_ALIAS_BIT;

	vad[1].format = VK_FORMAT_D32_SFLOAT_S8_UINT;
	vad[1].samples = VK_SAMPLE_COUNT_1_BIT;
	vad[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	vad[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	vad[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	vad[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	vad[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	vad[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	vad[1].flags = 0;

	VkAttachmentReference				colorReference;
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference				depthReference;
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription				vsd;
	vsd.flags = 0;
	vsd.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	vsd.inputAttachmentCount = 0;
	vsd.pInputAttachments = (VkAttachmentReference*)nullptr;
	vsd.colorAttachmentCount = 1;
	vsd.pColorAttachments = &colorReference;
	vsd.pResolveAttachments = (VkAttachmentReference*)nullptr;
	vsd.pDepthStencilAttachment = &depthReference;
	vsd.preserveAttachmentCount = 0;
	vsd.pPreserveAttachments = (uint32_t*)nullptr;

	VkRenderPassCreateInfo				vrpci;
	vrpci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	vrpci.pNext = nullptr;
	vrpci.flags = 0;
	vrpci.attachmentCount = 2;		// color and depth/stencil
	vrpci.pAttachments = vad;
	vrpci.subpassCount = 1;
	vrpci.pSubpasses = &vsd;
	vrpci.dependencyCount = 0;			// ********************* ERROR ?
	vrpci.pDependencies = (VkSubpassDependency*)nullptr;

	result = vkCreateRenderPass(LogicalDevice, IN & vrpci, PALLOCATOR, OUT & RenderPass);
	REPORT("vkCreateRenderPass");
	//vgpci.renderPass = RenderPass;

	return result;
}

VkResult Vulkan::Init11Framebuffers()
{
	VkResult result = VK_SUCCESS;
	VkImageView frameBufferAttachments[2];		// color + depth/stencil

	VkFramebufferCreateInfo				vfbci;
	vfbci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	vfbci.pNext = nullptr;
	vfbci.flags = 0;
	vfbci.renderPass = RenderPass;
	vfbci.attachmentCount = 2;
	vfbci.pAttachments = frameBufferAttachments;
	vfbci.width = Width;
	vfbci.height = Height;
	vfbci.layers = 1;

	frameBufferAttachments[0] = PresentImageViews[0];
	frameBufferAttachments[1] = DepthStencilImageView;
	result = vkCreateFramebuffer(LogicalDevice, IN & vfbci, PALLOCATOR, OUT & Framebuffers[0]);
	REPORT("vkCreateFrameBuffer - 0");

	frameBufferAttachments[0] = PresentImageViews[1];
	frameBufferAttachments[1] = DepthStencilImageView;
	result = vkCreateFramebuffer(LogicalDevice, IN & vfbci, PALLOCATOR, OUT & Framebuffers[1]);
	REPORT("vkCreateFrameBuffer - 1");

	return result;
}

VkResult Vulkan::Init06CommandPools()
{
	VkResult result = VK_SUCCESS;

	{
		VkCommandPoolCreateInfo				vcpci;
		vcpci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		vcpci.pNext = nullptr;
		vcpci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		//| VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		vcpci.queueFamilyIndex = FindQueueFamilyThatDoesGraphics();

		result = vkCreateCommandPool(LogicalDevice, IN & vcpci, PALLOCATOR, OUT & GraphicsCommandPool);
		REPORT("vkCreateCommandPool -- Graphics");
	}

	{
		VkCommandPoolCreateInfo				vcpci;
		vcpci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		vcpci.pNext = nullptr;
		vcpci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		//| VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		vcpci.queueFamilyIndex = FindQueueFamilyThatDoesTransfer();

		result = vkCreateCommandPool(LogicalDevice, IN & vcpci, PALLOCATOR, OUT & TransferCommandPool);
		REPORT("vkCreateCommandPool -- Transfer");
	}

	return result;
}

VkResult Vulkan::Init06CommandBuffers()
{
	VkResult result = VK_SUCCESS;

	// allocate 2 command buffers for the double-buffered rendering:

	{
		VkCommandBufferAllocateInfo			vcbai;
		vcbai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		vcbai.pNext = nullptr;
		vcbai.commandPool = GraphicsCommandPool;
		vcbai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		vcbai.commandBufferCount = 4;		// 2, because of double-buffering

		result = vkAllocateCommandBuffers(LogicalDevice, IN & vcbai, OUT & CommandBuffers[0]);
		REPORT("vkAllocateCommandBuffers - 1");
	}


	// allocate 1 command buffer for the transfering pixels from a staging buffer to a texture buffer:

	{
		VkCommandBufferAllocateInfo			vcbai;
		vcbai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		vcbai.pNext = nullptr;
		vcbai.commandPool = TransferCommandPool;
		vcbai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		vcbai.commandBufferCount = 1;

		result = vkAllocateCommandBuffers(LogicalDevice, IN & vcbai, OUT & TextureCommandBuffer);
		REPORT("vkAllocateCommandBuffers - 2");
	}

	return result;
}

VkResult Vulkan::Init12SpirvShader(std::string filename, VkShaderModule* pShaderModule)
{
	FILE* fp;
	(void)fopen_s(&fp, filename.c_str(), "rb");
	if (fp == NULL)
	{
		fprintf(FpDebug, "Cannot open shader file '%s'\n", filename.c_str());
		return VK_SHOULD_EXIT;
	}
	uint32_t magic;
	fread(&magic, 4, 1, fp);
	if (magic != SPIRV_MAGIC)
	{
		fprintf(FpDebug, "Magic number for spir-v file '%s is 0x%08x -- should be 0x%08x\n", filename.c_str(), magic, SPIRV_MAGIC);
		return VK_SHOULD_EXIT;
	}

	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	rewind(fp);
	unsigned char* code = new unsigned char[size];
	fread(code, size, 1, fp);
	fclose(fp);

	VkShaderModuleCreateInfo		vsmci;
	vsmci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vsmci.pNext = nullptr;
	vsmci.flags = 0;
	vsmci.codeSize = size;
	vsmci.pCode = (uint32_t*)code;

	VkResult result = vkCreateShaderModule(LogicalDevice, &vsmci, PALLOCATOR, pShaderModule);
	REPORT("vkCreateShaderModule");
	fprintf(FpDebug, "Shader Module '%s' successfully loaded\n", filename.c_str());

	delete[] code;
	return result;
}

VkResult Vulkan::Init13DescriptorSetPool()
{
	VkResult result = VK_SUCCESS;

	VkDescriptorPoolSize				vdps[21];
	vdps[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vdps[0].descriptorCount = 1;
	vdps[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vdps[1].descriptorCount = 1;
	vdps[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vdps[2].descriptorCount = 1;
	vdps[3].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vdps[3].descriptorCount = 1;
	vdps[4].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vdps[4].descriptorCount = 1;
	vdps[5].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//BulletPosFlip
	vdps[5].descriptorCount = 1;
	vdps[6].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//BCKG
	vdps[6].descriptorCount = 1;
	vdps[7].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//BUG
	vdps[7].descriptorCount = 1;
	vdps[8].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//Hpos
	vdps[8].descriptorCount = 1;
	vdps[9].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//Hero
	vdps[9].descriptorCount = 1;
	vdps[10].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//HeroFlip
	vdps[10].descriptorCount = 1;
	vdps[11].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//Bugflip
	vdps[11].descriptorCount = 1;
	vdps[12].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//BulletPos
	vdps[12].descriptorCount = 1;
	vdps[13].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//Bullet
	vdps[13].descriptorCount = 1;
	vdps[14].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//BulletF
	vdps[14].descriptorCount = 1;
	vdps[15].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;//Misc obj Pos
	vdps[15].descriptorCount = 1;
	vdps[16].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//Siren
	vdps[16].descriptorCount = 1;
	vdps[17].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//Fan
	vdps[17].descriptorCount = 1;
	vdps[18].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//Start Screen BCKG
	vdps[18].descriptorCount = 1;
	vdps[19].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//Start Button
	vdps[19].descriptorCount = 1;
	vdps[20].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//Game over screen BCKG
	vdps[20].descriptorCount = 1;

#ifdef CHOICES
	VkDescriptorType :
	VK_DESCRIPTOR_TYPE_SAMPLER
		VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
		VK_DESCRIPTOR_TYPE_STORAGE_IMAGE
		VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER
		VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
		VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
#endif

		VkDescriptorPoolCreateInfo			vdpci;
	vdpci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	vdpci.pNext = nullptr;
	vdpci.flags = 0;
#ifdef CHOICES
	0
		VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT
#endif
	vdpci.maxSets = 21;
	vdpci.poolSizeCount = 21;
	vdpci.pPoolSizes = &vdps[0];

	result = vkCreateDescriptorPool(LogicalDevice, IN & vdpci, PALLOCATOR, OUT & DescriptorPool);
	REPORT("vkCreateDescriptorPool");

	return result;
}

VkResult Vulkan::Init13DescriptorSetLayouts()
{
	VkResult result = VK_SUCCESS;

	// arrays of >= 1 layouts:
	//DS #0:
	VkDescriptorSetLayoutBinding		MatrixSet[1];
	MatrixSet[0].binding = 0;
	MatrixSet[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	MatrixSet[0].descriptorCount = 1;
	MatrixSet[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	MatrixSet[0].pImmutableSamplers = (VkSampler*)nullptr;

	// DS #1:
	VkDescriptorSetLayoutBinding		LightSet[1];
	LightSet[0].binding = 0;
	LightSet[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	LightSet[0].descriptorCount = 1;
	LightSet[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	LightSet[0].pImmutableSamplers = (VkSampler*)nullptr;

	//DS #2:
	VkDescriptorSetLayoutBinding		MiscSet[1];
	MiscSet[0].binding = 0;
	MiscSet[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	MiscSet[0].descriptorCount = 1;
	MiscSet[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	MiscSet[0].pImmutableSamplers = (VkSampler*)nullptr;

	// DS #3:
	VkDescriptorSetLayoutBinding		TexSamplerSet[1];
	TexSamplerSet[0].binding = 0;
	TexSamplerSet[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	TexSamplerSet[0].descriptorCount = 1;
	TexSamplerSet[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	TexSamplerSet[0].pImmutableSamplers = (VkSampler*)nullptr;

	VkDescriptorSetLayoutCreateInfo			vdslc0;
	vdslc0.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	vdslc0.pNext = nullptr;
	vdslc0.flags = 0;
	vdslc0.bindingCount = 1;
	vdslc0.pBindings = &MatrixSet[0];

	VkDescriptorSetLayoutCreateInfo			vdslc1;
	vdslc1.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	vdslc1.pNext = nullptr;
	vdslc1.flags = 0;
	vdslc1.bindingCount = 1;
	vdslc1.pBindings = &LightSet[0];

	VkDescriptorSetLayoutCreateInfo			vdslc3;
	vdslc3.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	vdslc3.pNext = nullptr;
	vdslc3.flags = 0;
	vdslc3.bindingCount = 1;
	vdslc3.pBindings = &TexSamplerSet[0];

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc0, PALLOCATOR, OUT & DescriptorSetLayouts[0]);
	REPORT("vkCreateDescriptorSetLayout - 0");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc0, PALLOCATOR, OUT & DescriptorSetLayouts[1]);
	REPORT("vkCreateDescriptorSetLayout - 1");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc0, PALLOCATOR, OUT & DescriptorSetLayouts[2]);
	REPORT("vkCreateDescriptorSetLayout - 2");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc0, PALLOCATOR, OUT & DescriptorSetLayouts[3]);
	REPORT("vkCreateDescriptorSetLayout - 3");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc1, PALLOCATOR, OUT & DescriptorSetLayouts[4]);
	REPORT("vkCreateDescriptorSetLayout - 4");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc0, PALLOCATOR, OUT & DescriptorSetLayouts[5]);
	REPORT("vkCreateDescriptorSetLayout - 5");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc3, PALLOCATOR, OUT & DescriptorSetLayouts[6]);
	REPORT("vkCreateDescriptorSetLayout - 6");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc3, PALLOCATOR, OUT & DescriptorSetLayouts[7]);
	REPORT("vkCreateDescriptorSetLayout - 7");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc3, PALLOCATOR, OUT & DescriptorSetLayouts[7]);
	REPORT("vkCreateDescriptorSetLayout - 8");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc0, PALLOCATOR, OUT & DescriptorSetLayouts[8]);
	REPORT("vkCreateDescriptorSetLayout - 9");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc3, PALLOCATOR, OUT & DescriptorSetLayouts[9]);
	REPORT("vkCreateDescriptorSetLayout - 10");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc3, PALLOCATOR, OUT & DescriptorSetLayouts[10]);
	REPORT("vkCreateDescriptorSetLayout - 11");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc3, PALLOCATOR, OUT & DescriptorSetLayouts[11]);
	REPORT("vkCreateDescriptorSetLayout - 12");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc0, PALLOCATOR, OUT & DescriptorSetLayouts[12]);
	REPORT("vkCreateDescriptorSetLayout - 13");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc3, PALLOCATOR, OUT & DescriptorSetLayouts[13]);
	REPORT("vkCreateDescriptorSetLayout - 14");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc3, PALLOCATOR, OUT & DescriptorSetLayouts[14]);
	REPORT("vkCreateDescriptorSetLayout - 15");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc0, PALLOCATOR, OUT & DescriptorSetLayouts[15]);
	REPORT("vkCreateDescriptorSetLayout - 16");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc3, PALLOCATOR, OUT & DescriptorSetLayouts[16]);
	REPORT("vkCreateDescriptorSetLayout - 17");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc3, PALLOCATOR, OUT & DescriptorSetLayouts[17]);
	REPORT("vkCreateDescriptorSetLayout - 18");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc3, PALLOCATOR, OUT & DescriptorSetLayouts[18]);
	REPORT("vkCreateDescriptorSetLayout - 19");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc3, PALLOCATOR, OUT & DescriptorSetLayouts[19]);
	REPORT("vkCreateDescriptorSetLayout - 20");

	result = vkCreateDescriptorSetLayout(LogicalDevice, &vdslc3, PALLOCATOR, OUT & DescriptorSetLayouts[20]);
	REPORT("vkCreateDescriptorSetLayout - 21");

	return result;
}

VkResult Vulkan::Init13DescriptorSets()
{
	VkResult result = VK_SUCCESS;

	VkDescriptorSetAllocateInfo			vdsai;
	vdsai.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	vdsai.pNext = nullptr;
	vdsai.descriptorPool = DescriptorPool;
	vdsai.descriptorSetCount = 21;
	vdsai.pSetLayouts = DescriptorSetLayouts;


	result = vkAllocateDescriptorSets(LogicalDevice, IN & vdsai, OUT & DescriptorSets[0]);
	REPORT("vkAllocateDescriptorSets");

	VkDescriptorBufferInfo				vdbi0tl;
	vdbi0tl.buffer = MyMatrixUniformBuffertopleft.buffer;
	vdbi0tl.offset = 0;	// bytes
	vdbi0tl.range = sizeof(Matricestl);

	VkDescriptorBufferInfo				vdbi0bl;
	vdbi0bl.buffer = MyMatrixUniformBufferbottomleft.buffer;
	vdbi0bl.offset = 0;	// bytes
	vdbi0bl.range = sizeof(Matricesbl);

	VkDescriptorBufferInfo				vdbi0tr;
	vdbi0tr.buffer = MyMatrixUniformBuffertopright.buffer;
	vdbi0tr.offset = 0;	// bytes
	vdbi0tr.range = sizeof(Matricestr);

	VkDescriptorBufferInfo				vdbi0br;
	vdbi0br.buffer = MyMatrixUniformBufferbottomright.buffer;
	vdbi0br.offset = 0;	// bytes
	vdbi0br.range = sizeof(Matricesbr);

	VkDescriptorBufferInfo				vdbi1;
	vdbi1.buffer = MyLightUniformBuffer.buffer;
	vdbi1.offset = 0;	// bytes
	vdbi1.range = sizeof(Light);

	VkDescriptorBufferInfo				vdbi2;
	vdbi2.buffer = MyMatrixUniformBufferBulPosFlip.buffer;
	vdbi2.offset = 0;	// bytes
	vdbi2.range = sizeof(Matricesbulposflip);

	VkDescriptorImageInfo				vdii0;
	vdii0.sampler = Background.texSampler;
	vdii0.imageView = Background.texImageView;
	vdii0.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDescriptorImageInfo				vdii1;
	vdii1.sampler = Bug.texSampler;
	vdii1.imageView = Bug.texImageView;
	vdii1.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDescriptorBufferInfo				vdbi0h;
	vdbi0h.buffer = HeroPosUniformBuffer.buffer;
	vdbi0h.offset = 0;	// bytes
	vdbi0h.range = sizeof(HeroPos);

	VkDescriptorImageInfo				vdii2;
	vdii2.sampler = Hero.texSampler;
	vdii2.imageView = Hero.texImageView;
	vdii2.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDescriptorImageInfo				vdii3;
	vdii3.sampler = HeroF.texSampler;
	vdii3.imageView = HeroF.texImageView;
	vdii3.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDescriptorImageInfo				vdii4;
	vdii4.sampler = BugF.texSampler;
	vdii4.imageView = BugF.texImageView;
	vdii4.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDescriptorBufferInfo				vdbi0bp;
	vdbi0bp.buffer = MyMatrixUniformBufferBulPos.buffer;
	vdbi0bp.offset = 0;	// bytes
	vdbi0bp.range = sizeof(Matricesbulpos);

	VkDescriptorImageInfo				vdii5;
	vdii5.sampler = Bullet.texSampler;
	vdii5.imageView = Bullet.texImageView;
	vdii5.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDescriptorImageInfo				vdii6;
	vdii6.sampler = BulletF.texSampler;
	vdii6.imageView = BulletF.texImageView;
	vdii6.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDescriptorBufferInfo				vdbi0ml;
	vdbi0ml.buffer = MiscLocUniformBuffer.buffer;
	vdbi0ml.offset = 0;	// bytes
	vdbi0ml.range = sizeof(MiscLocation);

	VkDescriptorImageInfo				vdii7;
	vdii7.sampler = Siren.texSampler;
	vdii7.imageView = Siren.texImageView;
	vdii7.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDescriptorImageInfo				vdii8;
	vdii8.sampler = Fan.texSampler;
	vdii8.imageView = Fan.texImageView;
	vdii8.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDescriptorImageInfo				vdii9;
	vdii9.sampler = StartScreen.texSampler;
	vdii9.imageView = StartScreen.texImageView;
	vdii9.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDescriptorImageInfo				vdii10;
	vdii10.sampler = StartButton.texSampler;
	vdii10.imageView = StartButton.texImageView;
	vdii10.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDescriptorImageInfo				vdii11;
	vdii11.sampler = Gameoverscreen.texSampler;
	vdii11.imageView = Gameoverscreen.texImageView;
	vdii11.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkWriteDescriptorSet				vwds0tl;
	// ds 0:
	vwds0tl.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds0tl.pNext = nullptr;
	vwds0tl.dstSet = DescriptorSets[0];
	vwds0tl.dstBinding = 0;
	vwds0tl.dstArrayElement = 0;
	vwds0tl.descriptorCount = 1;
	vwds0tl.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vwds0tl.pBufferInfo = &vdbi0tl;
	vwds0tl.pImageInfo = (VkDescriptorImageInfo*)nullptr;
	vwds0tl.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds0bl;
	// ds 0:
	vwds0bl.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds0bl.pNext = nullptr;
	vwds0bl.dstSet = DescriptorSets[1];
	vwds0bl.dstBinding = 0;
	vwds0bl.dstArrayElement = 0;
	vwds0bl.descriptorCount = 1;
	vwds0bl.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vwds0bl.pBufferInfo = &vdbi0bl;
	vwds0bl.pImageInfo = (VkDescriptorImageInfo*)nullptr;
	vwds0bl.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds0tr;
	// ds 0:
	vwds0tr.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds0tr.pNext = nullptr;
	vwds0tr.dstSet = DescriptorSets[2];
	vwds0tr.dstBinding = 0;
	vwds0tr.dstArrayElement = 0;
	vwds0tr.descriptorCount = 1;
	vwds0tr.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vwds0tr.pBufferInfo = &vdbi0tr;
	vwds0tr.pImageInfo = (VkDescriptorImageInfo*)nullptr;
	vwds0tr.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds0br;
	// ds 0:
	vwds0br.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds0br.pNext = nullptr;
	vwds0br.dstSet = DescriptorSets[3];
	vwds0br.dstBinding = 0;
	vwds0br.dstArrayElement = 0;
	vwds0br.descriptorCount = 1;
	vwds0br.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vwds0br.pBufferInfo = &vdbi0br;
	vwds0br.pImageInfo = (VkDescriptorImageInfo*)nullptr;
	vwds0br.pTexelBufferView = (VkBufferView*)nullptr;

	// ds 1:
	VkWriteDescriptorSet				vwds1;
	vwds1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds1.pNext = nullptr;
	vwds1.dstSet = DescriptorSets[4];
	vwds1.dstBinding = 0;
	vwds1.dstArrayElement = 0;
	vwds1.descriptorCount = 1;
	vwds1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vwds1.pBufferInfo = &vdbi1;
	vwds1.pImageInfo = (VkDescriptorImageInfo*)nullptr;
	vwds1.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds2;
	// ds 2:
	vwds2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds2.pNext = nullptr;
	vwds2.dstSet = DescriptorSets[5];
	vwds2.dstBinding = 0;
	vwds2.dstArrayElement = 0;
	vwds2.descriptorCount = 1;
	vwds2.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vwds2.pBufferInfo = &vdbi2;
	vwds2.pImageInfo = (VkDescriptorImageInfo*)nullptr;
	vwds2.pTexelBufferView = (VkBufferView*)nullptr;

	// ds 3:
	VkWriteDescriptorSet				vwds3;
	vwds3.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds3.pNext = nullptr;
	vwds3.dstSet = DescriptorSets[6];
	vwds3.dstBinding = 0;
	vwds3.dstArrayElement = 0;
	vwds3.descriptorCount = 1;
	vwds3.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vwds3.pBufferInfo = (VkDescriptorBufferInfo*)nullptr;
	vwds3.pImageInfo = &vdii0;
	vwds3.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds4;
	vwds4.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds4.pNext = nullptr;
	vwds4.dstSet = DescriptorSets[7];
	vwds4.dstBinding = 0;
	vwds4.dstArrayElement = 0;
	vwds4.descriptorCount = 1;
	vwds4.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vwds4.pBufferInfo = (VkDescriptorBufferInfo*)nullptr;
	vwds4.pImageInfo = &vdii1;
	vwds4.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds0h;
	// ds 0:
	vwds0h.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds0h.pNext = nullptr;
	vwds0h.dstSet = DescriptorSets[8];
	vwds0h.dstBinding = 0;
	vwds0h.dstArrayElement = 0;
	vwds0h.descriptorCount = 1;
	vwds0h.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vwds0h.pBufferInfo = &vdbi0h;
	vwds0h.pImageInfo = (VkDescriptorImageInfo*)nullptr;
	vwds0h.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds5;
	vwds5.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds5.pNext = nullptr;
	vwds5.dstSet = DescriptorSets[9];
	vwds5.dstBinding = 0;
	vwds5.dstArrayElement = 0;
	vwds5.descriptorCount = 1;
	vwds5.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vwds5.pBufferInfo = (VkDescriptorBufferInfo*)nullptr;
	vwds5.pImageInfo = &vdii2;
	vwds5.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds6;
	vwds6.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds6.pNext = nullptr;
	vwds6.dstSet = DescriptorSets[10];
	vwds6.dstBinding = 0;
	vwds6.dstArrayElement = 0;
	vwds6.descriptorCount = 1;
	vwds6.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vwds6.pBufferInfo = (VkDescriptorBufferInfo*)nullptr;
	vwds6.pImageInfo = &vdii3;
	vwds6.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds7;
	vwds7.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds7.pNext = nullptr;
	vwds7.dstSet = DescriptorSets[11];
	vwds7.dstBinding = 0;
	vwds7.dstArrayElement = 0;
	vwds7.descriptorCount = 1;
	vwds7.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vwds7.pBufferInfo = (VkDescriptorBufferInfo*)nullptr;
	vwds7.pImageInfo = &vdii4;
	vwds7.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds0bp;
	vwds0bp.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds0bp.pNext = nullptr;
	vwds0bp.dstSet = DescriptorSets[12];
	vwds0bp.dstBinding = 0;
	vwds0bp.dstArrayElement = 0;
	vwds0bp.descriptorCount = 1;
	vwds0bp.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vwds0bp.pBufferInfo = &vdbi0bp;
	vwds0bp.pImageInfo = (VkDescriptorImageInfo*)nullptr;
	vwds0bp.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds8;
	vwds8.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds8.pNext = nullptr;
	vwds8.dstSet = DescriptorSets[13];
	vwds8.dstBinding = 0;
	vwds8.dstArrayElement = 0;
	vwds8.descriptorCount = 1;
	vwds8.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vwds8.pBufferInfo = (VkDescriptorBufferInfo*)nullptr;
	vwds8.pImageInfo = &vdii5;
	vwds8.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds9;
	vwds9.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds9.pNext = nullptr;
	vwds9.dstSet = DescriptorSets[14];
	vwds9.dstBinding = 0;
	vwds9.dstArrayElement = 0;
	vwds9.descriptorCount = 1;
	vwds9.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vwds9.pBufferInfo = (VkDescriptorBufferInfo*)nullptr;
	vwds9.pImageInfo = &vdii6;
	vwds9.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds0ml;
	vwds0ml.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds0ml.pNext = nullptr;
	vwds0ml.dstSet = DescriptorSets[15];
	vwds0ml.dstBinding = 0;
	vwds0ml.dstArrayElement = 0;
	vwds0ml.descriptorCount = 1;
	vwds0ml.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vwds0ml.pBufferInfo = &vdbi0ml;
	vwds0ml.pImageInfo = (VkDescriptorImageInfo*)nullptr;
	vwds0ml.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds10;
	vwds10.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds10.pNext = nullptr;
	vwds10.dstSet = DescriptorSets[16];
	vwds10.dstBinding = 0;
	vwds10.dstArrayElement = 0;
	vwds10.descriptorCount = 1;
	vwds10.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vwds10.pBufferInfo = (VkDescriptorBufferInfo*)nullptr;
	vwds10.pImageInfo = &vdii7;
	vwds10.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds11;
	vwds11.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds11.pNext = nullptr;
	vwds11.dstSet = DescriptorSets[17];
	vwds11.dstBinding = 0;
	vwds11.dstArrayElement = 0;
	vwds11.descriptorCount = 1;
	vwds11.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vwds11.pBufferInfo = (VkDescriptorBufferInfo*)nullptr;
	vwds11.pImageInfo = &vdii8;
	vwds11.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds12;
	vwds12.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds12.pNext = nullptr;
	vwds12.dstSet = DescriptorSets[18];
	vwds12.dstBinding = 0;
	vwds12.dstArrayElement = 0;
	vwds12.descriptorCount = 1;
	vwds12.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vwds12.pBufferInfo = (VkDescriptorBufferInfo*)nullptr;
	vwds12.pImageInfo = &vdii9;
	vwds12.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds13;
	vwds13.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds13.pNext = nullptr;
	vwds13.dstSet = DescriptorSets[19];
	vwds13.dstBinding = 0;
	vwds13.dstArrayElement = 0;
	vwds13.descriptorCount = 1;
	vwds13.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vwds13.pBufferInfo = (VkDescriptorBufferInfo*)nullptr;
	vwds13.pImageInfo = &vdii10;
	vwds13.pTexelBufferView = (VkBufferView*)nullptr;

	VkWriteDescriptorSet				vwds14;
	vwds14.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	vwds14.pNext = nullptr;
	vwds14.dstSet = DescriptorSets[20];
	vwds14.dstBinding = 0;
	vwds14.dstArrayElement = 0;
	vwds14.descriptorCount = 1;
	vwds14.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vwds14.pBufferInfo = (VkDescriptorBufferInfo*)nullptr;
	vwds14.pImageInfo = &vdii11;
	vwds14.pTexelBufferView = (VkBufferView*)nullptr;

	uint32_t copyCount = 0;

	// this could have been done with one call and an array of VkWriteDescriptorSets:

	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds0tl, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds0bl, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds0tr, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds0br, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds1, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds2, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds3, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds4, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds0h, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds5, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds6, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds7, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds0bp, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds8, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds9, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds0ml, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds10, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds11, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds12, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds13, IN copyCount, (VkCopyDescriptorSet*)nullptr);
	vkUpdateDescriptorSets(LogicalDevice, 1, IN & vwds14, IN copyCount, (VkCopyDescriptorSet*)nullptr);

	return VK_SUCCESS;
}

VkResult Vulkan::Init14GraphicsPipelineLayout()
{
	VkResult result = VK_SUCCESS;

	VkPipelineLayoutCreateInfo			vplci;
	vplci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	vplci.pNext = nullptr;
	vplci.flags = 0;
	vplci.setLayoutCount = 21;
	vplci.pSetLayouts = &DescriptorSetLayouts[0];
	vplci.pushConstantRangeCount = 0;
	vplci.pPushConstantRanges = (VkPushConstantRange*)nullptr;

	result = vkCreatePipelineLayout(LogicalDevice, IN & vplci, PALLOCATOR, OUT & GraphicsPipelineLayout);
	REPORT("vkCreatePipelineLayout");

	return result;
}

VkResult Vulkan::Init14GraphicsVertexFragmentPipeline(VkShaderModule vertexShader[], VkShaderModule fragmentShader[], VkPrimitiveTopology topology, OUT VkPipeline pGraphicsPipeline[])
{
	VkResult result = VK_SUCCESS;

	VkPipelineLayoutCreateInfo			vplci;
	vplci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	vplci.pNext = nullptr;
	vplci.flags = 0;
	vplci.setLayoutCount = 21;
	vplci.pSetLayouts = &DescriptorSetLayouts[0];
	vplci.pushConstantRangeCount = 0;
	vplci.pPushConstantRanges = (VkPushConstantRange*)nullptr;

	result = vkCreatePipelineLayout(LogicalDevice, IN & vplci, PALLOCATOR, OUT & GraphicsPipelineLayout);
	REPORT("vkCreatePipelineLayout: 1");

	VkPipelineShaderStageCreateInfo				vpssci1[2];
	vpssci1[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci1[0].pNext = nullptr;
	vpssci1[0].flags = 0;
	vpssci1[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	vpssci1[0].module = vertexShader[0];
	vpssci1[0].pName = "main";
	vpssci1[0].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	vpssci1[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci1[1].pNext = nullptr;
	vpssci1[1].flags = 0;
	vpssci1[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vpssci1[1].module = fragmentShader[0];
	vpssci1[1].pName = "main";
	vpssci1[1].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	VkPipelineShaderStageCreateInfo				vpssci2[2];

	vpssci2[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci2[0].pNext = nullptr;
	vpssci2[0].flags = 0;
	vpssci2[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	vpssci2[0].module = vertexShader[1];
	vpssci2[0].pName = "main";
	vpssci2[0].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	vpssci2[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci2[1].pNext = nullptr;
	vpssci2[1].flags = 0;
	vpssci2[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vpssci2[1].module = fragmentShader[1];
	vpssci2[1].pName = "main";
	vpssci2[1].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	VkPipelineShaderStageCreateInfo				vpssci3[2];

	vpssci3[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci3[0].pNext = nullptr;
	vpssci3[0].flags = 0;
	vpssci3[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	vpssci3[0].module = vertexShader[2];
	vpssci3[0].pName = "main";
	vpssci3[0].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	vpssci3[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci3[1].pNext = nullptr;
	vpssci3[1].flags = 0;
	vpssci3[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vpssci3[1].module = fragmentShader[2];
	vpssci3[1].pName = "main";
	vpssci3[1].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	VkPipelineShaderStageCreateInfo				vpssci4[2];

	vpssci4[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci4[0].pNext = nullptr;
	vpssci4[0].flags = 0;
	vpssci4[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	vpssci4[0].module = vertexShader[3];
	vpssci4[0].pName = "main";
	vpssci4[0].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	vpssci4[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci4[1].pNext = nullptr;
	vpssci4[1].flags = 0;
	vpssci4[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vpssci4[1].module = fragmentShader[3];
	vpssci4[1].pName = "main";
	vpssci4[1].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	VkPipelineShaderStageCreateInfo				vpssci5[2];

	vpssci5[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci5[0].pNext = nullptr;
	vpssci5[0].flags = 0;
	vpssci5[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	vpssci5[0].module = vertexShader[4];
	vpssci5[0].pName = "main";
	vpssci5[0].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	vpssci5[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci5[1].pNext = nullptr;
	vpssci5[1].flags = 0;
	vpssci5[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vpssci5[1].module = fragmentShader[4];
	vpssci5[1].pName = "main";
	vpssci5[1].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	VkPipelineShaderStageCreateInfo				vpssci6[2];

	vpssci6[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci6[0].pNext = nullptr;
	vpssci6[0].flags = 0;
	vpssci6[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	vpssci6[0].module = vertexShader[5];
	vpssci6[0].pName = "main";
	vpssci6[0].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	vpssci6[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci6[1].pNext = nullptr;
	vpssci6[1].flags = 0;
	vpssci6[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vpssci6[1].module = fragmentShader[5];
	vpssci6[1].pName = "main";
	vpssci6[1].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	VkPipelineShaderStageCreateInfo				vpssci7[2];//Start

	vpssci7[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci7[0].pNext = nullptr;
	vpssci7[0].flags = 0;
	vpssci7[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	vpssci7[0].module = vertexShader[6];
	vpssci7[0].pName = "main";
	vpssci7[0].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	vpssci7[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci7[1].pNext = nullptr;
	vpssci7[1].flags = 0;
	vpssci7[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vpssci7[1].module = fragmentShader[6];
	vpssci7[1].pName = "main";
	vpssci7[1].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	VkPipelineShaderStageCreateInfo				vpssci8[2];

	vpssci8[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci8[0].pNext = nullptr;
	vpssci8[0].flags = 0;
	vpssci8[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	vpssci8[0].module = vertexShader[7];
	vpssci8[0].pName = "main";
	vpssci8[0].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	vpssci8[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci8[1].pNext = nullptr;
	vpssci8[1].flags = 0;
	vpssci8[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vpssci8[1].module = fragmentShader[7];
	vpssci8[1].pName = "main";
	vpssci8[1].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	VkPipelineShaderStageCreateInfo				vpssci9[2];

	vpssci9[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci9[0].pNext = nullptr;
	vpssci9[0].flags = 0;
	vpssci9[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	vpssci9[0].module = vertexShader[8];
	vpssci9[0].pName = "main";
	vpssci9[0].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	vpssci9[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci9[1].pNext = nullptr;
	vpssci9[1].flags = 0;
	vpssci9[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vpssci9[1].module = fragmentShader[8];
	vpssci9[1].pName = "main";
	vpssci9[1].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	VkPipelineShaderStageCreateInfo				vpssci10[2];

	vpssci10[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci10[0].pNext = nullptr;
	vpssci10[0].flags = 0;
	vpssci10[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	vpssci10[0].module = vertexShader[9];
	vpssci10[0].pName = "main";
	vpssci10[0].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	vpssci10[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci10[1].pNext = nullptr;
	vpssci10[1].flags = 0;
	vpssci10[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vpssci10[1].module = fragmentShader[9];
	vpssci10[1].pName = "main";
	vpssci10[1].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	VkPipelineShaderStageCreateInfo				vpssci11[2];

	vpssci11[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci11[0].pNext = nullptr;
	vpssci11[0].flags = 0;
	vpssci11[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	vpssci11[0].module = vertexShader[10];
	vpssci11[0].pName = "main";
	vpssci11[0].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	vpssci11[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci11[1].pNext = nullptr;
	vpssci11[1].flags = 0;
	vpssci11[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vpssci11[1].module = fragmentShader[10];
	vpssci11[1].pName = "main";
	vpssci11[1].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	VkPipelineShaderStageCreateInfo				vpssci12[2];

	vpssci12[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci12[0].pNext = nullptr;
	vpssci12[0].flags = 0;
	vpssci12[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	vpssci12[0].module = vertexShader[11];
	vpssci12[0].pName = "main";
	vpssci12[0].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	vpssci12[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci12[1].pNext = nullptr;
	vpssci12[1].flags = 0;
	vpssci12[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vpssci12[1].module = fragmentShader[11];
	vpssci12[1].pName = "main";
	vpssci12[1].pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	VkVertexInputBindingDescription			vvibd[1];	// an array containing one of these per buffer being used
	vvibd[0].binding = 0;		// which binding # this is
	vvibd[0].stride = sizeof(struct vertex);		// bytes between successive 
	vvibd[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
#ifdef CHOICES
	VK_VERTEX_INPUT_RATE_VERTEX
		VK_VERTEX_INPUT_RATE_INSTANCE
#endif

#ifdef COMMENT
		struct vertex
	{
		glm::vec3	position;
		glm::vec3	normal;
		glm::vec3	color;
		glm::vec2	texCoord;
	} Vertices;
#endif
	VkVertexInputAttributeDescription		vviad[4];		// an array containing one of these per vertex attribute in all bindings
		// 4 = vertex, normal, color, texture coord
	vviad[0].location = 0;			// location in the layout decoration
	vviad[0].binding = 0;			// which binding description this is part of
	vviad[0].format = VK_FORMAT_VEC3;	// x, y, z
	vviad[0].offset = offsetof(struct vertex, position);			// 0
#ifdef EXTRAS_DEFINED_AT_THE_TOP
	VK_FORMAT_VEC4 = VK_FORMAT_R32G32B32A32_SFLOAT
		VK_FORMAT_XYZW = VK_FORMAT_R32G32B32A32_SFLOAT
		VK_FORMAT_VEC3 = VK_FORMAT_R32G32B32_SFLOAT
		VK_FORMAT_STP = VK_FORMAT_R32G32B32_SFLOAT
		VK_FORMAT_XYZ = VK_FORMAT_R32G32B32_SFLOAT
		VK_FORMAT_VEC2 = VK_FORMAT_R32G32_SFLOAT
		VK_FORMAT_ST = VK_FORMAT_R32G32_SFLOAT
		VK_FORMAT_XY = VK_FORMAT_R32G32_SFLOAT
		VK_FORMAT_FLOAT = VK_FORMAT_R32_SFLOAT
		VK_FORMAT_S = VK_FORMAT_R32_SFLOAT
		VK_FORMAT_X = VK_FORMAT_R32_SFLOAT
#endif

		vviad[1].location = 1;
	vviad[1].binding = 0;
	vviad[1].format = VK_FORMAT_VEC3;	// nx, ny, nz
	vviad[1].offset = offsetof(struct vertex, normal);			// 12

	vviad[2].location = 2;
	vviad[2].binding = 0;
	vviad[2].format = VK_FORMAT_VEC3;	// r, g, b
	vviad[2].offset = offsetof(struct vertex, color);			// 24

	vviad[3].location = 3;
	vviad[3].binding = 0;
	vviad[3].format = VK_FORMAT_VEC2;	// s, t
	vviad[3].offset = offsetof(struct vertex, texCoord);			// 36


	VkPipelineVertexInputStateCreateInfo			vpvisci;			// used to describe the input vertex attributes
	vpvisci.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vpvisci.pNext = nullptr;
	vpvisci.flags = 0;
	vpvisci.vertexBindingDescriptionCount = 1;
	vpvisci.pVertexBindingDescriptions = vvibd;
	vpvisci.vertexAttributeDescriptionCount = 4;
	vpvisci.pVertexAttributeDescriptions = vviad;

	VkPipelineInputAssemblyStateCreateInfo			vpiasci;
	vpiasci.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	vpiasci.pNext = nullptr;
	vpiasci.flags = 0;
	vpiasci.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
#ifdef CHOICES
	VK_PRIMITIVE_TOPOLOGY_POINT_LIST
		VK_PRIMITIVE_TOPOLOGY_LINE_LIST
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
		VK_PRIMITIVE_TOPOLOGY_LINE_STRIP
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN
		VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY
		VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY
#endif
		vpiasci.primitiveRestartEnable = VK_FALSE;

	VkPipelineTessellationStateCreateInfo			vptsci;
	vptsci.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	vptsci.pNext = nullptr;
	vptsci.flags = 0;
	vptsci.patchControlPoints = 0;		// number of patch control points

// VkPipelineGeometryStateCreateInfo			vpgsci;
	// vptsci.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	// vptsci.pNext = nullptr;
	// vptsci.flags = 0;

	VkViewport						vv;
	vv.x = 0;
	vv.y = 0;
	vv.width = (float)Width;
	vv.height = (float)Height;
	vv.minDepth = 0.0f;
	vv.maxDepth = 1.0f;

	// scissoring:
	VkRect2D						vr;
	vr.offset.x = 0;
	vr.offset.y = 0;
	vr.extent.width = Width;
	vr.extent.height = Height;

	VkPipelineViewportStateCreateInfo			vpvsci;
	vpvsci.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	vpvsci.pNext = nullptr;
	vpvsci.flags = 0;
	vpvsci.viewportCount = 1;
	vpvsci.pViewports = &vv;
	vpvsci.scissorCount = 1;
	vpvsci.pScissors = &vr;

	VkPipelineRasterizationStateCreateInfo			vprsci;
	vprsci.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	vprsci.pNext = nullptr;
	vprsci.flags = 0;
	vprsci.depthClampEnable = VK_FALSE;
	vprsci.rasterizerDiscardEnable = VK_FALSE;
	vprsci.polygonMode = VK_POLYGON_MODE_FILL;
#ifdef CHOICES
	VK_POLYGON_MODE_FILL
		VK_POLYGON_MODE_LINE
		VK_POLYGON_MODE_POINT
#endif
		vprsci.cullMode = VK_CULL_MODE_NONE;	// recommend this because of the projMatrix[1][1] *= -1.;
#ifdef CHOICES
	VK_CULL_MODE_NONE
		VK_CULL_MODE_FRONT_BIT
		VK_CULL_MODE_BACK_BIT
		VK_CULL_MODE_FRONT_AND_BACK_BIT
#endif
		vprsci.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
#ifdef CHOICES
	VK_FRONT_FACE_COUNTER_CLOCKWISE
		VK_FRONT_FACE_CLOCKWISE
#endif
		vprsci.depthBiasEnable = VK_FALSE;
	vprsci.depthBiasConstantFactor = 0.f;
	vprsci.depthBiasClamp = 0.f;
	vprsci.depthBiasSlopeFactor = 0.f;
	vprsci.lineWidth = 1.f;

	VkPipelineMultisampleStateCreateInfo			vpmsci;
	vpmsci.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	vpmsci.pNext = nullptr;
	vpmsci.flags = 0;
	vpmsci.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	vpmsci.sampleShadingEnable = VK_FALSE;
	vpmsci.minSampleShading = 0;
	vpmsci.pSampleMask = (VkSampleMask*)nullptr;
	vpmsci.alphaToCoverageEnable = VK_FALSE;
	vpmsci.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState			vpcbas;
	vpcbas.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
		| VK_COLOR_COMPONENT_G_BIT
		| VK_COLOR_COMPONENT_B_BIT
		| VK_COLOR_COMPONENT_A_BIT;
	vpcbas.blendEnable = VK_TRUE;
	vpcbas.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	vpcbas.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	vpcbas.colorBlendOp = VK_BLEND_OP_ADD;
	vpcbas.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	vpcbas.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	vpcbas.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo			vpcbsci;
	vpcbsci.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	vpcbsci.pNext = nullptr;
	vpcbsci.flags = 0;
	vpcbsci.logicOpEnable = VK_FALSE;
	vpcbsci.logicOp = VK_LOGIC_OP_COPY;
#ifdef CHOICES
	VK_LOGIC_OP_CLEAR
		VK_LOGIC_OP_AND
		VK_LOGIC_OP_AND_REVERSE
		VK_LOGIC_OP_COPY
		VK_LOGIC_OP_AND_INVERTED
		VK_LOGIC_OP_NO_OP
		VK_LOGIC_OP_XOR
		VK_LOGIC_OP_OR
		VK_LOGIC_OP_NOR
		VK_LOGIC_OP_EQUIVALENT
		VK_LOGIC_OP_INVERT
		VK_LOGIC_OP_OR_REVERSE
		VK_LOGIC_OP_COPY_INVERTED
		VK_LOGIC_OP_OR_INVERTED
		VK_LOGIC_OP_NAND
		VK_LOGIC_OP_SET
#endif
		vpcbsci.attachmentCount = 1;
	vpcbsci.pAttachments = &vpcbas;
	vpcbsci.blendConstants[0] = 0;
	vpcbsci.blendConstants[1] = 0;
	vpcbsci.blendConstants[2] = 0;
	vpcbsci.blendConstants[3] = 0;

#ifdef EXAMPLE_OF_USING_DYNAMIC_STATE_VARIABLES
	VkDynamicState					vds[2];
	vds[0] = VK_DYNAMIC_STATE_VIEWPORT;
	vds[1] = VK_DYNAMIC_STATE_SCISSOR;
#endif

#ifdef CHOICES
	VK_DYNAMIC_STATE_VIEWPORT	--	vkCmdSetViewort()
		VK_DYNAMIC_STATE_SCISSOR	--	vkCmdSetScissor()
		VK_DYNAMIC_STATE_LINE_WIDTH	--	vkCmdSetLineWidth()
		VK_DYNAMIC_STATE_DEPTH_BIAS	--	vkCmdSetDepthBias()
		VK_DYNAMIC_STATE_BLEND_CONSTANTS	--	vkCmdSetBendConstants()
		VK_DYNAMIC_STATE_DEPTH_BOUNDS	--	vkCmdSetDepthZBounds()
		VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK	--	vkCmdSetStencilCompareMask()
		VK_DYNAMIC_STATE_STENCIL_WRITE_MASK	--	vkCmdSetStencilWriteMask()
		VK_DYNAMIC_STATE_STENCIL_REFERENCE	--	vkCmdSetStencilReferences()
#endif
		VkPipelineDynamicStateCreateInfo			vpdsci;
	vpdsci.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	vpdsci.pNext = nullptr;
	vpdsci.flags = 0;
	vpdsci.dynamicStateCount = 0;			// leave turned off for now
	vpdsci.pDynamicStates = (VkDynamicState*) nullptr;
#ifdef EXAMPLE_OF_USING_DYNAMIC_STATE_VARIABLES
	vpdsci.dynamicStateCount = 2;
	vpdsci.pDynamicStates = vds;
#endif

	VkStencilOpState					vsosf;	// front
	vsosf.failOp = VK_STENCIL_OP_KEEP;
	vsosf.passOp = VK_STENCIL_OP_KEEP;
	vsosf.depthFailOp = VK_STENCIL_OP_KEEP;
#ifdef CHOICES
	VK_STENCIL_OP_KEEP
		VK_STENCIL_OP_ZERO
		VK_STENCIL_OP_REPLACE
		VK_STENCIL_OP_INCREMENT_AND_CLAMP
		VK_STENCIL_OP_DECREMENT_AND_CLAMP
		VK_STENCIL_OP_INVERT
		VK_STENCIL_OP_INCREMENT_AND_WRAP
		VK_STENCIL_OP_DECREMENT_AND_WRAP
#endif
		vsosf.compareOp = VK_COMPARE_OP_NEVER;
#ifdef CHOICES
	VK_COMPARE_OP_NEVER
		VK_COMPARE_OP_LESS
		VK_COMPARE_OP_EQUAL
		VK_COMPARE_OP_LESS_OR_EQUAL
		VK_COMPARE_OP_GREATER
		VK_COMPARE_OP_NOT_EQUAL
		VK_COMPARE_OP_GREATER_OR_EQUAL
		VK_COMPARE_OP_ALWAYS
#endif
		vsosf.compareMask = ~0;
	vsosf.writeMask = ~0;
	vsosf.reference = 0;

	VkStencilOpState					vsosb;	// back
	vsosb.failOp = VK_STENCIL_OP_KEEP;
	vsosb.passOp = VK_STENCIL_OP_KEEP;
	vsosb.depthFailOp = VK_STENCIL_OP_KEEP;
	vsosb.compareOp = VK_COMPARE_OP_NEVER;
	vsosb.compareMask = ~0;
	vsosb.writeMask = ~0;
	vsosb.reference = 0;

	VkPipelineDepthStencilStateCreateInfo			vpdssci;
	vpdssci.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	vpdssci.pNext = nullptr;
	vpdssci.flags = 0;
	vpdssci.depthTestEnable = VK_TRUE;
	vpdssci.depthWriteEnable = VK_TRUE;
	vpdssci.depthCompareOp = VK_COMPARE_OP_LESS;
#ifdef CHOICES
	VK_COMPARE_OP_NEVER
		VK_COMPARE_OP_LESS
		VK_COMPARE_OP_EQUAL
		VK_COMPARE_OP_LESS_OR_EQUAL
		VK_COMPARE_OP_GREATER
		VK_COMPARE_OP_NOT_EQUAL
		VK_COMPARE_OP_GREATER_OR_EQUAL
		VK_COMPARE_OP_ALWAYS
#endif
		vpdssci.depthBoundsTestEnable = VK_FALSE;
	vpdssci.front = vsosf;
	vpdssci.back = vsosb;
	vpdssci.minDepthBounds = 0.;
	vpdssci.maxDepthBounds = 1.;
	vpdssci.stencilTestEnable = VK_FALSE;



	VkGraphicsPipelineCreateInfo				vgpci[12];
	vgpci[0].sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	vgpci[0].pNext = nullptr;
	vgpci[0].flags = 0;
#ifdef CHOICES
	VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT
		VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT
		VK_PIPELINE_CREATE_DERIVATIVE_BIT
#endif
		vgpci[0].stageCount = 2;				// number of stages in this pipeline
	vgpci[0].pStages = vpssci1;
	vgpci[0].pVertexInputState = &vpvisci;
	vgpci[0].pInputAssemblyState = &vpiasci;
	vgpci[0].pTessellationState = (VkPipelineTessellationStateCreateInfo*)nullptr;		// &vptsci
	vgpci[0].pViewportState = &vpvsci;
	vgpci[0].pRasterizationState = &vprsci;
	vgpci[0].pMultisampleState = &vpmsci;
	vgpci[0].pDepthStencilState = &vpdssci;
	vgpci[0].pColorBlendState = &vpcbsci;
	vgpci[0].pDynamicState = (VkPipelineDynamicStateCreateInfo*) nullptr;
	//vgpci.pDynamicState = &vpdsci;
	vgpci[0].layout = IN GraphicsPipelineLayout;
	vgpci[0].renderPass = IN RenderPass;
	vgpci[0].subpass = 0;				// subpass number
	vgpci[0].basePipelineHandle = (VkPipeline)VK_NULL_HANDLE;
	vgpci[0].basePipelineIndex = 0;

	vgpci[1].sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	vgpci[1].pNext = nullptr;
	vgpci[1].flags = 0;
#ifdef CHOICES
	VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT
		VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT
		VK_PIPELINE_CREATE_DERIVATIVE_BIT
#endif
	vgpci[1].stageCount = 2;				// number of stages in this pipeline
	vgpci[1].pStages = vpssci2;
	vgpci[1].pVertexInputState = &vpvisci;
	vgpci[1].pInputAssemblyState = &vpiasci;
	vgpci[1].pTessellationState = (VkPipelineTessellationStateCreateInfo*)nullptr;		// &vptsci
	vgpci[1].pViewportState = &vpvsci;
	vgpci[1].pRasterizationState = &vprsci;
	vgpci[1].pMultisampleState = &vpmsci;
	vgpci[1].pDepthStencilState = &vpdssci;
	vgpci[1].pColorBlendState = &vpcbsci;
	vgpci[1].pDynamicState = (VkPipelineDynamicStateCreateInfo*) nullptr;
	//vgpci.pDynamicState = &vpdsci;
	vgpci[1].layout = IN GraphicsPipelineLayout;
	vgpci[1].renderPass = IN RenderPass;
	vgpci[1].subpass = 0;				// subpass number
	vgpci[1].basePipelineHandle = (VkPipeline)VK_NULL_HANDLE;
	vgpci[1].basePipelineIndex = 0;

	vgpci[2] = vgpci[0];
	vgpci[2].pStages = vpssci3;
	vgpci[3] = vgpci[0];
	vgpci[3].pStages = vpssci4;
	vgpci[4] = vgpci[0];
	vgpci[4].pStages = vpssci5;
	vgpci[5] = vgpci[0];
	vgpci[5].pStages = vpssci6;
	vgpci[6] = vgpci[0];
	vgpci[6].pStages = vpssci7;
	vgpci[7] = vgpci[0];
	vgpci[7].pStages = vpssci8;
	vgpci[8] = vgpci[0];
	vgpci[8].pStages = vpssci9;
	vgpci[9] = vgpci[0];
	vgpci[9].pStages = vpssci10;
	vgpci[10] = vgpci[0];
	vgpci[10].pStages = vpssci11;
	vgpci[11] = vgpci[0];
	vgpci[11].pStages = vpssci12;

	result = vkCreateGraphicsPipelines(LogicalDevice, VK_NULL_HANDLE, 1, IN & vgpci[0], PALLOCATOR, OUT &pGraphicsPipeline[0]);
	REPORT("vkCreateGraphicsPipelines: 1");


	result = vkCreateGraphicsPipelines(LogicalDevice, VK_NULL_HANDLE, 1, IN & vgpci[1], PALLOCATOR, OUT &pGraphicsPipeline[1]);
	REPORT("vkCreateGraphicsPipelines: 2");

	result = vkCreateGraphicsPipelines(LogicalDevice, VK_NULL_HANDLE, 1, IN & vgpci[2], PALLOCATOR, OUT & pGraphicsPipeline[2]);
	REPORT("vkCreateGraphicsPipelines: 3");

	result = vkCreateGraphicsPipelines(LogicalDevice, VK_NULL_HANDLE, 1, IN & vgpci[3], PALLOCATOR, OUT & pGraphicsPipeline[3]);
	REPORT("vkCreateGraphicsPipelines: 4");

	result = vkCreateGraphicsPipelines(LogicalDevice, VK_NULL_HANDLE, 1, IN & vgpci[4], PALLOCATOR, OUT & pGraphicsPipeline[4]);
	REPORT("vkCreateGraphicsPipelines: 5");

	result = vkCreateGraphicsPipelines(LogicalDevice, VK_NULL_HANDLE, 1, IN & vgpci[5], PALLOCATOR, OUT & pGraphicsPipeline[5]);
	REPORT("vkCreateGraphicsPipelines: 6");

	result = vkCreateGraphicsPipelines(LogicalDevice, VK_NULL_HANDLE, 1, IN & vgpci[6], PALLOCATOR, OUT & pGraphicsPipeline[6]);
	REPORT("vkCreateGraphicsPipelines: 7");

	result = vkCreateGraphicsPipelines(LogicalDevice, VK_NULL_HANDLE, 1, IN & vgpci[7], PALLOCATOR, OUT & pGraphicsPipeline[7]);
	REPORT("vkCreateGraphicsPipelines: 8");

	result = vkCreateGraphicsPipelines(LogicalDevice, VK_NULL_HANDLE, 1, IN & vgpci[8], PALLOCATOR, OUT & pGraphicsPipeline[8]);
	REPORT("vkCreateGraphicsPipelines: 9");

	result = vkCreateGraphicsPipelines(LogicalDevice, VK_NULL_HANDLE, 1, IN & vgpci[9], PALLOCATOR, OUT & pGraphicsPipeline[9]);
	REPORT("vkCreateGraphicsPipelines: 10");

	result = vkCreateGraphicsPipelines(LogicalDevice, VK_NULL_HANDLE, 1, IN & vgpci[10], PALLOCATOR, OUT & pGraphicsPipeline[10]);
	REPORT("vkCreateGraphicsPipelines: 11");

	result = vkCreateGraphicsPipelines(LogicalDevice, VK_NULL_HANDLE, 1, IN & vgpci[11], PALLOCATOR, OUT & pGraphicsPipeline[11]);
	REPORT("vkCreateGraphicsPipelines: 12");

	return result;//Done till here!!!
}

VkResult Vulkan::Init14ComputePipeline(VkShaderModule computeShader, OUT VkPipeline* pComputePipeline)
{
	VkResult result = VK_SUCCESS;

	VkPipelineShaderStageCreateInfo			vpssci;
	vpssci.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vpssci.pNext = nullptr;
	vpssci.flags = 0;
	vpssci.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	vpssci.module = computeShader;
	vpssci.pName = "main";
	vpssci.pSpecializationInfo = (VkSpecializationInfo*)nullptr;

	VkPipelineLayoutCreateInfo				vplci;
	vplci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	vplci.pNext = nullptr;
	vplci.flags = 0;
	vplci.setLayoutCount = 1;
	vplci.pSetLayouts = DescriptorSetLayouts;
	vplci.pushConstantRangeCount = 0;
	vplci.pPushConstantRanges = (VkPushConstantRange*)nullptr;

	result = vkCreatePipelineLayout(LogicalDevice, IN & vplci, PALLOCATOR, OUT & ComputePipelineLayout);
	REPORT("vkCreatePipelineLayout");

	VkComputePipelineCreateInfo			vcpci[1];
	vcpci[0].sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	vcpci[0].pNext = nullptr;
	vcpci[0].flags = 0;
	vcpci[0].stage = vpssci;
	vcpci[0].layout = ComputePipelineLayout;
	vcpci[0].basePipelineHandle = VK_NULL_HANDLE;
	vcpci[0].basePipelineIndex = 0;

	result = vkCreateComputePipelines(LogicalDevice, VK_NULL_HANDLE, 1, &vcpci[0], PALLOCATOR, pComputePipeline);
	REPORT("vkCreateComputePipelines");
	return result;
}

VkResult Vulkan::InitFence()
{
	VkFenceCreateInfo			vfci;
	vfci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	vfci.pNext = nullptr;
	vfci.flags = 0;

	vkCreateFence(LogicalDevice, &vfci, PALLOCATOR, &Fence);


	VkSubmitInfo				vsi;
	vsi.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	vsi.pNext = nullptr;
	vsi.waitSemaphoreCount = 0;					// ??? uint32_t
	vsi.pWaitSemaphores = (VkSemaphore*)nullptr;			// ??????
	vsi.pWaitDstStageMask = (VkPipelineStageFlags*)nullptr;	// ?????
	vsi.commandBufferCount = 1;
	vsi.pCommandBuffers = CommandBuffers;
	vsi.signalSemaphoreCount = 0;					// ?????
	vsi.pSignalSemaphores = (VkSemaphore*)nullptr;			// ?????

	VkResult result = vkQueueSubmit(Queue, 1, IN & vsi, IN Fence);
	// Fence can be VK_NULL_HANDLE if have no fence
	REPORT("vkQueueSubmit");

#ifdef SAMPLE_CODE
	result = vkWaitForFences(LogicalDevice, 1, pFences, VK_TRUE, timeout);
	REPORT("vkWaitForFences");
#endif

	return result;
}

void Vulkan::PrintVkError(VkResult result, std::string prefix)
{
	if (Verbose && result == VK_SUCCESS)
	{
		fprintf(FpDebug, "%s: %s\n", prefix.c_str(), "Successful");
		fflush(FpDebug);
		return;
	}

	const int numErrorCodes = sizeof(ErrorCodes) / sizeof(struct errorcode);
	std::string meaning = "";
	for (int i = 0; i < numErrorCodes; i++)
	{
		if (result == ErrorCodes[i].resultCode)
		{
			meaning = ErrorCodes[i].meaning;
			break;
		}
	}

	fprintf(FpDebug, "\n%s: %s\n", prefix.c_str(), meaning.c_str());
	fflush(FpDebug);
}

VkResult Vulkan::InitSemaphore()
{
	VkResult result = VK_SUCCESS;

	VkSemaphoreCreateInfo			vsci;
	vsci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	vsci.pNext = nullptr;
	vsci.flags = 0;

	result = vkCreateSemaphore(LogicalDevice, IN & vsci, PALLOCATOR, OUT & SemaphoreImageAvailable);
	REPORT("vkCreateSemaphore -- image available");

	result = vkCreateSemaphore(LogicalDevice, IN & vsci, PALLOCATOR, OUT & SemaphoreRenderFinished);
	REPORT("vkCreateSemaphore -- render finished");

	// vkQueueSubmit waits for one set of semaphores and signals another
	// Can have 2 queues, one for compute and one for graphics
		// Graphics Queue can wait on signal from Compute Queue
		// Then, Compute Queue can wait on signal from Graphics Queue

	return result;
}

VkResult Vulkan::DestroyAllVulkan()
{
	VkResult result = VK_SUCCESS;

	result = vkDeviceWaitIdle(LogicalDevice);
	REPORT("vkWaitIdle");

	return result;
}

int Vulkan::FindMemoryThatIsDeviceLocal(uint32_t memoryTypeBits)
{
	return Vulkan::FindMemoryByFlagAndType(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memoryTypeBits);
}


int Vulkan::FindMemoryThatIsHostVisible(uint32_t memoryTypeBits)
{
	return Vulkan::FindMemoryByFlagAndType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, memoryTypeBits);
}


int Vulkan::FindMemoryByFlagAndType(VkMemoryPropertyFlagBits memoryFlagBits, uint32_t  memoryTypeBits)
{
	VkPhysicalDeviceMemoryProperties	vpdmp;
	vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, OUT & vpdmp);
	for (unsigned int i = 0; i < vpdmp.memoryTypeCount; i++)
	{
		VkMemoryType vmt = vpdmp.memoryTypes[i];
		VkMemoryPropertyFlags vmpf = vmt.propertyFlags;
		if ((memoryTypeBits & (1 << i)) != 0)
		{
			if ((vmpf & memoryFlagBits) != 0)
			{
				fprintf(FpDebug, "\n***** Found given memory flag (0x%08x) and type (0x%08x): i = %d *****\n", memoryFlagBits, memoryTypeBits, i);
				return i;
			}
		}
	}

	fprintf(FpDebug, "\n***** Could not find given memory flag (0x%08x) and type (0x%08x) *****\n", memoryFlagBits, memoryTypeBits);
	//throw  std::runtime_error( "Could not find given memory flag and type" );
	return  -1;
}


int Vulkan::FindQueueFamilyThatDoesGraphics()
{
	uint32_t count = -1;
	vkGetPhysicalDeviceQueueFamilyProperties(IN PhysicalDevice, &count, OUT(VkQueueFamilyProperties*)nullptr);
	VkQueueFamilyProperties* vqfp = new VkQueueFamilyProperties[count];
	vkGetPhysicalDeviceQueueFamilyProperties(IN PhysicalDevice, &count, OUT vqfp);
	for (unsigned int i = 0; i < count; i++)
	{
		if ((vqfp[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
		{
			delete[] vqfp;
			return i;
		}
	}

	delete[] vqfp;
	return -1;
}


int Vulkan::FindQueueFamilyThatDoesCompute()
{
	uint32_t count = -1;
	vkGetPhysicalDeviceQueueFamilyProperties(IN PhysicalDevice, &count, OUT(VkQueueFamilyProperties*)nullptr);
	VkQueueFamilyProperties* vqfp = new VkQueueFamilyProperties[count];
	vkGetPhysicalDeviceQueueFamilyProperties(IN PhysicalDevice, &count, OUT vqfp);
	for (unsigned int i = 0; i < count; i++)
	{
		if ((vqfp[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0)
		{
			delete[] vqfp;
			return i;
		}
	}

	delete[] vqfp;
	return -1;
}


int Vulkan::FindQueueFamilyThatDoesTransfer()
{
	uint32_t count = -1;
	vkGetPhysicalDeviceQueueFamilyProperties(IN PhysicalDevice, &count, OUT(VkQueueFamilyProperties*)nullptr);
	VkQueueFamilyProperties* vqfp = new VkQueueFamilyProperties[count];
	vkGetPhysicalDeviceQueueFamilyProperties(IN PhysicalDevice, &count, OUT vqfp);
	for (unsigned int i = 0; i < count; i++)
	{
		if ((vqfp[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0)
		{
			delete[] vqfp;
			return i;
		}
	}

	delete[] vqfp;
	return -1;
}

VkResult Vulkan::RenderScene()
{
	NumRenders++;
	if (NumRenders <= 2)
		HERE_I_AM("RenderScene");

	VkResult result = VK_SUCCESS;

	VkSemaphoreCreateInfo			vsci;
	vsci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	vsci.pNext = nullptr;
	vsci.flags = 0;

	VkSemaphore imageReadySemaphore;

	result = vkCreateSemaphore(LogicalDevice, IN & vsci, PALLOCATOR, OUT & imageReadySemaphore);

	uint32_t nextImageIndex;
	vkAcquireNextImageKHR(LogicalDevice, IN SwapChain, IN UINT64_MAX,
		IN imageReadySemaphore, IN VK_NULL_HANDLE, OUT & nextImageIndex);
	//REPORT( "vkCreateSemaphore" );

	if (Verbose && NumRenders <= 2)	fprintf(FpDebug, "nextImageIndex = %d\n", nextImageIndex);

	VkCommandBufferBeginInfo		vcbbi;
	vcbbi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	vcbbi.pNext = nullptr;
	vcbbi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	//vcbbi.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;   <----- or could use this one??
	vcbbi.pInheritanceInfo = (VkCommandBufferInheritanceInfo*)nullptr;

	result = vkBeginCommandBuffer(CommandBuffers[nextImageIndex], IN & vcbbi);
	//REPORT( "vkBeginCommandBuffer" );

	VkClearColorValue			vccv;
	vccv.float32[0] = 0.0;
	vccv.float32[1] = 0.0;
	vccv.float32[2] = 0.0;
	vccv.float32[3] = 1.0;

	VkClearDepthStencilValue		vcdsv;
	vcdsv.depth = 1.f;
	vcdsv.stencil = 0;

	VkClearValue				vcv[2];
	vcv[0].color = vccv;
	vcv[1].depthStencil = vcdsv;

	VkOffset2D o2d = { 0, 0 };
	VkExtent2D e2d = { Width, Height };
	VkRect2D r2d = { o2d, e2d };

	VkRenderPassBeginInfo				vrpbi1;
	vrpbi1.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	vrpbi1.pNext = nullptr;
	vrpbi1.renderPass = RenderPass;
	vrpbi1.framebuffer = Framebuffers[nextImageIndex];
	vrpbi1.renderArea = r2d;
	vrpbi1.clearValueCount = 2;
	vrpbi1.pClearValues = vcv;		// used for VK_ATTACHMENT_LOAD_OP_CLEAR
	
	vkCmdBeginRenderPass(CommandBuffers[nextImageIndex], IN & vrpbi1, IN VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(CommandBuffers[nextImageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline[1]);

	vkCmdBindDescriptorSets(CommandBuffers[nextImageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipelineLayout, 0, 21, DescriptorSets, 0, (uint32_t*)nullptr);

	VkBuffer buffers[1] = { MyVertexDataBuffer.buffer };
	VkBuffer vBuffers[1] = { MyJustVertexDataBuffer.buffer };
	VkBuffer iBuffer = { MyJustIndexDataBuffer.buffer };
	VkDeviceSize offsets[1] = { 0 };

	if (UseIndexBuffer)
	{
		vkCmdBindVertexBuffers(CommandBuffers[nextImageIndex], 0, 1, vBuffers, offsets);              // 0, 1 = firstBinding, bindingCount
		vkCmdBindIndexBuffer(CommandBuffers[nextImageIndex], iBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
	else
	{
		vkCmdBindVertexBuffers(CommandBuffers[nextImageIndex], 0, 1, buffers, offsets);               // 0, 1 = firstBinding, bindingCount
	}


	const uint32_t vertexCount = sizeof(VertexData) / sizeof(VertexData[0]);
	const uint32_t indexCount = sizeof(JustIndexData) / sizeof(JustIndexData[0]);
	//const uint32_t instanceCount = 1;
	uint32_t instanceCount = objcounttl;
	const uint32_t firstVertex = 0;
	const uint32_t firstIndex = 0;
	const uint32_t firstInstance = 0;
	const uint32_t vertexOffset = 0;

	if (UseIndexBuffer)
	{
		vkCmdDrawIndexed(CommandBuffers[nextImageIndex], indexCount, 1, firstIndex, vertexOffset, firstInstance);
	}
	else
	{
		vkCmdDraw(CommandBuffers[nextImageIndex], vertexCount, 1, firstVertex, firstInstance);
	}
	
//Bug TL Draw1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	vkCmdBindPipeline(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline[0]);

	vkCmdBindDescriptorSets(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipelineLayout, 0, 21, DescriptorSets, 0, (uint32_t*)nullptr);

	if (UseIndexBuffer)
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, vBuffers, offsets);              // 0, 1 = firstBinding, bindingCount
		vkCmdBindIndexBuffer(CommandBuffers[(nextImageIndex)], iBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
	else
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, buffers, offsets);               // 0, 1 = firstBinding, bindingCount
	}

	if (UseIndexBuffer)
	{
		vkCmdDrawIndexed(CommandBuffers[(nextImageIndex)], indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}
	else
	{
		vkCmdDraw(CommandBuffers[(nextImageIndex)], vertexCount, instanceCount, firstVertex, firstInstance);
	}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! 
//! 	//Bug BL Draw1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	instanceCount = objcountbl;

	vkCmdBindPipeline(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline[2]);

	vkCmdBindDescriptorSets(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipelineLayout, 0, 21, DescriptorSets, 0, (uint32_t*)nullptr);

	if (UseIndexBuffer)
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, vBuffers, offsets);              // 0, 1 = firstBinding, bindingCount
		vkCmdBindIndexBuffer(CommandBuffers[(nextImageIndex)], iBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
	else
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, buffers, offsets);               // 0, 1 = firstBinding, bindingCount
	}

	if (UseIndexBuffer)
	{
		vkCmdDrawIndexed(CommandBuffers[(nextImageIndex)], indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}
	else
	{
		vkCmdDraw(CommandBuffers[(nextImageIndex)], vertexCount, instanceCount, firstVertex, firstInstance);
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//! 
	//! 	//Bug TR Draw1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	instanceCount = objcounttr;

	vkCmdBindPipeline(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline[3]);

	vkCmdBindDescriptorSets(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipelineLayout, 0, 21, DescriptorSets, 0, (uint32_t*)nullptr);

	if (UseIndexBuffer)
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, vBuffers, offsets);              // 0, 1 = firstBinding, bindingCount
		vkCmdBindIndexBuffer(CommandBuffers[(nextImageIndex)], iBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
	else
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, buffers, offsets);               // 0, 1 = firstBinding, bindingCount
	}

	if (UseIndexBuffer)
	{
		vkCmdDrawIndexed(CommandBuffers[(nextImageIndex)], indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}
	else
	{
		vkCmdDraw(CommandBuffers[(nextImageIndex)], vertexCount, instanceCount, firstVertex, firstInstance);
	}

	//! 	//Bug BR Draw1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	instanceCount = objcountbr;

	vkCmdBindPipeline(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline[4]);

	vkCmdBindDescriptorSets(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipelineLayout, 0, 21, DescriptorSets, 0, (uint32_t*)nullptr);

	if (UseIndexBuffer)
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, vBuffers, offsets);              // 0, 1 = firstBinding, bindingCount
		vkCmdBindIndexBuffer(CommandBuffers[(nextImageIndex)], iBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
	else
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, buffers, offsets);               // 0, 1 = firstBinding, bindingCount
	}

	if (UseIndexBuffer)
	{
		vkCmdDrawIndexed(CommandBuffers[(nextImageIndex)], indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}
	else
	{
		vkCmdDraw(CommandBuffers[(nextImageIndex)], vertexCount, instanceCount, firstVertex, firstInstance);
	}
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//! 	//! 	//Siren Draw!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	vkCmdBindPipeline(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline[6]);

	vkCmdBindDescriptorSets(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipelineLayout, 0, 21, DescriptorSets, 0, (uint32_t*)nullptr);

	if (UseIndexBuffer)
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, vBuffers, offsets);              // 0, 1 = firstBinding, bindingCount
		vkCmdBindIndexBuffer(CommandBuffers[(nextImageIndex)], iBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
	else
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, buffers, offsets);               // 0, 1 = firstBinding, bindingCount
	}

	if (UseIndexBuffer)
	{
		vkCmdDrawIndexed(CommandBuffers[(nextImageIndex)], indexCount, 2, firstIndex, vertexOffset, firstInstance);
	}
	else
	{
		vkCmdDraw(CommandBuffers[(nextImageIndex)], vertexCount, 2, firstVertex, firstInstance);
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//! 	//! 	//Hero Draw!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	vkCmdBindPipeline(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline[5]);

	vkCmdBindDescriptorSets(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipelineLayout, 0, 21, DescriptorSets, 0, (uint32_t*)nullptr);

	if (UseIndexBuffer)
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, vBuffers, offsets);              // 0, 1 = firstBinding, bindingCount
		vkCmdBindIndexBuffer(CommandBuffers[(nextImageIndex)], iBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
	else
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, buffers, offsets);               // 0, 1 = firstBinding, bindingCount
	}

	if (UseIndexBuffer)
	{
		vkCmdDrawIndexed(CommandBuffers[(nextImageIndex)], indexCount, 1, firstIndex, vertexOffset, firstInstance);
	}
	else
	{
		vkCmdDraw(CommandBuffers[(nextImageIndex)], vertexCount, 1, firstVertex, firstInstance);
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//! 
	//! 			//! 	//! 	//Fan Draw!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	vkCmdBindPipeline(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline[7]);

	vkCmdBindDescriptorSets(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipelineLayout, 0, 21, DescriptorSets, 0, (uint32_t*)nullptr);

	if (UseIndexBuffer)
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, vBuffers, offsets);              // 0, 1 = firstBinding, bindingCount
		vkCmdBindIndexBuffer(CommandBuffers[(nextImageIndex)], iBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
	else
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, buffers, offsets);               // 0, 1 = firstBinding, bindingCount
	}

	if (UseIndexBuffer)
	{
		vkCmdDrawIndexed(CommandBuffers[(nextImageIndex)], indexCount, 2, firstIndex, vertexOffset, firstInstance);
	}
	else
	{
		vkCmdDraw(CommandBuffers[(nextImageIndex)], vertexCount, 2, firstVertex, firstInstance);
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//! 	//! 
	//! 			//! 	//! 	//Bullet Draw!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	vkCmdBindPipeline(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline[8]);

	vkCmdBindDescriptorSets(CommandBuffers[(nextImageIndex)], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipelineLayout, 0, 21, DescriptorSets, 0, (uint32_t*)nullptr);

	if (UseIndexBuffer)
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, vBuffers, offsets);              // 0, 1 = firstBinding, bindingCount
		vkCmdBindIndexBuffer(CommandBuffers[(nextImageIndex)], iBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
	else
	{
		vkCmdBindVertexBuffers(CommandBuffers[(nextImageIndex)], 0, 1, buffers, offsets);               // 0, 1 = firstBinding, bindingCount
	}

	instanceCount = Matricesbulpos.count;

	if (UseIndexBuffer)
	{
		vkCmdDrawIndexed(CommandBuffers[(nextImageIndex)], indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}
	else
	{
		vkCmdDraw(CommandBuffers[(nextImageIndex)], vertexCount, instanceCount, firstVertex, firstInstance);
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	vkCmdEndRenderPass(CommandBuffers[(nextImageIndex)]);

	vkEndCommandBuffer(CommandBuffers[(nextImageIndex)]);

	VkFenceCreateInfo			vfci;
	vfci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	vfci.pNext = nullptr;
	vfci.flags = 0;

	VkFence renderFence;
	vkCreateFence(LogicalDevice, &vfci, PALLOCATOR, OUT & renderFence);

	VkPipelineStageFlags waitAtBottom = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

	VkQueue presentQueue;
	vkGetDeviceQueue(LogicalDevice, FindQueueFamilyThatDoesGraphics(), 0, OUT & presentQueue);
	// 0 = queueIndex
	VkSubmitInfo				vsi;
	vsi.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	vsi.pNext = nullptr;
	vsi.waitSemaphoreCount = 1;
	vsi.pWaitSemaphores = &imageReadySemaphore;
	vsi.pWaitDstStageMask = &waitAtBottom;
	vsi.commandBufferCount = 1;
	vsi.pCommandBuffers = &CommandBuffers[nextImageIndex];
	///		vsi.signalSemaphoreCount = 1;
	vsi.signalSemaphoreCount = 0;
	vsi.pSignalSemaphores = &SemaphoreRenderFinished;

	result = vkQueueSubmit(presentQueue, 1, IN & vsi, IN renderFence);	// 1 = submitCount
	if (Verbose && NumRenders <= 2)	REPORT("vkQueueSubmit");

	result = vkWaitForFences(LogicalDevice, 1, IN & renderFence, VK_TRUE, UINT64_MAX);	// waitAll, timeout
	if (Verbose && NumRenders <= 2)		REPORT("vkWaitForFences");

	vkDestroyFence(LogicalDevice, renderFence, PALLOCATOR);

	VkPresentInfoKHR				vpi;
	vpi.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	vpi.pNext = nullptr;
	vpi.waitSemaphoreCount = 0;
	vpi.pWaitSemaphores = (VkSemaphore*)nullptr;
	vpi.swapchainCount = 1;
	vpi.pSwapchains = &SwapChain;
	vpi.pImageIndices = &nextImageIndex;
	vpi.pResults = (VkResult*)nullptr;

	result = vkQueuePresentKHR(presentQueue, IN & vpi);
	if (Verbose && NumRenders <= 2)		REPORT("vkQueuePresentKHR");

	vkDestroySemaphore(LogicalDevice, imageReadySemaphore, PALLOCATOR);

	return result;
}

void Vulkan::Reset()
{
	ActiveButton = 0;
	Mode = 0;
	NeedToExit = false;
	NumRenders = 0;
	Paused = false;
	Scale = 1.0;
	UseIndexBuffer = false;
	UseLighting = false;
	UseRotate = true;
	Verbose = true;
	Xrot = Yrot = 0.;

	// initialize the matrices:

	glm::vec3  eye(0., 0., EYEDIST);
	glm::vec3  look(0., 0., 0.);
	glm::vec3  up(0., 1., 0.);
	Matricestl.uModelMatrix[0] = glm::mat4();		// identity
	Matricestl.uModelMatrix[1] = glm::mat4();		// identity
	Matricestl.uModelMatrix[2] = glm::mat4();		// identity
	//Matrices.uModelMatrix[0] = glm::translate(Matrices.uModelMatrix[1], glm::vec3(-3.055, -2.305, 0));
	//float xsc = ((3.1)/sqrt(3))*3.055;
	Matricestl.uModelMatrix[0] = glm::scale(glm::mat4(), glm::vec3(3.055 , 2.305, 1)); //Coordinate of 240x180, Normalize 0.4, 0.3
	Matricestl.uModelMatrix[2] = glm::scale(glm::mat4(), glm::vec3(1., 1., 1.));
	//Matrices.uModelMatrix[0] = glm::scale(Matrices.uModelMatrix[0], glm::vec3(0.1, 0.1, 0.1));
	Matricestl.uViewMatrix = glm::lookAt(eye, look, up);
	Matricestl.uProjectionMatrix = glm::perspective(FOV, (double)Width / (double)Height, 0.1, 1000.);
	Matricestl.uProjectionMatrix[1][1] *= -1.;
	Matricestl.uNormalMatrix[0] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricestl.uModelMatrix[0])));
	Matricestl.uNormalMatrix[1] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricestl.uModelMatrix[1])));
	Matricestl.count = 0;

	Matricesbl.uModelMatrix[0] = glm::mat4();		// identity
	Matricesbl.uModelMatrix[1] = glm::mat4();		// identity
	Matricesbl.uModelMatrix[2] = glm::mat4();		// identity
	//Matrices.uModelMatrix[0] = glm::translate(Matrices.uModelMatrix[1], glm::vec3(-3.055, -2.305, 0));
	Matricesbl.uModelMatrix[0] = glm::scale(glm::mat4(), glm::vec3(3.055, 2.305, 1));
	Matricesbl.uModelMatrix[2] = glm::scale(glm::mat4(), glm::vec3(1., 1., 1.));
	//Matrices.uModelMatrix[0] = glm::scale(Matrices.uModelMatrix[0], glm::vec3(0.1, 0.1, 0.1));
	Matricesbl.uViewMatrix = glm::lookAt(eye, look, up);
	Matricesbl.uProjectionMatrix = glm::perspective(FOV, (double)Width / (double)Height, 0.1, 1000.);
	Matricesbl.uProjectionMatrix[1][1] *= -1.;
	Matricesbl.uNormalMatrix[0] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricesbl.uModelMatrix[0])));
	Matricesbl.uNormalMatrix[1] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricesbl.uModelMatrix[1])));
	Matricesbl.count = 0;

	Matricestr.uModelMatrix[0] = glm::mat4();		// identity
	Matricestr.uModelMatrix[1] = glm::mat4();		// identity
	Matricestr.uModelMatrix[2] = glm::mat4();		// identity
	//Matrices.uModelMatrix[0] = glm::translate(Matrices.uModelMatrix[1], glm::vec3(-3.055, -2.305, 0));
	Matricestr.uModelMatrix[0] = glm::scale(glm::mat4(), glm::vec3(3.055, 2.305, 1));
	Matricestr.uModelMatrix[2] = glm::scale(glm::mat4(), glm::vec3(1., 1., 1.));
	//Matrices.uModelMatrix[0] = glm::scale(Matrices.uModelMatrix[0], glm::vec3(0.1, 0.1, 0.1));
	Matricestr.uViewMatrix = glm::lookAt(eye, look, up);
	Matricestr.uProjectionMatrix = glm::perspective(FOV, (double)Width / (double)Height, 0.1, 1000.);
	Matricestr.uProjectionMatrix[1][1] *= -1.;
	Matricestr.uNormalMatrix[0] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricestr.uModelMatrix[0])));
	Matricestr.uNormalMatrix[1] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricestr.uModelMatrix[1])));
	Matricestr.count = 0;
	
	Matricesbr.uModelMatrix[0] = glm::mat4();		// identity
	Matricesbr.uModelMatrix[1] = glm::mat4();		// identity
	Matricesbr.uModelMatrix[2] = glm::mat4();		// identity
	//Matrices.uModelMatrix[0] = glm::translate(Matrices.uModelMatrix[1], glm::vec3(-3.055, -2.305, 0));
	Matricesbr.uModelMatrix[0] = glm::scale(glm::mat4(), glm::vec3(3.055, 2.305, 1));
	Matricesbr.uModelMatrix[2] = glm::scale(glm::mat4(), glm::vec3(1., 1., 1.));
	//Matrices.uModelMatrix[0] = glm::scale(Matrices.uModelMatrix[0], glm::vec3(0.1, 0.1, 0.1));
	Matricesbr.uViewMatrix = glm::lookAt(eye, look, up);
	Matricesbr.uProjectionMatrix = glm::perspective(FOV, (double)Width / (double)Height, 0.1, 1000.);
	Matricesbr.uProjectionMatrix[1][1] *= -1.;
	Matricesbr.uNormalMatrix[0] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricesbr.uModelMatrix[0])));
	Matricesbr.uNormalMatrix[1] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricesbr.uModelMatrix[1])));
	Matricesbr.count = 0;
	// initialize the lighting information:

	HeroPos.uModelMatrix = glm::mat4();		// identity
	//Matrices.uModelMatrix[0] = glm::translate(Matrices.uModelMatrix[1], glm::vec3(-3.055, -2.305, 0));
	HeroPos.uModelMatrix = glm::scale(glm::mat4(), glm::vec3(3.055, 2.305, 1));
	//Matrices.uModelMatrix[0] = glm::scale(Matrices.uModelMatrix[0], glm::vec3(0.1, 0.1, 0.1));
	HeroPos.uViewMatrix = glm::lookAt(eye, look, up);
	HeroPos.uProjectionMatrix = glm::perspective(FOV, (double)Width / (double)Height, 0.1, 1000.);
	HeroPos.uProjectionMatrix[1][1] *= -1.;
	HeroPos.uNormalMatrix[0] = glm::mat4(glm::inverseTranspose(glm::mat3(HeroPos.uModelMatrix)));
	HeroPos.uNormalMatrix[1] = glm::mat4(glm::inverseTranspose(glm::mat3(HeroPos.uModelMatrix)));
	HeroPos.direction = 0;

	Light.uKa = 0.2f;
	Light.uKd = 0.5f;
	Light.uKs = 0.3f;
	Light.uShininess = 100.f;
	Light.uEyePos = glm::vec4(eye, 1.);
	Light.uLightPos = glm::vec4(-50., -50., 10., 1.);
	Light.uLightSpecularColor = glm::vec4(1., 1., 1., 1.);


	// initialize the misc stuff:

	Matricesbulpos.uModelMatrix[0] = glm::mat4();
	Matricesbulpos.uViewMatrix = glm::lookAt(eye, look, up);
	Matricesbulpos.uProjectionMatrix = glm::perspective(FOV, (double)Width / (double)Height, 0.1, 1000.);
	Matricesbulpos.uProjectionMatrix[1][1] *= -1.;
	Matricesbulpos.uNormalMatrix[0] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricesbulpos.uModelMatrix[0])));
	Matricesbulpos.uNormalMatrix[1] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricesbulpos.uModelMatrix[1])));

	for (int i = 0; i < 10; i++)
		MiscLocation.uModelMatrix[i] = glm::mat4();

	MiscLocation.uViewMatrix = glm::lookAt(eye, look, up);
	MiscLocation.uProjectionMatrix = glm::perspective(FOV, (double)Width / (double)Height, 0.1, 1000.);
	MiscLocation.uProjectionMatrix[1][1] *= -1.;
	MiscLocation.uNormalMatrix[0] = glm::mat4(glm::inverseTranspose(glm::mat3(MiscLocation.uModelMatrix[0])));
	MiscLocation.uNormalMatrix[1] = glm::mat4(glm::inverseTranspose(glm::mat3(MiscLocation.uModelMatrix[0])));
}

void Vulkan::UpdateScene()
{
	int ictr = 1;
	ObjSpawn(ActiveButton, withinWindowCursorPosX, withinWindowCursorPosY,fElapsedTime/1000000., Keypress);

	for (auto& obj : objvectopleft)
	{
		Matricestl.count = objcounttl;
		Matricestl.uModelMatrix[ictr] = glm::scale(glm::mat4(),glm::vec3(0.05*3.055, 0.05*2.305, 1.));
		Matricestl.uModelMatrix[ictr] = glm::translate(Matricestl.uModelMatrix[ictr], glm::vec3(obj.x,-obj.y,1.));//15
		ictr++;
	}

	

	//testval = testtrans * testscale * glm::mat4();
	//glm::mat4 multmat = glm::mat4(()0.2, 0., 0., 0., 0., 0.2, 0., 0., -3.055, 2.305, 1., 1.);
	// change the object projection:

	//glm::mat4 reqval = testval*glm::inverse(Matricestl.uViewMatrix)*glm::inverse(Matricestl.uProjectionMatrix);

	Matricestl.uProjectionMatrix = glm::perspective(FOV, (double)Width / (double)Height, 0.1, 1000.);
	Matricestl.uProjectionMatrix[1][1] *= -1.;

	//testval = Matricestl.uProjectionMatrix * Matricestl.uViewMatrix * Matricestl.uModelMatrix[1];
	// change the normal matrix:

	Matricestl.uNormalMatrix[0] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricestl.uModelMatrix[0])));
	Matricestl.uNormalMatrix[1] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricestl.uModelMatrix[1])));
	Fill05DataBuffer(MyMatrixUniformBuffertopleft, (void*)&Matricestl);

	ictr = 1;

	for (auto& obj : objvecbottomleft)
	{
		Matricesbl.count = objcountbl;
		Matricesbl.uModelMatrix[ictr] = glm::scale(glm::mat4(), glm::vec3(0.05 * 3.055, 0.05 * 2.305, 1.));
		Matricesbl.uModelMatrix[ictr] = glm::translate(Matricesbl.uModelMatrix[ictr], glm::vec3(obj.x, -obj.y, 1.));
		ictr++;
	}

	// change the object projection:

	Matricesbl.uProjectionMatrix = glm::perspective(FOV, (double)Width / (double)Height, 0.1, 1000.);
	Matricesbl.uProjectionMatrix[1][1] *= -1.;


	// change the normal matrix:

	Matricesbl.uNormalMatrix[0] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricesbl.uModelMatrix[0])));
	Matricesbl.uNormalMatrix[1] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricesbl.uModelMatrix[1])));
	Fill05DataBuffer(MyMatrixUniformBufferbottomleft, (void*)&Matricesbl);

	ictr = 1;

	for (auto& obj : objvectopright)
	{
		Matricestr.count = objcounttr;
		Matricestr.uModelMatrix[ictr] = glm::scale(glm::mat4(), glm::vec3(0.05 * 3.055, 0.05 * 2.305, 1.));
		Matricestr.uModelMatrix[ictr] = glm::translate(Matricestr.uModelMatrix[ictr], glm::vec3(obj.x, -obj.y, 1.));
		ictr++;
	}

	// change the object projection:

	Matricestr.uProjectionMatrix = glm::perspective(FOV, (double)Width / (double)Height, 0.1, 1000.);
	Matricestr.uProjectionMatrix[1][1] *= -1.;


	// change the normal matrix:

	Matricestr.uNormalMatrix[0] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricestr.uModelMatrix[0])));
	Matricestr.uNormalMatrix[1] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricestr.uModelMatrix[1])));
	Fill05DataBuffer(MyMatrixUniformBuffertopright, (void*)&Matricestr);

	ictr = 1;

	for (auto& obj : objvecbottomright)
	{
		Matricesbr.count = objcountbr;
		Matricesbr.uModelMatrix[ictr] = glm::scale(glm::mat4(), glm::vec3(0.05 * 3.055, 0.05 * 2.305, 1.));
		Matricesbr.uModelMatrix[ictr] = glm::translate(Matricesbr.uModelMatrix[ictr], glm::vec3(obj.x, -obj.y, 1.));
		ictr++;
	}

	// change the object projection:

	Matricesbr.uProjectionMatrix = glm::perspective(FOV, (double)Width / (double)Height, 0.1, 1000.);
	Matricesbr.uProjectionMatrix[1][1] *= -1.;


	// change the normal matrix:

	Matricesbr.uNormalMatrix[0] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricesbr.uModelMatrix[0])));
	Matricesbr.uNormalMatrix[1] = glm::mat4(glm::inverseTranspose(glm::mat3(Matricesbr.uModelMatrix[1])));
	Fill05DataBuffer(MyMatrixUniformBufferbottomright, (void*)&Matricesbr);

	HeroPos.uModelMatrix = glm::scale(glm::mat4(), glm::vec3(0.2 * 3.055, 0.2 * 2.305, 1.));
	HeroPos.uModelMatrix= glm::translate(HeroPos.uModelMatrix, glm::vec3(hero.x, -hero.y, 1.));//2.88
	HeroPos.direction = directionofhero;

	//glm::mat4 testval = HeroPos.uModelMatrix;
	//glm::mat4 testscale = glm::scale(glm::mat4(), glm::vec3(0.6, 0.6, 1.));
	//glm::mat4 testtrans = glm::translate(glm::mat4(), glm::vec3(-2.1434, -0.479, 1.));

	//glm::mat4 testval = testtrans * testscale * glm::mat4();
	//glm::mat4 multmat = glm::mat4(()0.2, 0., 0., 0., 0., 0.2, 0., 0., -3.055, 2.305, 1., 1.);
	// change the object projection:

	//glm::mat4 multmat = HeroPos.uProjectionMatrix * HeroPos.uViewMatrix * testval;

	//glm::mat4 reqval = testval * glm::inverse(HeroPos.uViewMatrix) * glm::inverse(HeroPos.uProjectionMatrix);

	Vulkan::Fill05DataBuffer(HeroPosUniformBuffer, (void*)&HeroPos);

	glm::mat4 scalematsiren = glm::scale(glm::mat4(), glm::vec3(0.2 * 3.055, 0.2 * 3.055, 1.));
	glm::mat4 scalematfan = glm::scale(glm::mat4(), glm::vec3(0.1 * 3.055, 0.1 * 3.055, 1.));
	//glm::mat4 scalemat = glm::mat4();

	static double rottimer = 0;
	
	if (rottimer < 1.)
		rottimer += fElapsedTime/1000000000.;
	else
		rottimer = 0;

	const glm::vec3 axis = glm::vec3(0., 0., 1.);

	glm::mat4 rotmatsirenp = glm::rotate(glm::mat4(),(float)(2 * M_PI * rottimer), axis);
	glm::mat4 rotmatsirenn = glm::rotate(glm::mat4(), (float)((-2 * M_PI * rottimer)+M_PI/2.), axis);
	//glm::mat4 rotmat = glm::mat4();

	glm::mat4 transmatnsiren = glm::translate(glm::mat4(), glm::vec3(-1.2, 0.7, 0.));

	glm::mat4 transmatpsiren = glm::translate(glm::mat4(), glm::vec3(1.2, 0.7, 0.));

	glm::mat4 transmatlfanleft = glm::translate(glm::mat4(), glm::vec3(-2.42, 1.67, 0.));
	glm::mat4 transmatlfanright = glm::translate(glm::mat4(), glm::vec3(-2.14, 1.67, 0.));
	
	MiscLocation.uModelMatrix[0] = transmatnsiren * rotmatsirenn * scalematsiren * glm::mat4();

	MiscLocation.uModelMatrix[1] = transmatpsiren * rotmatsirenp * scalematsiren * glm::mat4();

	MiscLocation.uModelMatrix[2] = transmatlfanleft * rotmatsirenp * scalematfan * glm::mat4();

	MiscLocation.uModelMatrix[3] = transmatlfanright * rotmatsirenp * scalematfan * glm::mat4();

	Vulkan::Fill05DataBuffer(MiscLocUniformBuffer, (void*)&MiscLocation);

	ictr = 0;

	for (auto& bitr : bulletpos)
	{
		glm::mat4 scmat = glm::scale(glm::mat4(), glm::vec3(0.3055, 0.2305, 1.));
		glm::mat4 rtmat = glm::rotate(glm::mat4(),(float)(bitr.angle), axis);
		glm::mat4 tsmat= glm::translate(glm::mat4(), glm::vec3(bitr.x, -bitr.y, 1.));

		Matricesbulpos.uModelMatrix[ictr] = tsmat * rtmat * scmat;
		ictr++;
	}

	Matricesbulpos.count = bulletpos.size();

	Vulkan::Fill05DataBuffer(MyMatrixUniformBufferBulPos, (void*)&Matricesbulpos);

	//Fill05DataBuffer(MyMiscUniformBuffer, (void*)&Misc);

	Vulkan::CursorPosNorm(); //Update Normalized Cursor Position 
	//printf("%1f", fElapsedTime);
}

void Vulkan::InitGLFW()
{
	glfwSetErrorCallback(GLFWErrorCallback);
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	MainWindow = glfwCreateWindow(Width, Height, "Bugzilla", NULL, NULL);

	uint32_t count;
	const char** extensions = glfwGetRequiredInstanceExtensions(&count);
	fprintf(FpDebug, "\nFound %d GLFW Required Instance Extensions:\n", count);
	for (uint32_t i = 0; i < count; i++)
	{
		fprintf(FpDebug, "\t%s\n", extensions[i]);
	}

	glfwSetKeyCallback(MainWindow, GLFWKeyboard);
	glfwSetCursorPosCallback(MainWindow, GLFWMouseMotion);
	glfwSetMouseButtonCallback(MainWindow, GLFWMouseButton);
}


void Vulkan::InitGLFWSurface()
{
	VkResult result = glfwCreateWindowSurface(Instance, MainWindow, NULL, &Surface);
	REPORT("glfwCreateWindowSurface");
}

void Vulkan::Finish()
{
	vkQueueWaitIdle(Queue);
	vkDeviceWaitIdle(LogicalDevice);
	vulkan.DestroyAllVulkan();
	glfwDestroyWindow(MainWindow);
	glfwTerminate();
}

void Vulkan::CursorPosNorm()
{
	double overshootX, overshootY;

	overshootX = Mosx - offsetX;
	overshootY = Mosy - offsetY;

	if (overshootX > Width) {
		offsetX = offsetX + overshootX - Width;
	}
	if (overshootX < 0) {
		offsetX = offsetX + overshootX;
	}
	if (overshootY > Height) {
		offsetY = offsetY + overshootY - Height;
	}
	if (overshootY < 0) {
		offsetY = offsetY + overshootY;
	}

	withinWindowCursorPosX = (Mosx - offsetX)/Width;
	withinWindowCursorPosY = (Mosy - offsetY)/Height;

	//object.x = Xmouse;
	//object.y = Ymouse;
	//object.angle = 0;

	//printf("%i\n",ActiveButton);
}

int
main(int argc, char* argv[])
{
	srand(time(0));
	vulkan.Width = 800;
	vulkan.Height = 600;
	static bool cont = true;

#ifdef _WIN32
	errno_t err = fopen_s(&FpDebug, DEBUGFILE, "w");
	if (err != 0)
	{
		fprintf(stderr, "Cannot open debug print file '%s'\n", DEBUGFILE);
		FpDebug = stderr;
	}
#else
	FpDebug = fopen(DEBUGFILE, "w");
	if (FpDebug == NULL)
	{
		fprintf(stderr, "Cannot open debug print file '%s'\n", DEBUGFILE);
		FpDebug = stderr;
	}
#endif
	fprintf(FpDebug, "FpDebug: Width = %d ; Height = %d\n", vulkan.Width, vulkan.Height);

	vulkan.Reset();
	vulkan.InitGraphics();

	std::thread t1(taskaudiobckg, true);
	// loop until the user closes the window:

	while (glfwWindowShouldClose(vulkan.MainWindow) == 0)
	{
		auto start = std::chrono::high_resolution_clock::now();
		glfwPollEvents();
		Time = glfwGetTime();		// elapsed time, in double-precision seconds
		vulkan.UpdateScene();
		vulkan.RenderScene();
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
		fElapsedTime = duration.count();
		if (NeedToExit)
		{
			break;
		}
	}
	stopsound = true;
	fprintf(FpDebug, "Closing the GLFW window\n");
	cont = false;
	t1.join();
	vulkan.Finish();
	return 0;
}