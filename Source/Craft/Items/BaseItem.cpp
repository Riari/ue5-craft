#include "BaseItem.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseItem::SetDefinition(UItemDefinition* ItemDefinition)
{
	Definition = ItemDefinition;
}

UItemDefinition* ABaseItem::GetDefinition() const
{
	return Definition;
}

bool ABaseItem::TryEquip(class ACraftCharacter* Character)
{
	return false;
}

void ABaseItem::Unequip(class ACraftCharacter* Character)
{
	// no-op
}

void ABaseItem::ExecutePrimaryAction()
{
	// no-op
}

void ABaseItem::ExecuteSecondaryAction()
{
	// no-op
}


