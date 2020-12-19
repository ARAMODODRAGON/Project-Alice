#ifndef ALC_LAYERMASK_HPP
#define ALC_LAYERMASK_HPP
#include "General.hpp"

namespace ALC {

	template<typename I, typename = std::enable_if_t<std::is_integral_v<I>>>
	struct Layermask {

		// int with all layers set
		static constexpr I ALL = ~static_cast<I>(0);

		// int with no layers set
		static constexpr I NONE = static_cast<I>(0);

		// constructors / operators
		Layermask() : mask(0) { }
		Layermask(const I mask_) : mask(mask_) { }
		Layermask& operator=(const I mask_) { 
			mask = mask_; 
			return *this;
		}

		// operators

		// cast to unsigned int
		explicit operator I() const { return mask; }

		// checks if the masks match exactly
		bool operator==(const Layermask& other) const { return mask == other.mask; }

		// checks if the masks dont match exactly
		bool operator!=(const Layermask& other) const { return mask != other.mask; }

		// returns true if *any* layers match
		bool operator&&(const Layermask& other) const { return mask & other.mask; }

		// returns a layermask with only the matching layers
		Layermask operator&(const Layermask& other) const { return Layermask(mask & other.mask); }
		Layermask& operator&=(const Layermask& other) {
			mask &= other.mask;
			return *this;
		}

		// used to check if any layers are set
		operator bool() const { return mask != 0; }

		// creates a new layermask with the layers of the two old ones
		Layermask operator|(const Layermask& other) const { return Layermask(mask | other.mask); }
		Layermask& operator|=(const Layermask& other) {
			mask |= other.mask;
			return *this;
		}

		// returns a reversed layermask
		Layermask operator~() const { return Layermask(~mask); }

		// function to set/unset layers
		void SetLayer(const uint32 layer, bool value) {
			if (value)	mask |= (1 << layer);  // set true
			else		mask &= ~(1 << layer); // set false
		}

		// function to get layer
		bool GetLayer(const uint32 layer) const {
			return mask & (1 << layer);
		}

	private:

		// 32 layers of bools essentially
		I mask;

	};

	// 64 layers
	using Layermask64 = Layermask<uint64>;
	// 32 layers
	using Layermask32 = Layermask<uint32>;
	// 16 layers
	using Layermask16 = Layermask<uint16>;
	// 8 layers
	using Layermask8 = Layermask<uint8>;

}

#endif // !ALC_LAYERMASK_HPP