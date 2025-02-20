// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelSelectEntryWidget.h"
#include "LevelSelectData.h"

void ULevelSelectEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (!IsValid(ListItemObject))
	{
		UE_LOG(LogTemp, Error, TEXT("Null list item received when trying to create LevelSelectEntryWidget"));
		return;
	}

	const ULevelSelectData* LevelData = Cast<ULevelSelectData>(ListItemObject);
	if (LevelData)
	{
		LevelNameText->SetText(FText::FromName(LevelData->LevelName));
		LevelToLoad = LevelData->Level;
	}
}

