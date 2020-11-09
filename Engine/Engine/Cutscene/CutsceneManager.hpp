#ifndef CUTSCENE_MANAGER_HPP
#define CUTSCENE_MANAGER_HPP
#include "../General/Types.hpp"
#include "../General/Serialization.hpp"
#include "../Common.hpp"

class CAction;
class CActor;

class CutsceneManager {

	PRIVATE_SINGLETON(CutsceneManager);

	std::vector<CAction*> actions;
	int32 currentAction; // which action is running 

	std::map <std::string,CActor*> activeActors;
	std::map <std::string,CActor*> actors;

	CutsceneManager* currentCutscene;
	FileIndex* cutscenesIndex;
	


public:
	CutsceneManager();
	~CutsceneManager();
	
	static void Init(const string& cutsceneFilePath);
	static void loadCutscene(const std::string& name_);

	static void AddActor(std::string actorName_ ,CActor* actor_);
	static void RemoveActor(std::string actorName_, CActor* actor_);

	static void Update();

	//actor getters and setters 
	static CActor* GetActiveActor(std::string& actorName_);

protected:

};

#endif // !


