#include "CutsceneManager.hpp"
#include "CActor.hpp"


CutsceneManager::CutsceneManager() : cutscenesIndex(nullptr), currentAction(0)
{
}

CutsceneManager::~CutsceneManager()
{
	if (cutscenesIndex) { delete cutscenesIndex; }
	cutscenesIndex = nullptr;
}

void CutsceneManager::Init(const string& actionFilePath_)
{
	if (!Get()->cutscenesIndex) {
		Get()->cutscenesIndex = new FileIndex(actionFilePath_);
	}
	else {
		DEBUG_ERROR("Init has already been called!");
	}
}
 
void CutsceneManager::loadCutscene(const std::string& name_)
{
}

void CutsceneManager::AddActor(std::string actorName_, CActor* actor_)
{
	auto& actors = Get()->actors;

    /* iterate from the root node through the map if the key is found return it. if it reaches the end of the map and has not found a match  
		continue with adding the actor to the map */
	
	if (actors.find(actorName_) != actors.end()) {
		DEBUG_ERROR("Actor Already Exist");
		return;
	}

	actor_->SetActorName(actorName_);
	actors.insert(std::pair<std::string, CActor*>(actorName_, actor_));
	DEBUG_LOG("Actor Added");

}

void CutsceneManager::RemoveActor(std::string actorName_,CActor* actor_)
{
	auto& actors = Get()->actors;
	// same idea as the add function instead it searches the map for a key and erases the node with the corresponding	key
	if (actors.find(actorName_) != actors.end()) {
		actors.erase(actorName_);
		delete actor_;
		actor_ = nullptr;
		DEBUG_LOG("Actor " + VTOS(actorName_) + " Removed");
		return;
	}

	//if matching key isnt found display a error messege
	DEBUG_ERROR("No Actor found with name : " + VTOS(actorName_));
}

void CutsceneManager::Update()
{
}

CActor* CutsceneManager::GetActiveActor(std::string& actorName_)
{
	for (auto& actor : Get()->activeActors) {
		if (actor.first == actorName_) { return actor.second; }
		else { 
			DEBUG_ERROR("Actor : " + actor.first + "is not an active actor"); 
			return nullptr;
		}
	}
}
									  