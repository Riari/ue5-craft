#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Interfaces/AnimNotifiable.h"
#include "Items/EquippableItem.h"
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
class ACraftCharacter : public ACharacter, public IAnimNotifiable, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar01Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar02Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar03Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar04Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar05Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar06Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar07Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar08Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar09Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Hotbar10Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PrimaryActionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SecondaryActionAction;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaminaAttributeSet> StaminaAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UHealthAttributeSet> HealthAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<class UBaseGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<class UGameplayEffect>> DefaultEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UItemContainerComponent> InventoryContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UItemContainerComponent> HotbarContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UEquipmentComponent> EquipmentComponent;

public:
	ACraftCharacter();

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UEquipmentComponent* GetEquipmentComponent() const;

	virtual void AnimNotify(FName NotifyName) override;

	void OnStaminaAttributeChanged(const FOnAttributeChangeData& Data);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnStaminaChanged(const float OldValue, const float NewValue, const float MaxValue);

	UFUNCTION(BlueprintCallable)
	void OnItemPickUp(ABaseItem* Item)
	{
		int HotbarSlotIndex = HotbarContainer->FindFirstEmptySlotIndex();
		if (HotbarSlotIndex != -1)
		{
			HotbarContainer->AddItemByActor(Item, 1, HotbarSlotIndex);
		}
	}

protected:
	void InitializeAttributes();
	void InitializeAbilities();
	void InitializeEffects();

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void PrimaryAction(const FInputActionValue& Value);

	void SecondaryAction(const FInputActionValue& Value);

	void ActivateHotbar(int32 SlotIndex);

	void OnExecutePrimaryAction(TObjectPtr<UAnimMontage> Montage);
	void OnExecuteSecondaryAction(TObjectPtr<UAnimMontage> Montage);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID);

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

