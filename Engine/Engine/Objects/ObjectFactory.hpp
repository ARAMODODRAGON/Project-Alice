#ifndef _CORE_ENTITY_FACTORY_HPP
#define _CORE_ENTITY_FACTORY_HPP
#include "../General/Types.hpp"
#include "../General/Macros.hpp"
#include "../General/FileIndex.hpp"
#include "../Core/Debugger.hpp"

class Object;
class ILevel;

class ObjectFactory {
	PRIVATE_SINGLETON(ObjectFactory);

public:

	ObjectFactory();
	~ObjectFactory();

	// events
	static void Init(const string& indexPath);
	static void Update();
	static void LateUpdate();
	static void Cleanup();
	static void Exit();
	static void Clear();

	// factory methods
	template<class T> static T* Make();
	static void Destroy(Object* entity);

private:

	// an object with a bool. this bool is whether or not the object should destroy
	using PairType = pair<Object*, bool>;
	// all the entities
	vector<PairType> objects; // TODO: seperate array for new objects, these then get pushed into this array during cleanup
	list<Object*> newObjects;

	// an object index & level
	FileIndex* index;

	bool disableDestroy;

	// helper functions
	void Add(Object* e);
	void DestroyAll();

};


template<class T>
inline T* ObjectFactory::Make() {
	T* e = new T();
	Get()->Add(e);
	return e;
}


#endif // !_CORE_ENTITY_FACTORY_HPP
