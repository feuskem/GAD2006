	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Baa.h"
#include "Baa_Avatar.generated.h"

/**
 * 
 */
UCLASS()
class ABaa_Avatar : public ABaa
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
public:
	ABaa_Avatar();

	virtual void Tick(float DeltaTime) override;

		UPROPERTY(EditAnywhere, Category = "COA")
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "COA")
		UCameraComponent* Camera;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
		float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
		float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
		float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
		float StaminaGainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
		float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
		bool StaminaDrained;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
		bool HoldingRunKey;

private:

	void MoveForward(float Amount);
	void MoveRight(float Amount);


	void RunPressed();
	void RunReleased();
	void UpdateMovementParams();
};
