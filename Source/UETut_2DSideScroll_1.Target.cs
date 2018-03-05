// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UETut_2DSideScroll_1Target : TargetRules
{
	public UETut_2DSideScroll_1Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "UETut_2DSideScroll_1" } );
	}
}
