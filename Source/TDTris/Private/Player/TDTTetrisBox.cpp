// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TDTTetrisBox.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Figures/TDTBaseFigure.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ATDTTetrisBox::ATDTTetrisBox()
{
    PrimaryActorTick.bCanEverTick = true;

    GridZeroPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GridComponent"));
    RootComponent = GridZeroPoint;

    CameraPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CameraPoint"));
    CameraPoint->SetupAttachment(GridZeroPoint);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(CameraPoint);
    SpringArm->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    Camera->SetupAttachment(SpringArm);

    FigureSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
    FigureSpawnPoint->SetupAttachment(GridZeroPoint);
}

void ATDTTetrisBox::BeginPlay()
{
    Super::BeginPlay();

    GenerateGrid(BoxSizeX, BoxSizeY, BoxSizeZ, BoxTileSize);
    CameraPoint->SetRelativeLocation(FVector(BoxSizeX / 2 * BoxTileSize - BoxTileSize / 2, BoxSizeY / 2 * BoxTileSize - BoxTileSize / 2,
        BoxSizeZ / 2 * BoxTileSize - BoxTileSize / 2));

    FigureSpawnPoint->SetRelativeLocation(FVector(BoxSizeX / 2 * BoxTileSize,
        BoxSizeY / 2 * BoxTileSize, BoxSizeZ * BoxTileSize));

    SpawnFigure();

    GetWorldTimerManager().SetTimer(AutoFallTimerHandle, this, &ATDTTetrisBox::MoveDown, 1.f, true);
}

void ATDTTetrisBox::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATDTTetrisBox::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("LookUp", this, &ATDTTetrisBox::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ATDTTetrisBox::AddControllerYawInput);
    PlayerInputComponent->BindAction("MoveUp", IE_Pressed, this, &ATDTTetrisBox::MoveForward);
    PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ATDTTetrisBox::MoveBackward);
    PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ATDTTetrisBox::MoveRight);
    PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ATDTTetrisBox::MoveLeft);
    PlayerInputComponent->BindAction("RotateRight", IE_Pressed, this, &ATDTTetrisBox::RotateRight);
    PlayerInputComponent->BindAction("RotateLeft", IE_Pressed, this, &ATDTTetrisBox::RotateLeft);
    PlayerInputComponent->BindAction("RotateFront", IE_Pressed, this, &ATDTTetrisBox::RotateFront);
    PlayerInputComponent->BindAction("RotateBack", IE_Pressed, this, &ATDTTetrisBox::RotateBack);
    PlayerInputComponent->BindAction("FallDown", IE_Pressed, this, &ATDTTetrisBox::EnableFallDown);
    PlayerInputComponent->BindAction("FallDown", IE_Released, this, &ATDTTetrisBox::DisableFallDown);
}

void ATDTTetrisBox::SpawnFigure()
{
    CurrentFigure = GetWorld()->SpawnActor<ATDTBaseFigure>(FigureClass, FigureSpawnPoint->GetComponentTransform());
}

