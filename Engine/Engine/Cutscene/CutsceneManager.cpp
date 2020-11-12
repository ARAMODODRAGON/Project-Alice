#include "CutsceneManager.hpp"
#include "CActor.hpp"




CutsceneManager::CutsceneManager() : cutscenesIndex(nullptr), currentAction(0)
{
}

CutsceneManager::~CutsceneManager()
{

	if (cutscenesIndex) { delete cutscenesIndex; }
	cutscenesIndex = nullptr;

	if (activeActors.size() > 0) {

		for (auto actor : activeActors) {
			actor.second = nullptr;
			delete actor.second;
		}

		activeActors.clear();

		if (activeActors.empty()) {
			DEBUG_LOG("active actors map cleared noice");
		}
	}

	if (actors.size() > 0) {
		for (auto actor : actors)
		{
			actor.second = nullptr;
			delete actor.second;
		}

		actors.clear();

		if (actors.empty()) {
			DEBUG_LOG("actors map cleared noice");
		}
	}
	
}

void CutsceneManager::Init(const string& cutsceneIndexPath_)
{
	if (!Get()->cutscenesIndex) {
		Get()->cutscenesIndex = new FileIndex(cutsceneIndexPath_);

		DEBUG_LOG(" Successfully initiated CUTSCENE MANAGER");
	}
	else {
		DEBUG_ERROR("Init has already been called!");
	}
}
 
void CutsceneManager::loadCutscene(const std::string& filePath_)
{
	auto* index = Get()->cutscenesIndex;

	if (index->Contains(filePath_)) {
		std::ifstream stream;

		//open the .cutscene file 
		stream.open(index->GetJSONPath(filePath_));
		
		if (!stream.is_open()) {
			DEBUG_ERROR("stream could not open " + filePath_);
		}

		json file;
		stream >> file;
		stream.close();

		if (file.contains("actors") && file["actors"].is_array()) {

			json& actorsNames = file["actors"];
			auto& activeActors_ = Get()->activeActors;

			for (int i = 0; i != actorsNames.size(); ++i) {

				for (auto& actor : Get()->actors) {

					if (actorsNames[i] == actor.first) {

						if (actor.second) {

							activeActors_.insert(std::pair<std::string, CActor*>(actor.first, actor.second));
							actor.second->SetIsControlled(true);
							DEBUG_LOG(actor.first + " was added to active actors and is set to controlled");
						}
						else {

							DEBUG_ERROR("Actor " + (std::string)actorsNames[i] + "was nullptr");
							return;
						}
					}

					else {

						DEBUG_ERROR("Actor : " + (std::string)actorsNames[i] + " Was not found or has not been created");
						return;
					}
					
				}
				
			}
		}
	}

	else { 
		DEBUG_ERROR(filePath_ + " Not Found"); 
		return;
	}

	size_t maxActions = 4;
	auto& actions_ = Get()->actions;
	size_t& currentAction_ = Get()->currentAction;

	for (size_t i = 0; i < maxActions; ++i) {
		auto* moveAction = new CMoveAction();

		actions_.push_back(moveAction);
	}


	for (auto& actor : Get()->activeActors) {
		static_cast<CMoveAction*>(actions_[currentAction_])->SetName(actor.first);
		currentAction_++;
	}
	

	
	

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
	DEBUG_LOG("Actor : " + actorName_ + " Added");

}

void CutsceneManager::RemoveActor(std::string actorName_)
{
	auto& actors = Get()->actors;
	// same idea as the add function instead it searches the map for a key and erases the node with the corresponding	key
	if (actors.find(actorName_) != actors.end()) {
		actors.erase(actorName_);

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
	for (auto& activeActor : Get()->activeActors) {
		if (activeActor.first == actorName_) { return activeActor.second; }
		else { 
			DEBUG_ERROR("Actor : " + activeActor.first + " is not an active actor");
			return nullptr;
		}
	}
}

CActor* CutsceneManager::GetActor(std::string& actorName_)
{
	for (auto& actor : Get()->activeActors) {
		if (actor.first == actorName_) { DEBUG_ERROR("Actor : " + actor.first + " is  an actor"); return actor.second; }
		else {
			DEBUG_ERROR("Actor : " + actor.first + " is not an actor");
			return nullptr;
		}
	}
}
									  