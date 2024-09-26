#include "rendering/render_systems/light_manager.hpp"

#include <iostream>

#include "rendering/rhi.hpp"
#include "rendering/rhi_typedef.hpp"
#include "resource/resource_manager.hpp"
#include "scene/entity.hpp"
#include "utils/logger.hpp"
#include "rendering/renderer.hpp"

using namespace XnorCore;

LightManager::~LightManager()
{
	delete m_DirectionalShadowMaps;
	m_DirectionalShadowMaps = nullptr;

	delete m_SpotLightShadowMapTextureArray;
	m_SpotLightShadowMapTextureArray = nullptr;
	delete m_PointLightShadowMapCubemapArrayPixelDistance;
	m_PointLightShadowMapCubemapArrayPixelDistance = nullptr;
	delete m_DepthBufferForPointLightPass;
	m_DepthBufferForPointLightPass = nullptr;
	delete m_ShadowFrameBufferPointLight;
	m_ShadowFrameBufferPointLight = nullptr;


	delete m_GpuLightData;
	m_GpuLightData = nullptr;
}

void LightManager::InitResources()
{
	m_GpuLightData = new GpuLightData();
	m_RenderingLightStruct.dirLightTexture = ResourceManager::Get<Texture>("assets_internal/editor/gizmos/dirlight_icon.png");
	m_RenderingLightStruct.pointLightTexture = ResourceManager::Get<Texture>("assets_internal/editor/gizmos/point_light.png");
	m_RenderingLightStruct.spotLightTexture = ResourceManager::Get<Texture>("assets_internal/editor/gizmos/spot_light.png");

	m_ShadowFrameBuffer = new Framebuffer;
	m_ShadowFrameBufferPointLight = new Framebuffer;
	
	InitShader();
	InitShadowMap();
	
	// need to attach it one time 
	m_ShadowFrameBufferPointLight->AttachTexture(*m_DepthBufferForPointLightPass, Attachment::Depth, 0);
}

void LightManager::BeginFrame(const Scene& scene,const Viewport& viewport, Renderer& renderer)
{
	scene.GetAllComponentsOfType<PointLight>(&m_PointLights);
	scene.GetAllComponentsOfType<SpotLight>(&m_SpotLights);
	scene.GetAllComponentsOfType<DirectionalLight>(&m_DirectionalLights);

	FecthLightInfo();
	ComputeShadow(scene, viewport, renderer);
	Rhi::UpdateLight(*m_GpuLightData);
}

void LightManager::EndFrame(const Scene&)
{
	// This function is call at the end of the frame  
}

void LightManager::DrawLightGizmo(const Camera& camera, const Scene& scene) const
{
	scene.GetAllComponentsOfType<PointLight>(&m_PointLights);
	scene.GetAllComponentsOfType<SpotLight>(&m_SpotLights);
	scene.GetAllComponentsOfType<DirectionalLight>(&m_DirectionalLights);
	DrawLightGizmoWithShader(camera, scene, m_RenderingLightStruct.editorUi);
}

void LightManager::DrawLightGizmoWithShader(const Camera& camera, const Scene& scene, const Pointer<Shader>& shader) const
{
	shader->Use();
	
	std::map<float_t, GizmoLight> sortedLight;

	GetDistanceFromCamera(&sortedLight, camera);
	
	for (decltype(sortedLight)::reverse_iterator it = sortedLight.rbegin(); it != sortedLight.rend(); it++)
	{
		ModelUniformData modelData;
		float_t scaleScalar = m_RenderingLightStruct.scaleFactor;

		float_t distance = (it->second.pos - camera.position).SquaredLength();
		if (distance < m_RenderingLightStruct.maxScalarFactor * m_RenderingLightStruct.maxScalarFactor)
			scaleScalar = scaleScalar * (1.f / distance) * (1.f / distance);

		scaleScalar = std::clamp(scaleScalar, m_RenderingLightStruct.maxScalarFactor, m_RenderingLightStruct.minScalarFactor);
		Matrix scale = Matrix::Scaling(Vector3(scaleScalar));
		
		modelData.model = (scale * Matrix::LookAt(it->second.pos, camera.position, Vector3::UnitY())).Inverted();
		modelData.normalInvertMatrix = Matrix::Identity();

		// +1 to avoid the black color of the attachment be a valid index  
		modelData.meshRenderIndex = scene.GetEntityIndex(it->second.light->GetEntity()) + 1;
		
		switch (it->second.type)
		{
			case RenderingLight::PointLight:
				m_RenderingLightStruct.pointLightTexture->BindTexture(0);
				break;
			
			case RenderingLight::SpothLight:
				m_RenderingLightStruct.spotLightTexture->BindTexture(0);
				break;
			
			case RenderingLight::DirLight:
				m_RenderingLightStruct.dirLightTexture->BindTexture(0);
				break;
		}
		
		Rhi::UpdateModelUniform(modelData);
		Rhi::DrawModel(DrawMode::Triangles, m_RenderingLightStruct.quad->models[0]->GetId());
	}

	shader->Unuse();
}

