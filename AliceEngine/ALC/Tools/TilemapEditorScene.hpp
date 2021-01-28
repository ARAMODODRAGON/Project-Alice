#ifndef ALC_TOOLS_TILEMAPEDITORSCENE_HPP
#define ALC_TOOLS_TILEMAPEDITORSCENE_HPP
#include "../SceneManager.hpp"
#include "../Input.hpp"
#include "../2D/Tilemap.hpp"
#include "../Rendering.hpp"
#include "../StateMachine.hpp"

namespace ALC::TOOLS {

	class TilemapEditorScene final : public IScene {
	public:

		TilemapEditorScene();
		~TilemapEditorScene();

		void Init() override;
		void Exit() override;
		void Step(Timestep ts) override;
		void PreDraw() override;
		void Draw() override;
		void PostDraw() override;

	private:

		enum class EditorState : uint8 {
			EditingPath,
			EditingMap
		};

		void EditingPathBegin(const EditorState, Timestep ts);
		void EditingPathState(Timestep ts);
		void EditingMapState(Timestep ts);

		StateMachine<TilemapEditorScene, EditorState, Timestep> m_state;
		uint32 m_selectedpath;
		vector<string> m_allfiles;
		uvec2 m_mappos;
		Font m_font;

		Tilemap m_tilemap;
		SpriteBatch m_spritebatch;
		UIBatch m_uibatch;
		ContentStorage m_storage;

	};

}

#endif // !ALC_TOOLS_TILEMAPEDITORSCENE_HPP