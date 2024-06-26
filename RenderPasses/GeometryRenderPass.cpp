#include "stdafx.h"
#include "GeometryRenderPass.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "PerspectiveCamera.h"
#include "CommandBuffer.h"
#include "Framebuffer.h"
#include "SwapChain.h"
#include "CommandPool.h"
#include "Pipeline.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "RendererBatch.h"
#include "Buffer.h"
#include "InstanceData.h"
#include "Component.h"

namespace Core
{
	GeometryRenderPass::GeometryRenderPass(Device& device, 
		Scene& scene, SwapChain& swapChain, RenderTarget* colorRenderTarget, RenderTarget* depthRenderTarget)
		:RenderPass(device), _scene(scene)
	{
		auto extent = swapChain.GetSwapChainExtent();
		CreateColorAttachment(colorRenderTarget,
			VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE);
		CreateDepthAttachment(depthRenderTarget,
			VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE);
		CreateRenderPass();

		_framebuffer = new Framebuffer(device, swapChain, *this);

		_instanceBuffer = new InstanceBuffer(device);
	}

	GeometryRenderPass::~GeometryRenderPass()
	{
		for (auto&& batch : _batches)
		{
			delete(batch.second);
		}

		_batches.clear();

		delete(_instanceBuffer);
	}

	void GeometryRenderPass::Prepare()
	{
		auto meshRenderers = _scene.GetComponents<Core::MeshRenderer>();
		for (auto&& meshRenderer : meshRenderers)
		{
			auto& shader = meshRenderer->GetMaterial().GetShader();

			if (shader.GetPass() == "Geometry")
			{
				auto key = shader.GetType();
				auto batch = _batches[key];
				if (batch == nullptr)
				{
					batch = new RendererBatch(_device, shader, *this);
					_batches[key] = batch;
				}

				batch->Add(*meshRenderer);
			}
		}
	}

	void GeometryRenderPass::Draw(
		CommandBuffer& commandBuffer, 
		uint32_t currentFrame, uint32_t imageIndex)
	{
		PerspectiveCamera& camera = _scene.GetMainCamera();

		auto framebuffer = _framebuffer->GetHandle(imageIndex);
		auto renderPassBeginInfo = CreateRenderPassBeginInfo(framebuffer, _framebuffer->GetExtent());
		commandBuffer.BeginRenderPass(renderPassBeginInfo);

		for (auto&& batch : _batches)
		{
			auto& pipeline = batch.second->Pipeline;
			commandBuffer.BindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipeline->GetGraphicsPipeline());

			auto vertexAttibuteNames = batch.second->SharedShader.GetVertexAttirbuteNames();

			auto& materials = batch.second->Materials;
			for (auto&& material : materials)
			{
				material.second->SetBuffer(currentFrame, 0, &camera.Matrices);

				commandBuffer.BindDescriptorSets(
					VK_PIPELINE_BIND_POINT_GRAPHICS, *material.second, currentFrame);

				auto& meshBatches = 
					batch.second->MeshBatches[material.first];

				for (auto&& meshBatch : meshBatches)
				{
					RendererBatch::Sort(material.first);

					commandBuffer.Flush(*material.second);

					auto& meshRenderers = meshBatch.second;
					for (size_t i = 0; i < meshRenderers.size(); ++i)
					{
						auto& entity = meshRenderers[i]->GetEntity();
						_instanceBuffer->SetBuffer(i, entity.GetTransform());
					}
					_instanceBuffer->Copy();

					auto& mesh = meshBatch.second[0]->GetMesh();
					commandBuffer.BindVertexBuffers(mesh.GetVertexBuffers(vertexAttibuteNames), 0);

					//fixme: hardcoded as 3.
					commandBuffer.BindVertexBuffers(
						_instanceBuffer->GetBuffer(), 3);

					commandBuffer.BindIndexBuffer(mesh.GetIndexBuffer(), VK_INDEX_TYPE_UINT32);

					commandBuffer.DrawIndexed(mesh.GetIndexCount(), static_cast<uint32_t>(meshRenderers.size()));
				}
			}
		}
		
		//ImGui::Render();
		//ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
		//	commandBuffer.GetHandle());

		commandBuffer.EndRenderPass();
	}
}
