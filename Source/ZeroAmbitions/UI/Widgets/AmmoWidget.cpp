// Fill out your copyright notice in the Description page of Project Settings.x
#include "UI/Widgets/AmmoWidget.h"

void UAmmoWidget::UpdateAmmoCount(int32 NewAmmo, int32 NewTotalAmmo)
{ 
	Ammo = NewAmmo;
	TotalAmmo = NewTotalAmmo;
}
