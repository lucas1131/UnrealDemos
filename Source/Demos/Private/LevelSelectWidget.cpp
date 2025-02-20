// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelSelectWidget.h"
#include "Demos/Public/LevelSelectData.h"

void ULevelSelectWidget::PopulateTileView()
{
	if (LevelSelectView == nullptr)
	{
		LevelSelectView = WidgetTree->ConstructWidget<UTileView>(UTileView::StaticClass(), FName("LevelSelectTileView"));
	}
	
	LevelSelectView->ClearListItems();

	for (TSoftObjectPtr<UWorld> LevelRef : Levels)
	{
		const UWorld* Level = LevelRef.LoadSynchronous();
		if (Level != nullptr)
		{
			ULevelSelectData* Data = NewObject<ULevelSelectData>();
			Data->Level = LevelRef;
			Data->LevelName = *LevelRef.GetAssetName();
			LevelSelectView->AddItem(Data);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid level found"));
		}
	}
}
