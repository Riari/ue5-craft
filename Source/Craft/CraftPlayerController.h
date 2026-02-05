#pragma once

#include "CoreMinimal.h"
#include "CraftPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "CraftPlayerController.generated.h"

UCLASS(Blueprintable)
class CRAFT_API ACraftPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void OnRep_PlayerState() override;

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (AllowPrivateAccess = true))
	void BP_Client_OnPlayerStateReady(ACraftPlayerState* PS);
};
