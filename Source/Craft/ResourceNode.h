#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/HealthAttributeSet.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHarvestable.h"
#include "Interfaces/IHittable.h"
#include "Items/ItemDefinition.h"
#include "ResourceNode.generated.h"

UCLASS(Blueprintable, BlueprintType)
class CRAFT_API AResourceNode : public AActor, public IAbilitySystemInterface, public IHittable, public IHarvestable
{
	GENERATED_BODY()

public:
	AResourceNode();

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual bool CanBeHitWith_Implementation(class AEquippableItem* Item) const override;

	virtual TSubclassOf<ABaseItem> GetResourceItemActorClass_Implementation() const override;
	virtual int32 GetResourceQuantityPerHit_Implementation() const override;
	virtual void OnHarvest_Implementation(AActor* Harvester) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UHealthAttributeSet> HealthAttributeSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> DestroySound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UNiagaraSystem> DestroyParticles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource Node", meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseItem> ResourceItemActorClass;

	void InitializeAttributes();

	void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDestroy();
};
