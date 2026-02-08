#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UCLASS()
class CRAFT_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseItem();

	void SetDefinition(UItemDefinition* ItemDefinition);
	UItemDefinition* GetDefinition() const;
	
	virtual void OnEquip(class ACraftCharacter* Character);
	virtual void OnUnequip(class ACraftCharacter* Character);

	virtual void ExecutePrimaryAction();
	virtual void ExecuteSecondaryAction();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true));
	UItemDefinition* Definition;

};
