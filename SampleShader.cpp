#include "stdafx.h"
#include "SampleShader.h"
#include "TextureBuffer.h"
#include "Mesh.h"
#include "UniformBuffer.h"
#include "Vertex.h"
#include "InstanceData.h"

namespace Core
{
	SampleShader::SampleShader(Device& device)
		:Shader(device,
			"shaders/simple_vs.vert.spv",
			"shaders/simple_fs.frag.spv")
	{
		auto cameraBuffer = new UniformBufferLayoutBinding(0);

		auto textureBuffer = new TextureBufferLayoutBinding(1);

		CreatePipelineLayout({ cameraBuffer, textureBuffer }, {});

		delete(cameraBuffer);
		delete(textureBuffer);
	}

	SampleShader::~SampleShader()
	{
	}

	type_index SampleShader::GetType()
	{
		return typeid(SampleShader);
	}

	string SampleShader::GetPass()
	{
		return "Geometry";
	}

	VkPipelineVertexInputStateCreateInfo SampleShader::GetVertexInputStateCreateInfo()
	{
		_vertexBindings = {
			Vertex::GetBindingDescription(
		0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX),
			Vertex::GetBindingDescription(
		1, sizeof(InstanceData), VK_VERTEX_INPUT_RATE_INSTANCE)
		};

		_vertexAttributes = Vertex::GetAttributeDescriptions();
		_vertexAttributes.push_back(Vertex::GetAttributeDescription(
			1, 3, VK_FORMAT_R32G32B32A32_SFLOAT, 0));
		_vertexAttributes.push_back(Vertex::GetAttributeDescription(
			1, 4, VK_FORMAT_R32G32B32A32_SFLOAT, sizeof(float) * 4));
		_vertexAttributes.push_back(Vertex::GetAttributeDescription(
			1, 5, VK_FORMAT_R32G32B32A32_SFLOAT, sizeof(float) * 8));
		_vertexAttributes.push_back(Vertex::GetAttributeDescription(
			1, 6, VK_FORMAT_R32G32B32A32_SFLOAT, sizeof(float) * 12));

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 
			static_cast<uint32_t>(_vertexBindings.size());
		vertexInputInfo.vertexAttributeDescriptionCount = 
			static_cast<uint32_t>(_vertexAttributes.size());
		vertexInputInfo.pVertexBindingDescriptions = _vertexBindings.data();
		vertexInputInfo.pVertexAttributeDescriptions = _vertexAttributes.data();

		return vertexInputInfo;
	}

	bool SampleShader::UseInstancing()
	{
		return true;
	}
}
