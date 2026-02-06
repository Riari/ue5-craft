#include "ItemDataSubsystem.h"

#include "ItemRarityColourTableRow.h"
#include "Engine/Engine.h"

void UItemDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    LoadedTable = nullptr;
    ItemRarityColourCache.Empty();

    if (!ItemRarityColourTable.IsNull())
    {
        LoadedTable = ItemRarityColourTable.LoadSynchronous();
    }

    if (LoadedTable)
    {
        BuildCache();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UItemDataSubsystem: ColourTable not set or failed to load."));
    }
}

bool UItemDataSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    return this->GetClass()->IsInBlueprint() && Super::ShouldCreateSubsystem(Outer);
}

void UItemDataSubsystem::BuildCache()
{
    ItemRarityColourCache.Empty();
    if (!LoadedTable) return;

    static const FString Context(TEXT("UItemDataSubsystem::BuildCache"));
    TArray<FName> RowNames = LoadedTable->GetRowNames();

    for (const FName& RowName : RowNames)
    {
        if (FItemRarityColourTableRow* Row = LoadedTable->FindRow<FItemRarityColourTableRow>(RowName, Context))
        {
            ItemRarityColourCache.Add(Row->Rarity, Row->Colour);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("UItemDataSubsystem: Cached %d rarity colours."), ItemRarityColourCache.Num());
}

bool UItemDataSubsystem::GetItemRarityColour(EItemRarity Rarity, FLinearColor& OutColour) const
{
    const FLinearColor* Found = ItemRarityColourCache.Find(Rarity);
    if (Found)
    {
        OutColour = *Found;
        return true;
    }
    return false;
}
