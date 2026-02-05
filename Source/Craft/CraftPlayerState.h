#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Abilities/HealthAttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "Items/EquipmentComponent.h"
#include "Items/ItemContainerComponent.h"
#include "CraftPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, float, OldValue, float, NewValue, float, MaxValue);

UCLASS()
class CRAFT_API ACraftPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ACraftPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	void InitializeAbilitySystem();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UItemContainerComponent* GetInventoryContainer() const;
	UItemContainerComponent* GetHotbarContainer() const;
	UEquipmentComponent* GetEquipmentComponent() const;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged Client_OnStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged Client_OnHealthChanged;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilitySystemInitialized);

	/** Fired on the owning client when the server has finished InitializeAbilitySystem() */
	UPROPERTY(BlueprintAssignable, Category = "AbilitySystem")
	FOnAbilitySystemInitialized OnAbilitySystemInitialized;

	UFUNCTION(Client, Reliable)
	void RPC_Client_OnAbilitySystemInitialized();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Abilities, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaminaAttributeSet> StaminaAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Abilities, meta = (AllowPrivateAccess = true))
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
	
	UFUNCTION(Client, Reliable)
	void RPC_Client_OnStaminaChanged(float OldValue, float NewValue, float Max);

	UFUNCTION(Client, Reliable)
	void RPC_Client_OnHealthChanged(float OldValue, float NewValue, float Max);

	void OnStaminaAttributeChanged(const FOnAttributeChangeData& Data);
	void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);
};
