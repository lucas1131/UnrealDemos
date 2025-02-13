#pragma once

#include "InteractableStencilMask.generated.h"

/* Using a enum for this is a pretty bad idea, this forces artists to follow the code, it should be the other way around */
UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor=true))
enum class EInteractableStencilMask : uint8
{
	None      = 0,
	Outline   = 1 << 0,
	Highlight = 1 << 1,
	Unused1   = 1 << 2,
	Unused2   = 1 << 3,
	Unused3   = 1 << 4,
	Unused4   = 1 << 5,
	Unused5   = 1 << 6,
	Unused6   = 1 << 7,
};
