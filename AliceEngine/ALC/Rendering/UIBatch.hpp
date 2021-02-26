#ifndef ALC_RENDERING_UIBATCH_HPP
#define ALC_RENDERING_UIBATCH_HPP
#include "../General.hpp"
#include "../Content/Font.hpp"
#include "../Content/Texture.hpp"
#include "../Content/Shader.hpp"

namespace ALC {

	// used to draw ui to the screen
	// draws in pixel coordinates
	class UIBatch final {
	public:

		UIBatch();
		~UIBatch();

		// returns the screen size used inside the ui batch
		vec2 GetInternalScreenSize() const { return m_screensize; }

		// sets the internal screen size
		// if screensize is <= 0.0f in each dimension then it uses the real screensize
		void SetInternalScreenSize(const vec2& screensize) { m_screensize = screensize;}

		// drawing functions

		// starts drawing the UI
		void Begin(Shader shader = nullptr);

		// draws a quad to screen
		void DrawQuad(const rect& position, const vec4& color = ALC_COLOR_WHITE, const rect& target = rect(0.0f), const Texture& texture = nullptr);

		// draws text to the screen
		void DrawText(const string& text, const Font& font, const vec2& position, const vec4& color = ALC_COLOR_WHITE, const HAlign hAlign = HAlign::Left, const VAlign vAlign = VAlign::Top, const vec2& scale = vec2(1.0f));

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
		uint32 m_vao, m_vbo;
		uint32 m_TextureCountLoc;
		uint32 m_bufferSize;
		Shader m_defaultShader;
		Shader m_currentShader;
		vec2 m_screensize;

		uint32 TryAddTexture(const Texture& texture);
		void DrawCurrent();
	};

}

#endif // !ALC_RENDERING_UIBATCH_HPP