#ifndef ALICE_ALICEGAME_HPP
#define ALICE_ALICEGAME_HPP
#include <ALC\SceneManager.hpp>
#include <ALC\Content.hpp>
#include <ALC\Rendering.hpp>
#include <ALC\Input.hpp>

class AliceGame final : public ALC::Game {
public:
	AliceGame();
	~AliceGame();

	void Init() override;
	void Exit() override;

	void Step(ALC::Timestep t) override;
	void PreDraw() override;
	void Draw() override;
	void PostDraw() override;

private:

};

#endif // !ALICE_ALICEGAME_HPP