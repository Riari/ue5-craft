#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

DECLARE_DELEGATE_OneParam(FOnExecutePrimaryAction, TObjectPtr<UAnimMontage>);
DECLARE_DELEGATE_OneParam(FOnExecuteSecondaryAction, TObjectPtr<UAnimMontage>);

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

	FOnExecutePrimaryAction OnExecutePrimaryAction;
	FOnExecuteSecondaryAction OnExecuteSecondaryAction;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true));
	UItemDefinition* Definition;

};
