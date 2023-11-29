// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGameManager.h"
#include "TilePlayerController.h"



// Sets default values
ATileGameManager::ATileGameManager():
	GridSize(100),
	GridOffset(0,0,0.5f),
	MapExtendsInGrids(25)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GridSelection = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	GridSelection->SetupAttachment(RootComponent);

	Selected_Tile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectedTile"));
	Selected_Tile->SetupAttachment(GridSelection);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		PlaneMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		GridMaterial(TEXT("/Script/Engine.Material'/Game/UI/MAT_GridSlot.MAT_GridSlot'"));

	GridSelection->SetStaticMesh(PlaneMesh.Object);
	GridSelection->SetMaterial(0, GridMaterial.Object);
	GridSelection->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Selected_Tile->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}



// Called when the game starts or when spawned
void ATileGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (auto PlayerController =
		Cast<ATilePlayerController>(GWorld->GetFirstPlayerController()))
	{
		PlayerController->GameManager = this;
		Selected_Tile->SetStaticMesh(TileTypes[CurrentTileIndex]->InstancedMesh->GetStaticMesh());
		Selected_Tile->SetRelativeScale3D(TileTypes[CurrentTileIndex]->InstancedMesh->GetRelativeScale3D());
	}
	
}

// Called every frame
void ATileGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileGameManager::OnActorInteraction(AActor* HitActor, FVector& Location, bool IsPressed)
{
	if (TileTypes.Num() == 0) return;
	
		FVector GridLoc = GridOffset;
		GridLoc.X += FMath::GridSnap(Location.X, GridSize);
		GridLoc.Y += FMath::GridSnap(Location.Y, GridSize);
		GridLoc.Z += Location.Z;

		UPlayerInput* Input = GWorld->GetFirstPlayerController()->PlayerInput;

		if (Input->WasJustPressed(EKeys::LeftMouseButton))
		{
			int GridX = GridLoc.X / GridSize + MapExtendsInGrids;
			int GridY = GridLoc.Y / GridSize + MapExtendsInGrids;

			if (GridX < 0 || GridY < 0 || GridX >= MapExtendsInGrids * 2 || GridY >= MapExtendsInGrids * 2)
			{
				return;
			}

			if (Map[GridX][GridY] != nullptr) return;

			if (TileTypes.IsValidIndex(CurrentTileIndex))
			{
				ATileBase* SelectedTile = TileTypes[CurrentTileIndex];
				Map[GridX][GridY] = SelectedTile;

				FTransform TileTransform(GridLoc + GridOffset);
				SelectedTile->InstancedMesh->AddInstance(
					Selected_Tile->GetRelativeTransform() * TileTransform,
					true);

				UE_LOG(LogTemp, Warning, TEXT("Hit: %s - %f, %f, %f"),
					HitActor ? *HitActor->GetActorLabel() : TEXT("None"),
					Location.X,
					Location.Y,
					Location.Z);

				Selected_Tile->SetStaticMesh(TileTypes[CurrentTileIndex]->InstancedMesh->GetStaticMesh());
				Selected_Tile->SetRelativeScale3D(TileTypes[CurrentTileIndex]->InstancedMesh->GetRelativeScale3D());
				SelectedTileRotation = SelectedTile->GetActorRotation();
				
			}

		}

		else if (Input->WasJustPressed(EKeys::MouseScrollDown))
		{
			CurrentTileIndex = (CurrentTileIndex + 1) % TileTypes.Num();
			UE_LOG(LogTemp, Warning, TEXT("TileType: %s"), *TileTypes[CurrentTileIndex]->GetActorLabel());

			Selected_Tile->SetStaticMesh(TileTypes[CurrentTileIndex]->InstancedMesh->GetStaticMesh());
			Selected_Tile->SetRelativeScale3D(TileTypes[CurrentTileIndex]->InstancedMesh->GetRelativeScale3D());

		}


		else if (Input->WasJustPressed(EKeys::MouseScrollUp))
		{
			if (CurrentTileIndex == 0)
			{
				CurrentTileIndex = TileTypes.Num() - 1;
			}
			else
			{
				CurrentTileIndex = (CurrentTileIndex - 1) % TileTypes.Num();
			}

			UE_LOG(LogTemp, Warning, TEXT("TileType: %s"), *TileTypes[CurrentTileIndex]->GetActorLabel());
			Selected_Tile->SetStaticMesh(TileTypes[CurrentTileIndex]->InstancedMesh->GetStaticMesh());
			Selected_Tile->SetRelativeScale3D(TileTypes[CurrentTileIndex]->InstancedMesh->GetRelativeScale3D());
			
		}
		else if (Input->WasJustPressed(EKeys::RightMouseButton))
		{

			SelectedTileRotation.Yaw += 90.0f;
			Selected_Tile->SetRelativeRotation(SelectedTileRotation);
		}
		else
		{
			GridSelection->SetWorldLocation(GridLoc + GridOffset);
			Selected_Tile->SetWorldLocation(GridSelection->GetComponentLocation());
		}
		

	
}

