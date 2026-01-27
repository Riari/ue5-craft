#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class CRAFT_API IDamageable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Damage")
	void ApplyDamage(float Damage, AActor* DamageCauser, AController* InstigatorController);
    
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Damage")
	bool IsDead() const;
};
