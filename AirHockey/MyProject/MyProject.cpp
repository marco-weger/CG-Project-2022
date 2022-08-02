// This has been adapted from the Vulkan tutorial
#include "Game.cpp"
#include "MyProject.hpp"

#include <algorithm>
#include <string>

#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const std::string MODEL_DIR = "models";
const std::string TEXTURE_DIR = "textures";

// The uniform buffer object used in this example
struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};
struct SkyBoxUniformBufferObject {
 alignas(16) glm::mat4 mvpMat;
 alignas(16) glm::mat4 mMat;
 alignas(16) glm::mat4 nMat;
};

// MAIN ! 
class MyProject : public BaseProject {
	protected:
	// Here you list all the Vulkan objects you need:
	
	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSL1;

	// Pipelines [Shader couples]
	Pipeline P1;
    
    // SkyBox
    DescriptorSetLayout DSL_SB;
    Pipeline P_SB;
    DescriptorSetSkyBox DS_SB;
    
    //std::vector<VkBuffer> SkyBoxUniformBuffers;
    //std::vector<VkDescriptorSet> SkyBoxDescriptorSets;

	// Models, textures and Descriptors (values assigned to the uniforms)
    //int elemCount = 4 + 4 * 5 + 10; //10 + 12 * 10;
    int elemCount = 34;
    
    Model M_Table;
    Texture T_Table;
    DescriptorSet DS_Table;

    Model M_BluePaddle;
    Texture T_BluePaddle;
    DescriptorSet DS_BluePaddle;
    Model M_RedPaddle;
    Texture T_RedPaddle;
    DescriptorSet DS_RedPaddle;
    
    Model M_Disk;
    Texture T_Disk;
    DescriptorSet DS_Disk;

    Model M_PL1_V1[5];
    Texture T_PL1_V1[5];
    DescriptorSet DS_PL1_V1[5];
    
    Model M_PL2_V1[5];
    Texture T_PL2_V1[5];
    DescriptorSet DS_PL2_V1[5];
    
    Model M_PL1_V2[5];
    Texture T_PL1_V2[5];
    DescriptorSet DS_PL1_V2[5];
    
    Model M_PL2_V2[5];
    Texture T_PL2_V2[5];
    DescriptorSet DS_PL2_V2[5];
    
    // Game
    Game game;
    
	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Air Hockey";
		//initialBackgroundColor = {0.1f, 0.0f, 0.0f, 1.0f};
		
