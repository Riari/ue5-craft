#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffect.h"
#include "../../../../../../../../Program Files/Epic Games/UE_5.4/Engine/Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "EquippableItem.generated.h"

DECLARE_DELEGATE_OneParam(FOnExecutePrimaryAction, TObjectPtr<UAnimMontage>);
DECLARE_DELEGATE_OneParam(FOnExecuteSecondaryAction, TObjectPtr<UAnimMontage>);

UCLASS()
class CRAFT_API AEquippableItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AEquippableItem();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Equip(class ACraftCharacter* Character);
	void Unequip(class ACraftCharacter* Character);

	UFUNCTION(BlueprintNativeEvent)
	void ExecutePrimaryAction();

	UFUNCTION(BlueprintNativeEvent)
	void ExecuteSecondaryAction();

	FOnExecutePrimaryAction OnExecutePrimaryAction;
	FOnExecuteSecondaryAction OnExecuteSecondaryAction;

protected:
	// The owning character
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<ACraftCharacter> Character;

	// The item mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	// Gameplay effects to apply to valid object on hit
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

	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
};
