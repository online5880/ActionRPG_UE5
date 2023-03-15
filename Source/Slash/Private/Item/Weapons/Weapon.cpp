// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapons/Weapon.h"

#include "NiagaraComponent.h"
#include "Characters/SlashCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Wepaon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this,&AWeapon::OnBoxOverlap);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	
	BoxTraceEnd= CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::Equip(USceneComponent* InParents, FName InSocketName)
{
	AttachMeshToSocket(InParents, InSocketName);
	ItemState = EItemState::EIS_Equipeped;
	if(EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquipSound,
			GetActorLocation()
			);
	}
	if(Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if(EmbersEffect)
	{
		EmbersEffect->Deactivate();
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParents, FName InSocketName)
{
	const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget,true);
	ItemMesh->AttachToComponent(InParents,TransformRules,InSocketName);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}


void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == this) {return;}
	
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();
	
	IgnoreActors.Add(this);
	
	FHitResult BoxHit;
	
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		BoxHit,
		true
		);
	
	if(BoxHit.GetActor())
	{
		GEngine->AddOnScreenDebugMessage(1,5.f,FColor::Orange,FString::Printf(TEXT("%s"),*BoxHit.GetActor()->GetName()));

		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if(HitInterface)
		{
			HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
		}
		IgnoreActors.AddUnique(BoxHit.GetActor());
		CreateFields(BoxHit.ImpactPoint);
	}
}

void AWeapon::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}