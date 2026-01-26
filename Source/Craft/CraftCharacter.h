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

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar01Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar02Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar03Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar04Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar05Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar06Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar07Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar08Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar09Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar10Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryActionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SecondaryActionAction;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	class UPlayerAttributeSet* AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UBaseGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UItemContainerComponent* InventoryContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UItemContainerComponent* HotbarContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	class UEquipmentComponent* EquipmentComponent;

public:
	ACraftCharacter();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UEquipmentComponent* GetEquipmentComponent() const;

	virtual void AnimNotify(FName NotifyName) override;

	void OnStaminaAttributeChanged(const FOnAttributeChangeData& Data);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnStaminaChanged(const float OldValue, const float NewValue);

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

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void PrimaryAction(const FInputActionValue& Value);

	void SecondaryAction(const FInputActionValue& Value);

	void ActivateHotbar(int32 SlotIndex);
			
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID);

	void InitializeAbilities();
	void InitializeEffects();

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

