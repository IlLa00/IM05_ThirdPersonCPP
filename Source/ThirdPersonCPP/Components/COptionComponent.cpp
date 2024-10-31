#include "COptionComponent.h"

UCOptionComponent::UCOptionComponent()
{
	MouseXSpeed = 90.f;
	MouseYSpeed = 45.f;
}

void UCOptionComponent::SetMouseXSpeed(float InSpeed)
{
	MouseXSpeed = InSpeed;
}

void UCOptionComponent::SetMouseYSpeed(float InSpeed)
{
	MouseYSpeed = InSpeed;
}
