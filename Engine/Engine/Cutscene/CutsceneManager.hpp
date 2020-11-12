#ifndef CUTSCENE_MANAGER_HPP
#define CUTSCENE_MANAGER_HPP
#include "../General/Types.hpp"
#include "../General/Serialization.hpp"
#include "../Common.hpp"
#include <iostream>
#include <fstream>
#include "CAction.hpp"
#include "Actions/CMoveAction.hpp"



class CActor;
class CAction;

class CutsceneManager {

	PRIVATE_SINGLETON(CutsceneManager);

	std::vector<CAction*> actions;
	size_t currentAction; // which action is running 

	std::map <std::string,CActor*> activeActors;
	std::map <std::string,CActor*> actors;

	FileIndex* cutscenesIndex;
	


public:
	CutsceneManager();
	~CutsceneManager();
	
	static void Init(const std::string& cutsceneIndexPath_);
	static void loadCutscene(const std::string& filePath_);

	static void AddActor(std::string actorName_ ,CActor* actor_);
	static void RemoveActor(std::string actorName_);

	static void Update();
	static void Exit();

	//actor getters and setters 
	static CActor* GetActiveActor(std::string& actorName_);
	static CActor* GetActor(std::string& actorName_);

protected:

};

#endif // !


