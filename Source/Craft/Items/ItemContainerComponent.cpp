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

int32 UItemContainerComponent::FindFirstEmptySlotIndex()
{
	for (int32 i = 0; i < Capacity; i++)
	{
		if (Slots[i].IsEmpty()) return i;
	}

	return -1;
}

FSlotTransactionResult UItemContainerComponent::AddItemByDefinition(UItemDefinition* ItemDefinition, int32 Quantity, int32 SlotIndex)
{
	FSlotTransactionResult Result{.Success = false};

	FInventorySlot& Slot = Slots[SlotIndex];
	if (!Slot.IsFull())
	{
		// TODO: Implement proper moving and replacement of items
		// For now, just write to the slot without validating anything
		Slot.Quantity = Quantity;
		Slot.ItemDefinition = ItemDefinition;
		Result.Success = true;
		SlotUpdatedDelegate.Broadcast(SlotIndex, Slot);
	}

	return Result;
}

FSlotTransactionResult UItemContainerComponent::AddItemByActor(ABaseItem* Item, int32 Quantity, int32 SlotIndex)
{
	return AddItemByDefinition(Item->GetDefinition(), Quantity, SlotIndex);
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
	if (Slot.IsEmpty())
	{
		ActiveSlot = SlotIndex;
		SlotActivatedDelegate.Broadcast(SlotIndex, Slot);
		return {.Success = true};
	}

	if (Slot.ItemDefinition)
	{
		if (Slot.ItemDefinition->IsEquippable)
		{
			ActiveSlot = SlotIndex;
			SlotActivatedDelegate.Broadcast(SlotIndex, Slot);
			return {
				.Success = true,
				.ItemDefinition = Slot.ItemDefinition,
			};
		}
	}
	
	return {.Success = false};
}

FSlotActivationResult UItemContainerComponent::DeactivateSlot(int32 SlotIndex)
{
	SlotDeactivatedDelegate.Broadcast(SlotIndex, Slots[SlotIndex]);
	ActiveSlot = -1;
	return {.Success = true};
}
