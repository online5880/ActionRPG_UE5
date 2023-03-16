// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class UWidgetComponent;
class UAttributeComponent;
UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
protected:
	virtual void BeginPlay() override;

	/**
	 * Play montage functions
	 */
	void PlayHitReactMontage(const FName& SectionName);

	void DirectionalHitReact(const FVector& ImpactPoint);
private:

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attribute;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HealthBarWidget;
	
	/**
	 * Animation montages
	 */
	UPROPERTY(EditDefaultsOnly,Category= "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere,Category= "Sounds")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	UParticleSystem* HitParticles;
	

};
