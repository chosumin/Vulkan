#pragma once

namespace Core
{
	class IDescriptor;
	class SwapChain;
	class Pipeline
	{
	public:
		Pipeline(const SwapChain& swapChain,
			const string& vertFilePath, const string& fragFilePath,
			vector<IDescriptor*> descriptors);
		~Pipeline();

		VkPipeline GetGraphicsPipeline() { return _graphicsPipeline; }
		VkPipelineLayout GetPipelineLayout() { return _pipelineLayout; }
		VkDescriptorSet* GetDescriptorSet(size_t index) { return &_descriptorSets[index]; }
	private:
		static vector<char> ReadFile(const string& filePath);

		void CreateGraphicsPipeline(
			VkDevice& device, VkRenderPass renderPass, VkSampleCountFlagBits msaaSamples,
			const string& vertFilePath, const string& fragFilePath);
		VkShaderModule CreateShaderModule(VkDevice& device, const vector<char>& code) const;

		void CreateDescriptors(vector<IDescriptor*> descriptors);
		void CreateDescriptorSetLayout(vector<IDescriptor*> descriptors);
		void CreateDescriptorPool(vector<IDescriptor*> descriptors);
		void CreateDescriptorSets(vector<IDescriptor*> descriptors);
	private:
		VkPipelineLayout _pipelineLayout;
		VkPipeline _graphicsPipeline;
		VkDescriptorSetLayout _descriptorSetLayout;
		VkDescriptorPool _descriptorPool;
		vector<VkDescriptorSet> _descriptorSets;
	};
}

