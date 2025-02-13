// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"


UCLASS(ClassGroup=(InteractableSystem), meta=(BlueprintSpawnableComponent))
class INTERACTABLE_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanInteract;

	/* Enabling this will enable render custom depth pass for this actor's meshes. This requires Custom Depth to be
	 * enabled with stencil in project settings. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEnableHighlightEffects;
	/* Control if it should apply highlight effect to owner actor to how it can be interacted with */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bStartWithHighlightedActor;

public:
	UInteractableComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void EnableHighlightEffects(bool Enabled);
	FString& GetDisplayDescription() { return DisplayDescription; }

	bool CanInteract() const { return bCanInteract; }

	UFUNCTION(BlueprintCallable)
	void ActivateInteractionHighlight() const;
	UFUNCTION(BlueprintCallable)
	void DeactivateInteractionHighlight() const;
	UFUNCTION(BlueprintCallable)
	void ActivateInteractionOutline() const;
	UFUNCTION(BlueprintCallable)
	void DeactivateInteractionOutline() const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginInteraction(AActor* Interactor) const;
	UFUNCTION(BlueprintImplementableEvent)
	void OnEndInteraction(AActor* Interactor) const;

protected:
	virtual void BeginPlay() override;
};
