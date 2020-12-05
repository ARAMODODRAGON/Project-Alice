#ifndef ALC_INPUT_KEYBOARD_HPP
#define ALC_INPUT_KEYBOARD_HPP
#include "../General.hpp"
#include "Button.hpp"

namespace ALC {

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
	constexpr size_t KEYCODE_COUNT = static_cast<size_t>(KeyCode::__SIZE__);

	class Keyboard {
		ALC_NON_CONSTRUCTABLE(Keyboard);
	public:

		/// getters
		static Button GetKey(KeyCode key);
		static string KeyToString(KeyCode key);

	public:
		static void __ClearChangedBits();
		static void __SetKey(KeyCode key, bool value);
		struct KeyValue {
			bool isPressed : 1;
			bool hasChanged : 1;

			KeyValue() : isPressed(false), hasChanged(false) { }
		};
		static array<KeyValue, KEYCODE_COUNT> keyValues;
	};

}

#endif // !ALC_INPUT_KEYBOARD_HPP