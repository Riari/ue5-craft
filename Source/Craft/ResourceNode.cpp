#include "ResourceNode.h"

#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Abilities/HealthAttributeSet.h"
#include "Kismet/GameplayStatics.h"

AResourceNode::AResourceNode()
	: AbilitySystemComponent{CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"))}
	, HealthAttributeSet{CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"))}
{
	PrimaryActorTick.bCanEverTick = false;
}

void AResourceNode::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystemComponent) return;

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
}

UAbilitySystemComponent* AResourceNode::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AResourceNode::InitializeAttributes()
{
	if (!AbilitySystemComponent) return;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthAttributeChanged);
}

void AResourceNode::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0.0f)
	{
		if (DestroySound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				DestroySound,
				GetActorLocation()
			);
		}

		if (DestroyParticles)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				DestroyParticles,
				GetActorLocation()
			);
		}

		Destroy();
	}
}
