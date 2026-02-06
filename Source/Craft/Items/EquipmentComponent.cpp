#include "EquipmentComponent.h"

#include "Craft/CraftCharacter.h"
#include "Craft/CraftPlayerState.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::Initialize()
{
	ACraftPlayerState* PS = Cast<ACraftPlayerState>(GetOwner());
	check(PS);
	Character = Cast<ACraftCharacter>(PS->GetPawn());
	check(Character);
}

bool UEquipmentComponent::TryEquipMainHandItem(ABaseItem* Item)
{
	check(Item);

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
