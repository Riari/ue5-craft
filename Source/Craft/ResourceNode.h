#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/HealthAttributeSet.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHittable.h"
#include "ResourceNode.generated.h"

UCLASS(Blueprintable, BlueprintType)
class CRAFT_API AResourceNode : public AActor, public IAbilitySystemInterface, public IHittable
{
	GENERATED_BODY()

public:
	AResourceNode();

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual bool CanBeHitWith_Implementation(class AEquippableItem* Item) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UHealthAttributeSet> HealthAttributeSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> DestroySound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UNiagaraSystem> DestroyParticles;

	void InitializeAttributes();

	void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDestroy();
};