void LightManager::BindShadowMap() const
{
	m_DirectionalShadowMaps->BindTexture(ShadowTextureBinding::Directional);
	m_SpotLightShadowMapTextureArray->BindTexture(ShadowTextureBinding::SpotLight);
	m_PointLightShadowMapCubemapArrayPixelDistance->BindTexture(ShadowTextureBinding::PointLightCubemapArrayPixelDistance);
}

void LightManager::UnBindShadowMap() const
{
	m_DirectionalShadowMaps->UnbindTexture(ShadowTextureBinding::Directional);
	m_SpotLightShadowMapTextureArray->BindTexture(ShadowTextureBinding::SpotLight);
	m_PointLightShadowMapCubemapArrayPixelDistance->BindTexture(ShadowTextureBinding::PointLightCubemapArrayPixelDistance);
}

void LightManager::FecthLightInfo() const
{
	 if (m_DirectionalLights.size() > MaxDirectionalLights)
		Logger::LogWarning("You cannot have more than 1 directional light in the scene");
	
	const size_t nbrOfpointLight = std::clamp<size_t>(m_PointLights.size(), 0, MaxPointLights);
	const size_t nbrOfSpotLight = std::clamp<size_t>(m_SpotLights.size(), 0, MaxSpotLights);
	const size_t nbrOfDirectionalLight = std::clamp<size_t>(m_DirectionalLights.size(), 0, MaxDirectionalLights);

	m_GpuLightData->nbrOfDirLight = static_cast<uint32_t>(nbrOfDirectionalLight);
	m_GpuLightData->nbrOfPointLight = static_cast<uint32_t>(nbrOfpointLight);
	m_GpuLightData->nbrOfSpotLight = static_cast<uint32_t>(nbrOfSpotLight);

	for (size_t i = 0; i < nbrOfpointLight; i++)
	{
		const PointLight* pointLight = m_PointLights[i];
		
		m_GpuLightData->pointLightData[i] =
		{
			.color = static_cast<Vector3>(pointLight->color),
			.intensity = pointLight->intensity,
			.position = static_cast<Vector3>(pointLight->GetEntity()->transform.worldMatrix[3]),
			.radius = LightThreshold * sqrt(pointLight->intensity),
			.isCastingShadow = pointLight[i].castShadow
		};
	}

	for (size_t i = 0 ; i < nbrOfSpotLight ; i++)
	{
		const SpotLight* spotLight = m_SpotLights[i];
		
		m_GpuLightData->spotLightData[i] =
		{
			.color = static_cast<Vector3>(spotLight->color),
			.intensity = spotLight->intensity,
			.position = static_cast<Vector3>(spotLight->GetEntity()->transform.worldMatrix[3]),
			.cutOff = std::cos(spotLight->cutOff * Calc::Deg2Rad),
			.direction = spotLight->GetLightDirection(),
			.outerCutOff = std::cos(spotLight->outerCutOff * Calc::Deg2Rad),
			.isCastingShadow = spotLight->castShadow,
		};
	}
	

	if (nbrOfDirectionalLight != 0)
	{
		for (size_t i = 0 ; i < MaxDirectionalLights ; i++)
		{
			const Vector3 direction = m_DirectionalLights[i]->GetLightDirection(); 
			m_GpuLightData->directionalData[i] =
			{
				.color = static_cast<Vector3>(m_DirectionalLights[i]->color),
				.intensity = m_DirectionalLights[i]->intensity,	
				.direction = { direction.x, direction.y, direction.z },
			};
		}
	}
}

void LightManager::ComputeShadow(const Scene& scene, const Viewport& viewport, Renderer& renderer)
{
	ComputeShadowDirLight(scene, *viewport.camera,viewport.viewPortSize, renderer);
	ComputeShadowSpotLight(scene, renderer);
	ComputeShadowPointLight(scene, renderer);
}

