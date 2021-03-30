#ifndef ALICE_DIALOGUEHANDLER_HPP
#define ALICE_DIALOGUEHANDLER_HPP
#include <ALC\General.hpp>
#include <ALC\Content.hpp>

struct DHActor final {
	ALC::string name;
	ALC::vec3 nameColor;
	ALC::vec3 windowColor;
};

struct DHDialogue final {
	ALC::uint32 actorIndex;
	ALC::string text;
};

class DialogueHandler final {
public:
	DialogueHandler(ALC::ContentStorage* _preferredStorage = nullptr);

	// Loads in a dialogue file.
	bool LoadDialogue(const ALC::string& _filepath);

	// returns if the dialogue file loaded correctly
	bool IsLoaded() const;
	void Unload();

	// gets the number of actors
	ALC::uint32 ActorsSize() const;

	// returns the actor at index
	const DHActor GetActor(const ALC::uint32 index) const;

	// gets the number of dialogues
	ALC::uint32 DialoguesSize() const;

	// returns the dialogue at index
	const DHDialogue GetDialogue(const ALC::uint32 index) const;

private:

	ALC::Texture LoadTexture(const ALC::string& path);
	ALC::ContentStorage* m_preferredStorage;
	ALC::vector<DHActor> m_actors;
	ALC::vector<DHDialogue> m_dialogue;
};

#endif // !ALICE_DIALOGUEHANDLER_HPP