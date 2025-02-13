// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableComponent.h"

#include "InteractableStencilMask.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bCanInteract = true;
	bStartWithHighlightedActor = true;
	bEnableHighlightEffects = true;
	Mesh = nullptr;
}

void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	if (Mesh == nullptr)
	{
		Mesh = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
	}

	if (bEnableHighlightEffects && Mesh != nullptr)
	{
		Mesh->bRenderCustomDepth = bEnableHighlightEffects;
		if (bStartWithHighlightedActor)
		{
			ActivateInteractionHighlight();
		}
	}
}

void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractableComponent::EnableHighlightEffects(const bool Enabled)
{
	bEnableHighlightEffects = Enabled;
	if (bEnableHighlightEffects && Mesh != nullptr)
	{
		Mesh->bRenderCustomDepth = bEnableHighlightEffects;
		Mesh->MarkRenderStateDirty();
	}
}

void UInteractableComponent::ActivateInteractionHighlight() const
{
	if (Mesh != nullptr)
	{
		Mesh->CustomDepthStencilValue |= (int) EInteractableStencilMask::Highlight;
		Mesh->MarkRenderStateDirty();
	}
}

void UInteractableComponent::DeactivateInteractionHighlight() const
{
	if (Mesh != nullptr)
	{
		Mesh->CustomDepthStencilValue &= ~((int) EInteractableStencilMask::Highlight);
		Mesh->MarkRenderStateDirty();
	}
}

void UInteractableComponent::ActivateInteractionOutline() const
{
	if (Mesh != nullptr)
	{
		Mesh->CustomDepthStencilValue |= (int) EInteractableStencilMask::Outline;
		Mesh->MarkRenderStateDirty();
	}
}

void UInteractableComponent::DeactivateInteractionOutline() const
{
	if (Mesh != nullptr)
	{
		Mesh->CustomDepthStencilValue &= ~((int) EInteractableStencilMask::Outline);
		Mesh->MarkRenderStateDirty();
	}
}
