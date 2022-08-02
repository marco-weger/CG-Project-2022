// This has been adapted from the Vulkan tutorial
#include "MyProject.hpp"
#include "Game.cpp"

#include <algorithm>
#include <string>

const std::string MODEL_DIR = "models/";
const std::string TEXTURE_PATH = "textures/StarWarsPinball.png";

// The uniform buffer object used in this example
struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

// MAIN ! 
class MyProject : public BaseProject {
	protected:
	// Here you list all the Vulkan objects you need:
	
	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSL1;

	// Pipelines [Shader couples]
	Pipeline P1;

	// Models, textures and Descriptors (values assigned to the uniforms)
    int elemCount = 10 + 12 * 10;
    
	Model M_Body;
	Texture T_Body;
	DescriptorSet DS_Body;
    
    Model M_Ball;
    Texture T_Ball;
    DescriptorSet DS_Ball;
    
    Model M_bumper1;
    Texture T_bumper1;
    DescriptorSet DS_bumper1;
    Model M_bumper2;
    Texture T_bumper2;
    DescriptorSet DS_bumper2;
    Model M_bumper3;
    Texture T_bumper3;
    DescriptorSet DS_bumper3;
    
    Model M_LeftButton;
    Texture T_LeftButton;
    DescriptorSet DS_LeftButton;
    Model M_RightButton;
    Texture T_RightButton;
    DescriptorSet DS_RightButton;
    Model M_LeftFlipper;
    Texture T_LeftFlipper;
    DescriptorSet DS_LeftFlipper;
    Model M_RightFlipper;
    Texture T_RightFlipper;
    DescriptorSet DS_RightFlipper;
    
    Model M_Puller;
    Texture T_Puller;
    DescriptorSet DS_Puller;
    
    Model M_DL1[10];
    Texture T_DL1[10];
    DescriptorSet DS_DL1[10];
    Model M_DL2[10];
    Texture T_DL2[10];
    DescriptorSet DS_DL2[10];
    Model M_DL3[10];
    Texture T_DL3[10];
    DescriptorSet DS_DL3[10];
    Model M_DL4[10];
    Texture T_DL4[10];
    DescriptorSet DS_DL4[10];
    Model M_DL5[10];
    Texture T_DL5[10];
    DescriptorSet DS_DL5[10];
    Model M_DL6[10];
    Texture T_DL6[10];
    DescriptorSet DS_DL6[10];
    Model M_DR1[10];
    Texture T_DR1[10];
    DescriptorSet DS_DR1[10];
    Model M_DR2[10];
    Texture T_DR2[10];
    DescriptorSet DS_DR2[10];
    Model M_DR3[10];
    Texture T_DR3[10];
    DescriptorSet DS_DR3[10];
    Model M_DR4[10];
    Texture T_DR4[10];
    DescriptorSet DS_DR4[10];
    Model M_DR5[10];
    Texture T_DR5[10];
    DescriptorSet DS_DR5[10];
    Model M_DR6[10];
    Texture T_DR6[10];
    DescriptorSet DS_DR6[10];
    
    // Camera
    glm::mat3 CamDir = glm::mat3(1.0f);
    glm::vec3 CamPos = glm::vec3(9.83158f, 14.6194f, -15.6189f);
    glm::vec3 CamAng = glm::vec3(-0.300168f, 2.41573f, 0.0702695f);
    
    // Game
    Game game;

    // Bumper scale time
    float bumper1Time = 0;
    float bumper2Time = 0;
    float bumper3Time = 0;
    
	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Pinball";
		initialBackgroundColor = {1.0f, 1.0f, 1.0f, 1.0f};
		