void LightManager::ComputeShadowDirLight(const Scene& scene,const Camera& viewPortCamera, const Vector2i viewportSize, Renderer& renderer)
{
	for (const DirectionalLight* const directionalLight : m_DirectionalLights)
	{
		m_GpuLightData->directionalData->isDirlightCastingShadow = directionalLight->castShadow;
		
		if (!directionalLight->castShadow)
			continue;

		const Texture& shadowMap = *m_DirectionalShadowMaps;
		const Vector2i shadowMapSize = shadowMap.GetSize(); 
		
		const Vector3 lightDir = directionalLight->GetLightDirection();
		
		// HardCoded Shadow Cascade distance by level
		std::array<float_t, DirectionalCascadeLevel> shadowCascadeLevels =
		{
		    viewPortCamera.far / 100.f,
			viewPortCamera.far / 50.f,
			viewPortCamera.far / 20.f,
			viewPortCamera.far / 4.f,
		
		};

		m_CascadeShadowMap.SetCascadeLevel(shadowCascadeLevels);
		m_CascadeShadowMap.SetZMultiplicator(directionalLight->zCascadeShadowMapZMultiplactor);

		for (size_t k = 0; k < shadowCascadeLevels.size(); k++)
		{
			m_GpuLightData->directionalData->cascadePlaneDistance[k] = shadowCascadeLevels[k];
		}
		
		std::vector<Camera> cascadedCameras;
		m_CascadeShadowMap.GetCascadeCameras(&cascadedCameras, viewPortCamera ,lightDir, viewportSize);
		
		for (size_t i = 0; i < cascadedCameras.size(); i++)
		{
			cascadedCameras[i].isOrthographic = true;
			cascadedCameras[i].GetVp(viewportSize, &m_GpuLightData->dirLightSpaceMatrix[i]);

			
			m_ShadowFrameBuffer->AttachTextureLayer(*m_DirectionalShadowMaps, Attachment::Depth, 0, static_cast<uint32_t>(i));
			RenderPassBeginInfo renderPassBeginInfo =
			{
				.frameBuffer = m_ShadowFrameBuffer,
				.renderAreaOffset = { 0, 0 },
				.renderAreaExtent = shadowMapSize,
				.clearBufferFlags = BufferFlag::DepthBit,
				.clearColor = Vector4::Zero()
			};
			renderer.RenderNonShadedPass(scene,cascadedCameras.at(i) , renderPassBeginInfo, m_ShadowRenderPass,m_ShadowMapShader, m_ShadowMapShaderSkinned, false);
		}
	}
}

void LightManager::ComputeShadowSpotLight(const Scene& scene, Renderer& renderer)
{
	for (size_t i = 0; i < m_SpotLights.size(); i++)
	{
		if (!m_SpotLights[i]->castShadow)
			return;
		
		Camera cam;
		cam.position = m_SpotLights[i]->entity->transform.GetPosition();
		cam.front = m_SpotLights[i]->GetLightDirection();
		cam.up = Vector3::UnitY();
		cam.right = Vector3::Cross(cam.front, cam.up).Normalized();
		cam.near = m_SpotLights[i]->near;
		cam.far = m_SpotLights[i]->far;
		cam.GetVp(SpotLightShadowMapSize, &m_GpuLightData->spotLightSpaceMatrix[i]);
		
		m_ShadowFrameBuffer->AttachTextureLayer(*m_SpotLightShadowMapTextureArray, Attachment::Depth, 0, static_cast<uint32_t>(i));

		RenderPassBeginInfo renderPassBeginInfo =
		{
			.frameBuffer = m_ShadowFrameBuffer,
			.renderAreaOffset = { 0, 0 },
			.renderAreaExtent = SpotLightShadowMapSize,
			.clearBufferFlags = BufferFlag::DepthBit,
			.clearColor = Vector4(0.f)
		};
		
		renderer.RenderNonShadedPass(scene, cam, renderPassBeginInfo, m_ShadowRenderPass,m_ShadowMapShader, m_ShadowMapShaderSkinned, false);
	}
}

