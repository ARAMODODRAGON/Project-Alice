#ifndef __INPUT_KEYBOARD_HPP
#define __INPUT_KEYBOARD_HPP
#include "../General/Macros.hpp"
#include "../General/Types.hpp"
#include "Button.hpp"

enum class KeyCode : int32 {
	KeyA,
	KeyB,
	KeyC,
	KeyD,
	KeyE,
	KeyF,
	KeyG,
	KeyH,
	KeyI,
	KeyJ,
	KeyK,
	KeyL,
	KeyM,
	KeyN,
	KeyO,
	KeyP,
	KeyQ,
	KeyR,
	KeyS,
	KeyT,
	KeyU,
	KeyV,
	KeyW,
	KeyX,
	KeyY,
	KeyZ,
	Num0,
	Num1,
	Num2,
	Num3,
	Num4,
	Num5,
	Num6,
	Num7,
	Num8,
	Num9,
	LeftShift,
	RightShift,
	LeftCtrl,
	RightCtrl,
	LeftAlt,
	RightAlt,
	Return,
	Escape,
	Backspace,
	Tab,
	Space,
	ArrowUp,
	ArrowDown,
	ArrowLeft,
	ArrowRight,
	__SIZE__
};

class Keyboard {
	NON_CONSTRUCTABLE(Keyboard);
	friend class SystemEvents; // SystemEvents modifies this class

	// should only take up one byte
	struct KeyValue {
		bool isPressed : 1;
		bool hasChanged : 1;

		KeyValue() : isPressed(false), hasChanged(false) { }
	};

	static array<KeyValue, ENUM_SIZE(KeyCode)> keyValues;

public:

	/// getters

	static Button GetKey(KeyCode key);
	static string KeyToString(KeyCode key);

private:

	/// functions

	static void ClearChangedBits();
	static void SetKey(KeyCode key, bool value);

};

/// to_string overload
namespace std {
	inline string to_string(KeyCode key) {
		return Keyboard::KeyToString(key);
	}
}

#endif // !__INPUT_KEYBOARD_HPP