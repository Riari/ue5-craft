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

	class AEquippableItem* EquipMainHandItem(class UItemDefinition* ItemDefinition);
	void UnequipMainHandItem();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ExecutePrimaryAction();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ExecuteSecondaryAction();

protected:
	TObjectPtr<class ACraftCharacter> Character;

	TObjectPtr<class AEquippableItem> MainHandItem;
	TObjectPtr<class AEquippableItem> OffHandItem;
		
};
