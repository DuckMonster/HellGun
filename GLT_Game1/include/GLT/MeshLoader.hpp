#pragma once
#include <vector>
#include "Mesh.hpp"
#include <fbxsdk.h>

namespace glt {
	class MeshLoader {
	public:
		static std::vector<Mesh*> loadFBX(const char* fileName, bool triangulate = true);
		static std::vector<std::vector<float>> loadFBXRaw(const char* fileName, bool triangulate = true);

	private:
		static FbxManager* manager;

		static void addFace(std::vector<float>* list, FbxVector4* vertices, int* indices, size_t indexCount);
	};
}