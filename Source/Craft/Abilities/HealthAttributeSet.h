#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.h"
#include "HealthAttributeSet.generated.h"

UCLASS(BlueprintType)
class CRAFT_API UHealthAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()

public:
	UHealthAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, HealthRegen);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Damage)

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

protected:
	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	 
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
