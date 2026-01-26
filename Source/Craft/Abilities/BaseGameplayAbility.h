#pragma once

#include "CoreMinimal.h"
#include "InputID.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

UCLASS()
class CRAFT_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Ability")
	EAbilityInputID AbilityInputID{ EAbilityInputID::None };

public:
	EAbilityInputID GetAbilityInputID() const { return AbilityInputID; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	bool IsInputPressed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	UEquipmentComponent* GetEquipmentComponent();

protected:
	UEquipmentComponent* EquipmentComponent;
};
