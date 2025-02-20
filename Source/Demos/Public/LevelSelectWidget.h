// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TileView.h"
#include "LevelSelectWidget.generated.h"

UCLASS()
class DEMOS_API ULevelSelectWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UTileView* LevelSelectView;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftObjectPtr<UWorld>> Levels;

	UFUNCTION(BlueprintCallable)
	void PopulateTileView();
};

