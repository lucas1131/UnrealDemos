// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "LevelSelectEntryWidget.generated.h"

UCLASS(BlueprintType)
class DEMOS_API ULevelSelectEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UWorld> LevelToLoad;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* LevelNameText;
	
	UPROPERTY(meta=(BindWidget))
	UButton* LoadLevelButton;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};


