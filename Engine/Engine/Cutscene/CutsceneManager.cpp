#include "CutsceneManager.hpp"





CutsceneManager::CutsceneManager() : cutscenesIndex(nullptr), currentAction(-1),isComplete(false)
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

		//DEBUG_LOG(" Successfully initiated CUTSCENE MANAGER");
	}
	else {
		DEBUG_ERROR("Init has already been called!");
	}
}
 
void CutsceneManager::loadCutscene(const std::string& filePath_)
{
	auto* index = Get()->cutscenesIndex;
	auto& activeActors_ = Get()->activeActors;

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

			auto& actorsNames = file["actors"];
			std::sort(actorsNames.begin(), actorsNames.end());
			for (auto& actor = Get()->actors.begin(); actor != Get()->actors.end();) {
				for (size_t i = 0; i < actorsNames.size(); ++i) {
					json& tmp = actorsNames[i];
					std::string name = tmp;

					if (actor->first == name) {

						if (actor->second) {
							activeActors_.insert(std::pair<std::string, CActor*>(actor->first, actor->second));
							actor->second->SetIsControlled(true);
							DEBUG_LOG(actor->first + " was added to active actors and is set to controlled");
							++actor;
						}

						else {

							DEBUG_ERROR("Actor " + name + "was nullptr");
							return;
						}
					}
				}
			}
		}
		

		if (file.contains("actions") && file["actions"].is_array()) {
			json& actions = file["actions"];
			auto& actions_ = Get()->actions;
			json data;
			
			for (size_t i = 0; i < actions.size(); ++i) {
				json& tmp = actions[i];

				std::string& type = (std::string)tmp["type"];
				std::string& actionName = (std::string)tmp["data"]["name"];
				std::string& actorName = (std::string)tmp["data"]["actorName"];
				glm::vec2 actionPosition((float)tmp["data"]["position"]["x"], (float)tmp["data"]["position"]["y"]);
				
				//DEBUG_ERROR("action poition " + VTOS(actionPosition));

				if (type == "CMoveAction") {

					if (activeActors_.find(actorName) != activeActors_.end()) {
						auto* moveAction = new CMoveAction();
						moveAction->SetActorName(actorName);
						moveAction->SetName(actionName);
						moveAction->SetPosition(actionPosition);
						actions_.push_back(moveAction);
						DEBUG_LOG(actionName + " was added to the actions vector, good job");
					}
				}

				else {
					DEBUG_ERROR(type + "could not be found in the .cutscene file foo.");
				}

			}
		}
	}

	else { 
		DEBUG_ERROR(filePath_ + " Not Found"); 
		return;
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
	bool& isComplete_ = Get()->isComplete;

	if (!isComplete_) {
	   
		size_t& currentAction_ = Get()->currentAction;
		auto& actions_ = Get()->actions;

		if (!actions_.empty()) {
			//start the current action only called once for each current action
			if (currentAction_ == -1) {
				currentAction_ += 1;
				actions_[currentAction_]->Start();
			}

			if (actions_[currentAction_]) {

				if (actions_[currentAction_]->isRunning()) {

					actions_[currentAction_]->Update();
				}


				else {
					//get rid of the action once its complete
					actions_[currentAction_] = nullptr;
					actions_.erase(actions_.begin() + currentAction_);

					//no more actions in the vector cutscene is over 
					if (actions_.empty()) {
						DEBUG_LOG("cutscene has finished");
						isComplete_ = true;
					}
					//action array is now set to -1 to call START() on the next action
					currentAction_ = -1;
				}
			}
		}
		else {
			//do nothing 
		}
	}

}



CActor* CutsceneManager::GetActiveActor(std::string& actorName_)
{	
	auto& activeActors_ = Get()->activeActors;
	auto& itter = activeActors_.find(actorName_);

	if (itter != activeActors_.end()) {
		return itter->second;
	}
	else {
		DEBUG_ERROR("Actor : " + itter->first + " is not an active actor");
		return nullptr;
	}
}

CActor* CutsceneManager::GetActor(std::string& actorName_)
{
	auto& actors_ = Get()->actors;
	auto& itter = actors_.find(actorName_);

	if (itter != actors_.end()) {
		return itter->second;
	}
	else {
		DEBUG_ERROR("Actor : " + itter->first + " is not an active actor");
		return nullptr;
	}
}
									  