// Fill out your copyright notice in the Description page of Project Settings.


#include "Figures/TDTPlayableFigure.h"

ATDTPlayableFigure::ATDTPlayableFigure()
{
    PrimaryActorTick.bCanEverTick = false;

    FigureRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("FigureRoot"));
    RootComponent = FigureRootComp;

    int32 Index = FMath::Rand() % FigureTypesEnum->GetMaxEnumValue();
    CurrentFigureType = static_cast<ETDTFigureType>(Index);

    GeneratePlayableFigure(CurrentFigureType);
}

void ATDTPlayableFigure::GeneratePlayableFigure(ETDTFigureType FigureType)
{
    if (BlocksPointers.Num())
    {
        BlocksPointers.Empty();
    }

    int32 BlocksCount;
    FColor FigureColor;

    BlocksOffsets.Empty();

    BlocksOffsets = GenerateFigurePattern(FigureType, BlocksCount, FigureColor);

    CurrentColor = FigureColor;

    for (int32 Count = 0; Count < BlocksCount; Count++)
    {
        const FName Name = *FString::Printf(TEXT("Block%i"), Count);
        UStaticMeshComponent* TempComp = CreateDefaultSubobject<UStaticMeshComponent>(Name);
        TempComp->SetupAttachment(RootComponent);
        BlocksPointers.Add(TempComp);
    }
}

void ATDTPlayableFigure::BeginPlay()
{
    Super::BeginPlay();

    SetFigureVisual();
}
