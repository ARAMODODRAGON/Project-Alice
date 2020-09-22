#include "ColType.hpp"
#include "../General/Serialization.hpp"

RTTR_REGISTRATION {
	registration::enumeration<ColType>("ColType") (
			rttr::value("None",		ColType::None),
			rttr::value("Circle",	ColType::Circle),
			rttr::value("Box",		ColType::Box),
			rttr::value("Tilemap",	ColType::Tilemap),
			rttr::value("Line",		ColType::Line)
	);
}
