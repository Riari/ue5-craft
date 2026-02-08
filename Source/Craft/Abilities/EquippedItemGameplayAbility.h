#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "EquippedItemGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EItemAbilityCooldownType : uint8
{
	Default,
	PrimaryActionMontageLength,
	SecondaryActionMontageLength,
};

UCLASS()
class CRAFT_API UEquippedItemGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	EItemAbilityCooldownType CooldownType{ EItemAbilityCooldownType::Default };
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float MontageCooldownAdjustment{-0.1f};

	FName CooldownMagnitudeTagName = FName("Gameplay.Effect.Magnitude");

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

};
