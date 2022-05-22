// Fill out your copyright notice in the Description page of Project Settings.


#include "Figures/TDTTemplateFigure.h"

#include "Player/TDTTetrisBox.h"

ATDTTemplateFigure::ATDTTemplateFigure()
{
    PrimaryActorTick.bCanEverTick = false;

    FigureRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("FigureRoot"));
    RootComponent = FigureRootComp;

    GenerateTemplateFigure();
}

void ATDTTemplateFigure::GenerateTemplateFigure()
{
    if (BlocksPointers.Num())
    {
        BlocksPointers.Empty();
    }

    for (int32 Count = 0; Count < 4; Count++)
    {
        const FName Name = *FString::Printf(TEXT("Block%i"), Count);
        UStaticMeshComponent* TempComp = CreateDefaultSubobject<UStaticMeshComponent>(Name);
        TempComp->SetupAttachment(RootComponent);
        BlocksPointers.Add(TempComp);
    }
}

void ATDTTemplateFigure::SetTemplateFigureVisual(ATDTPlayableFigure* Figure)
{
    if (!Figure) return;

    // AttachToActor(Figure, FAttachmentTransformRules::KeepRelativeTransform);

    // SetOwner(Figure);

    ParentFigure = Figure;

    CurrentColor = ParentFigure->GetColor();

    BlocksOffsets.Empty();

    BlocksOffsets = ParentFigure->GetBlocksOffsets();

    if (!DynamicMaterialFull)
    {
        if (!BlockMaterialFull) return;
        DynamicMaterialFull = UMaterialInstanceDynamic::Create(BlockMaterialFull, this, "FigureMaterial1");
    }

    DynamicMaterialFull->SetVectorParameterValue("Color", FVector(CurrentColor.R, CurrentColor.G, CurrentColor.B));

    if (!BlockMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("Static mesh not set in %s"), *GetName());
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("BlocksOffsets: %i"), BlocksOffsets.Num());

    if (!BlocksPointers.Num() || !BlocksOffsets.Num()) return;

    for (int32 i = 0, n = BlocksPointers.Num(); i < n; i++)
    {
        if (BlocksPointers[i])
        {
            BlocksPointers[i]->SetStaticMesh(BlockMesh);

            BlocksPointers[i]->SetMaterial(0, DynamicMaterialFull);
            BlocksPointers[i]->SetRelativeLocation(BlocksOffsets[i]);
        }
    }
}

void ATDTTemplateFigure::SyncFigures()
{
    if (!ParentFigure) return;
    FTransform ParentTransform = ParentFigure->GetActorTransform();
    SetActorTransform(ParentTransform);
}

void ATDTTemplateFigure::BeginPlay()
{
    Super::BeginPlay();
}
