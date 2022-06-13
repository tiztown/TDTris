// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TDTTetrisBox.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Figures/TDTBaseFigure.h"
#include "Figures/TDTTemplateFigure.h"
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

    QueueSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("QueueSpawnPoint"));
    QueueSpawnPoint->SetupAttachment(GridZeroPoint);

    BoxMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMeshComponent"));
    BoxMeshComp->SetupAttachment(RootComponent);
}

void ATDTTetrisBox::BeginPlay()
{
    Super::BeginPlay();

    if (!BoxMeshComp) return;

    GenerateGrid(BoxSizeX, BoxSizeY, BoxSizeZ, BoxTileSize);

    BoxMeshComp->SetWorldScale3D(FVector(BoxSizeX, BoxSizeY, BoxSizeZ - 0.5));
    BoxMeshComp->SetRelativeLocation(FVector(BoxSizeX / 2 * BoxTileSize - BoxTileSize / 2, BoxSizeY / 2 * BoxTileSize - BoxTileSize / 2,
        BoxSizeZ / 2 * BoxTileSize - BoxTileSize * 0.75));

    CameraPoint->SetRelativeLocation(FVector(BoxSizeX / 2 * BoxTileSize - BoxTileSize / 2, BoxSizeY / 2 * BoxTileSize - BoxTileSize / 2,
        BoxSizeZ / 2 * BoxTileSize - BoxTileSize / 2));

    FigureSpawnPoint->SetRelativeLocation(FVector(BoxSizeX / 2 * BoxTileSize,
        BoxSizeY / 2 * BoxTileSize, BoxSizeZ * BoxTileSize));

    FVector QueueLocation = FigureSpawnPoint->GetComponentLocation() - Camera->GetComponentLocation();
    QueueLocation.Y += 1.5f * BoxSizeY * 100;
    QueueLocation.X += 1.5f * BoxSizeX * 100;
    QueueSpawnPoint->SetRelativeLocation(QueueLocation);

    //SpawnFigure();
    GenerateQueue(QueueFiguresNum);

    TemplateFigure = GetWorld()->SpawnActor<ATDTTemplateFigure>(TemplateFigureClass, FigureSpawnPoint->GetComponentTransform());

    MoveFigureToSpawn();

    StartPlay();
}

void ATDTTetrisBox::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
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

    // for (int32 X = 0; X < BoxSizeX; X++)
    // {
    //     for (int32 Y = 0; Y < BoxSizeY; Y++)
    //     {
    //         FVector WorldLoc = GridIndexToWorldLoc(FVector(X, Y, 0));
    //
    //         DrawDebugBox(GetWorld(),                                        //
    //             FVector(WorldLoc.X, WorldLoc.Y, WorldLoc.Z - TileSize / 2), //
    //             FVector(TileSize / 2, TileSize / 2, 0),                     //
    //             FColor::Black,                                              //
    //             true, -1, 0, 10);
    //     }
    // }
    //
    // // FVector WorldLoc = GridIndexToWorldLoc(FVector(BoxSizeX / 2, BoxSizeY / 2, BoxSizeZ / 2));
    //
    // DrawDebugBox(GetWorld(),                                                                    //
    //     FVector(-TileSize / 2, IndexToFloat(BoxSizeY - 1) / 2, IndexToFloat(BoxSizeZ - 1) / 2), //
    //     FVector(0, IndexToFloat(BoxSizeY) / 2, IndexToFloat(BoxSizeZ) / 2),                     //
    //     FColor::Red,                                                                            //
    //     true, -1, 0, 10);
    //
    // DrawDebugBox(GetWorld(),                                                                                                //
    //     FVector(IndexToFloat(BoxSizeX - 1) + TileSize / 2, IndexToFloat(BoxSizeY - 1) / 2, IndexToFloat(BoxSizeZ - 1) / 2), //
    //     FVector(0, IndexToFloat(BoxSizeY) / 2, IndexToFloat(BoxSizeZ) / 2),                                                 //
    //     FColor::Red,                                                                                                        //
    //     true, -1, 0, 10);
    //
    // DrawDebugBox(GetWorld(),                                                                    //
    //     FVector(IndexToFloat(BoxSizeX - 1) / 2, -TileSize / 2, IndexToFloat(BoxSizeZ - 1) / 2), //
    //     FVector(IndexToFloat(BoxSizeX) / 2, 0, IndexToFloat(BoxSizeZ) / 2),                     //
    //     FColor::Green,                                                                          //
    //     true, -1, 0, 10);
    //
    // DrawDebugBox(GetWorld(),                                                                                                //
    //     FVector(IndexToFloat(BoxSizeX - 1) / 2, IndexToFloat(BoxSizeY - 1) + TileSize / 2, IndexToFloat(BoxSizeZ - 1) / 2), //
    //     FVector(IndexToFloat(BoxSizeX) / 2, 0, IndexToFloat(BoxSizeZ) / 2),                                                 //
    //     FColor::Green,                                                                                                      //
    //     true, -1, 0, 10);
}