void LightManager::ComputeShadowPointLight(const Scene& scene, Renderer& renderer)
{
	Camera cam;
	for (size_t i = 0; i < m_PointLights.size(); i++)
	{
		if (!m_PointLights[i]->castShadow)	
			continue;
		
		const Vector3&& pos = static_cast<Vector3>(m_PointLights[i]->entity->transform.worldMatrix[3]);
		
		// Render fo each face of a the CubeMap
		for (size_t k = 0; k < 6; k++)
		{
			GetPointLightDirection(k, &cam.front, &cam.up);
			cam.position = pos;
			cam.near = m_PointLights[i]->near;
			cam.far = m_PointLights[i]->far;
			cam.right = Vector3::Cross(cam.front, cam.up).Normalized();
			
			// Get Current CubeMap faces In Cubemap Array
			const uint32_t currentFace = static_cast<uint32_t>(k + i * 6);
			m_ShadowFrameBufferPointLight->AttachTextureLayer(*m_PointLightShadowMapCubemapArrayPixelDistance, Attachment::Color00, 0, currentFace);
			
			RenderPassBeginInfo renderPassBeginInfo =
			{
				.frameBuffer = m_ShadowFrameBufferPointLight,
				.renderAreaOffset = { 0, 0 },
				.renderAreaExtent = SpotLightShadowMapSize ,
				.clearBufferFlags = static_cast<BufferFlag::BufferFlag>(BufferFlag::DepthBit | BufferFlag::ColorBit),
				.clearColor = Vector4(std::numeric_limits<float_t>::max())
			};
			
		
			renderer.RenderNonShadedPass(scene, cam, renderPassBeginInfo, m_ShadowRenderPass, m_ShadowMapShaderPointLight, m_ShadowMapShaderPointLightSkinned, false);
		}
	}
}


void LightManager::GetDistanceFromCamera(std::map<float_t, GizmoLight>* sortedLight, const Camera& camera) const
{
	
	for (const PointLight* const pointLight : m_PointLights)
	{
		if (pointLight == nullptr)
			continue;

		const Vector3 entityPosition = static_cast<Vector3>(pointLight->GetEntity()->transform.worldMatrix[3]);
		
		GizmoLight gizmoLight = {
			.pos = entityPosition,
			.light = pointLight,
			.type = RenderingLight::PointLight,
		};
		
		const float_t distance = (camera.position - entityPosition).SquaredLength();
		sortedLight->emplace(distance, gizmoLight);
	}
	
	for (const SpotLight* const spotLight : m_SpotLights)
	{
		if (spotLight == nullptr)
			continue;

		const Vector3 entityPosition = static_cast<Vector3>(spotLight->GetEntity()->transform.worldMatrix[3]);
		
		GizmoLight gizmoLight = {
			.pos = entityPosition,
			.light = spotLight,
			.type = RenderingLight::SpothLight
		};
		
		const float_t distance = (camera.position - entityPosition).SquaredLength();
		sortedLight->emplace(distance, gizmoLight);
	}
	
	for (const DirectionalLight* const dirLight : m_DirectionalLights)
	{
		if (dirLight == nullptr)
			continue;

		const Vector3 entityPosition = static_cast<Vector3>(dirLight->GetEntity()->transform.worldMatrix[3]);
		
		GizmoLight gizmoLight = {
			.pos = entityPosition,
			.light = dirLight,
			.type = RenderingLight::DirLight
		};
		
		const float_t distance = (camera.position - entityPosition).SquaredLength();
		sortedLight->emplace(distance, gizmoLight);
	}
}

void LightManager::GetPointLightDirection(const size_t index, Vector3* front, Vector3* up) const
{
	switch (index)
	{
	case 0:
		*front = Vector3::UnitX();
		*up = -Vector3::UnitY();
		break;

	case 1:
		*front = -Vector3::UnitX();
		*up = -Vector3::UnitY();
		break;

	case 2:
		*front = Vector3::UnitY();
		*up = Vector3::UnitZ();
		break;

	case 3:
		*front = -Vector3::UnitY();
		*up = -Vector3::UnitZ();
		break;

	case 4:
		*front = Vector3::UnitZ();
		*up = -Vector3::UnitY();
		break;

	case 5:
		*front = -Vector3::UnitZ();
		*up = -Vector3::UnitY();
		break;

	default:
		Logger::LogError("Unreachable Face of CubeMap shadowMap PointLight !");
		break;
	}
}

