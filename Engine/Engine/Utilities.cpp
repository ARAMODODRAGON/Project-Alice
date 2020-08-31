#include "Utilities.hpp"
#include "Core/Engine.hpp"

void Util::App::Run() { Engine::Get()->Run(); }

void Util::App::Quit() { Engine::Get()->Quit(); }
