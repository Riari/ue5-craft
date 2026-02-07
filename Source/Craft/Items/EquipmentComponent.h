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

	bool TryEquipMainHandItem(class ABaseItem* Item);
	void UnequipMainHandItem();
	
	UFUNCTION(Server, Reliable)
	void Server_TryEquip(ABaseItem* Item);
	
	UFUNCTION(Server, Reliable)
	void Server_Unequip(ABaseItem* Item);

	virtual void Equip(ABaseItem* Item);
	virtual void Unequip(ABaseItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ExecutePrimaryAction();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ExecuteSecondaryAction();

protected:
	UPROPERTY(Transient)
	TObjectPtr<class ABaseItem> MainHandItem;

	UPROPERTY(Transient)
	TObjectPtr<class ABaseItem> OffHandItem;
	
	class ACraftCharacter* GetCharacter() const;
};
