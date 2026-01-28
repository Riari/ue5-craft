#include "EquipmentComponent.h"

#include "EquippableItem.h"
#include "Craft/CraftCharacter.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACraftCharacter>(GetOwner());
}

bool UEquipmentComponent::TryEquipMainHandItem(ABaseItem* Item)
{
	ensureMsgf(IsValid(Item), TEXT("Item invalid. PendingKill=%d"), Item && Item->IsPendingKillPending());

	if (Item->TryEquip(Character))
	{
		MainHandItem = Item;
		return true;
	}
	
	return false;
}

void UEquipmentComponent::UnequipMainHandItem()
{
	if (MainHandItem)
	{
		MainHandItem->Unequip(Character);
		MainHandItem = nullptr;
	}
}

void UEquipmentComponent::ExecutePrimaryAction()
{
	MainHandItem->ExecutePrimaryAction();
}

void UEquipmentComponent::ExecuteSecondaryAction()
{
	MainHandItem->ExecuteSecondaryAction();
}
