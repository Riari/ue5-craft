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

void ABaseItem::OnEquip(ACraftCharacter* Character)
{
	// no-op
}

void ABaseItem::OnUnequip(ACraftCharacter* Character)
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


