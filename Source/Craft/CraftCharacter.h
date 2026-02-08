#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "Animations/ActionMontageComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/IAnimNotifiable.h"
#include "Interfaces/IHittable.h"
#include "Items/EquipmentComponent.h"
#include "Items/ItemContainerComponent.h"
#include "Logging/LogMacros.h"
#include "CraftCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACraftCharacter : public ACharacter, public IAnimNotifiable, public IAbilitySystemInterface, public IHittable
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = true))
	TObjectPtr<UActionMontageComponent> ActionMontageComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> Hotbar01Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> Hotbar02Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> Hotbar03Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> Hotbar04Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> Hotbar05Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> Hotbar06Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> Hotbar07Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> Hotbar08Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> Hotbar09Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> Hotbar10Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> PrimaryActionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> SecondaryActionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USoundBase> ItemPickUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> HitReactionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> DeathMontage;

public:
	ACraftCharacter();
	
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void OnRep_PlayerState() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UEquipmentComponent* GetEquipmentComponent() const;

	float PlayActionMontageForItem(TObjectPtr<ABaseItem> Item);

	virtual void AnimNotify(FName NotifyName) override;

	bool TrySpawnItemToInventory(TSubclassOf<ABaseItem> ItemClass, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable)
	void OnItemPickUp(ABaseItem* Item);
	
	UFUNCTION(Client, Reliable)
	void Client_OnItemPickUp(ABaseItem* Item);

	bool TryAddItemToInventory(ABaseItem* Item, int32 Quantity = 1);

	virtual bool CanBeHitWith_Implementation(class AEquippableItem* Item) const override;
	
	void OnDeath();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_RemoteRotation)
	FRotator ReplicatedRemoteRotation;

	bool bIsDead{false};

	void Initialize();

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void PrimaryAction(const FInputActionValue& Value);

	void SecondaryAction(const FInputActionValue& Value);

	void ActivateHotbar(int32 SlotIndex);
	
	UFUNCTION()
	void OnRep_RemoteRotation();
	
	UFUNCTION(Server, Reliable)
	void Server_SetActorRotation(FRotator NewRotation);
	
	UFUNCTION(Server, Reliable)
	void Server_ActivateHotbar(int32 SlotIndex);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayMontage(UAnimMontage* Montage);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

