#ifndef ALICE_ALICEGAME_HPP
#define ALICE_ALICEGAME_HPP
#include <ALC\SceneManager.hpp>

class AliceGame final : public ALC::Game {
public:
	AliceGame() { }
	~AliceGame() { }

	void Init() override;
	void Exit() override;

	void Step(ALC::Timestep t) override;
	void PreDraw() override;
	void Draw() override;
	void PostDraw() override;

};

#endif // !ALICE_ALICEGAME_HPP