#pragma once
#include "RenderPass.h"

class InstanceData;

namespace Core
{
	class Scene;
	class SwapChain;
	class RendererBatch;
	class InstanceBuffer;

	class GeometryRenderPass : public RenderPass
	{
	public:
		GeometryRenderPass(Device& device, 
			Scene& scene, SwapChain& swapChain, RenderTarget* colorRenderTarget, RenderTarget* depthRenderTarget);
		virtual ~GeometryRenderPass() override;

		virtual void Prepare() override;
		virtual void Draw(CommandBuffer& commandBuffer, uint32_t currentFrame, uint32_t imageIndex) override;
	private:
		Scene& _scene;

		unordered_map<type_index, RendererBatch*> _batches;
		InstanceBuffer* _instanceBuffer;
	};
}

