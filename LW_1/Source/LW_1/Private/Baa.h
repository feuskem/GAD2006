// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Baa.generated.h"


UCLASS()
class ABaa : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaa();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void  OnConstruction(const FTransform& Transform);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "COA")
		float Health;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
		float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
		float HealingRate;

	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
		float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
		bool Update;

	UFUNCTION(BlueprintCallable, Category = "COA")
		void SetHealth(float NewHealth);
	
	UFUNCTION(BlueprintPure, Category = "COA")
		float GetHealth();

	
		
private:

	

	
};
