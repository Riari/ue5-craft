#include "ItemContainerComponent.h"

#include "BaseItem.h"

UItemContainerComponent::UItemContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
		Slot.Quantity = Quantity;
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