void ATDTTetrisBox::GenerateGrid(int32 SizeX, int32 SizeY, int32 SizeZ, float TileSize)
{
    UE_LOG(LogTemp, Warning, TEXT("Spheres Created in %s"), *GetName());

    for (int32 Z = 0; Z <= SizeZ; Z++)
    {
        for (int32 Y = 0; Y < SizeY; Y++)
        {
            for (int32 X = 0; X < SizeX; X++)
            {
                GridTiles.Add(FVector(X, Y, Z), nullptr);
            }
        }
    }

    for (int32 X = 0; X < BoxSizeX; X++)
    {
        for (int32 Y = 0; Y < BoxSizeY; Y++)
        {
            FVector WorldLoc = GridIndexToWorldLoc(FVector(X, Y, 0));

            DrawDebugBox(GetWorld(),                                        //
                FVector(WorldLoc.X, WorldLoc.Y, WorldLoc.Z - TileSize / 2), //
                FVector(TileSize / 2, TileSize / 2, 0),                     //
                FColor::Black,                                              //
                true, -1, 0, 10);
        }
    }

    // FVector WorldLoc = GridIndexToWorldLoc(FVector(BoxSizeX / 2, BoxSizeY / 2, BoxSizeZ / 2));

    DrawDebugBox(GetWorld(),                                                                    //
        FVector(-TileSize / 2, IndexToFloat(BoxSizeY - 1) / 2, IndexToFloat(BoxSizeZ - 1) / 2), //
        FVector(0, IndexToFloat(BoxSizeY) / 2, IndexToFloat(BoxSizeZ) / 2),                     //
        FColor::Red,                                                                            //
        true, -1, 0, 10);

    DrawDebugBox(GetWorld(),                                                                                                //
        FVector(IndexToFloat(BoxSizeX - 1) + TileSize / 2, IndexToFloat(BoxSizeY - 1) / 2, IndexToFloat(BoxSizeZ - 1) / 2), //
        FVector(0, IndexToFloat(BoxSizeY) / 2, IndexToFloat(BoxSizeZ) / 2),                                                 //
        FColor::Red,                                                                                                        //
        true, -1, 0, 10);

    DrawDebugBox(GetWorld(),                                                                    //
        FVector(IndexToFloat(BoxSizeX - 1) / 2, -TileSize / 2, IndexToFloat(BoxSizeZ - 1) / 2), //
        FVector(IndexToFloat(BoxSizeX) / 2, 0, IndexToFloat(BoxSizeZ) / 2),                     //
        FColor::Green,                                                                          //
        true, -1, 0, 10);

    DrawDebugBox(GetWorld(),                                                                                                //
        FVector(IndexToFloat(BoxSizeX - 1) / 2, IndexToFloat(BoxSizeY - 1) + TileSize / 2, IndexToFloat(BoxSizeZ - 1) / 2), //
        FVector(IndexToFloat(BoxSizeX) / 2, 0, IndexToFloat(BoxSizeZ) / 2),                                                 //
        FColor::Green,                                                                                                      //
        true, -1, 0, 10);

    //     FVector TileCenter = GridIndexToWorldLoc(Tile.Key);
    //
    //     if (Tile.Key.X == 0)
    //     {
    //         DrawDebugBox(GetWorld(),                                              //
    //             FVector(TileCenter.X - TileSize / 2, TileCenter.Y, TileCenter.Z), //
    //             FVector(0, TileSize / 2, TileSize / 2),                           //
    //             FColor::Red,                                                      //
    //             true, -1, 0, 10);
    //     }
    //     if (Tile.Key.Y == 0)
    //     {
    //         DrawDebugBox(GetWorld(),                                              //
    //             FVector(TileCenter.X, TileCenter.Y - TileSize / 2, TileCenter.Z), //
    //             FVector(TileSize / 2, 0, TileSize / 2),                           //
    //             FColor::Blue,                                                     //
    //             true, -1, 0, 10);
    //     }
    //     if (Tile.Key.Z == 0)
    //     {
    //     }
    //     if (Tile.Key.X == SizeX - 1)
    //     {
    //         DrawDebugBox(GetWorld(),                                              //
    //             FVector(TileCenter.X + TileSize / 2, TileCenter.Y, TileCenter.Z), //
    //             FVector(0, TileSize / 2, TileSize / 2),                           //
    //             FColor::Red,                                                      //
    //             true, -1, 0, 10);
    //     }
    //     if (Tile.Key.Y == SizeY - 1)
    //     {
    //         DrawDebugBox(GetWorld(),                                              //
    //             FVector(TileCenter.X, TileCenter.Y + TileSize / 2, TileCenter.Z), //
    //             FVector(TileSize / 2, 0, TileSize / 2),                           //
    //             FColor::Blue,                                                     //
    //             true, -1, 0, 10);
    //     }
    // }
}

FVector ATDTTetrisBox::GridIndexToWorldLoc(FVector GridIndex) const
{
    FVector GridLocation = GetActorLocation();

    float X = GridLocation.X + GridIndex.X * BoxTileSize;
    float Y = GridLocation.Y + GridIndex.Y * BoxTileSize;
    float Z = GridLocation.Z + GridIndex.Z * BoxTileSize;

    return FVector(X, Y, Z);
}

FVector ATDTTetrisBox::WorldLocToGridIndex(const FVector WorldLoc) const
{
    const FVector GridLoc = GetActorLocation();
    int32 X = FMath::RoundToInt((WorldLoc.X - GridLoc.X) / BoxTileSize);
    int32 Y = FMath::RoundToInt((WorldLoc.Y - GridLoc.Y) / BoxTileSize);
    int32 Z = FMath::RoundToInt((WorldLoc.Z - GridLoc.Z) / BoxTileSize);

    return FVector(X, Y, Z);
}

