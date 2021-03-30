#include "SaveSystem.hpp"
#include <fstream>
#include <filesystem>
#include <shlobj_core.h>

namespace fs = std::filesystem;

namespace {

	fs::path g_folderPath;
	fs::path g_savepath;
	fs::path g_settingspath;
	//#define GAME_SAVE_PATH "/CelestialVoid_StarsOfTheAbyss"
	//#define SAVE_FILE "/save.dat"
	//#define SETTINGS_FILE "/settings.json"
	#define GAME_SAVE_PATH "saves"
	#define SAVE_FILE "/save.dat"
	#define SETTINGS_FILE "/settings.json"

}

void SaveSystem::InitSavedata() {

	//constexpr int appdatapath_size = 50;
	//char appdatapath[appdatapath_size];
	//
	//// get appdata path
	//ZeroMemory(appdatapath, appdatapath_size);
	//appdatapath[appdatapath_size - 1] = '\0';
	//SHGetSpecialFolderPathA(NULL, appdatapath, CSIDL_LOCAL_APPDATA, FALSE);

	// confirm the existence of the base folder
	g_folderPath = fs::path(GAME_SAVE_PATH);
	if (!fs::exists(g_folderPath)) {
		fs::create_directory(g_folderPath);
	}

	// confirm the save and settings files exist
	g_savepath = fs::path(g_folderPath.string() + SAVE_FILE);
	if (!fs::exists(g_savepath)) {
		ResetSave();
	}

	g_settingspath = fs::path(g_folderPath.string() + SETTINGS_FILE);
	if (!fs::exists(g_settingspath)) {
		// set up file
		std::ofstream file(g_settingspath);
		file << ALC::json::object();
		file.close();
	}

}

void SaveSystem::ResetSave() {
	std::ofstream file(g_savepath);
	ALC::json j = ALC::json::object();
	j[STORY_PROGRESS] = 0;
	ALC::json::to_bson(j, file); // empty json file
	file.close();
}

void SaveSystem::Set(const ALC::string& key, const ALC::int32 value) {
	if (!fs::exists(g_savepath)) {
		ALC_DEBUG_ERROR("Save file does not exist");
		return;
	}
	std::fstream file(g_savepath);
	ALC::json j = ALC::json::from_bson(file);
	try {
		j[key] = value;
	} catch (ALC::json::exception& e) {
		ALC_DEBUG_ERROR(e.what());
	}
	ALC::json::to_bson(j, file);
	file.close();
}

ALC::int32 SaveSystem::Get(const ALC::string& key) {
	if (!fs::exists(g_savepath)) {
		ALC_DEBUG_ERROR("Save file does not exist");
		return 0;
	}
	std::ifstream file(g_savepath);
	if (!file.is_open()) {
		ALC_DEBUG_ERROR("Save file could not be opened");
		return 0;
	}
	ALC::json j = ALC::json::from_bson(file);
	file.close();
	if (j.contains(key)) {
		if (j[key].is_number())
			return j[key];
		ALC_DEBUG_ERROR("Save file incorrect value format");
	}
	return 0;
}

ALC::json SaveSystem::GetSettings() {
	if (!fs::exists(g_settingspath)) {
		ALC_DEBUG_ERROR("Settings file does not exist");
		return ALC::json();
	}
	std::fstream file(g_settingspath);
	if (!file.is_open()) {
		ALC_DEBUG_ERROR("Settings file could not be opened");
		return ALC::json();
	}
	ALC::json j;
	file >> j;
	file.close();
	return j;
}

void SaveSystem::UpdateSettings(const ALC::json& settings) {
	if (!fs::exists(g_settingspath)) {
		ALC_DEBUG_ERROR("Settings file does not exist");
		return;
	}
	std::ofstream file(g_settingspath);
	file << settings;
	file.close();
}
