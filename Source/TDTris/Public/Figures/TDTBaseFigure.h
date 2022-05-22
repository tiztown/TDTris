// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDTFigureTypes.h"
#include "GameFramework/Actor.h"
#include "TDTBaseFigure.generated.h"


UCLASS()
class TDTRIS_API ATDTBaseFigure : public AActor
{
    GENERATED_BODY()

public:
    ATDTBaseFigure();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, Category = "Figure")
    ETDTFigureType CurrentFigureType;

    UPROPERTY(EditAnywhere)
    bool DebugFigure = false;

    UFUNCTION()
    void SetMaterialToBlock(UStaticMeshComponent* Block, int32 MaterialIndex);

    UFUNCTION()
    TArray<FVector> GetBlocksOffsets() const { return BlocksOffsets; }

    UFUNCTION()
    FLinearColor GetColor() const { return CurrentColor; }


protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* FigureRootComp = nullptr;

    UPROPERTY(EditAnywhere, Category = "Block")
    UStaticMesh* BlockMesh;

    UPROPERTY(EditAnywhere, Category = "Block")
    UMaterialInstance* BlockMaterialFull;

    UPROPERTY(EditAnywhere, Category = "Block")
    UMaterialInstance* BlockMaterialEmpty;

    UPROPERTY(EditAnywhere, Category = "Block")
    UMaterialInstanceDynamic* DynamicMaterialFull;

    UPROPERTY(EditAnywhere, Category = "Block")
    UMaterialInstanceDynamic* DynamicMaterialEmpty;

    UPROPERTY(EditAnywhere, Category = "Figure")
    FLinearColor CurrentColor = FColor::Red;

    UPROPERTY(EditAnywhere, Category = "Abilities")
    bool IsGhosty = false;

    // UPROPERTY(EditAnywhere)
    // FFigureData CurrentFigureData;

    UPROPERTY(EditAnywhere)
    float BlockSize = 100.f;

    TArray<FVector> BlocksOffsets;

    UPROPERTY()
    TArray<UStaticMeshComponent*> BlocksPointers;

    UPROPERTY(VisibleAnywhere)
    UEnum* FigureTypesEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETDTFigureType"));

    virtual void SetFigureVisual();
};
