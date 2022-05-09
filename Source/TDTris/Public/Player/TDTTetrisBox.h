// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDTCoreTypes.h"
#include "GameFramework/Pawn.h"
#include "TDTTetrisBox.generated.h"

class USphereComponent;
class ATDTBaseFigure;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TDTRIS_API ATDTTetrisBox : public APawn
{
    GENERATED_BODY()

public:
    ATDTTetrisBox();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void SpawnFigure();

    bool IsFigureInBounds();

    UFUNCTION()
    FVector WorldLocToGridIndex(const FVector WorldLoc) const;

    UFUNCTION()
    int32 FloatToIndex(float Coord);

    UFUNCTION()
    float IndexToFloat(int32 Index);

    UPROPERTY(EditAnywhere, Category = "Figures", meta = (ClampMin = 4, ClampMax = 9))
    int32 FigureBlocksCount = 6;

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
    FVector GridIndexToWorldLoc(const FVector GridIndex) const;


    UPROPERTY(EditDefaultsOnly, Category = "Figures")
    TSubclassOf<ATDTBaseFigure> FigureClass;

    UPROPERTY(EditDefaultsOnly, Category = "Figures")
    ATDTBaseFigure* CurrentFigure;


    // UPROPERTY(EditAnywhere, Category = "Figures")
    // ETDTFigureType FigureType;

    bool IsFloorFull(int32 FloorIndex);


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

    void LockFigure();

    bool IsFastFalling = false;

    FTimerHandle AutoFallTimerHandle;
    FTimerHandle FallTimerHandle;

    TArray<UStaticMeshComponent*> GetFigureBlocks();

    FVector GetCameraForwardVector();
    FVector GetCameraRightVector();
};
