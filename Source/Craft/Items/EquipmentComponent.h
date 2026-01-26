#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRAFT_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentComponent();

	virtual void BeginPlay() override;

	void EquipMainHandItem(class UItemDefinition* ItemDefinition);
	void UnequipMainHandItem();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ExecutePrimaryAction();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ExecuteSecondaryAction();

protected:
	class ACraftCharacter* Character;

	class AEquippableItem* MainHandItem;
	class AEquippableItem* OffHandItem;
		
};
