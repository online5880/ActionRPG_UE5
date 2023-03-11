// Fill out your copyright notice in the Description page of Project Settings.
#include "Slash/Public/Item.h"

#include "Characters/SlashCharacter.h"
#include "Components/SphereComponent.h"
#include "Slash/DebugMacro.h"

// Sets default values
AItem::AItem() :
	Amplitude(0.25f),
	TimeConstant(5.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	Sphere->OnComponentBeginOverlap.AddDynamic(this,&AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this,&AItem::OnSphereEndOverlap);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	if(ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f,0.f,TransformedSin()));
	}
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime*TimeConstant); // period = 2*pi/K
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime*TimeConstant); // period = 2*pi/K
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if(SlashCharacter)
	{
		SlashCharacter->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if(SlashCharacter)
	{
		SlashCharacter->SetOverlappingItem(nullptr);
	}
}
