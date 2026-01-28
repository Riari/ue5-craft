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

	bool TryEquipMainHandItem(class ABaseItem* Item);
	void UnequipMainHandItem();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ExecutePrimaryAction();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ExecuteSecondaryAction();

protected:
	TObjectPtr<class ACraftCharacter> Character;

	UPROPERTY(Transient)
	TObjectPtr<class ABaseItem> MainHandItem;

	UPROPERTY(Transient)
	TObjectPtr<class ABaseItem> OffHandItem;
		
};
