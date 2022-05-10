// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDTCoreTypes.h"
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
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Figure")
    ETDTFigureType CurrentFigureType;

    UFUNCTION()
    void GenerateFigure(ETDTFigureType FigureType);

    bool DebugFigure = false;

    void MoveUp();
    void MoveDown();
    void MoveRight();
    void MoveLeft();

private:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* FigureRootComp = nullptr;

    UPROPERTY(EditAnywhere, Category = "Block")
    UStaticMesh* BlockMesh;

    UPROPERTY(EditAnywhere, Category = "Block")
    UMaterialInstance* BlockMaterial;

    UPROPERTY(EditAnywhere, Category = "Block")
    UMaterialInstanceDynamic* DynamicMaterial;

    UPROPERTY(EditAnywhere, Category = "Figure")
    FLinearColor CurrentColor = FColor::Red;

    UPROPERTY(EditAnywhere, Category = "Abilities")
    bool IsGhosty = false;

    UPROPERTY(EditAnywhere)
    FFigureData CurrentFigureData;

    UPROPERTY(EditAnywhere)
    float BlockSize = 100.f;

    TArray<FVector> BlocksOffsets;


    void CalculateFigureBounds();

    UPROPERTY()
    TArray<UStaticMeshComponent*> BlocksPointers;


    UPROPERTY(VisibleAnywhere)
    UEnum* FigureTypesEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETDTFigureType"));

    void SetFigureVisual();
};