		// Descriptor pool sizes
		uniformBlocksInPool = elemCount;
		texturesInPool = elemCount;
		setsInPool = elemCount;
	}
    
	// Here you load and setup all your Vulkan objects
	void localInit() {
		// Descriptor Layouts [what will be passed to the shaders]
		DSL1.init(this, {
					// this array contains the binding:
					// first  element : the binding number
					// second element : the time of element (buffer or texture)
					// third  element : the pipeline stage where it will be used
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
				  });

		// Pipelines [Shader couples]
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P1.init(this, "shaders/vert.spv", "shaders/frag.spv", {&DSL1}, VK_COMPARE_OP_LESS);
        
        DSL_SB.init(this, {
                    {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
                    {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
                  });
        P_SB.init(this, "shaders/SkyBoxVert.spv", "shaders/SkyBoxFrag.spv", {&DSL_SB}, VK_COMPARE_OP_LESS_OR_EQUAL);
        DS_SB.init(this, &DSL_SB, {
                    {0, UNIFORM, sizeof(SkyBoxUniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &(SkyBox.TD)}
                });
        //createSkyBoxUniformBuffersAndDescriptorSet();

		// Models, textures and Descriptors (values assigned to the uniforms)
        M_Table.init(this, MODEL_DIR + "/AirHockeyTable.obj");
		T_Table.init(this, TEXTURE_DIR + "/airhockey-background.png");
		DS_Table.init(this, &DSL1, {
		// the second parameter, is a pointer to the Uniform Set Layout of this set
		// the last parameter is an array, with one element per binding of the set.
		// first  elmenet : the binding number
		// second element : UNIFORM or TEXTURE (an enum) depending on the type
		// third  element : only for UNIFORMs, the size of the corresponding C++ object
		// fourth element : only for TEXTUREs, the pointer to the corresponding texture object
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T_Table}
				});
        
        M_BluePaddle.init(this, MODEL_DIR + "/Paddle.obj");
        T_BluePaddle.init(this, TEXTURE_DIR + "/blue.png");
        DS_BluePaddle.init(this, &DSL1, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &T_BluePaddle}
                });
        M_RedPaddle.init(this, MODEL_DIR + "/Paddle.obj");
        T_RedPaddle.init(this, TEXTURE_DIR + "/red.jpeg");
        DS_RedPaddle.init(this, &DSL1, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &T_RedPaddle}
                });
        
        M_Disk.init(this, MODEL_DIR + "/Disk.obj");
        T_Disk.init(this, TEXTURE_DIR + "/yellow.jpeg");
        DS_Disk.init(this, &DSL1, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &T_Disk}
                });
        
        const char* map[5] = {"DR3", "DL6", "DL5", "DL4", "DL3"}; //, "DL2", "DL1", "DR6", "DR5", "DR4"};
        for(int i = 0; i < 5; i++)
        {
            M_PL1_V1[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_PL1_V1[i].init(this, TEXTURE_DIR + "/scoreBlue.png");
            DS_PL1_V1[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_PL1_V1[i]}
                    });
            M_PL2_V1[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_PL2_V1[i].init(this, TEXTURE_DIR + "/scoreRed.png");
            DS_PL2_V1[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_PL2_V1[i]}
                    });
            M_PL1_V2[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_PL1_V2[i].init(this, TEXTURE_DIR + "/scoreBlue.png");
            DS_PL1_V2[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_PL1_V2[i]}
                    });
            M_PL2_V2[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_PL2_V2[i].init(this, TEXTURE_DIR + "/scoreRed.png");
            DS_PL2_V2[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_PL2_V2[i]}
                    });
        }
	}

	// Here you destroy all the objects you created!		
	void localCleanup() {
		DS_Table.cleanup();
		T_Table.cleanup();
		M_Table.cleanup();
        
        DS_BluePaddle.cleanup();
        T_BluePaddle.cleanup();
        M_BluePaddle.cleanup();
        DS_RedPaddle.cleanup();
        T_RedPaddle.cleanup();
        M_RedPaddle.cleanup();
        
        DS_Disk.cleanup();
        T_Disk.cleanup();
        M_Disk.cleanup();
        
        for(int i = 0; i < 5; i++)
        {
            DS_PL1_V1[i].cleanup();
            T_PL1_V1[i].cleanup();
            M_PL1_V1[i].cleanup();
         
            DS_PL2_V1[i].cleanup();
            T_PL2_V1[i].cleanup();
            M_PL2_V1[i].cleanup();
            
            DS_PL1_V2[i].cleanup();
            T_PL1_V2[i].cleanup();
            M_PL1_V2[i].cleanup();
         
            DS_PL2_V2[i].cleanup();
            T_PL2_V2[i].cleanup();
            M_PL2_V2[i].cleanup();
        }
        
        DS_SB.cleanup();
        
		P1.cleanup();
		DSL1.cleanup();
        
        P_SB.cleanup();
        DSL_SB.cleanup();
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
			
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                P_SB.graphicsPipeline);

        VkBuffer vertexBuffers_SB[] = {SkyBox.MD.vertexBuffer};
        VkDeviceSize offsets_SB[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_SB, offsets_SB);
        vkCmdBindIndexBuffer(commandBuffer, SkyBox.MD.indexBuffer, 0,
                                VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        P_SB.pipelineLayout, 0, 1, &DS_SB.descriptorSets[currentImage],
                        0, nullptr);
        vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(SkyBox.MD.indices.size()), 1, 0, 0, 0);
        
        /* ********* */
        
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
				P1.graphicsPipeline);
				
		VkBuffer vertexBuffers1[] = {M_Table.vertexBuffer};
		// property .vertexBuffer of models, contains the VkBuffer handle to its vertex buffer
		VkDeviceSize offsets1[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers1, offsets1);
		// property .indexBuffer of models, contains the VkBuffer handle to its index buffer
		vkCmdBindIndexBuffer(commandBuffer, M_Table.indexBuffer, 0,
								VK_INDEX_TYPE_UINT32);

		// property .pipelineLayout of a pipeline contains its layout.
		// property .descriptorSets of a descriptor set contains its elements.
		vkCmdBindDescriptorSets(commandBuffer,
						VK_PIPELINE_BIND_POINT_GRAPHICS,
						P1.pipelineLayout, 0, 1, &DS_Table.descriptorSets[currentImage],
						0, nullptr);
						
		// property .indices.size() of models, contains the number of triangles * 3 of the mesh.
		vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(M_Table.indices.size()), 1, 0, 0, 0);
        
        VkBuffer vertexBuffers2[] = {M_BluePaddle.vertexBuffer};
        VkDeviceSize offsets2[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers2, offsets2);
        vkCmdBindIndexBuffer(commandBuffer, M_BluePaddle.indexBuffer, 0,
                                VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        P1.pipelineLayout, 0, 1, &DS_BluePaddle.descriptorSets[currentImage],
                        0, nullptr);
        vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M_BluePaddle.indices.size()), 1, 0, 0, 0);
        
        VkBuffer vertexBuffers3[] = {M_RedPaddle.vertexBuffer};
        VkDeviceSize offsets3[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers3, offsets3);
        vkCmdBindIndexBuffer(commandBuffer, M_RedPaddle.indexBuffer, 0,
                                VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        P1.pipelineLayout, 0, 1, &DS_RedPaddle.descriptorSets[currentImage],
                        0, nullptr);
        vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M_RedPaddle.indices.size()), 1, 0, 0, 0);
        
        VkBuffer vertexBuffers4[] = {M_Disk.vertexBuffer};
        VkDeviceSize offsets4[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers4, offsets4);
        vkCmdBindIndexBuffer(commandBuffer, M_Disk.indexBuffer, 0,
                                VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        P1.pipelineLayout, 0, 1, &DS_Disk.descriptorSets[currentImage],
                        0, nullptr);
        vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M_Disk.indices.size()), 1, 0, 0, 0);
        
        for(int i = 0; i < 5; i++)
        {
            VkBuffer vertexBuffers_PL1_V1[] = {M_PL1_V1[i].vertexBuffer};
            VkDeviceSize offsets_PL1_V1[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_PL1_V1, offsets_PL1_V1);
            vkCmdBindIndexBuffer(commandBuffer, M_PL1_V1[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_PL1_V1[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_PL1_V1[i].indices.size()), 1, 0, 0, 0);
            
            VkBuffer vertexBuffers_PL2_V1[] = {M_PL2_V1[i].vertexBuffer};
            VkDeviceSize offsets_PL2_V1[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_PL2_V1, offsets_PL2_V1);
            vkCmdBindIndexBuffer(commandBuffer, M_PL2_V1[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_PL2_V1[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_PL2_V1[i].indices.size()), 1, 0, 0, 0);
            
            VkBuffer vertexBuffers_PL1_V2[] = {M_PL1_V2[i].vertexBuffer};
            VkDeviceSize offsets_PL1_V2[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_PL1_V2, offsets_PL1_V2);
            vkCmdBindIndexBuffer(commandBuffer, M_PL1_V2[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_PL1_V2[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_PL1_V1[i].indices.size()), 1, 0, 0, 0);
            
            VkBuffer vertexBuffers_PL2_V2[] = {M_PL2_V2[i].vertexBuffer};
            VkDeviceSize offsets_PL2_V2[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_PL2_V2, offsets_PL2_V2);
            vkCmdBindIndexBuffer(commandBuffer, M_PL2_V2[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_PL2_V2[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_PL2_V2[i].indices.size()), 1, 0, 0, 0);
        }
	}
    
    // Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
        static auto startTime = std::chrono::high_resolution_clock::now();
        static float lastTime = 0.0f;
        
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>
                    (currentTime - startTime).count();
        float deltaT = time - lastTime;
        lastTime = time;
                    
        static float debounce = time;
        
        if(glfwGetKey(window, GLFW_KEY_SPACE)) {
            if(time - debounce > 0.33) {
                debounce = time;
                game.changeView();
            }
        }
        
        if((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) && glfwGetKey(window, GLFW_KEY_C))
        {
            game.newGame();
        }
        
        UniformBufferObject ubo{};
        glm::mat4 initalModel = glm::rotate(glm::mat4(1.0f),
                                glm::radians(0.0f),
                                glm::vec3(0.0f, 0.0f, 1.0f));
            
        
        ubo.view = glm::lookAt(game.getCamPos(deltaT),game.getAimPos(),glm::vec3(0.0f, 1.0f, 0.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f),
                        swapChainExtent.width / (float) swapChainExtent.height,
                        0.1f, 10.0f);
        ubo.proj[1][1] *= -1;
        
        void* data;
        
        CamType currentCamType = game.getCamType();
        glm::vec2 currentPosition;
        
        // updates SkyBox uniforms
        SkyBoxUniformBufferObject subo{};
        subo.mMat = glm::mat4(1.0f);
        subo.nMat = glm::mat4(1.0f);
        subo.mvpMat = ubo.proj * ubo.view;
        
        subo.mvpMat = glm::scale(subo.mvpMat, glm::vec3(3.0f));
        
        vkMapMemory(device, DS_SB.uniformBuffersMemory[0][currentImage], 0,
                            sizeof(subo), 0, &data);
        memcpy(data, &subo, sizeof(subo));
        vkUnmapMemory(device, DS_SB.uniformBuffersMemory[0][currentImage]);

        // Here is where you actually update your uniforms
        ubo.model = initalModel;
        vkMapMemory(device, DS_Table.uniformBuffersMemory[0][currentImage], 0,
                            sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device, DS_Table.uniformBuffersMemory[0][currentImage]);
        
        /* ******** */
        
        game.detectCollision();
        
        if(currentCamType != CamType::upCenter)
        {
            game.updateDiskPosition(deltaT);
        }
        else
        {
            if(glfwGetKey(window, GLFW_KEY_A)) {
                game.decrementDiskPosition(1,deltaT);
            }
            if(glfwGetKey(window, GLFW_KEY_D)) {
                game.incrementDiskPosition(1,deltaT);
            }
            if(glfwGetKey(window, GLFW_KEY_S)) {
                game.incrementDiskPosition(0,deltaT);
            }
            if(glfwGetKey(window, GLFW_KEY_W)) {
                game.decrementDiskPosition(0,deltaT);
            }
        }
        
        game.detectGoal();
        
        /* ******** */
        
        // PLAYER 1
        ubo.model = initalModel;
        ubo.model = glm::translate(ubo.model, glm::vec3(0.0f, 0.04f, 0.0f));
        
        if(currentCamType == CamType::player1 || currentCamType == CamType::upPlayer1)
        {
            if(currentCamType == CamType::player1)
            {
                if(glfwGetKey(window, GLFW_KEY_A)) {
                    game.decrementPlayer1Position(0,deltaT);
                }
                if(glfwGetKey(window, GLFW_KEY_D)) {
                    game.incrementPlayer1Position(0,deltaT);
                }
                if(glfwGetKey(window, GLFW_KEY_S)) {
                    game.decrementPlayer1Position(1,deltaT);
                }
                if(glfwGetKey(window, GLFW_KEY_W)) {
                    game.incrementPlayer1Position(1,deltaT);
                }
            }
            else if(currentCamType == CamType::upPlayer1)
            {
                double xPos, yPos;
                glfwGetCursorPos(window, &xPos, &yPos);
                if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                {
                    int width, height;
                    glfwGetWindowSize(window, &width, &height);
                    
                    if(xPos > 0 and xPos < width and yPos > 0 and yPos < height)
                    {
                        xPos -= width/2.0;
                        yPos -= height/2.0;
                         
                        xPos = xPos / (width/2.0);
                        yPos = yPos / (height/2.0);
                                           
                        game.updatePlayer1Position(glm::vec2(xPos, yPos), deltaT);
                    }
                }
            }
            
            game.updatePlayer2Cpu(deltaT);
        }
        
        currentPosition = game.getPlayer1Position();
        ubo.model = glm::translate(ubo.model, glm::vec3(currentPosition[1], 0.0f, currentPosition[0]));
        
        vkMapMemory(device, DS_BluePaddle.uniformBuffersMemory[0][currentImage], 0,
                            sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device, DS_BluePaddle.uniformBuffersMemory[0][currentImage]);
        
        // PLAYER 2
        ubo.model = initalModel;
        ubo.model = glm::translate(ubo.model, glm::vec3(0.0f, 0.04f, 0.0f));
        
        if(currentCamType == CamType::player2 || currentCamType == CamType::upPlayer2)
        {
            if(currentCamType == CamType::player2)
            {
                if(glfwGetKey(window, GLFW_KEY_A)) {
                    game.incrementPlayer2Position(0,deltaT);
                }
                if(glfwGetKey(window, GLFW_KEY_D)) {
                    game.decrementPlayer2Position(0,deltaT);
                }
                if(glfwGetKey(window, GLFW_KEY_S)) {
                    game.incrementPlayer2Position(1,deltaT);
                }
                if(glfwGetKey(window, GLFW_KEY_W)) {
                    game.decrementPlayer2Position(1,deltaT);
                }
            }
            else if(currentCamType == CamType::upPlayer2)
            {
                double xPos, yPos;
                glfwGetCursorPos(window, &xPos, &yPos);
                if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
                {
                    int width, height;
                    glfwGetWindowSize(window, &width, &height);
                    
                    if(xPos > 0 and xPos < width and yPos > 0 and yPos < height)
                    {
                        xPos -= width/2.0;
                        yPos -= height/2.0;
                         
                        xPos = xPos / (width/2.0);
                        yPos = yPos / (height/2.0);
                                           
                        game.updatePlayer2Position(glm::vec2(xPos, yPos), deltaT);
                    }
                }
            }
            
            game.updatePlayer1Cpu(deltaT);
        }
    
        currentPosition = game.getPlayer2Position();
        ubo.model = glm::translate(ubo.model, glm::vec3(currentPosition[1], 0.0f, currentPosition[0]));
        
        vkMapMemory(device, DS_RedPaddle.uniformBuffersMemory[0][currentImage], 0,
                            sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device, DS_RedPaddle.uniformBuffersMemory[0][currentImage]);
        
        // DISK
        ubo.model = initalModel;
        ubo.model = glm::translate(ubo.model, glm::vec3(0.0f, 0.001f, 0.0f));
        
        currentPosition = game.getDiskPosition();
        ubo.model = glm::translate(ubo.model, glm::vec3(currentPosition[1], 0.0f, currentPosition[0]));
        
        ubo.model = glm::scale(ubo.model, glm::vec3(1.75f, 4.0f, 1.75f));
        vkMapMemory(device, DS_Disk.uniformBuffersMemory[0][currentImage], 0,
                            sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device, DS_Disk.uniformBuffersMemory[0][currentImage]);
        
        //SCORE
        for(int i = 0; i < 5; i++)
        {
            // SCORE PL2 V1
            ubo.model = initalModel;
            if(i == game.getPlayer2Score())
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0.0f, 0.0001f, 0.0f));
            }
            
            ubo.model = glm::rotate(ubo.model, glm::radians(90.0f), glm::vec3(0.0,1.0,0.0));
            ubo.model = glm::translate(ubo.model, glm::vec3(0.1952f, 0.0f, -0.568f));
            ubo.model = glm::scale(ubo.model, glm::vec3(0.45));
            
            vkMapMemory(device, DS_PL2_V1[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_PL2_V1[i].uniformBuffersMemory[0][currentImage]);
            
            // SCORE PL1 V1
            ubo.model = initalModel;
            if(i == game.getPlayer1Score())
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0.0f, 0.0001f, 0.0f));
            }
            
            ubo.model = glm::rotate(ubo.model, glm::radians(90.0f), glm::vec3(0.0,1.0,0.0));
            ubo.model = glm::translate(ubo.model, glm::vec3(-0.1952f, 0.0f, -0.568f));
            ubo.model = glm::scale(ubo.model, glm::vec3(0.45));
            
            vkMapMemory(device, DS_PL1_V1[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_PL1_V1[i].uniformBuffersMemory[0][currentImage]);
            
            // SCORE PL2 V2
            ubo.model = initalModel;
            if(i == game.getPlayer2Score())
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0.0f, 0.0001f, 0.0f));
            }
            
            ubo.model = glm::rotate(ubo.model, glm::radians(-90.0f), glm::vec3(0.0,1.0,0.0));
            ubo.model = glm::translate(ubo.model, glm::vec3(-0.1952f, 0.0f, -0.568f));
            ubo.model = glm::scale(ubo.model, glm::vec3(0.45));
            
            vkMapMemory(device, DS_PL2_V2[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_PL2_V2[i].uniformBuffersMemory[0][currentImage]);
            
            // SCORE PL1 V2
            ubo.model = initalModel;
            if(i == game.getPlayer1Score())
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0.0f, 0.0001f, 0.0f));
            }
            
            ubo.model = glm::rotate(ubo.model, glm::radians(-90.0f), glm::vec3(0.0,1.0,0.0));
            ubo.model = glm::translate(ubo.model, glm::vec3(0.1952f, 0.0f, -0.568f));
            ubo.model = glm::scale(ubo.model, glm::vec3(0.45));
            
            vkMapMemory(device, DS_PL1_V2[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_PL1_V2[i].uniformBuffersMemory[0][currentImage]);
        }
	}
};

// This is the main: probably you do not need to touch this!
int main() {
    MyProject app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


