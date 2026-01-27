#include "PlayerAttributeSet.h"

UPlayerAttributeSet::UPlayerAttributeSet()
	: Stamina{0.0f}
	, MaxStamina{100.0f}
	, StaminaRegen{2.0f}
{
}

void UPlayerAttributeSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
}
