#ifndef __INPUT_BUTTON_HPP
#define __INPUT_BUTTON_HPP

/// return type for buttons
class Button {
	bool held : 1;
	bool changed : 1;

public:

	Button(bool held_, bool changed_) : held(held_), changed(changed_) { }

	bool IsHeld() const { return held; }
	bool Pressed() const { return held && changed; }
	bool Released() const { return !held && changed; }

	operator bool() const { return held; }
};

#endif // !__INPUT_BUTTON_HPP