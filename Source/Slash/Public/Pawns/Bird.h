// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bird.generated.h"

struct FInputActionValue;
UCLASS()
class SLASH_API ABird : public APawn
{
	GENERATED_BODY()

public:
	ABird();
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* BirdMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BirdMesh;
	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* ViewCamera;
};
