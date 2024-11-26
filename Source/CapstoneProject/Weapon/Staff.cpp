// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Staff.h"

AStaff::AStaff()
{
	StaffMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaffMesh"));
	StaffMesh->SetupAttachment(RootComponent);
}
