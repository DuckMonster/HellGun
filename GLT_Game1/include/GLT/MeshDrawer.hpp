#pragma once
#include "Mesh.hpp"

namespace glt {
	class MeshDrawer {
	public:
		//Transform
		glm::vec3 position = glm::vec3(0.f);
		glm::vec3 scale = glm::vec3(1.f);
		glm::vec3 rotation = glm::vec3(0.f);

		glm::mat4 modelMatrix = glm::mat4(1.f);
		bool autoGenMatrix = true;

		//Color
		glm::vec4 color = glm::vec4(1.f);

		//Texture
		Texture* texture = nullptr;

		//---------

		MeshDrawer();
		MeshDrawer(MeshDrawer* copy);
		MeshDrawer(Mesh* mesh);

		void setShader(Shader* shader);
		void setMesh(Mesh* mesh);
		Mesh* getMesh() { return mesh; }

		//Transform
		void reset();
		void genModelMatrix();

		virtual void draw();

	private:
		//
		static Shader* DEFAULT_SHADER;
		static void compileDefaultShader();
		//

		Mesh* mesh = nullptr;

		glm::vec3 prev_position;
		glm::vec3 prev_scale;
		glm::vec3 prev_rotation;

		bool isDirty = true;

		Shader* shader;

		//-----------

		void updateUniforms();
		void bindAttributes();

		void checkDirty();
		void clean();
	};
}