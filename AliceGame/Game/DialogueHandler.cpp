#include "DialogueHandler.hpp"
#include <fstream>

DialogueHandler::DialogueHandler(ALC::ContentStorage* _preferredStorage)
	: m_preferredStorage(_preferredStorage) {
	m_actors.reserve(2);
	m_dialogue.reserve(10);
}

bool DialogueHandler::LoadDialogue(const ALC::string& _filepath){
	// open and load the dialogue file into a nlohmann::json object
	std::fstream file(_filepath);
	if (!file.is_open()) {
		ALC_DEBUG_ERROR("Could not load file " + _filepath);
		return false;
	}

	// Clearing out the previous dialogue data when loading in new dialogue
	if (IsLoaded()) {Unload();}

	// load json
	ALC::json j;
	file >> j;
	file.close();

	// ensure the file is formatted correctly
	if (!j.contains("Actors")) {
		ALC_DEBUG_ERROR("Could not find 'Actors' key in json file " + _filepath);
		return false;
	}
	if (!j["Actors"].is_array()) {
		ALC_DEBUG_ERROR("'Actors' key must be an array in json file " + _filepath);
		return false;
	}
	if (!j.contains("Dialogue")) {
		ALC_DEBUG_ERROR("Could not find 'Dialogue' key in json file " + _filepath);
		return false;
	}
	if (!j["Dialogue"].is_array()) {
		ALC_DEBUG_ERROR("'Dialogue' key must be an array in json file " + _filepath);
		return false;
	}

	// load actors
	ALC::json& actors = j["Actors"];
	for (size_t i = 0; i < actors.size(); i++) {
		ALC::json& actr = actors[i];
		DHActor dhactor{};

		// try load name, name color, and window color
		try {
			dhactor.name = actr["Name"];
			dhactor.nameColor = ALC::vec3(actr["Color"]["r"], actr["Color"]["g"], actr["Color"]["b"]);
			dhactor.windowColor = ALC::vec3(actr["WindowColor"]["r"], actr["WindowColor"]["g"], actr["WindowColor"]["b"]);
		} catch (const std::exception&) {
			ALC_DEBUG_WARNING("Could not load name, name color, or window color, actor ignored");
			continue;
		}

		// push the actor into the vector
		m_actors.push_back(dhactor);
	}

	if (m_actors.size() == 0) {
		ALC_DEBUG_WARNING("No actors were loaded, dialogue ignored");
		return false;
	}

	// load dialogue
	ALC::json& dialogues = j["Dialogue"];
	for (size_t i = 0; i < dialogues.size(); i++) {
		ALC::json& dial = dialogues[i];
		DHDialogue dhdialogue{};
		ALC::string actor;

		// try load actor and text
		try {
			actor = dial["Actor"];
		} catch (const std::exception&) {
			ALC_DEBUG_WARNING("Could not find or load 'Actor' key in " + _filepath + " at dialogue " + VTOS(i));
			continue;
		}
		try {
			dhdialogue.text = dial["Text"];
		} catch (const std::exception&) {
			ALC_DEBUG_WARNING("Could not find or load 'Text' key in " + _filepath + " at dialogue " + VTOS(i));
			continue;
		}

		// find actor index
		dhdialogue.actorIndex = 0;
		for (;dhdialogue.actorIndex < m_actors.size(); ++dhdialogue.actorIndex)
			if (m_actors[dhdialogue.actorIndex].name == actor)
				break;
		if (dhdialogue.actorIndex == m_actors.size()) {
			ALC_DEBUG_WARNING("Could not find actor with name " + actor + ". Ignored dialogue " + VTOS(i));
			continue;
		}

		// push back the dialogue
		m_dialogue.push_back(dhdialogue);
	}

	if (m_dialogue.size() == 0) {
		ALC_DEBUG_WARNING("No dialogue was loaded, actors ignored");
		m_actors.clear();
		return false;
	}

	return true;
}

bool DialogueHandler::IsLoaded() const {
	return m_actors.size() > 0 && m_dialogue.size() > 0;
}

void DialogueHandler::Unload() {
	m_actors.clear();
	m_dialogue.clear();
}

ALC::uint32 DialogueHandler::ActorsSize() const {
	return m_actors.size();
}

const DHActor DialogueHandler::GetActor(const ALC::uint32 index) const {
	return m_actors[index];
}

ALC::uint32 DialogueHandler::DialoguesSize() const {
	return m_dialogue.size();
}

const DHDialogue DialogueHandler::GetDialogue(const ALC::uint32 index) const {
	return m_dialogue[index];
}

ALC::Texture DialogueHandler::LoadTexture(const ALC::string& path) {
	if (m_preferredStorage)
		return ALC::ContentManager::LoadTexture(*m_preferredStorage, path);
	return ALC::ContentManager::LoadTexture(path);
}
