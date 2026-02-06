#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "ItemRarity.h"
#include "ItemDataSubsystem.generated.h"

UCLASS(Blueprintable)
class CRAFT_API UItemDataSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

    UFUNCTION(BlueprintCallable, Category = "Item Rarity")
    bool GetItemRarityColour(EItemRarity Rarity, FLinearColor& OutColour) const;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "DataTable")
    TSoftObjectPtr<UDataTable> ItemRarityColourTable;

private:
    void BuildCache();
    UDataTable* LoadedTable = nullptr;
    TMap<EItemRarity, FLinearColor> ItemRarityColourCache;
};
