#pragma once

#include "HydraPluginPrivatePCH.h"
#include "HydraInterface.h"

UHydraInterface::UHydraInterface(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

//This is required for compiling, would also let you know if somehow you called
//the base event/function rather than the over-rided version
FString IHydraInterface::ToString()
{
	return "IHydraInterface::ToString()";
}
