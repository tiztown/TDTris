// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDTCoreTypes.h"
#include "Figures/TDTPlayableFigure.h"
#include "GameFramework/Pawn.h"
#include "TDTTetrisBox.generated.h"

class ATDTTemplateFigure;
class USphereComponent;
class ATDTPlayableFigure;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TDTRIS_API ATDTTetrisBox : public APawn
{
    GENERATED_BODY()

public:
    ATDTTetrisBox();

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditAnywhere, Category = "Figures", meta = (ClampMin = 4, ClampMax = 9))
    int32 FigureBlocksCount = 6;

    UFUNCTION(BlueprintCallable, Category = "TetrisBox")
    void SetBoxSizeX(int32 INBoxSizeX) { BoxSizeX = INBoxSizeX; }

    UFUNCTION(BlueprintCallable, Category = "TetrisBox")
    void SetBoxSizeY(int32 INBoxSizeY) { BoxSizeY = INBoxSizeY; }

    UFUNCTION(BlueprintCallable, Category = "TetrisBox")
    void SetBoxSizeZ(int32 INBoxSizeZ) { BoxSizeZ = INBoxSizeZ; }

    UFUNCTION(BlueprintCallable, Category = "TetrisBox")
    void SetBoxTileSize(float INBoxTileSize) { BoxTileSize = INBoxTileSize; }

    UFUNCTION(BlueprintCallable, Category = "TetrisBox")
    void SetFloorsDestroyedCounter(int32 INFloorsDestroyedCounter) { FloorsDestroyedCounter = INFloorsDestroyedCounter; }

    UFUNCTION(BlueprintCallable, Category = "TetrisBox")
    void SetInitialScoreMultiplier(float INInitialScoreMultiplier) { InitialScoreMultiplier = INInitialScoreMultiplier; }

    UFUNCTION(BlueprintCallable, Category = "TetrisBox")
    void SetInitialFallingSpeed(float INInitialFallingSpeed) { InitialFallingSpeed = INInitialFallingSpeed; }

    UFUNCTION(BlueprintCallable, Category = "TetrisBox")
    void SetLevelsToLevelUp(int32 INLevelsToLevelUp) { LevelsToLevelUp = INLevelsToLevelUp; }


    UFUNCTION(BlueprintCallable, Category = "Info")
    int32 GetFloorsDestroyedCounter() const { return FloorsDestroyedCounter; }

    UFUNCTION(BlueprintCallable, Category = "Info")
    int32 GetTotalScore() const { return TotalScore; }

    UFUNCTION(BlueprintCallable, Category = "Figures")
    ATDTPlayableFigure* GetCurrentFigure() const { return CurrentFigure; }


protected:
    virtual void BeginPlay() override;

private:
    // UPROPERTY(VisibleAnywhere)
    // UStaticMeshComponent* BoxMesh = nullptr;

    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArm = nullptr;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* Camera = nullptr;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* FigureSpawnPoint = nullptr;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* QueueSpawnPoint = nullptr;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* GridZeroPoint = nullptr;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* CameraPoint = nullptr;

    UPROPERTY(VisibleAnywhere)
    TMap<FVector, UStaticMeshComponent*> GridTiles = {};

    UPROPERTY(EditAnywhere, Category = "Size")
    int32 BoxSizeX = 8;

    UPROPERTY(EditAnywhere, Category = "Size")
    int32 BoxSizeY = 8;

    UPROPERTY(EditAnywhere, Category = "Size")
    int32 BoxSizeZ = 20;

    UPROPERTY(EditAnywhere, Category = "Size")
    float BoxTileSize = 100.f;

    UFUNCTION()
    void GenerateGrid(int32 SizeX, int32 SizeY, int32 SizeZ, float TileSize);

    UFUNCTION()
    void GenerateQueue(int32 FiguresNum);

    void MoveUpQueueFigures();

    UFUNCTION()
    void AddFigureToQueue();

    void SpawnFigure();

    void MoveFigureToSpawn();

    void PlaceTemplateFigure();

    void DropTemplate();

    void StartPlay();

    UFUNCTION()
    FVector GridIndexToWorldLoc(const FVector GridIndex) const;

    UFUNCTION()
    FVector WorldLocToGridIndex(const FVector WorldLoc) const;

    UFUNCTION()
    float IndexToFloat(int32 Index);

    UPROPERTY(EditDefaultsOnly, Category = "Figures")
    TSubclassOf<ATDTPlayableFigure> FigureClass;

    UPROPERTY(EditDefaultsOnly, Category = "Figures")
    TSubclassOf<ATDTTemplateFigure> TemplateFigureClass;

    UPROPERTY(VisibleInstanceOnly, Category = "Figures")
    ATDTPlayableFigure* CurrentFigure;


    UPROPERTY(VisibleInstanceOnly, Category = "Figures")
    ATDTTemplateFigure* TemplateFigure;


    UPROPERTY(VisibleInstanceOnly, Category = "Figures")
    TArray<ATDTPlayableFigure*> FiguresQueue;

    UPROPERTY(EditAnywhere, Category = "Figures")
    int32 QueueFiguresNum = 3;

    FVector GetCameraForwardVector();
    FVector GetCameraRightVector();
    TArray<UStaticMeshComponent*> GetFigureBlocks(const ATDTBaseFigure* Figure) const;

    bool IsFigureInBounds(ATDTBaseFigure* Figure);

    void MoveForward();
    void MoveBackward();
    void MoveRight();
    void MoveLeft();
    void RotateFront();
    void RotateBack();
    void RotateRight();
    void RotateLeft();

    void MoveDown();

    void EnableFallDown();
    void DisableFallDown();

    bool IsFastFalling = false;

    FTimerHandle AutoFallTimerHandle;
    FTimerHandle FallTimerHandle;

    void LockFigure();

    bool IsFloorFull(int32 FloorIndex);

    void CheckFloors();
    void DestroyFloor(int32 FloorIndex);
    void MoveFloor(int32 FloorIndex);
    void CheckAndDestroyFigures();

    void LevelUp();


    bool FloorDestroyed = false;
    int32 FloorsDestroyedCounter = 0;


    UPROPERTY(EditDefaultsOnly, Category = "Debug")
    bool DebugFigures = false;


    int32 TotalScore = 0;
    const int32 EachFloorScore = 100;
    const int32 EachFigureScore = 20;
    float InitialScoreMultiplier = 1.f;

    float InitialFallingSpeed = 1.f;

    UPROPERTY(EditAnywhere, Category = "Gameplay")
    int32 LevelsToLevelUp = 3;

    void GameOver();
};
