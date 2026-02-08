#include "IHittable.h"

#include "Craft/Items/EquippableItem.h"

bool IHittable::CanBeHitWith_Default(const UObject* Hittable, const AEquippableItem* Item)
{
	FGameplayTagQuery ValidItemTagQuery = Execute_GetValidItemTagQuery(Hittable);
	int32 MinimumValidItemGrade = Execute_GetMinimumValidItemGrade(Hittable);
	UItemDefinition* ItemDefinition = Item->GetDefinition();

	return ValidItemTagQuery.Matches(ItemDefinition->Tags) && ItemDefinition->Grade >= MinimumValidItemGrade;
}
