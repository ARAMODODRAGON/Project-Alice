#ifndef ALC_SAVESYSTEM_HPP
#define ALC_SAVESYSTEM_HPP
#include <ALC\General.hpp>

#define STORY_PROGRESS "StoryProgress"

class SaveSystem {
	ALC_NON_CONSTRUCTABLE(SaveSystem);
public:

	// checks if there is a save file, creates a new one if there isnt
	static void InitSavedata();

	// resets the save file to its initial state
	static void ResetSave();

	// sets a value in the save file
	static void Set(const ALC::string& key, const ALC::int32 value);

	// gets a value from the save file
	static ALC::int32 Get(const ALC::string& key);

	// gets the settings json file
	static ALC::json GetSettings();

	// updates the settings json file
	static void UpdateSettings(const ALC::json& settings);

private:
};

#endif // !ALC_SAVESYSTEM_HPP