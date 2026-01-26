#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "GameplayAbilitySpecHandle.h"
#include "EquippableItem.generated.h"

UCLASS()
class CRAFT_API AEquippableItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AEquippableItem();

	void Equip(class ACraftCharacter* Character);
	void Unequip(class ACraftCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent)
	void ExecutePrimaryAction();

	UFUNCTION(BlueprintImplementableEvent)
	void ExecuteSecondaryAction();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	ACraftCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMeshComponent;

	// The abilities granted by this item when equipped
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UBaseGameplayAbility>> Abilities;

	TArray<FGameplayAbilitySpecHandle> GrantedAbilities;
};
