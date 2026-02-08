#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffect.h"
#include "NiagaraSystem.h"
#include "EquippableItem.generated.h"

UCLASS()
class CRAFT_API AEquippableItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AEquippableItem();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnEquip(ACraftCharacter* Character) override;
	virtual void OnUnequip(ACraftCharacter* Character) override;
	
	virtual void ExecutePrimaryAction() override;
	virtual void ExecuteSecondaryAction() override;

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetPrimaryActionMontage() const;

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetSecondaryActionMontage() const;

protected:
	// The item mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	// Gameplay effects to apply to a valid object on hit
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UGameplayEffect>> HitEffects;

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

	const char* MainHandSocketName = "Socket_HandR";
	const char* SwingStartNotify = "SwingStart";
	const char* SwingEndNotify = "SwingEnd";

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void OnHit(AActor* OtherActor);

	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayHitEffects(const FVector& Location, const FRotator& Rotation);
};
