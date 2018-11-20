#pragma once
#pragma warning (disable : 4715)
#include <iostream>
#include <math.h>
#include <algorithm>
#include <assert.h>
#include <math.h>
#include <string>
#include <thread>
#include <fstream>
#include <sstream>

#include "3DEngine/src/Structs.h"
#include "3DEngine/src/3D Engine/Extern.h"
#include "3DEngine/src/3D Engine/Model/Texture.h"
#include "3DEngine/src/3D Engine/RenderingManager.h"
#include "3DEngine/src/3D Engine/Components/Camera.h"
#include "3DEngine/src/3D Engine/Model/Meshes/StaticMesh.h"
#include "3DEngine/src/3D Engine/Components/Base/Drawable.h"
#include "3DEngine/src/3D Engine/Model/Managers/MeshManager.h"
#include "3DEngine/src/3D Engine/Model/Managers/TextureManager.h"
#include "3DEngine/src/3D Engine/3DRendering/Framework/Engine3D.h"
#include "3DEngine/src/3D Engine/3DRendering/Rendering/ShadowMap.h"
#include "3DEngine/src/3D Engine/3DRendering/Rendering/ForwardRender.h"
#include "3DEngine/src/3D Engine/3DRendering/Framework/DirectXRenderingHelpClass.h"
#include "3DEngine/src/3D Engine/3DRendering/Rendering/VisabilityPass/VisabilityPass.h"
#include "3DEngine/src/3D Engine/3DRendering/Rendering/VisabilityPass/Component/VisibilityComponent.h"

#include "3DEngine/src/Helper/Timer.h"
#include "3DEngine/src/Window/window.h"
#include "3DEngine/src/Light/PointLight.h"
#include "3DEngine/src/Shader/ShaderManager.h"
#include "3DEngine/src/SettingsLoader/SettingsLoader.h"
#include "3DEngine/src/3D Engine/WICTextureLoader/WICTextureLoader.h"

#include "2DEngine/src/Render2D.h"

#include "Input/InputManager/InputHandler.h"