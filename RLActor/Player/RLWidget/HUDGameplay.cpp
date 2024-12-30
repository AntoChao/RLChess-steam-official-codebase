#include "HUDGameplay.h"

void UHUDGameplay::updateSetupTimeDef()
{
    switch (hudLanguage)
    {
    case ELanguage::EEnglish:
    {
        isSetupTimeDef = isSetupTimeEng;
        break;
    }
    case ELanguage::EChinese:
    {
        isSetupTimeDef = isSetupTimeChi;
        break;
    }
    case ELanguage::ESpanish:
    {
        isSetupTimeDef = isSetupTimeSpa;
        break;
    }
    default:
        isSetupTimeDef = isSetupTimeEng;
        break;
    }
}
void UHUDGameplay::updatePlayerTurnDef()
{
    switch (hudLanguage)
    {
    case ELanguage::EEnglish:
    {
        isPlayerTurnDef = isPlayerTurnEng;
        break;
    }
    case ELanguage::EChinese:
    {
        isPlayerTurnDef = isPlayerTurnChi;
        break;
    }
    case ELanguage::ESpanish:
    {
        isPlayerTurnDef = isPlayerTurnSpa;
        break;
    }
    default:
    {
        isPlayerTurnDef = isPlayerTurnEng;
        break;
    }
    }
}
void UHUDGameplay::updatePieceMovingDef()
{
    switch (hudLanguage)
    {
    case ELanguage::EEnglish:
    {
        pieceMovingDef = pieceMovingEng;
        break;
    }
    case ELanguage::EChinese:
    {
        pieceMovingDef = pieceMovingChi;
        break;
    }
    case ELanguage::ESpanish:
    {
        pieceMovingDef = pieceMovingSpa;
        break;
    }
    default:
    {
        pieceMovingDef = pieceMovingEng;
        break;
    }
    }
}