void LightManager::InitShadowMap()
{
	const TextureCreateInfo dirLightShadowMap =
		{
		.textureType = TextureType::Texture2DArray,
		.mipMaplevel = 1,
		// TODO NOT A VARIABLE constexpr
		.depth = DirectionalCascadeLevel + 1,
		.size = DirectionalShadowMapSize,
		.filtering = TextureFiltering::Nearest,
		.wrapping = TextureWrapping::ClampToBorder,
		.format = TextureFormat::DepthComponent,
		.internalFormat = ShadowDepthTextureInternalFormat,
		.dataType = DataType::Float
	};
	
	m_DirectionalShadowMaps = new Texture(dirLightShadowMap);
	
	const TextureCreateInfo spothLightShadowArray =
	{
		.textureType = TextureType::Texture2DArray,
		.mipMaplevel = 1,
		.depth = MaxSpotLights,
		.size = SpotLightShadowMapSize,
		.filtering = TextureFiltering::Nearest,
		.wrapping = TextureWrapping::ClampToBorder,
		.format = TextureFormat::DepthComponent,
		.internalFormat = ShadowDepthTextureInternalFormat,
		.dataType = DataType::Float
	};

	m_SpotLightShadowMapTextureArray = new Texture(spothLightShadowArray);

	const TextureCreateInfo pointLightDepthBufferCreateInfo =
	{
		.textureType = TextureType::Texture2D,
		.mipMaplevel = 1,
		.depth = MaxPointLights,
		.size = PointLightLightShadowMapSize,
		.filtering = TextureFiltering::Linear,
		.wrapping = TextureWrapping::ClampToEdge,
		.format = TextureFormat::DepthComponent,
		.internalFormat =  ShadowDepthTextureInternalFormat,
		.dataType =  DataType::Float
	};

	m_DepthBufferForPointLightPass = new Texture(pointLightDepthBufferCreateInfo);

	const TextureCreateInfo pointLightCubeMapArrayWorldSpaceInfo =
	{
		.textureType = TextureType::TextureCubeMapArray,
		.mipMaplevel = 1,
		.depth = MaxPointLights,
		.size = PointLightLightShadowMapSize,
		.filtering = TextureFiltering::Linear,
		.wrapping = TextureWrapping::ClampToEdge,
		.format = TextureFormat::Red,
		.internalFormat =  TextureInternalFormat::R32F,
		.dataType =  DataType::Float
	};

	m_PointLightShadowMapCubemapArrayPixelDistance = new Texture(pointLightCubeMapArrayWorldSpaceInfo);
}

void LightManager::InitShader()
{
	m_RenderingLightStruct.editorUi = ResourceManager::Get<Shader>("editor_ui_shader");

	constexpr BlendFunction blendFunction =
	{
		.isBlending = true,
		.sValue = BlendValue::SrcAlpha,
		.dValue = BlendValue::OneMinusSrcAlpha
	};
	
	m_RenderingLightStruct.editorUi->SetBlendFunction(blendFunction);
	m_RenderingLightStruct.editorUi->CreateInInterface();
	m_RenderingLightStruct.editorUi->SetInt("uiTexture",0);
	m_RenderingLightStruct.quad = ResourceManager::Get<Mesh>("assets/models/quad.obj");
	
	m_ShadowMapShader = ResourceManager::Get<Shader>("depth_shader");
	constexpr ShaderProgramCullInfo cullInfo =
	{
		.enableCullFace = true,
		.cullFace = CullFace::Back,
		.frontFace = FrontFace::CCW
	};

	m_ShadowMapShader->SetFaceCullingInfo(cullInfo);
	m_ShadowMapShader->CreateInInterface();

	m_ShadowMapShaderPointLight = ResourceManager::Get<Shader>("depth_shader_point_light");
	m_ShadowMapShaderPointLight->SetFaceCullingInfo(cullInfo);
	m_ShadowMapShaderPointLight->CreateInInterface();
	
	// Skinned
	m_ShadowMapShaderSkinned = ResourceManager::Get<Shader>("depth_shader_skinned");
	m_ShadowMapShaderSkinned->SetFaceCullingInfo(cullInfo);
	m_ShadowMapShaderSkinned->CreateInInterface();

	m_ShadowMapShaderPointLightSkinned = ResourceManager::Get<Shader>("depth_shader_point_light_skinned");
	m_ShadowMapShaderPointLightSkinned->SetFaceCullingInfo(cullInfo);
	m_ShadowMapShaderPointLightSkinned->CreateInInterface();
}
