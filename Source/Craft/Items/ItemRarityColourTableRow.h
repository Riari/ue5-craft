#pragma once

#include "CoreMinimal.h"
#include "ItemDefinition.h"
#include "Engine/DataTable.h"
#include "ItemRarityColourTableRow.generated.h"

USTRUCT(BlueprintType)
struct FItemRarityColourTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Colour;
};
