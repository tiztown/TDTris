#pragma once
#include "TDTCoreTypes.h"

//#include "TDTFigureTypes.generated.h"


inline TArray<FVector> GenerateFigurePattern(ETDTFigureType FigureType, int32& BlocksCount, FColor& FigureColor)
{
    TArray<FVector> BlocksOffsets;
    switch (FigureType)
    {
        case ETDTFigureType::I:
        {
            BlocksCount = 4;

            FigureColor = FColor::Red;

            BlocksOffsets.Add({-100.f, 0.f, 0.f});
            BlocksOffsets.Add({0.f, 0.f, 0.f});
            BlocksOffsets.Add({100.f, 0.f, 0.f});
            BlocksOffsets.Add({200.f, 0.f, 0.f});

            break;
        }
        case ETDTFigureType::J:
        {
            BlocksCount = 4;

            FigureColor = FColor::Blue;

            BlocksOffsets.Add({0.f, 0.f, 0.f});
            BlocksOffsets.Add({-100.f, 0.f, 0.f});
            BlocksOffsets.Add({-100.f, 0.f, 100.f});
            BlocksOffsets.Add({100.f, 0.f, 0.f});

            break;
        }
        case ETDTFigureType::L:
        {
            BlocksCount = 4;

            FigureColor = FColor::Orange;

            BlocksOffsets.Add({-100.f, 0.f, 0.f});
            BlocksOffsets.Add({0.f, 0.f, 0.f});
            BlocksOffsets.Add({100.f, 0.f, 0.f});
            BlocksOffsets.Add({100.f, 0.f, 100.f});

            break;
        }
        case ETDTFigureType::O:
        {
            BlocksCount = 4;

            FigureColor = FColor::Green;

            BlocksOffsets.Add({0.f, 0.f, 100.f});
            BlocksOffsets.Add({0.f, 0.f, 0.f});
            BlocksOffsets.Add({100.f, 0.f, 0.f});
            BlocksOffsets.Add({100.f, 0.f, 100.f});

            break;
        }
        case ETDTFigureType::S:
        {
            BlocksCount = 4;

            FigureColor = FColor::Cyan;

            BlocksOffsets.Add({-100.f, 0.f, 0.f});
            BlocksOffsets.Add({0.f, 0.f, 0.f});
            BlocksOffsets.Add({100.f, 0.f, 100.f});
            BlocksOffsets.Add({0.f, 0.f, 100.f});

            break;
        }
        case ETDTFigureType::T:
        {
            BlocksCount = 4;

            FigureColor = FColor::Purple;

            BlocksOffsets.Add({-100.f, 0.f, 0.f});
            BlocksOffsets.Add({0.f, 0.f, 0.f});
            BlocksOffsets.Add({100.f, 0.f, 0.f});
            BlocksOffsets.Add({0.f, 0.f, 100.f});

            break;
        }
        case ETDTFigureType::Z:
        {
            BlocksCount = 4;

            FigureColor = FColor::Yellow;

            BlocksOffsets.Add({100.f, 0.f, 0.f});
            BlocksOffsets.Add({0.f, 0.f, 0.f});
            BlocksOffsets.Add({0.f, 0.f, 100.f});
            BlocksOffsets.Add({-100.f, 0.f, 100.f});

            break;
        }
    }

    return BlocksOffsets;
}