		// Descriptor pool sizes
		uniformBlocksInPool = elemCount;
		texturesInPool = elemCount;
		setsInPool = elemCount;
	}
	
	// Here you load and setup all your Vulkan objects
	void localInit() {
        game.initializeBall();
        
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
		P1.init(this, "shaders/vert.spv", "shaders/frag.spv", {&DSL1});

		// Models, textures and Descriptors (values assigned to the uniforms)
		M_Body.init(this, MODEL_DIR + "/Body.obj");
		T_Body.init(this, TEXTURE_PATH);
		DS_Body.init(this, &DSL1, {
		// the second parameter, is a pointer to the Uniform Set Layout of this set
		// the last parameter is an array, with one element per binding of the set.
		// first  elmenet : the binding number
		// second element : UNIFORM or TEXTURE (an enum) depending on the type
		// third  element : only for UNIFORMs, the size of the corresponding C++ object
		// fourth element : only for TEXTUREs, the pointer to the corresponding texture object
					{0, UNIFORM, sizeof(UniformBufferObject), nullptr},
					{1, TEXTURE, 0, &T_Body}
				});
        
        M_Ball.init(this, MODEL_DIR + "/Ball.obj");
        T_Ball.init(this, TEXTURE_PATH);
        DS_Ball.init(this, &DSL1, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &T_Ball}
                });
        
        M_bumper1.init(this, MODEL_DIR + "/bumper1.obj");
        T_bumper1.init(this, TEXTURE_PATH);
        DS_bumper1.init(this, &DSL1, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &T_bumper1}
                });
        M_bumper2.init(this, MODEL_DIR + "/bumper2.obj");
        T_bumper2.init(this, TEXTURE_PATH);
        DS_bumper2.init(this, &DSL1, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &T_bumper2}
                });
        M_bumper3.init(this, MODEL_DIR + "/bumper3.obj");
        T_bumper3.init(this, TEXTURE_PATH);
        DS_bumper3.init(this, &DSL1, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &T_bumper3}
                });
        
        M_LeftButton.init(this, MODEL_DIR + "/LeftButton.obj");
        T_LeftButton.init(this, TEXTURE_PATH);
        DS_LeftButton.init(this, &DSL1, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &T_LeftButton}
                });
        M_RightButton.init(this, MODEL_DIR + "/RightButton.obj");
        T_RightButton.init(this, TEXTURE_PATH);
        DS_RightButton.init(this, &DSL1, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &T_RightButton}
                });
        M_LeftFlipper.init(this, MODEL_DIR + "/LeftFlipper.obj");
        T_LeftFlipper.init(this, TEXTURE_PATH);
        DS_LeftFlipper.init(this, &DSL1, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &T_LeftFlipper}
                });
        M_RightFlipper.init(this, MODEL_DIR + "/RightFlipper.obj");
        T_RightFlipper.init(this, TEXTURE_PATH);
        DS_RightFlipper.init(this, &DSL1, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &T_RightFlipper}
                });
        
        M_Puller.init(this, MODEL_DIR + "/Puller.obj");
        T_Puller.init(this, TEXTURE_PATH);
        DS_Puller.init(this, &DSL1, {
                    {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                    {1, TEXTURE, 0, &T_Puller}
                });
        
        const char* map[10] = {"DR3", "DL6", "DL5", "DL4", "DL3", "DL2", "DL1", "DR6", "DR5", "DR4"};
        for(int i = 0; i < 10; i++)
        {
            M_DL1[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_DL1[i].init(this, TEXTURE_PATH);
            DS_DL1[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_DL1[i]}
                    });
            M_DL2[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_DL2[i].init(this, TEXTURE_PATH);
            DS_DL2[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_DL2[i]}
                    });
            M_DL3[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_DL3[i].init(this, TEXTURE_PATH);
            DS_DL3[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_DL3[i]}
                    });
            M_DL4[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_DL4[i].init(this, TEXTURE_PATH);
            DS_DL4[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_DL4[i]}
                    });
            M_DL5[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_DL5[i].init(this, TEXTURE_PATH);
            DS_DL5[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_DL5[i]}
                    });
            M_DL6[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_DL6[i].init(this, TEXTURE_PATH);
            DS_DL6[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_DL6[i]}
                    });
            
            M_DR1[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_DR1[i].init(this, TEXTURE_PATH);
            DS_DR1[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_DR1[i]}
                    });
            M_DR2[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_DR2[i].init(this, TEXTURE_PATH);
            DS_DR2[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_DR2[i]}
                    });
            M_DR3[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_DR3[i].init(this, TEXTURE_PATH);
            DS_DR3[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_DR3[i]}
                    });
            M_DR4[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_DR4[i].init(this, TEXTURE_PATH);
            DS_DR4[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_DR4[i]}
                    });
            M_DR5[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_DR5[i].init(this, TEXTURE_PATH);
            DS_DR5[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_DR5[i]}
                    });
            M_DR6[i].init(this, MODEL_DIR + "/" + map[i] + ".obj");
            T_DR6[i].init(this, TEXTURE_PATH);
            DS_DR6[i].init(this, &DSL1, {
                        {0, UNIFORM, sizeof(UniformBufferObject), nullptr},
                        {1, TEXTURE, 0, &T_DR6[i]}
                    });
        }
	}

	// Here you destroy all the objects you created!		
	void localCleanup() {
		DS_Body.cleanup();
		T_Body.cleanup();
		M_Body.cleanup();
        
        DS_Ball.cleanup();
        T_Ball.cleanup();
        M_Ball.cleanup();
        
        DS_bumper1.cleanup();
        T_bumper1.cleanup();
        M_bumper1.cleanup();
        DS_bumper2.cleanup();
        T_bumper2.cleanup();
        M_bumper2.cleanup();
        DS_bumper3.cleanup();
        T_bumper3.cleanup();
        M_bumper3.cleanup();
        
        DS_LeftButton.cleanup();
        T_LeftButton.cleanup();
        M_LeftButton.cleanup();
        DS_RightButton.cleanup();
        T_RightButton.cleanup();
        M_RightButton.cleanup();
        DS_LeftFlipper.cleanup();
        T_LeftFlipper.cleanup();
        M_LeftFlipper.cleanup();
        DS_RightFlipper.cleanup();
        T_RightFlipper.cleanup();
        M_RightFlipper.cleanup();
        
        DS_Puller.cleanup();
        T_Puller.cleanup();
        M_Puller.cleanup();
        
        for(int i = 0; i < 10; i++)
        {
            DS_DL1[i].cleanup();
            T_DL1[i].cleanup();
            M_DL1[i].cleanup();
            DS_DL2[i].cleanup();
            T_DL2[i].cleanup();
            M_DL2[i].cleanup();
            DS_DL3[i].cleanup();
            T_DL3[i].cleanup();
            M_DL3[i].cleanup();
            DS_DL4[i].cleanup();
            T_DL4[i].cleanup();
            M_DL4[i].cleanup();
            DS_DL5[i].cleanup();
            T_DL5[i].cleanup();
            M_DL5[i].cleanup();
            DS_DL6[i].cleanup();
            T_DL6[i].cleanup();
            M_DL6[i].cleanup();
            
            DS_DR1[i].cleanup();
            T_DR1[i].cleanup();
            M_DR1[i].cleanup();
            DS_DR2[i].cleanup();
            T_DR2[i].cleanup();
            M_DR2[i].cleanup();
            DS_DR3[i].cleanup();
            T_DR3[i].cleanup();
            M_DR3[i].cleanup();
            DS_DR4[i].cleanup();
            T_DR4[i].cleanup();
            M_DR4[i].cleanup();
            DS_DR5[i].cleanup();
            T_DR5[i].cleanup();
            M_DR5[i].cleanup();
            DS_DR6[i].cleanup();
            T_DR6[i].cleanup();
            M_DR6[i].cleanup();
        }
        
		P1.cleanup();
		DSL1.cleanup();
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
				
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
				P1.graphicsPipeline);
				
		VkBuffer vertexBuffers1[] = {M_Body.vertexBuffer};
		// property .vertexBuffer of models, contains the VkBuffer handle to its vertex buffer
		VkDeviceSize offsets1[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers1, offsets1);
		// property .indexBuffer of models, contains the VkBuffer handle to its index buffer
		vkCmdBindIndexBuffer(commandBuffer, M_Body.indexBuffer, 0,
								VK_INDEX_TYPE_UINT32);

		// property .pipelineLayout of a pipeline contains its layout.
		// property .descriptorSets of a descriptor set contains its elements.
		vkCmdBindDescriptorSets(commandBuffer,
						VK_PIPELINE_BIND_POINT_GRAPHICS,
						P1.pipelineLayout, 0, 1, &DS_Body.descriptorSets[currentImage],
						0, nullptr);
						
		// property .indices.size() of models, contains the number of triangles * 3 of the mesh.
		vkCmdDrawIndexed(commandBuffer,
					static_cast<uint32_t>(M_Body.indices.size()), 1, 0, 0, 0);
        
        VkBuffer vertexBuffers2[] = {M_Ball.vertexBuffer};
        VkDeviceSize offsets2[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers2, offsets2);
        vkCmdBindIndexBuffer(commandBuffer, M_Ball.indexBuffer, 0,
                                VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        P1.pipelineLayout, 0, 1, &DS_Ball.descriptorSets[currentImage],
                        0, nullptr);
        vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M_Ball.indices.size()), 1, 0, 0, 0);
        
        VkBuffer vertexBuffers3[] = {M_bumper1.vertexBuffer};
        VkDeviceSize offsets3[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers3, offsets3);
        vkCmdBindIndexBuffer(commandBuffer, M_bumper1.indexBuffer, 0,
                                VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        P1.pipelineLayout, 0, 1, &DS_bumper1.descriptorSets[currentImage],
                        0, nullptr);
        vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M_bumper1.indices.size()), 1, 0, 0, 0);
        
        VkBuffer vertexBuffers4[] = {M_bumper2.vertexBuffer};
        VkDeviceSize offsets4[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers4, offsets4);
        vkCmdBindIndexBuffer(commandBuffer, M_bumper2.indexBuffer, 0,
                                VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        P1.pipelineLayout, 0, 1, &DS_bumper2.descriptorSets[currentImage],
                        0, nullptr);
        vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M_bumper2.indices.size()), 1, 0, 0, 0);
        
        VkBuffer vertexBuffers5[] = {M_bumper3.vertexBuffer};
        VkDeviceSize offsets5[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers5, offsets5);
        vkCmdBindIndexBuffer(commandBuffer, M_bumper3.indexBuffer, 0,
                                VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        P1.pipelineLayout, 0, 1, &DS_bumper3.descriptorSets[currentImage],
                        0, nullptr);
        vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M_bumper3.indices.size()), 1, 0, 0, 0);
        
        VkBuffer vertexBuffers6[] = {M_LeftButton.vertexBuffer};
        VkDeviceSize offsets6[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers6, offsets6);
        vkCmdBindIndexBuffer(commandBuffer, M_LeftButton.indexBuffer, 0,
                                VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        P1.pipelineLayout, 0, 1, &DS_LeftButton.descriptorSets[currentImage],
                        0, nullptr);
        vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M_LeftButton.indices.size()), 1, 0, 0, 0);
        
        VkBuffer vertexBuffers7[] = {M_RightButton.vertexBuffer};
        VkDeviceSize offsets7[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers7, offsets7);
        vkCmdBindIndexBuffer(commandBuffer, M_RightButton.indexBuffer, 0,
                                VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        P1.pipelineLayout, 0, 1, &DS_RightButton.descriptorSets[currentImage],
                        0, nullptr);
        vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M_RightButton.indices.size()), 1, 0, 0, 0);
        
        VkBuffer vertexBuffers8[] = {M_LeftFlipper.vertexBuffer};
        VkDeviceSize offsets8[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers8, offsets8);
        vkCmdBindIndexBuffer(commandBuffer, M_LeftFlipper.indexBuffer, 0,
                                VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        P1.pipelineLayout, 0, 1, &DS_LeftFlipper.descriptorSets[currentImage],
                        0, nullptr);
        vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M_LeftFlipper.indices.size()), 1, 0, 0, 0);
        
        VkBuffer vertexBuffers9[] = {M_RightFlipper.vertexBuffer};
        VkDeviceSize offsets9[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers9, offsets9);
        vkCmdBindIndexBuffer(commandBuffer, M_RightFlipper.indexBuffer, 0,
                                VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        P1.pipelineLayout, 0, 1, &DS_RightFlipper.descriptorSets[currentImage],
                        0, nullptr);
        vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M_RightFlipper.indices.size()), 1, 0, 0, 0);
        
        VkBuffer vertexBuffers10[] = {M_Puller.vertexBuffer};
        VkDeviceSize offsets10[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers10, offsets10);
        vkCmdBindIndexBuffer(commandBuffer, M_Puller.indexBuffer, 0,
                                VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        P1.pipelineLayout, 0, 1, &DS_Puller.descriptorSets[currentImage],
                        0, nullptr);
        vkCmdDrawIndexed(commandBuffer,
                    static_cast<uint32_t>(M_Puller.indices.size()), 1, 0, 0, 0);
        
        for(int i = 0; i < 10; i++)
        {
            VkBuffer vertexBuffers_DL1[] = {M_DL1[i].vertexBuffer};
            VkDeviceSize offsets_DL1[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_DL1, offsets_DL1);
            vkCmdBindIndexBuffer(commandBuffer, M_DL1[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_DL1[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_DL1[i].indices.size()), 1, 0, 0, 0);
            
            VkBuffer vertexBuffers_DL2[] = {M_DL2[i].vertexBuffer};
            VkDeviceSize offsets_DL2[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_DL2, offsets_DL2);
            vkCmdBindIndexBuffer(commandBuffer, M_DL2[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_DL2[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_DL2[i].indices.size()), 1, 0, 0, 0);
            
            VkBuffer vertexBuffers_DL3[] = {M_DL3[i].vertexBuffer};
            VkDeviceSize offsets_DL3[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_DL3, offsets_DL3);
            vkCmdBindIndexBuffer(commandBuffer, M_DL3[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_DL3[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_DL3[i].indices.size()), 1, 0, 0, 0);
            
            VkBuffer vertexBuffers_DL4[] = {M_DL4[i].vertexBuffer};
            VkDeviceSize offsets_DL4[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_DL4, offsets_DL4);
            vkCmdBindIndexBuffer(commandBuffer, M_DL4[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_DL4[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_DL4[i].indices.size()), 1, 0, 0, 0);
            
            VkBuffer vertexBuffers_DL5[] = {M_DL5[i].vertexBuffer};
            VkDeviceSize offsets_DL5[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_DL5, offsets_DL5);
            vkCmdBindIndexBuffer(commandBuffer, M_DL5[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_DL5[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_DL5[i].indices.size()), 1, 0, 0, 0);
            
            VkBuffer vertexBuffers_DL6[] = {M_DL6[i].vertexBuffer};
            VkDeviceSize offsets_DL6[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_DL6, offsets_DL6);
            vkCmdBindIndexBuffer(commandBuffer, M_DL6[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_DL6[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_DL6[i].indices.size()), 1, 0, 0, 0);
            
            VkBuffer vertexBuffers_DR1[] = {M_DR1[i].vertexBuffer};
            VkDeviceSize offsets_DR1[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_DR1, offsets_DR1);
            vkCmdBindIndexBuffer(commandBuffer, M_DR1[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_DR1[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_DR1[i].indices.size()), 1, 0, 0, 0);
            
            VkBuffer vertexBuffers_DR2[] = {M_DR2[i].vertexBuffer};
            VkDeviceSize offsets_DR2[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_DR2, offsets_DR2);
            vkCmdBindIndexBuffer(commandBuffer, M_DR2[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_DR2[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_DR2[i].indices.size()), 1, 0, 0, 0);
            
            VkBuffer vertexBuffers_DR3[] = {M_DR3[i].vertexBuffer};
            VkDeviceSize offsets_DR3[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_DR3, offsets_DR3);
            vkCmdBindIndexBuffer(commandBuffer, M_DR3[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_DR3[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_DR3[i].indices.size()), 1, 0, 0, 0);
            
            VkBuffer vertexBuffers_DR4[] = {M_DR4[i].vertexBuffer};
            VkDeviceSize offsets_DR4[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_DR4, offsets_DR4);
            vkCmdBindIndexBuffer(commandBuffer, M_DR4[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_DR4[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_DR4[i].indices.size()), 1, 0, 0, 0);
            
            VkBuffer vertexBuffers_DR5[] = {M_DR5[i].vertexBuffer};
            VkDeviceSize offsets_DR5[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_DR5, offsets_DR5);
            vkCmdBindIndexBuffer(commandBuffer, M_DR5[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_DR5[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_DR5[i].indices.size()), 1, 0, 0, 0);
            
            VkBuffer vertexBuffers_DR6[] = {M_DR6[i].vertexBuffer};
            VkDeviceSize offsets_DR6[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers_DR6, offsets_DR6);
            vkCmdBindIndexBuffer(commandBuffer, M_DR6[i].indexBuffer, 0,
                                    VK_INDEX_TYPE_UINT32);
            vkCmdBindDescriptorSets(commandBuffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            P1.pipelineLayout, 0, 1, &DS_DR6[i].descriptorSets[currentImage],
                            0, nullptr);
            vkCmdDrawIndexed(commandBuffer,
                        static_cast<uint32_t>(M_DR6[i].indices.size()), 1, 0, 0, 0);
        }
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
        /* FROM A19 */
        static auto startTime = std::chrono::high_resolution_clock::now();
        static float lastTime = 0.0f;
        
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>
                    (currentTime - startTime).count();
        float deltaT = time - lastTime;
        lastTime = time;
    
        const float ROT_SPEED = glm::radians(60.0f);
        const float MOVE_SPEED = 2.5f;
        const float MOUSE_RES = 500.0f;
        
        static double old_xpos = 0, old_ypos = 0;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        double m_dx = xpos - old_xpos;
        double m_dy = ypos - old_ypos;
        old_xpos = xpos; old_ypos = ypos;
        //std::cout << xpos << " " << ypos << " " << m_dx << " " << m_dy << "\n";

        glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            CamAng.y += m_dx * ROT_SPEED / MOUSE_RES;
            CamAng.x += m_dy * ROT_SPEED / MOUSE_RES;
        }

        if(glfwGetKey(window, GLFW_KEY_LEFT)) {
            CamAng.y += deltaT * ROT_SPEED;
        }
        if(glfwGetKey(window, GLFW_KEY_RIGHT)) {
            CamAng.y -= deltaT * ROT_SPEED;
        }
        if(glfwGetKey(window, GLFW_KEY_UP)) {
            CamAng.x -= deltaT * ROT_SPEED;
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN)) {
            CamAng.x += deltaT * ROT_SPEED;
        }
        if(glfwGetKey(window, GLFW_KEY_Q)) {
            CamAng.z -= deltaT * ROT_SPEED;
        }
        if(glfwGetKey(window, GLFW_KEY_E)) {
            CamAng.z += deltaT * ROT_SPEED;
        }
        //std::cout << "Cam Ang: " << CamAng.x << " " << CamAng.y << " " << CamAng.z << "\n";
        
        glm::mat3 CamDir = glm::mat3(glm::rotate(glm::mat4(1.0f), CamAng.y, glm::vec3(0.0f, 1.0f, 0.0f))) *
                           glm::mat3(glm::rotate(glm::mat4(1.0f), CamAng.x, glm::vec3(1.0f, 0.0f, 0.0f))) *
                           glm::mat3(glm::rotate(glm::mat4(1.0f), CamAng.z, glm::vec3(0.0f, 0.0f, 1.0f)));
        
        
        if(glfwGetKey(window, GLFW_KEY_A)) {
            CamPos -= MOVE_SPEED * glm::vec3(glm::rotate(glm::mat4(1.0f), CamAng.y,
                                    glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(1,0,0,1)) * deltaT;
        }
        if(glfwGetKey(window, GLFW_KEY_D)) {
            CamPos += MOVE_SPEED * glm::vec3(glm::rotate(glm::mat4(1.0f), CamAng.y,
                                    glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(1,0,0,1)) * deltaT;
        }
        if(glfwGetKey(window, GLFW_KEY_S)) {
            CamPos += MOVE_SPEED * glm::vec3(glm::rotate(glm::mat4(1.0f), CamAng.y,
                                    glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0,0,1,1)) * deltaT;
        }
        if(glfwGetKey(window, GLFW_KEY_W)) {
            CamPos -= MOVE_SPEED * glm::vec3(glm::rotate(glm::mat4(1.0f), CamAng.y,
                                    glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0,0,1,1)) * deltaT;
        }
        if(glfwGetKey(window, GLFW_KEY_F)) {
            CamPos -= MOVE_SPEED * glm::vec3(0,1,0) * deltaT;
        }
        if(glfwGetKey(window, GLFW_KEY_R)) {
            CamPos += MOVE_SPEED * glm::vec3(0,1,0) * deltaT;
        }
        //std::cout << "Cam Pos: " << CamPos[0] << " " << CamPos[1] << " " << CamPos[2] << "\n";
        /* FROM A19 */
					
					
		UniformBufferObject ubo{};
        ubo.view = glm::lookAt(glm::vec3(1.0f, 1.0f, 10.0f),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f));
        
        
        //ubo.view = LookInDirMat(CamPos, CamAng);
		ubo.proj = glm::perspective(glm::radians(45.0f),
						swapChainExtent.width / (float) swapChainExtent.height,
						0.1f, 50.0f);
		ubo.proj[1][1] *= -1;
		
		void* data;
        
        glm::vec3 initial;

		// Body
        ubo.model = glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos);
		vkMapMemory(device, DS_Body.uniformBuffersMemory[0][currentImage], 0,
							sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(device, DS_Body.uniformBuffersMemory[0][currentImage]);
        
        // LeftButton
        initial = glm::vec3(2.6175f, 8.7853f, -6.6902f);
        ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
        ubo.model = glm::rotate(ubo.model, glm::radians(-90.0f), glm::vec3(0.0f,0.0f,1.0f));
        
        // LeftButton manager
        if(game.getStatus() == status::started && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
        {
            ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.01f,0));
        }
        
        vkMapMemory(device, DS_LeftButton.uniformBuffersMemory[0][currentImage], 0,
                            sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device, DS_LeftButton.uniformBuffersMemory[0][currentImage]);
        
        // RightButton
        initial = glm::vec3(-2.97f, 8.7853f, -6.6902f);
        ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
        ubo.model = glm::rotate(ubo.model, glm::radians(-90.0f), glm::vec3(0.0f,0.0f,1.0f));
        
        // RightButton manager
        if(game.getStatus() == status::started && glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))
        {
            ubo.model = glm::translate(ubo.model, glm::vec3(0,0.01f,0));
        }
        
        vkMapMemory(device, DS_RightButton.uniformBuffersMemory[0][currentImage], 0,
                            sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device, DS_RightButton.uniformBuffersMemory[0][currentImage]);
        
        // LeftFlipper
        initial = glm::vec3(0.6906f, 8.4032f, -5.6357f);
        ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
        
        // LeftFlipper manager
        if(game.getStatus() == status::started && game.isBallInGame())
        {
            if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
            {
                game.setLeftFlipperDegree(game.getLeftFlipperDegree() + 90.0f * deltaT / Game::openFlipperTime);
            }
            else
            {
                game.setLeftFlipperDegree(game.getLeftFlipperDegree() - 90.0f * deltaT / Game::openFlipperTime);
            }
            ubo.model = glm::rotate(ubo.model, glm::radians(game.getLeftFlipperDegree()), glm::vec3(0.0f,1.0f,0.0f));
        }
        else
        {
            game.setLeftFlipperDegree(90.0f);
            ubo.model = glm::rotate(ubo.model, glm::radians(90.0f), glm::vec3(0.0f,1.0f,0.0f));
        }
        
        vkMapMemory(device, DS_LeftFlipper.uniformBuffersMemory[0][currentImage], 0,
                            sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device, DS_LeftFlipper.uniformBuffersMemory[0][currentImage]);
        
        // RightFlipper
        initial = glm::vec3(-1.307f, 8.4032f, -5.6357f);
        ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
        ubo.model = glm::rotate(ubo.model, glm::radians(180.0f), glm::vec3(0.0f,1.0f,0.0f));
        
        // RightFlipper manager
        if(game.getStatus() == status::started && game.isBallInGame())
        {
            if(glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))
            {
                game.setRightFlipperDegree(game.getRightFlipperDegree() + 90.0f * deltaT / Game::openFlipperTime);
            }
            else
            {
                game.setRightFlipperDegree(game.getRightFlipperDegree() - 90.0f * deltaT / Game::openFlipperTime);
            }
            ubo.model = glm::rotate(ubo.model, glm::radians(-game.getRightFlipperDegree()), glm::vec3(0.0f,1.0f,0.0f));
        }
        else
        {
            game.setRightFlipperDegree(90.0f);
            ubo.model = glm::rotate(ubo.model, glm::radians(-90.0f), glm::vec3(0.0f,1.0f,0.0f));
        }
        
        vkMapMemory(device, DS_RightFlipper.uniformBuffersMemory[0][currentImage], 0,
                            sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device, DS_RightFlipper.uniformBuffersMemory[0][currentImage]);
        
        // Puller
        initial = glm::vec3(-2.5264f, 8.3925f, -7.5892f);
        ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
        ubo.model = glm::rotate(ubo.model, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
        ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.4f,0));
        
        // Puller manager
        if(glfwGetKey(window, GLFW_KEY_SPACE) && CamPos[0] >= -5 && CamPos[0] <= 5 && CamPos[1] >= 5 && CamPos[1] <= 15 && CamPos[2] >= -10 && CamPos[2] <= 10)
        {
            //std::cout << CamPos[0] << " " << CamPos[1] << " " << CamPos[2] << "\n";
            
            if(game.getStatus() == status::stopped)
            {
                game.setStatus(status::starting);
                game.setWaitingTime(0);
            }
            
            if(game.getStatus() == status::starting)
            {
                if(game.getWaitingTime() < Game::maxWaitingTime){
                    game.setWaitingTime(game.getWaitingTime() + deltaT);
                }
                
                ubo.model = glm::translate(ubo.model, glm::vec3(0, (MOVE_SPEED/2) * game.getWaitingTime() / 10,0));
            }
        }
        else
        {
            if(game.getStatus() == status::starting)
            {
                game.initializeBall();
                game.setStatus(status::started);
            }
        }
        
        if((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) && glfwGetKey(window, GLFW_KEY_C))
        {
            game.initializeBall();
            game.setStatus(status::stopped);
        }
        
        vkMapMemory(device, DS_Puller.uniformBuffersMemory[0][currentImage], 0,
                            sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device, DS_Puller.uniformBuffersMemory[0][currentImage]);
        
        // Ball
        initial = glm::vec3(-0.30053f, 8.5335f, -5.9728f);
        ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
        //game.setBallPosition(ubo.model);
        
        // Ball manager
        if(game.getStatus() == status::started)
        {
            game.detectCollision();
            game.setBallPosition(game.getBallPosition() + game.getBallDirection() * game.getBallVelocity());
            game.detectEndGame();
        }
        else
        {
            game.initializeBall();
        }
        ubo.model = glm::translate(ubo.model, game.getBallPosition());
        
        vkMapMemory(device, DS_Ball.uniformBuffersMemory[0][currentImage], 0,
                            sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device, DS_Ball.uniformBuffersMemory[0][currentImage]);
        
        // bumper scale increment
        float bumperInc = 1.15f;
        
        // bumper1
        initial = glm::vec3(1.1819f, 9.1362f, 0.020626f);
        ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
        ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05f,0));
        ubo.model = glm::rotate(ubo.model, glm::radians(-8.0f), glm::vec3(1,0,0));
        if(game.getCollisionType() == "BU1")
        {
            bumper1Time = 0.075f;
        }
        if(bumper1Time > 0)
        {
            bumper1Time -= deltaT;
            ubo.model = glm::scale(ubo.model, glm::vec3(bumperInc,bumperInc,bumperInc));
        }
        vkMapMemory(device, DS_bumper1.uniformBuffersMemory[0][currentImage], 0,
                            sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device, DS_bumper1.uniformBuffersMemory[0][currentImage]);
        
        // bumper2
        initial = glm::vec3(-1.5055f, 9.1362f, 0.020626f);
        ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
        ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05f,0));
        ubo.model = glm::rotate(ubo.model, glm::radians(-8.0f), glm::vec3(1,0,0));
        if(game.getCollisionType() == "BU2")
        {
            bumper2Time = 0.075f;
        }
        if(bumper2Time > 0)
        {
            bumper2Time -= deltaT;
            ubo.model = glm::scale(ubo.model, glm::vec3(bumperInc,bumperInc,bumperInc));
        }
        vkMapMemory(device, DS_bumper2.uniformBuffersMemory[0][currentImage], 0,
                            sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device, DS_bumper2.uniformBuffersMemory[0][currentImage]);
        
        // bumper3
        initial = glm::vec3(-0.11626f, 9.1362f, 0.020626f);
        ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
        ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05f,0));
        ubo.model = glm::rotate(ubo.model, glm::radians(-8.0f), glm::vec3(1,0,0));
        if(game.getCollisionType() == "BU3")
        {
            bumper3Time = 0.075f;
        }
        if(bumper3Time > 0)
        {
            bumper3Time -= deltaT;
            ubo.model = glm::scale(ubo.model, glm::vec3(bumperInc,bumperInc,bumperInc));
        }
        vkMapMemory(device, DS_bumper3.uniformBuffersMemory[0][currentImage], 0,
                            sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(device, DS_bumper3.uniformBuffersMemory[0][currentImage]);
        
        std::string stringRecord = std::to_string(game.getRecord());
        if(stringRecord == "-1")
        {
            stringRecord = "000000";
        }
        if(stringRecord.length() < 6)
        {
            stringRecord = std::string(6 - stringRecord.length(), '0') + stringRecord;
        }
        
        std::string stringScore = std::to_string(game.getScore());
        if(stringScore == "-1")
        {
            stringScore = "000000";
        }
        if(stringScore.length() < 6)
        {
            stringScore = std::string(6 - stringScore.length(), '0') + stringScore;
        }

        for(int i = 0; i < 10; i++)
        {
            // DL1
            initial = glm::vec3(0.4366f, 12.789f, 4.1852f);
            ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
            ubo.model = glm::rotate(ubo.model, glm::radians(-100.0f), glm::vec3(1,0,0));
            if(i != ((int)stringRecord.at(5)-48))
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05,0));
            }
            vkMapMemory(device, DS_DL1[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_DL1[i].uniformBuffersMemory[0][currentImage]);
            
            // DL2
            initial = glm::vec3(0.713f, 12.789f, 4.1852f);
            ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
            ubo.model = glm::rotate(ubo.model, glm::radians(-100.0f), glm::vec3(1,0,0));
            if(i != ((int)stringRecord.at(4)-48))
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05,0));
            }
            vkMapMemory(device, DS_DL2[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_DL2[i].uniformBuffersMemory[0][currentImage]);
            
            // DL3
            initial = glm::vec3(0.9923f, 12.789f, 4.1852f);
            ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
            ubo.model = glm::rotate(ubo.model, glm::radians(-100.0f), glm::vec3(1,0,0));
            if(i != ((int)stringRecord.at(3)-48))
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05,0));
            }
            vkMapMemory(device, DS_DL3[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_DL3[i].uniformBuffersMemory[0][currentImage]);
            
            // DL4
            initial = glm::vec3(1.3917f, 12.789f, 4.1852f);
            ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
            ubo.model = glm::rotate(ubo.model, glm::radians(-100.0f), glm::vec3(1,0,0));
            if(i != ((int)stringRecord.at(2)-48))
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05,0));
            }
            vkMapMemory(device, DS_DL4[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_DL4[i].uniformBuffersMemory[0][currentImage]);
            
            // DL5
            initial = glm::vec3(1.6681f, 12.789f, 4.1852f);
            ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
            ubo.model = glm::rotate(ubo.model, glm::radians(-100.0f), glm::vec3(1,0,0));
            if(i != ((int)stringRecord.at(1)-48))
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05,0));
            }
            vkMapMemory(device, DS_DL5[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_DL5[i].uniformBuffersMemory[0][currentImage]);
            
            // DL6
            initial = glm::vec3(1.9474f, 12.789f, 4.1852f);
            ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
            ubo.model = glm::rotate(ubo.model, glm::radians(-100.0f), glm::vec3(1,0,0));
            if(i != ((int)stringRecord.at(0)-48))
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05,0));
            }
            vkMapMemory(device, DS_DL6[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_DL6[i].uniformBuffersMemory[0][currentImage]);
            
            // DR1
            initial = glm::vec3(-2.8273f, 12.789f, 4.1852f);
            ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
            ubo.model = glm::rotate(ubo.model, glm::radians(-100.0f), glm::vec3(1,0,0));
            if(i != ((int)stringScore.at(5)-48))
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05,0));
            }
            vkMapMemory(device, DS_DR1[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_DR1[i].uniformBuffersMemory[0][currentImage]);
            
            // DR2
            initial = glm::vec3(-2.5509f, 12.789f, 4.1852f);
            ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
            ubo.model = glm::rotate(ubo.model, glm::radians(-100.0f), glm::vec3(1,0,0));
            if(i != ((int)stringScore.at(4)-48))
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05,0));
            }
            vkMapMemory(device, DS_DR2[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_DR2[i].uniformBuffersMemory[0][currentImage]);
            
            // DR3
            initial = glm::vec3(-2.2716f, 12.789f, 4.1852f);
            ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
            ubo.model = glm::rotate(ubo.model, glm::radians(-100.0f), glm::vec3(1,0,0));
            if(i != ((int)stringScore.at(3)-48))
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05,0));
            }
            vkMapMemory(device, DS_DR3[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_DR3[i].uniformBuffersMemory[0][currentImage]);
            
            // DR4
            initial = glm::vec3(-1.8722f, 12.789f, 4.1852f);
            ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
            ubo.model = glm::rotate(ubo.model, glm::radians(-100.0f), glm::vec3(1,0,0));
            if(i != ((int)stringScore.at(2)-48))
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05,0));
            }
            vkMapMemory(device, DS_DR4[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_DR4[i].uniformBuffersMemory[0][currentImage]);
            
            // DL5
            initial = glm::vec3(-1.5958f, 12.789f, 4.1852f);
            ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
            ubo.model = glm::rotate(ubo.model, glm::radians(-100.0f), glm::vec3(1,0,0));
            if(i != ((int)stringScore.at(1)-48))
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05,0));
            }
            vkMapMemory(device, DS_DR5[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_DR5[i].uniformBuffersMemory[0][currentImage]);
            
            // DR6
            initial = glm::vec3(-1.316f, 12.789f, 4.1852f);
            ubo.model = glm::translate(glm::translate(glm::transpose(glm::mat4(CamDir)), -CamPos), initial);
            ubo.model = glm::rotate(ubo.model, glm::radians(-100.0f), glm::vec3(1,0,0));
            if(i != ((int)stringScore.at(0)-48))
            {
                ubo.model = glm::translate(ubo.model, glm::vec3(0,-0.05,0));
            }
            vkMapMemory(device, DS_DR6[i].uniformBuffersMemory[0][currentImage], 0,
                                sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
            vkUnmapMemory(device, DS_DR6[i].uniformBuffersMemory[0][currentImage]);
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


