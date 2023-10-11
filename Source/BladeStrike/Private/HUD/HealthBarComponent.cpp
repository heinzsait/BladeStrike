// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercentage(float hp)
{
	if (healthbarWidget == nullptr)
	{
		healthbarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (healthbarWidget && healthbarWidget->healthBar)
	{
		healthbarWidget->healthBar->SetPercent(hp);
	}
}
