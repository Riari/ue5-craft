#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AnimNotifiable.generated.h"

UINTERFACE(MinimalAPI)
class UAnimNotifiable : public UInterface
{
	GENERATED_BODY()
};

class CRAFT_API IAnimNotifiable
{
	GENERATED_BODY()

public:
	virtual void AnimNotify(FName NotifyName) = 0;
};
