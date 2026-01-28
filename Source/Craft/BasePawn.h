#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS(Blueprintable)
class CRAFT_API ABasePawn : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABasePawn();

protected:
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void InitializeEffects();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UHealthAttributeSet> HealthAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<class UGameplayEffect>> DefaultEffects;
};
