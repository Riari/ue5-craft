#pragma once

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None = 0,
	Confirm = 1,
	Cancel = 2,
	PrimaryAction = 3,
	SecondaryAction = 4,
};