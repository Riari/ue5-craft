#include "CraftPlayerController.h"

#include "CraftPlayerState.h"

void ACraftPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (ACraftPlayerState* PS = GetPlayerState<ACraftPlayerState>())
	{
		BP_Client_OnPlayerStateReady(PS);
	}
}