void ATDTTetrisBox::GenerateQueue(int32 FiguresNum)
{
    if (!QueueSpawnPoint) return;

    for (int32 I = 0; I < FiguresNum; I++)
    {
        FVector NewLocation = QueueSpawnPoint->GetComponentLocation();
        NewLocation.Z -= I * 500;
        ATDTPlayableFigure* NewFigure = GetWorld()->SpawnActor<ATDTPlayableFigure>(FigureClass, NewLocation, FRotator(0));
        FiguresQueue.Add(NewFigure);
        NewFigure->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);
    }
}

void ATDTTetrisBox::MoveUpQueueFigures()
{
    for (auto Figure : FiguresQueue)
    {
        Figure->AddActorLocalOffset(FVector(0.f, 0.f, 500.f));
    }
}

void ATDTTetrisBox::AddFigureToQueue()
{
    if (!FiguresQueue.Num()) return;

    FVector NewLocation = QueueSpawnPoint->GetComponentLocation();
    NewLocation.Z -= (FiguresQueue.Num()) * 500;
    ATDTPlayableFigure* NewFigure = GetWorld()->SpawnActor<ATDTPlayableFigure>(FigureClass, NewLocation, FRotator(0));
    FiguresQueue.Add(NewFigure);
    NewFigure->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);
}

// player input

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

void ATDTTetrisBox::MoveForward()
{
    if (!CurrentFigure) return;

    FVector CameraVector = GetCameraForwardVector();
    FVector CurLocation = CurrentFigure->GetActorLocation();
    FVector NewLocation = CurLocation + CameraVector * 100.f;
    CurrentFigure->SetActorLocation(NewLocation);

    if (!IsFigureInBounds(CurrentFigure))
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorLocation(CurLocation);
    }

    DropTemplate();
}

void ATDTTetrisBox::MoveBackward()
{
    if (!CurrentFigure) return;

    FVector CameraVector = GetCameraForwardVector();
    FVector CurLocation = CurrentFigure->GetActorLocation();
    FVector NewLocation = CurLocation + CameraVector * -100.f;
    CurrentFigure->SetActorLocation(NewLocation);

    if (!IsFigureInBounds(CurrentFigure))
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorLocation(CurLocation);
    }

    DropTemplate();
}

void ATDTTetrisBox::MoveRight()
{
    if (!CurrentFigure) return;

    FVector CameraVector = GetCameraRightVector();
    FVector CurLocation = CurrentFigure->GetActorLocation();
    FVector NewLocation = CurLocation + CameraVector * 100.f;
    CurrentFigure->SetActorLocation(NewLocation);

    if (!IsFigureInBounds(CurrentFigure))
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorLocation(CurLocation);
    }

    DropTemplate();
}

void ATDTTetrisBox::MoveLeft()
{
    if (!CurrentFigure) return;

    FVector CameraVector = GetCameraRightVector();
    FVector CurLocation = CurrentFigure->GetActorLocation();
    FVector NewLocation = CurLocation + CameraVector * -100.f;
    CurrentFigure->SetActorLocation(NewLocation);

    if (!IsFigureInBounds(CurrentFigure))
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorLocation(CurLocation);
    }

    DropTemplate();
}

void ATDTTetrisBox::RotateFront()
{
    if (!CurrentFigure) return;

    FRotator CurRotation = CurrentFigure->GetActorRotation();
    FRotator NewRotation = UKismetMathLibrary::RotatorFromAxisAndAngle(GetCameraRightVector(), 90.f);
    // NewRotation.Yaw += 90.f;

    CurrentFigure->AddActorWorldRotation(NewRotation);

    if (!IsFigureInBounds(CurrentFigure))
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorRotation(CurRotation);
    }

    DropTemplate();
}

