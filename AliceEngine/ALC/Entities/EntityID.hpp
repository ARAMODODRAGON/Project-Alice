#ifndef ALC_ENTITIES_ID_HPP
#define ALC_ENTITIES_ID_HPP
#include "../General.hpp"

namespace ALC {

	struct EntityID {

		EntityID() : m_ID(-1) { }

		EntityID(std::nullptr_t) : m_ID(-1) { }

		// returns true if this is a valid UID
		bool IsValid() const { return m_ID != -1; }

		// returns true if this is a valid UID
		operator bool() const { return m_ID != -1; }

		// compares the two entity UIDs
		bool operator==(const EntityID& other) {
			return m_ID == other.m_ID;
		}

		// compares the two entity UIDs
		bool operator!=(const EntityID& other) {
			return m_ID == other.m_ID;
		}

		explicit EntityID(const uint64 ID_) ALC_INTERNAL
			: m_ID(ID_) { }

	private:
		uint64 m_ID;
	};

}

#endif // !ALC_ENTITIES_ID_HPP