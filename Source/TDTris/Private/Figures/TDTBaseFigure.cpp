// Fill out your copyright notice in the Description page of Project Settings.


#include "Figures/TDTBaseFigure.h"

#include "TDTFigureTypes.h"
#include "Math/BigInt.h"

ATDTBaseFigure::ATDTBaseFigure()
{
}

void ATDTBaseFigure::BeginPlay()
{
    Super::BeginPlay();
}

void ATDTBaseFigure::SetFigureVisual()
{
    if (!DynamicMaterialFull)
    {
        if (!BlockMaterialFull) return;
        DynamicMaterialFull = UMaterialInstanceDynamic::Create(BlockMaterialFull, this, "FigureMaterial1");
    }

    DynamicMaterialFull->SetVectorParameterValue("Color", FVector(CurrentColor.R, CurrentColor.G, CurrentColor.B));

    if (!DynamicMaterialEmpty)
    {
        if (!BlockMaterialEmpty) return;
        DynamicMaterialEmpty = UMaterialInstanceDynamic::Create(BlockMaterialEmpty, this, "FigureMaterial2");
    }

    DynamicMaterialEmpty->SetVectorParameterValue("Color", FVector(CurrentColor.R, CurrentColor.G, CurrentColor.B));

    if (!BlockMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("Static mesh not set in %s"), *GetName());
        return;
    }

    if (!BlocksPointers.Num() || !BlocksOffsets.Num()) return;

    for (int32 i = 0, n = BlocksPointers.Num(); i < n; i++)
    {
        if (BlocksPointers[i] && BlockMesh)
        {
            BlocksPointers[i]->SetStaticMesh(BlockMesh);

            BlocksPointers[i]->SetMaterial(0, DynamicMaterialFull);
            BlocksPointers[i]->SetRelativeLocation(BlocksOffsets[i]);
        }
    }
}


void ATDTBaseFigure::SetMaterialToBlock(UStaticMeshComponent* Block, int32 MaterialIndex)
{
    if (!DynamicMaterialEmpty || !DynamicMaterialFull) return;

    // UE_LOG(LogTemp, Warning, TEXT("%s"), *Block->GetOwner()->GetName());

    UMaterialInstanceDynamic* DynamicMaterial = MaterialIndex ? DynamicMaterialFull : DynamicMaterialEmpty;
    Block->SetMaterial(0, DynamicMaterial);
}
