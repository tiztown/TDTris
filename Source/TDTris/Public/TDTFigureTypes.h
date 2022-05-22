#pragma once

#include "TDTFigureTypes.generated.h"

UENUM(BlueprintType)
enum class ETDTFigureType : uint8
{
    I = 0,
    O,
    T,
    J,
    L,
    S,
    Z,
    Default
};


inline TArray<FVector> GenerateFigurePattern(ETDTFigureType FigureType, int32& BlocksCount, FColor& FigureColor)
{
    TArray<FVector> BlocksOffsets;
    switch (FigureType)
    {
        case ETDTFigureType::I:
        {
            BlocksCount = 4;

            FigureColor = FColor::Red;

            BlocksOffsets.Add({0.f, -100.f, 0.f});
            BlocksOffsets.Add({0.f, 0.f, 0.f});
            BlocksOffsets.Add({0.f, 100.f, 0.f});
            BlocksOffsets.Add({0.f, 200.f, 0.f});

            break;
        }
        case ETDTFigureType::J:
        {
            BlocksCount = 4;

            FigureColor = FColor::Blue;

            BlocksOffsets.Add({0.f, 0.f, 0.f});
            BlocksOffsets.Add({0.f, -100.f, 0.f});
            BlocksOffsets.Add({0.f, -100.f, 100.f});
            BlocksOffsets.Add({0.f, 100.f, 0.f});

            break;
        }
        case ETDTFigureType::L:
        {
            BlocksCount = 4;

            FigureColor = FColor::Orange;

            BlocksOffsets.Add({0.f, -100.f, 0.f});
            BlocksOffsets.Add({0.f, 0.f, 0.f});
            BlocksOffsets.Add({0.f, 100.f, 0.f});
            BlocksOffsets.Add({0.f, 100.f, 100.f});

            break;
        }
        case ETDTFigureType::O:
        {
            BlocksCount = 4;

            FigureColor = FColor::Green;

            BlocksOffsets.Add({0.f, 0.f, 100.f});
            BlocksOffsets.Add({0.f, 0.f, 0.f});
            BlocksOffsets.Add({0.f, 100.f, 0.f});
            BlocksOffsets.Add({0.f, 100.f, 100.f});

            break;
        }
        case ETDTFigureType::S:
        {
            BlocksCount = 4;

            FigureColor = FColor::Cyan;

            BlocksOffsets.Add({0.f, -100.f, 0.f});
            BlocksOffsets.Add({0.f, 0.f, 0.f});
            BlocksOffsets.Add({0.f, 100.f, 100.f});
            BlocksOffsets.Add({0.f, 0.f, 100.f});

            break;
        }
        case ETDTFigureType::T:
        {
            BlocksCount = 4;

            FigureColor = FColor::Purple;

            BlocksOffsets.Add({0.f, -100.f, 0.f});
            BlocksOffsets.Add({0.f, 0.f, 0.f});
            BlocksOffsets.Add({0.f, 100.f, 0.f});
            BlocksOffsets.Add({0.f, 0.f, 100.f});

            break;
        }
        case ETDTFigureType::Z:
        {
            BlocksCount = 4;

            FigureColor = FColor::Yellow;

            BlocksOffsets.Add({0.f, 100.f, 0.f});
            BlocksOffsets.Add({0.f, 0.f, 0.f});
            BlocksOffsets.Add({0.f, 0.f, 100.f});
            BlocksOffsets.Add({0.f, -100.f, 100.f});

            break;
        }
        case ETDTFigureType::Default:
        {
            BlocksCount = 4;
            
            FigureColor = FColor::White;
        }
    }

    return BlocksOffsets;
}
