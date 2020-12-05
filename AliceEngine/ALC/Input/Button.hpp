#ifndef ALC_INPUT_BUTTON_HPP
#define ALC_INPUT_BUTTON_HPP

namespace ALC {

	/// return type for buttons
	class Button {
	public:

		Button() : held(false), changed(false) { }
		Button(bool held_, bool changed_) : held(held_), changed(changed_) { }

		bool IsHeld() const { return held; }
		bool Pressed() const { return held && changed; }
		bool Released() const { return !held && changed; }

		operator bool() const { return held; }

	private:
		bool held : 1;
		bool changed : 1;
	};

}

#endif // !ALC_INPUT_BUTTON_HPP