#include "EquipmentComponent.h"

#include "Craft/CraftCharacter.h"
#include "Craft/CraftPlayerState.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UEquipmentComponent::TryEquipMainHandItem(ABaseItem* Item)
{
	check(Item);

	Equip(Item);
	MainHandItem = Item;
	return true;
}

void UEquipmentComponent::UnequipMainHandItem()
{
	if (MainHandItem)
	{
		Unequip(MainHandItem);
		MainHandItem = nullptr;
	}
}

void UEquipmentComponent::Server_TryEquip_Implementation(ABaseItem* Item)
{
}

void UEquipmentComponent::Server_Unequip_Implementation(ABaseItem* Item)
{
}

void UEquipmentComponent::Equip(ABaseItem* Item)
{
	Item->OnEquip(GetCharacter());
}

void UEquipmentComponent::Unequip(ABaseItem* Item)
{

	Item->OnUnequip(GetCharacter());
}

void UEquipmentComponent::ExecutePrimaryAction()
{
	MainHandItem->ExecutePrimaryAction();
}

void UEquipmentComponent::ExecuteSecondaryAction()
{
	MainHandItem->ExecuteSecondaryAction();
}

ACraftCharacter* UEquipmentComponent::GetCharacter() const
{
	// TODO: Cache this during initialisation (note - BeginPlay seems to be too early)
	ACraftPlayerState* PS = Cast<ACraftPlayerState>(GetOwner());
	check(PS);
	ACraftCharacter* Character = Cast<ACraftCharacter>(PS->GetPawn());
	check(Character);
	return Character;
}
