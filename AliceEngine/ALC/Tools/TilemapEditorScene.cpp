#include "TilemapEditorScene.hpp"
#include <filesystem>

#define FONT_SIZE 20

namespace ALC::TOOLS {
	using CM = ContentManager;
	using KB = Keyboard;
	using KC = KeyCode;

	TilemapEditorScene::TilemapEditorScene()
		: m_state(this, EditorState::EditingPath), m_selectedpath(0), m_mappos(0, 0) {
		m_state.Bind(EditorState::EditingPath, &TilemapEditorScene::EditingPathState, &TilemapEditorScene::EditingPathBegin);
		m_state.Bind(EditorState::EditingMap, &TilemapEditorScene::EditingMapState);
	}

	TilemapEditorScene::~TilemapEditorScene() { }

	void TilemapEditorScene::Init() {
		CM::SetContext(m_storage);

		// load the font to be used
		m_font = CM::LoadFont("Resources/Fonts/arial.ttf", FONT_SIZE);

	}

	void TilemapEditorScene::Exit() { }

	void TilemapEditorScene::Step(Timestep ts) {

		auto ctrl = KB::GetKey(KC::LeftCtrl);
		auto keyP = KB::GetKey(KC::KeyP);

		if (ctrl.Pressed() && keyP || ctrl && keyP.Pressed())
			m_state.ChangeState(EditorState::EditingPath);

		m_state(ts);
	}

	void TilemapEditorScene::EditingPathBegin(const EditorState, Timestep ts) {
		namespace fs = std::filesystem;

		// find all json files to load
		m_allfiles.clear();
		for (const auto& entry : fs::recursive_directory_iterator("Resources/Tilemaps")) {
			const auto& path = entry.path();
			if (path.has_extension() && path.extension() == ".json") {
				const auto& str = path.native();
				m_allfiles.push_back(string(str.begin(), str.end()));
			}
		}

		if (m_selectedpath > m_allfiles.size()) m_selectedpath = m_allfiles.size() - 1;
	}

	void TilemapEditorScene::EditingPathState(Timestep ts) {
		if (m_allfiles.size() == 0) {
			m_state.ChangeState(EditorState::EditingMap);
			return;
		}

		auto up = KB::GetKey(KC::ArrowUp);
		auto down = KB::GetKey(KC::ArrowDown);

		if (up.Pressed() != down.Pressed()) {
			if (up.Pressed())
				m_selectedpath--;
			if (down.Pressed())
				m_selectedpath++;

			m_selectedpath = m_selectedpath % m_allfiles.size();
		}

		if (KB::GetKey(KC::Return)) {

			// load the tilemapfile
			ALC_DEBUG_LOG("Loaded: " + m_allfiles[m_selectedpath]);

			// change state
			m_state.ChangeState(EditorState::EditingMap);
		}
	}

	void TilemapEditorScene::EditingMapState(Timestep ts) { }

	void TilemapEditorScene::PreDraw() { }

	void TilemapEditorScene::Draw() {

		// draw the tilemap


		// draw ui stuffs
		string path = "";
		if (m_allfiles.size() != 0) path = m_allfiles[m_selectedpath];

		m_uibatch.Begin();
		m_uibatch.DrawText(path, m_font, vec2(0.0f, FONT_SIZE));
		m_uibatch.End();
	}

	void TilemapEditorScene::PostDraw() { }

}