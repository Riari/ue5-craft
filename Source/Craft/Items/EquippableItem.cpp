#include "EquippableItem.h"

#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Craft/CraftCharacter.h"
#include "Craft/Abilities/BaseGameplayAbility.h"
#include "Craft/Interfaces/Harvestable.h"
#include "Kismet/GameplayStatics.h"

AEquippableItem::AEquippableItem()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	StaticMesh->CanCharacterStepUpOn = ECB_No;
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	SetRootComponent(StaticMesh);

	HitSphere = CreateDefaultSubobject<USphereComponent>(FName("HitSphere"));
	HitSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitSphere->SetupAttachment(StaticMesh);
}

void AEquippableItem::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AEquippableItem::OnBeginOverlap);
}

void AEquippableItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnActorEndOverlap.RemoveAll(this);
}

void AEquippableItem::Equip(ACraftCharacter* CraftCharacter)
{
	if (UAbilitySystemComponent* ASC = CraftCharacter->GetAbilitySystemComponent())
	{
		for (TSubclassOf<UBaseGameplayAbility>& Ability : Abilities)
		{
			FGameplayAbilitySpec Spec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->GetAbilityInputID()), this);
			FGameplayAbilitySpecHandle AbilityHandle = ASC->GiveAbility(Spec);
			GrantedAbilities.Add(AbilityHandle);
		}
	}

	StaticMesh->AttachToComponent(CraftCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Socket_HandR"));
	Character = CraftCharacter;

	Character->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AEquippableItem::OnMontageNotifyBegin);
}

void AEquippableItem::Unequip(ACraftCharacter* CraftCharacter)
{
	if (UAbilitySystemComponent* ASC = CraftCharacter->GetAbilitySystemComponent())
	{
		for (FGameplayAbilitySpecHandle& AbilityHandle : GrantedAbilities)
		{
			ASC->ClearAbility(AbilityHandle);
		}

		GrantedAbilities.Reset();
	}

	Character->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.RemoveAll(this);
	Character = nullptr;
}

void AEquippableItem::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	bool bValidHit = false;
	if (OtherActor->Implements<UHarvestable>())
	{
		EItemType RequiredToolType = IHarvestable::Execute_GetRequiredToolType(OtherActor);

		if (RequiredToolType == Definition->Type)
		{
			DrawDebugSphere(GetWorld(), HitSphere->GetComponentLocation(), 10, 12, FColor::Green, false, 1.0f);
			bValidHit = true;
		}
		else
		{
			// TODO: Notify the player?
		}
	}

	if (bValidHit)
	{
		if (OtherActor->Implements<UAbilitySystemInterface>())
		{
			UAbilitySystemComponent* OtherASC = OtherActor->FindComponentByClass<UAbilitySystemComponent>();
			if (OtherASC)
			{
				for (TSubclassOf<UGameplayEffect>& HitEffectClass : HitEffects)
				{
					FGameplayEffectContextHandle EffectContext = OtherASC->MakeEffectContext();
					FGameplayEffectSpecHandle SpecHandle = OtherASC->MakeOutgoingSpec(HitEffectClass, 1.0f, EffectContext);
	 
					if (SpecHandle.IsValid())
					{
						OtherASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					}
				}
			}
		}

		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				HitSound,
				HitSphere->GetComponentLocation()
			);
		}

		if (HitParticles)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				HitParticles,
				HitSphere->GetComponentLocation(),
				Character->GetActorRotation()
			);
		}

		if (HitCameraShakeClass)
		{
			Character->GetLocalViewingPlayerController()->ClientStartCameraShake(
				HitCameraShakeClass,
				1.0f,
				ECameraShakePlaySpace::CameraLocal,
				FRotator::ZeroRotator
			);
		}
	}

	HitSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEquippableItem::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == "CollisionEnd")
	{
		HitSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEquippableItem::ExecutePrimaryAction_Implementation()
{
	OnExecutePrimaryAction.ExecuteIfBound(PrimaryActionMontage);
	HitSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AEquippableItem::ExecuteSecondaryAction_Implementation()
{
	OnExecuteSecondaryAction.ExecuteIfBound(SecondaryActionMontage);
}
