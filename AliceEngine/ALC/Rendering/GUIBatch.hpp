#ifndef ALC_RENDERING_GUIBATCH_HPP
#define ALC_RENDERING_GUIBATCH_HPP
#include "../General.hpp"
#include "../Content/Font.hpp"
#include "../Content/Texture.hpp"
#include "../Content/Shader.hpp"

namespace ALC {

	// used to draw ui to the screen
	// draws in pixel coordinates
	class GUIBatch final {
	public:

		GUIBatch();
		~GUIBatch();

		// drawing functions

		// starts drawing the UI
		void Begin(Shader shader = nullptr);

		// draws a quad to screen
		void DrawQuad(rect position, vec4 color = ALC_COLOR_WHITE, rect target = rect(0.0f), Texture texture = nullptr);

		// draws text to the screen
		//void DrawText(string text, rect position, Font font);

		// stops drawing and outputs to the screen
		void End();

	private:

		struct vertex {
			vec2 position;
			vec2 uvcoords;
			vec4 color;
			int32 textureIndex = -1;
		};

		vector<vertex> m_verticies;
		vector<uint32> m_textures;
		Shader m_defaultShader;
		Shader m_currentShader;

	};

}

#endif // !ALC_RENDERING_GUIBATCH_HPP