// Fill out your copyright notice in the Description page of Project Settings.


#include "Baa.h"

// Sets default values
ABaa::ABaa() :
	MaxHealth(100.0f),
	Health(0.0f),
	HealingRate(10.0f),
	WalkSpeed(300.0f)
	


{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	




}

// Called when the game starts or when spawned
void ABaa::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	
}

void ABaa::OnConstruction(const FTransform& Transform)
{
	Health = MaxHealth;
	Update = false;
}


// Called every frame
void ABaa::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Health = FMath::Min(MaxHealth, Health + HealingRate * DeltaTime);

	

}

// Called to bind functionality to input


void ABaa::SetHealth(float NewHealth)
{
	Health = NewHealth;
}

float ABaa::GetHealth()
{
	return Health;
}








