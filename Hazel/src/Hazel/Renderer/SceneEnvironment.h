#pragma once
#include "Texture.h"
namespace Hazel {
	struct Environment
	{
		std::string FilePath;
		Ref<TextureCube> RadianceMap;
		Ref<TextureCube> IrradianceMap;
		static Environment Load(const std::string& filepath);
	};
}