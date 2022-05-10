// Fill out your copyright notice in the Description page of Project Settings.


#include "Figures/TDTBaseFigure.h"

#include "TDTFigureTypes.h"
#include "Math/BigInt.h"

ATDTBaseFigure::ATDTBaseFigure()
{
    PrimaryActorTick.bCanEverTick = true;

    FigureRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("FigureRoot"));
    RootComponent = FigureRootComp;

    if (!DebugFigure)
    {
        int32 Index = FMath::Rand() % FigureTypesEnum->GetMaxEnumValue();
        CurrentFigureType = static_cast<ETDTFigureType>(Index);

        GenerateFigure(CurrentFigureType);
    }
    else
    {
        GenerateFigure(ETDTFigureType::O);
    }
}

void ATDTBaseFigure::BeginPlay()
{
    Super::BeginPlay();

    SetFigureVisual();

    //CalculateFigureBounds();
}


void ATDTBaseFigure::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
}

void ATDTBaseFigure::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


void ATDTBaseFigure::MoveUp()
{
    FVector CurLocation = GetActorLocation();
    FVector NewLocation = CurLocation;
    NewLocation.X += 100.f;
    FFigureData NewFigureData = CurrentFigureData;
    NewFigureData.LowerXBound += 100.f;
    NewFigureData.UpperXBound += 100.f;

    // if (GetOwner->IsInBounds(NewFigureData))
    // {
    //     SetActorLocation(NewLocation);
    //     CurrentFigureData.LowerXBound += 100.f;
    //     CurrentFigureData.UpperXBound += 100.f;
    // }
}

void ATDTBaseFigure::MoveDown()
{
}

void ATDTBaseFigure::MoveRight()
{
}

void ATDTBaseFigure::MoveLeft()
{
}

void ATDTBaseFigure::CalculateFigureBounds()
{
    //
    // CurrentFigureData.LowerXBound = BlocksPointers[0]->GetComponentLocation().X;
}

void ATDTBaseFigure::SetFigureVisual()
{
    if (!DynamicMaterial)
    {
        if (!BlockMaterial) return;
        DynamicMaterial = UMaterialInstanceDynamic::Create(BlockMaterial, this, "FigureMaterial");
    }

    DynamicMaterial->SetVectorParameterValue("Color", FVector(CurrentColor.R, CurrentColor.G, CurrentColor.B));

    if (!BlockMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("Static mesh not set in %s"), *GetName());
        return;
    }

    if (!BlocksPointers.Num()) return;

    for (int32 i = 0, n = BlocksPointers.Num(); i < n; i++)
    {
        if (BlocksPointers[i] && BlockMesh)
        {
            BlocksPointers[i]->SetStaticMesh(BlockMesh);

            BlocksPointers[i]->SetMaterial(0, DynamicMaterial);
            BlocksPointers[i]->SetRelativeLocation(BlocksOffsets[i]);
        }
    }
}


void ATDTBaseFigure::GenerateFigure(ETDTFigureType FigureType)
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