int32 ATDTTetrisBox::FloatToIndex(float Coord)
{
    return FMath::RoundToInt(Coord / BoxTileSize);
}

float ATDTTetrisBox::IndexToFloat(int32 Index)
{
    return Index * BoxTileSize;
}

bool ATDTTetrisBox::IsFloorFull(int32 FloorIndex)
{
    for (int32 X = 0; X < BoxSizeX; X++)
    {
        for (int32 Y = 0; Y < BoxSizeY; Y++)
        {
            if (!GridTiles[FVector(X * BoxTileSize, Y * BoxTileSize, FloorIndex * BoxTileSize)]) return false;
        }
    }
    return true;
}


void ATDTTetrisBox::MoveDown()
{
    if (!CurrentFigure) return;
    FVector CurLocation = CurrentFigure->GetActorLocation();
    FVector NewLocation = CurLocation;
    NewLocation.Z -= 100.f;

    CurrentFigure->SetActorLocation(NewLocation);

    if (!IsFigureInBounds() || GridTiles[WorldLocToGridIndex(NewLocation)] != nullptr)
    {
        CurrentFigure->SetActorLocation(CurLocation);
        LockFigure();
    }
}

void ATDTTetrisBox::EnableFallDown()
{
    IsFastFalling = true;
    GetWorldTimerManager().PauseTimer(AutoFallTimerHandle);
    GetWorldTimerManager().SetTimer(FallTimerHandle, this, &ATDTTetrisBox::MoveDown, 0.05f, true);
}

void ATDTTetrisBox::DisableFallDown()
{
    IsFastFalling = false;
    GetWorldTimerManager().ClearTimer(FallTimerHandle);
    GetWorldTimerManager().UnPauseTimer(AutoFallTimerHandle);
}

void ATDTTetrisBox::LockFigure()
{
    TArray<UStaticMeshComponent*> Blocks = GetFigureBlocks();
    for (auto Block : Blocks)
    {
        FVector GridIndex = WorldLocToGridIndex(Block->GetComponentLocation());
        if (!GridTiles.Contains(GridIndex))
        {
            GetWorldTimerManager().ClearTimer(AutoFallTimerHandle);
            UE_LOG(LogTemp, Warning, TEXT("--------Game Over!--------"));
            CurrentFigure = nullptr;
            DisableInput(GetWorld()->GetFirstPlayerController());

            for (auto Tile : GridTiles)
            {
                if (Tile.Value != nullptr)
                    UE_LOG(LogTemp, Warning, TEXT("\t%s\t%s"), *Tile.Key.ToString(), *Tile.Value->GetName());
            }
            //GameOver();
            return;
        }

        GridTiles.Emplace(GridIndex, Block);
    }
    CurrentFigure = nullptr;
    SpawnFigure();
}

TArray<UStaticMeshComponent*> ATDTTetrisBox::GetFigureBlocks()
{
    TArray<UStaticMeshComponent*> Blocks;
    CurrentFigure->GetComponents<UStaticMeshComponent>(Blocks);
    return Blocks;
}

FVector ATDTTetrisBox::GetCameraForwardVector()
{
    FVector CamVector = Camera->GetForwardVector();
    FVector AbsVector = CamVector.GetAbs();
    FVector NewUnitVector;
    NewUnitVector.X = UKismetMathLibrary::SignOfFloat(CamVector.X) * (AbsVector.X >= AbsVector.Y);
    NewUnitVector.Y = UKismetMathLibrary::SignOfFloat(CamVector.Y) * (AbsVector.X < AbsVector.Y);
    NewUnitVector.Z = 0;

    return NewUnitVector;
}

FVector ATDTTetrisBox::GetCameraRightVector()
{
    FVector CamVector = Camera->GetRightVector();
    FVector AbsVector = CamVector.GetAbs();
    FVector NewUnitVector;
    NewUnitVector.X = UKismetMathLibrary::SignOfFloat(CamVector.X) * (AbsVector.X >= AbsVector.Y);
    NewUnitVector.Y = UKismetMathLibrary::SignOfFloat(CamVector.Y) * (AbsVector.X < AbsVector.Y);
    NewUnitVector.Z = 0;

    return NewUnitVector;
}

