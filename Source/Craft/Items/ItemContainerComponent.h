#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ItemDefinition.h"
#include "Components/ActorComponent.h"
#include "ItemContainerComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemDefinition* ItemDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ABaseItem> ItemActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 0;
    
	bool IsEmpty() const
	{
		return ItemDefinition == nullptr || Quantity <= 0;
	}

	bool IsFull() const 
	{ 
		return ItemDefinition && Quantity == ItemDefinition->MaxStackSize;
	}

	int32 GetRemainingCapacity() const
	{
		return ItemDefinition ? ItemDefinition->MaxStackSize - Quantity : 0;
	}

	bool operator==(const FInventorySlot& CurrentSlot) const
	{
		return ItemDefinition == CurrentSlot.ItemDefinition
			&& ItemActor == CurrentSlot.ItemActor
			&& Quantity == CurrentSlot.Quantity;
	}
};

USTRUCT(BlueprintType)
struct FSlotTransactionResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool Success;
};

USTRUCT(BlueprintType)
struct FSlotActivationResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool Success;

	UPROPERTY(BlueprintReadOnly)
	UItemDefinition* ItemDefinition = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ABaseItem> ItemActor = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSlotDelegate, int32, Index, FInventorySlot, Slot);

// TODO: Refactor item containers so that item actors are only spawned lazily
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRAFT_API UItemContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemContainerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	int32 FindFirstUsableSlotIndex(const UItemDefinition* ItemDefinition, int32 RequiredCapacity = 1);

	UFUNCTION(BlueprintCallable)
	FSlotTransactionResult AddItem(ABaseItem* Item, int32 Quantity, int32 SlotIndex = 0);

	FSlotActivationResult TryActivateSlot(int32 SlotIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 Capacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool CanActivateSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_ActiveSlot, meta = (AllowPrivateAccess = true))
	int32 ActiveSlot{-1};

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = true))
	FSlotDelegate SlotUpdatedDelegate;

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = true))
	FSlotDelegate SlotActivatedDelegate;

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = true))
	FSlotDelegate SlotDeactivatedDelegate;

	UPROPERTY(ReplicatedUsing=OnRep_Slots)
	TArray<FInventorySlot> Slots;

	UFUNCTION()
	void OnRep_Slots();

	UFUNCTION()
	void OnRep_ActiveSlot();
	
	// Client-side cache to detect which slots have changed
	TArray<FInventorySlot> PreviousSlots;
	int32 PreviousActiveSlot{-1};

	FSlotActivationResult ActivateSlot(int32 SlotIndex);
	FSlotActivationResult DeactivateSlot(int32 SlotIndex);
};
