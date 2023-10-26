// Fill out your copyright notice in the Description page of Project Settings.


#include "Baa_Avatar.h"

ABaa_Avatar::ABaa_Avatar():
	RunSpeed(600.0f),
	MaxStamina(100.0f),
	StaminaGainRate(15.0f),
	StaminaDrainRate(20.0f)
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetupAttachment(RootComponent);


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritYaw = true;
	Camera->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = false;

}

void ABaa_Avatar::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->bOrientRotationToMovement = true;
	RunReleased();

	Stamina = MaxStamina;

	HoldingRunKey = false;
	StaminaDrained = false;


}

void ABaa_Avatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Health = FMath::Min(MaxHealth, Health + HealingRate * DeltaTime);

	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking) {
		if (StaminaDrained)
		{
			Stamina = FMath::Min(MaxStamina, StaminaGainRate * DeltaTime + Stamina);

			if (MaxStamina == Stamina)
			{
				StaminaDrained = false;
				UpdateMovementParams();
			}
		}
		else
		{
			if (HoldingRunKey)
			{
				if (!GetCharacterMovement()->Velocity.IsNearlyZero(0.01f))
				{


					Stamina = FMath::Max(Stamina - StaminaDrainRate * DeltaTime, 0.0f);

					if (Stamina == 0.0f)
					{
						StaminaDrained = true;
						UpdateMovementParams();
					}
				}
				else {
					Stamina = FMath::Min(MaxStamina, Stamina + StaminaGainRate * DeltaTime);

					if (MaxStamina == Stamina)
					{
						StaminaDrained = false;
						UpdateMovementParams();
					}

				}
			}
			else
			{
				Stamina = FMath::Min(MaxStamina, Stamina + StaminaGainRate * DeltaTime);

				if (MaxStamina == Stamina)
				{
					StaminaDrained = false;
					UpdateMovementParams();
				}
			}
		}
	}

	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow,
		FString::Printf(TEXT("Stamina :  %f"), Stamina));

}

void ABaa_Avatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);


	PlayerInputComponent->BindAxis("MoveForward", this, &ABaa_Avatar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaa_Avatar::MoveRight);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ABaa_Avatar::RunPressed);

	PlayerInputComponent->BindAction("Run", IE_Released, this, &ABaa_Avatar::RunReleased);


}

void ABaa_Avatar::MoveForward(float Amount)
{
	FRotator ControlRotation = GetControlRotation();
	FVector ForwardDirection = FRotationMatrix(FRotator(0.0f, ControlRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::X);

	AddMovementInput(ForwardDirection, Amount);
}

void ABaa_Avatar::MoveRight(float Amount)
{
	FRotator ControlRotation = GetControlRotation();
	FVector RightDirection = FRotationMatrix(FRotator(0.0f, ControlRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::Y);

	AddMovementInput(RightDirection, Amount);

}

void ABaa_Avatar::RunPressed()
{


	HoldingRunKey = true;
	UpdateMovementParams();

}

void ABaa_Avatar::RunReleased()
{

	HoldingRunKey = false;
	UpdateMovementParams();

}

void ABaa_Avatar::UpdateMovementParams()
{
	if (!StaminaDrained && HoldingRunKey)
	{

		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}

	else
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