void ATDTTetrisBox::RotateBack()
{
    if (!CurrentFigure) return;

    FRotator CurRotation = CurrentFigure->GetActorRotation();
    FRotator NewRotation = UKismetMathLibrary::RotatorFromAxisAndAngle(GetCameraRightVector(), -90.f);
    // NewRotation.Yaw += -90.f;

    CurrentFigure->AddActorWorldRotation(NewRotation);

    if (!IsFigureInBounds(CurrentFigure))
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorRotation(CurRotation);
    }

    DropTemplate();
}

void ATDTTetrisBox::RotateRight()
{
    if (!CurrentFigure) return;

    FRotator CurRotation = CurrentFigure->GetActorRotation();
    FRotator NewRotation = UKismetMathLibrary::RotatorFromAxisAndAngle(GetCameraForwardVector(), -90.f);
    // NewRotation.Roll += 90.f;

    CurrentFigure->AddActorWorldRotation(NewRotation);

    if (!IsFigureInBounds(CurrentFigure))
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorRotation(CurRotation);
    }

    DropTemplate();
}

void ATDTTetrisBox::RotateLeft()
{
    if (!CurrentFigure) return;

    FRotator CurRotation = CurrentFigure->GetActorRotation();
    FRotator NewRotation = UKismetMathLibrary::RotatorFromAxisAndAngle(GetCameraForwardVector(), 90.f);
    //NewRotation.Roll += -90.f;

    CurrentFigure->AddActorWorldRotation(NewRotation);

    if (!IsFigureInBounds(CurrentFigure))
    {
        // UE_LOG(LogTemp, Warning, TEXT("FigureMoveBack"));
        CurrentFigure->SetActorRotation(CurRotation);
    }

    DropTemplate();
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

// gameplay

void ATDTTetrisBox::SpawnFigure()
{
    CurrentFigure = GetWorld()->SpawnActor<ATDTPlayableFigure>(FigureClass, FigureSpawnPoint->GetComponentTransform());
}

void ATDTTetrisBox::PlaceTemplateFigure()
{
    TemplateFigure->SetTemplateFigureVisual(CurrentFigure);

    DropTemplate();
}

void ATDTTetrisBox::DropTemplate()
{
    if (!CurrentFigure) return;
    FTransform ParentTransform = CurrentFigure->GetActorTransform();
    TemplateFigure->SetActorTransform(ParentTransform);

    FVector PrevLocation = TemplateFigure->GetActorLocation();

    FVector NewLocation = PrevLocation;

    for (int NewZ = CurrentFigure->GetActorLocation().Z; NewZ >= 0; NewZ -= 100)
    {
        NewLocation.Z = NewZ;
        TemplateFigure->SetActorLocation(NewLocation);

        if (!IsFigureInBounds(TemplateFigure))
        {
            UE_LOG(LogTemp, Warning, TEXT("Figure %s OUT"), *CurrentFigure->GetName());

            TemplateFigure->SetActorLocation(PrevLocation);
            break;
        }

        PrevLocation = NewLocation;
    }
    UE_LOG(LogTemp, Warning, TEXT("Location of figure %s = %s"), *CurrentFigure->GetName(), *TemplateFigure->GetActorLocation().ToString());
}

void ATDTTetrisBox::MoveFigureToSpawn()
{
    if (!FiguresQueue.Num()) return;

    CurrentFigure = FiguresQueue[0];
    FiguresQueue.RemoveAt(0);
    CurrentFigure->SetActorLocation(FigureSpawnPoint->GetComponentLocation());
    CurrentFigure->AttachToComponent(FigureSpawnPoint, FAttachmentTransformRules::KeepWorldTransform);
    CurrentFigure->SetActorRotation(FRotator::ZeroRotator);

    MoveUpQueueFigures();

    AddFigureToQueue();

    if (TemplateFigure) PlaceTemplateFigure();
}

void ATDTTetrisBox::StartPlay()
{
    GetWorldTimerManager().SetTimer(AutoFallTimerHandle, this, &ATDTTetrisBox::MoveDown, InitialFallingSpeed, true);
}

void ATDTTetrisBox::MoveDown()
{
    if (!CurrentFigure) return;
    FVector CurLocation = CurrentFigure->GetActorLocation();
    FVector NewLocation = CurLocation;
    NewLocation.Z -= 100.f;

    CurrentFigure->SetActorLocation(NewLocation);

    if (!IsFigureInBounds(CurrentFigure)) //  || GridTiles[WorldLocToGridIndex(NewLocation)] != nullptr
    {
        CurrentFigure->SetActorLocation(CurLocation);
        LockFigure();
    }
}

void ATDTTetrisBox::LockFigure()
{
    TArray<UStaticMeshComponent*> Blocks = GetFigureBlocks(CurrentFigure);
    for (auto Block : Blocks)
    {
        FVector GridIndex = WorldLocToGridIndex(Block->GetComponentLocation());
        if (!GridTiles.Contains(GridIndex))
        {
            GameOver();
            return;
        }

        GridTiles.Emplace(GridIndex, Block);
    }

    for (auto Block : Blocks)
    {
        FVector GridIndex = WorldLocToGridIndex(Block->GetComponentLocation());

        GridIndex.Z -= 1;

        if (GridIndex.Z >= 0 && !GridTiles[GridIndex])
        {
            CurrentFigure->SetMaterialToBlock(Block, 0);
        }

        GridIndex.Z += 2;

        if (GridIndex.Z < BoxSizeZ && GridTiles[GridIndex])
        {
            ATDTPlayableFigure* Figure = Cast<ATDTPlayableFigure>(GridTiles[GridIndex]->GetOwner());
            if (!Figure) continue;

            Figure->SetMaterialToBlock(GridTiles[GridIndex], 1);
        }
    }

    TotalScore += EachFigureScore * InitialScoreMultiplier;

    CheckFloors();

    CheckAndDestroyFigures();

    CurrentFigure = nullptr;

    MoveFigureToSpawn();
}

void ATDTTetrisBox::CheckFloors()
{
    TSet<int32> FloorsIndexes;
    for (auto Block : GetFigureBlocks(CurrentFigure))
    {
        FVector CurrentTile = WorldLocToGridIndex(Block->GetComponentLocation());
        FloorsIndexes.Add(CurrentTile.Z);
    }

    for (int32 FloorIndex = FloorsIndexes.Num(); FloorIndex >= 0; FloorIndex--)
    {
        if (IsFloorFull(FloorIndex))
        {
            DestroyFloor(FloorIndex);
            MoveFloor(FloorIndex);
        }
    }
}

bool ATDTTetrisBox::IsFloorFull(int32 FloorIndex)
{
    if (!GridTiles.Contains(FVector(0, 0, FloorIndex))) return false;

    for (int32 X = 0; X < BoxSizeX; X++)
        for (int32 Y = 0; Y < BoxSizeY; Y++)
            if (!GridTiles[FVector(X, Y, FloorIndex)]) return false;

    return true;
}

void ATDTTetrisBox::DestroyFloor(int32 FloorIndex)
{
    for (int32 X = 0; X < BoxSizeX; X++)
    {
        for (int32 Y = 0; Y < BoxSizeY; Y++)
        {
            GridTiles[FVector(X, Y, FloorIndex)]->DestroyComponent();
            GridTiles[FVector(X, Y, FloorIndex)] = nullptr;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Floor destroyed!"));
    TotalScore += EachFloorScore * InitialScoreMultiplier;
    FloorsDestroyedCounter++;

    if (FloorsDestroyedCounter % LevelsToLevelUp == 0 && InitialScoreMultiplier < 3)
        LevelUp();

    FloorDestroyed = true;
}

void ATDTTetrisBox::MoveFloor(int32 FloorIndex)
{
    if (!FloorDestroyed) return;

    for (int32 Z = FloorIndex + 1; Z < BoxSizeZ; Z++)
    {
        for (int32 X = 0; X < BoxSizeX; X++)
        {
            for (int32 Y = 0; Y < BoxSizeY; Y++)
            {
                FVector Tile(X, Y, Z);
                // UE_LOG(LogTemp, Warning, TEXT("GridIndex : %s"), *Tile.ToCompactString());

                if (GridTiles[Tile] != nullptr)
                {
                    FVector OldLocation = Tile;

                    FVector NewLocation = OldLocation;
                    NewLocation.Z -= 1;
                    GridTiles[Tile]->SetWorldLocation(GridIndexToWorldLoc(NewLocation));
                    // UE_LOG(LogTemp, Warning, TEXT("New GridIndex : %s"), *NewLocation.ToString());

                    GridTiles.Emplace(WorldLocToGridIndex(GridTiles[Tile]->GetComponentLocation()), GridTiles[Tile]);
                    GridTiles.Emplace(OldLocation, nullptr);
                }
            }
        }
    }

    for (int32 Z = FloorIndex; Z < BoxSizeZ; Z++)
    {
        for (int32 X = 0; X < BoxSizeX; X++)
        {
            for (int32 Y = 0; Y < BoxSizeY; Y++)
            {
                FVector Tile(X, Y, Z);

                if (GridTiles[Tile] != nullptr)
                {
                    FVector LowerTile, UpperTile;
                    LowerTile = UpperTile = Tile;
                    LowerTile.Z -= 1;
                    UpperTile.Z += 1;

                    if (LowerTile.Z >= 0 && !GridTiles[LowerTile])
                    {
                        ATDTPlayableFigure* Figure = Cast<ATDTPlayableFigure>(GridTiles[Tile]->GetOwner());
                        if (!Figure) continue;

                        Figure->SetMaterialToBlock(GridTiles[Tile], 0);
                    }

                    if (UpperTile.Z < BoxSizeZ && GridTiles[UpperTile])
                    {
                        ATDTPlayableFigure* Figure = Cast<ATDTPlayableFigure>(GridTiles[UpperTile]->GetOwner());
                        if (!Figure) continue;

                        Figure->SetMaterialToBlock(GridTiles[UpperTile], 1);
                    }
                }
            }
        }
    }

    FloorDestroyed = false;
}

void ATDTTetrisBox::CheckAndDestroyFigures()
{
    TArray<AActor*> Figures;
    UGameplayStatics::GetAllActorsOfClass(this, FigureClass, Figures);

    for (auto FigureActor : Figures)
    {
        ATDTPlayableFigure* Figure = Cast<ATDTPlayableFigure>(FigureActor);

        if (!Figure || GetFigureBlocks(Figure).Num()) return;

        Figure->Destroy();
    }
}

void ATDTTetrisBox::LevelUp()
{
    InitialScoreMultiplier += 0.2f;
    InitialFallingSpeed *= 0.9f;

    GetWorldTimerManager().ClearTimer(AutoFallTimerHandle);
    GetWorldTimerManager().SetTimer(AutoFallTimerHandle, this, &ATDTTetrisBox::MoveDown, InitialFallingSpeed, true);
}

void ATDTTetrisBox::GameOver()
{
    GetWorldTimerManager().ClearTimer(AutoFallTimerHandle);
    UE_LOG(LogTemp, Warning, TEXT("--------Game Over!--------"));
    CurrentFigure = nullptr;
    DisableInput(GetWorld()->GetFirstPlayerController());

    FString GameOverMessage = "---Game Over---\nYour score is: " + FString::FromInt(TotalScore);

    UE_LOG(LogTemp, Warning, TEXT("%s"), *GameOverMessage);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, GameOverMessage);
    }

    // UEngine::AddOnScreenDebugMessage(0, 50.f, FColor::Red, GameOverMessage);
}

// utilities

bool ATDTTetrisBox::IsFigureInBounds(ATDTBaseFigure* Figure)
{
    TArray<UStaticMeshComponent*> Blocks = GetFigureBlocks(Figure);

    for (auto Block : Blocks)
    {
        FVector BlockIndex = WorldLocToGridIndex(Block->GetComponentLocation());
        int32 X = BlockIndex.X;
        int32 Y = BlockIndex.Y;
        int32 Z = BlockIndex.Z;

        // UE_LOG(LogTemp, Warning, TEXT("Info:\n\t%s\n\t%i\n\t%i\n\t%i\n\t%i"), *Block->GetName(), Blocks.Num(), X, Y, Z);

        if (X < 0 || X > BoxSizeX - 1 || Y < 0 || Y > BoxSizeY - 1 || Z < 0 || GridTiles.Contains(FVector(X, Y, Z)) &&
            GridTiles[FVector(X, Y, Z)] != nullptr)
        {
            return false;
        }
    }
    return true;
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

float ATDTTetrisBox::IndexToFloat(int32 Index)
{
    return Index * BoxTileSize;
}

TArray<UStaticMeshComponent*> ATDTTetrisBox::GetFigureBlocks(const ATDTBaseFigure* Figure) const
{
    TArray<UStaticMeshComponent*> Blocks;
    Figure->GetComponents<UStaticMeshComponent>(Blocks);
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
