#include "StaminaAttributeSet.h"

UStaminaAttributeSet::UStaminaAttributeSet()
	: Stamina{0.0f}
	, MaxStamina{100.0f}
	, StaminaRegen{2.0f}
{
}

void UStaminaAttributeSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
}
