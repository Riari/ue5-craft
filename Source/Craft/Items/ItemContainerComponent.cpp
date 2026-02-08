#include "ItemContainerComponent.h"

#include "BaseItem.h"
#include "Net/UnrealNetwork.h"

UItemContainerComponent::UItemContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UItemContainerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemContainerComponent, Slots);
	DOREPLIFETIME(UItemContainerComponent, ActiveSlot);
}

void UItemContainerComponent::BeginPlay()
{
	Slots.SetNumZeroed(Capacity);
}

int32 UItemContainerComponent::FindFirstUsableSlotIndex(const UItemDefinition* ItemDefinition, int32 RequiredCapacity)
{
	bool bStackable = ItemDefinition->MaxStackSize > 1;
	for (int32 i = 0; i < Capacity; i++)
	{
		FInventorySlot& Slot = Slots[i];

		if (Slot.IsEmpty()) return i;
		
		int32 RemainingCapacity = Slot.GetRemainingCapacity();
		if (bStackable && RemainingCapacity >= RequiredCapacity) return i;
	}

	return -1;
}

FSlotTransactionResult UItemContainerComponent::AddItem(ABaseItem* Item, int32 Quantity, int32 SlotIndex)
{
	FSlotTransactionResult Result{.Success = false};

	FInventorySlot& Slot = Slots[SlotIndex];
	if (!Slot.IsFull())
	{
		// TODO: Implement proper moving and replacement of items
		// For now, just write to the slot without validating anything
		Slot.Quantity += Quantity;
		Slot.ItemDefinition = Item->GetDefinition();
		Slot.ItemActor = Item;
		Result.Success = true;
		SlotUpdatedDelegate.Broadcast(SlotIndex, Slot);
	}

	return Result;
}

FSlotActivationResult UItemContainerComponent::TryActivateSlot(int32 SlotIndex)
{
	if (SlotIndex == ActiveSlot)
		return {.Success = false};

	return ActivateSlot(SlotIndex);
}

void UItemContainerComponent::OnRep_Slots()
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		FInventorySlot& CurrentSlot = Slots[i];
		if (PreviousSlots.IsValidIndex(i))
		{
			FInventorySlot& PreviousSlot = PreviousSlots[i];

			if (PreviousSlot != CurrentSlot)
			{
				SlotUpdatedDelegate.Broadcast(i, CurrentSlot);
			}
		}
		else
		{
			SlotUpdatedDelegate.Broadcast(i, CurrentSlot);
		}
	}
}

void UItemContainerComponent::OnRep_ActiveSlot()
{
	if (PreviousActiveSlot != ActiveSlot)
	{
		if (PreviousActiveSlot != INDEX_NONE)
		{
			SlotDeactivatedDelegate.Broadcast(PreviousActiveSlot, Slots[PreviousActiveSlot]);
		}

		if (ActiveSlot != INDEX_NONE)
		{
			SlotActivatedDelegate.Broadcast(ActiveSlot, Slots[ActiveSlot]);
		}

		PreviousActiveSlot = ActiveSlot;
	}
}

FSlotActivationResult UItemContainerComponent::ActivateSlot(int32 SlotIndex)
{
	if (!CanActivateSlots)
		return {.Success = false};

	FInventorySlot& Slot = Slots[SlotIndex];
	ActiveSlot = SlotIndex;
	SlotActivatedDelegate.Broadcast(SlotIndex, Slot);
	
	return {
		.Success = true,
		.ItemDefinition = Slot.ItemDefinition,
		.ItemActor = Slot.ItemActor
	};
}

FSlotActivationResult UItemContainerComponent::DeactivateSlot(int32 SlotIndex)
{
	SlotDeactivatedDelegate.Broadcast(SlotIndex, Slots[SlotIndex]);
	ActiveSlot = -1;
	return {.Success = true};
}
