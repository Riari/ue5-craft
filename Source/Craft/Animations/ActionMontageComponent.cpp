#include "ActionMontageComponent.h"

UAnimMontage* UActionMontageComponent::GetNextMontageByTag(FGameplayTag ActionTag)
{
	if (!ActionTag.IsValid())
	{
		return nullptr;
	}

	FMontageSet* Found = MontagesByTag.Find(ActionTag);
	if (!Found)
	{
		return nullptr;
	}

	return GetNextMontageInternal(*Found);
}

UAnimMontage* UActionMontageComponent::GetNextMontageInternal(FMontageSet& List)
{
	const int32 Count = List.Montages.Num();
	if (Count <= 0)
	{
		return nullptr;
	}

	List.NextIndex = (Count > 0) ? (List.NextIndex % Count) : 0;

	UAnimMontage* Result = List.Montages.IsValidIndex(List.NextIndex)
		? List.Montages[List.NextIndex].Get()
		: nullptr;

	List.NextIndex = (List.NextIndex + 1) % Count;
	return Result;
}
