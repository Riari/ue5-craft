#include "DroppedItem.h"

#include "ItemDataSubsystem.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Craft/CraftCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ADroppedItem::ADroppedItem()
	: CollisionCapsule{CreateDefaultSubobject<UCapsuleComponent>("CollisionCapsule")}
{
	SetRootComponent(CollisionCapsule);
	bReplicates = true;
}

void ADroppedItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SpawnItemFromDefinition();
		OnActorBeginOverlap.AddDynamic(this, &ADroppedItem::OnBeginOverlap);
	}
}

void ADroppedItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (SequenceActor)
	{
		SequenceActor->Destroy();
	}
}

void ADroppedItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADroppedItem, Item);
}

void ADroppedItem::SpawnItemFromDefinition()
{
	if (!ItemDefinition) return;

	UWorld* World = GetWorld();
	if (!World) return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Params.Owner = this;
	Params.Instigator = GetInstigator();

	Item = World->SpawnActor<ABaseItem>(
		ItemDefinition->ActorClass,
		GetActorLocation(),
		GetActorRotation(),
		Params
	);

	check(Item);
}

void ADroppedItem::ApplyLevelSequence()
{
	check(LevelSequence);
    
	FMovieSceneSequencePlaybackSettings Settings;
	Settings.LoopCount.Value = -1;
	Settings.PlayRate = 1.0f;
	Settings.bAutoPlay = false;
    
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		LevelSequence,
		Settings,
		SequenceActor
	);
    
	if (SequencePlayer)
	{
		SequenceActor->SetBindingByTag(FName("Item"), { Item });
		SequencePlayer->Play();
	}
}

void ADroppedItem::SpawnNiagaraSystem()
{
	check(NiagaraSystem);

	NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NiagaraSystem,
		RootComponent,
		NAME_None, 
		FVector::ZeroVector,
		NiagaraRotation, 
		EAttachLocation::KeepRelativeOffset, 
		true);

	UItemDataSubsystem* ItemDataSubsystem = GetGameInstance()->GetSubsystem<UItemDataSubsystem>();
	FLinearColor ItemColour;
	ItemDataSubsystem->GetItemRarityColour(ItemDefinition->Rarity, ItemColour);
	NiagaraComponent->SetVariableLinearColor(TEXT("Color"), ItemColour);
}

void ADroppedItem::OnRep_Item()
{
	ApplyLevelSequence();
	SpawnNiagaraSystem();
}

void ADroppedItem::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ACraftCharacter* CraftCharacter = Cast<ACraftCharacter>(OtherActor))
	{
		CraftCharacter->OnItemPickUp(Item);
		Destroy();
	}
}