bool ATDTTetrisBox::IsFigureInBounds()
{
    TArray<UStaticMeshComponent*> Blocks = GetFigureBlocks();

    for (auto Block : Blocks)
    {
        int32 X = FloatToIndex(Block->GetComponentLocation().X);
        int32 Y = FloatToIndex(Block->GetComponentLocation().Y);
        int32 Z = FloatToIndex(Block->GetComponentLocation().Z);

        // UE_LOG(LogTemp, Warning, TEXT("Info:\n\t%s\n\t%i\n\t%i\n\t%i\n\t%i"), *Block->GetName(), Blocks.Num(), X, Y, Z);

        if (X < 0 || X > BoxSizeX - 1 || Y < 0 || Y > BoxSizeY - 1 || Z < 0 || GridTiles.Contains(FVector(X, Y, Z)) &&
            GridTiles[FVector(X, Y, Z)] != nullptr)
        {
            return false;
        }
    }
    return true;
}

void ATDTTetrisBox::MoveForward()
{
    if (!CurrentFigure) return;

    FVector CameraVector = GetCameraForwardVector();
    FVector CurLocation = CurrentFigure->GetActorLocation();
    FVector NewLocation = CurLocation + CameraVector * 100.f;
    CurrentFigure->SetActorLocation(NewLocation);

    if (!IsFigureInBounds())
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorLocation(CurLocation);
    }
}

void ATDTTetrisBox::MoveBackward()
{
    if (!CurrentFigure) return;

    FVector CameraVector = GetCameraForwardVector();
    FVector CurLocation = CurrentFigure->GetActorLocation();
    FVector NewLocation = CurLocation + CameraVector * -100.f;
    CurrentFigure->SetActorLocation(NewLocation);

    if (!IsFigureInBounds())
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorLocation(CurLocation);
    }
}

void ATDTTetrisBox::MoveRight()
{
    if (!CurrentFigure) return;

    FVector CameraVector = GetCameraRightVector();
    FVector CurLocation = CurrentFigure->GetActorLocation();
    FVector NewLocation = CurLocation + CameraVector * 100.f;
    CurrentFigure->SetActorLocation(NewLocation);

    if (!IsFigureInBounds())
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorLocation(CurLocation);
    }
}

void ATDTTetrisBox::MoveLeft()
{
    if (!CurrentFigure) return;

    FVector CameraVector = GetCameraRightVector();
    FVector CurLocation = CurrentFigure->GetActorLocation();
    FVector NewLocation = CurLocation + CameraVector * -100.f;
    CurrentFigure->SetActorLocation(NewLocation);

    if (!IsFigureInBounds())
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorLocation(CurLocation);
    }
}

void ATDTTetrisBox::RotateFront()
{
    if (!CurrentFigure) return;

    FRotator CurRotation = CurrentFigure->GetActorRotation();
    FRotator NewRotation = CurRotation;
    NewRotation.Pitch += 90.f;
    CurrentFigure->SetActorRotation(NewRotation);

    if (!IsFigureInBounds())
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorRotation(CurRotation);
    }
}

void ATDTTetrisBox::RotateBack()
{
    if (!CurrentFigure) return;

    FRotator CurRotation = CurrentFigure->GetActorRotation();
    FRotator NewRotation = CurRotation;
    NewRotation.Pitch -= 90.f;
    CurrentFigure->SetActorRotation(NewRotation);

    if (!IsFigureInBounds())
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorRotation(CurRotation);
    }
}

void ATDTTetrisBox::RotateRight()
{
    if (!CurrentFigure) return;

    FRotator CurRotation = CurrentFigure->GetActorRotation();
    FRotator NewRotation = CurRotation;
    NewRotation.Roll += 90.f;
    CurrentFigure->SetActorRotation(NewRotation);

    if (!IsFigureInBounds())
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorRotation(CurRotation);
    }
}

void ATDTTetrisBox::RotateLeft()
{
    if (!CurrentFigure) return;

    FRotator CurRotation = CurrentFigure->GetActorRotation();
    FRotator NewRotation = CurRotation;
    NewRotation.Roll -= 90.f;
    CurrentFigure->SetActorRotation(NewRotation);

    if (!IsFigureInBounds())
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorRotation(CurRotation);
    }
}
