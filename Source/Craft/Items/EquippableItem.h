#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffect.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "EquippableItem.generated.h"

UCLASS()
class CRAFT_API AEquippableItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AEquippableItem();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual bool TryEquip(class ACraftCharacter* Character) override;
	virtual void Unequip(class ACraftCharacter* Character) override;
	
	virtual void ExecutePrimaryAction() override;
	virtual void ExecuteSecondaryAction() override;

protected:
	// The owning character
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<ACraftCharacter> Character;

	// The item mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	// Gameplay effects to apply to a valid object on hit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UGameplayEffect>> HitEffects;

	// Sphere used to detect collision with other objects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USphereComponent> HitSphere;

	// Sound to play when hitting a valid object
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> HitSound;

	// Particles to spawn when hitting a valid object
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraSystem> HitParticles;

	// Camera shake used for hitting a valid object
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> HitCameraShakeClass;

	// The abilities granted by this item when equipped
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UBaseGameplayAbility>> Abilities;

	// Handles of granted abilities, used for removing them on unequip
	TArray<FGameplayAbilitySpecHandle> GrantedAbilities;

	// Montage to play when executing primary action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> PrimaryActionMontage;

	// Montage to play when executing secondary action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> SecondaryActionMontage;

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
	void OnHit(AActor* OtherActor);

	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
};
