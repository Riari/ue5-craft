#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ActionMontageComponent.generated.h"

USTRUCT(BlueprintType)
struct FMontageSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionMontages")
	TArray<TObjectPtr<UAnimMontage>> Montages;

	int32 NextIndex = 0;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRAFT_API UActionMontageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionMontages", meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, FMontageSet> MontagesByTag;

	UFUNCTION(BlueprintCallable, Category = "ActionMontages")
	UAnimMontage* GetNextMontageByTag(FGameplayTag ActionTag);
	
protected:
	UAnimMontage* GetNextMontageInternal(FMontageSet& List);
};
