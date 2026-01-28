#include "EquippableItem.h"

#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Craft/CraftCharacter.h"
#include "Craft/Abilities/BaseGameplayAbility.h"
#include "Craft/Interfaces/Hittable.h"
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

bool AEquippableItem::TryEquip(ACraftCharacter* CraftCharacter)
{
	SetActorHiddenInGame(false);

	if (UAbilitySystemComponent* ASC = CraftCharacter->GetAbilitySystemComponent())
	{
		for (TSubclassOf<UBaseGameplayAbility>& Ability : Abilities)
		{
			FGameplayAbilitySpec Spec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->GetAbilityInputID()), this);
			FGameplayAbilitySpecHandle AbilityHandle = ASC->GiveAbility(Spec);
			GrantedAbilities.Add(AbilityHandle);
		}
	}

	StaticMesh->AttachToComponent(CraftCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Socket_HandR"));
	Character = CraftCharacter;

	USkeletalMeshComponent* Mesh = Character ? Character->GetMesh() : nullptr;
	UAnimInstance* AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;
	
	ensureMsgf(AnimInstance, TEXT("Character %s does not have an AnimInstance"), *GetNameSafe(Character));
	
	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AEquippableItem::OnMontageNotifyBegin);
	
	return true;
}

void AEquippableItem::Unequip(ACraftCharacter* CraftCharacter)
{
	SetActorHiddenInGame(true);

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
	if (OtherActor->Implements<UHittable>())
	{
		FGameplayTagQuery ValidItemTagQuery = IHittable::Execute_GetValidItemTagQuery(OtherActor);
		int32 MinimumValidItemGrade = IHittable::Execute_GetMinimumValidItemGrade(OtherActor);

		if (ValidItemTagQuery.Matches(Definition->Tags) && Definition->Grade >= MinimumValidItemGrade)
		{
			OnHit(OtherActor);
		}
	}

	HitSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEquippableItem::OnHit(AActor* OtherActor)
{
	DrawDebugSphere(GetWorld(), HitSphere->GetComponentLocation(), 10, 12, FColor::Green, false, 1.0f);
			
	IHittable::Execute_OnHit(OtherActor);

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
}

void AEquippableItem::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == "CollisionEnd")
	{
		HitSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEquippableItem::ExecutePrimaryAction()
{
	OnExecutePrimaryAction.ExecuteIfBound(PrimaryActionMontage);
	HitSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AEquippableItem::ExecuteSecondaryAction()
{
	OnExecuteSecondaryAction.ExecuteIfBound(SecondaryActionMontage);
}
