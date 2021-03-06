#include <sourcemod>
#undefine REQUIRE_EXTENSION

#define TEAM_UNASSIGNED		0
#define TEAM_SPECTATOR		1

#define HL2DM_TEAM_COMBINE	2
#define HL2DM_TEAM_REBEL	3

#define DOD_TEAM_ALLIES		2
#define DOD_TEAM_AXIS		3

// Defined in cstrike.inc
#if !defined CS_TEAM_T
#define CS_TEAM_T			2
#endif
#if !defined CS_TEAM_CT
#define CS_TEAM_CT			3
#endif

// These are an enum in tf2.inc, but that's not useful for certain cases
#define TF_TEAM_RED			2
#define TF_TEAM_BLU			3

#define L4D_TEAM_SURVIVOR	2
#define L4D_TEAM_INFECTED	3

/**
 * Add some game-specific multi-target filters
 * 
 * Remember: These return translation phrases.  To use them, you need to do this in OnPluginStart:
 * LoadTranslations("team_filter.phrases");
 */
stock Register_Filters()
{
	new EngineVersion:engine = GetEngineVersion();
	
	switch (engine)
	{
		case Engine_Left4Dead, Engine_Left4Dead2:
		{
			AddMultiTargetFilter("survivors", L4DTeamFilter, "L4D Survivors", false);
			AddMultiTargetFilter("blue", L4DTeamFilter, "L4D Survivors", false);
			AddMultiTargetFilter("infected", L4DTeamFilter, "L4D Infected", false);
			AddMultiTargetFilter("red", L4DTeamFilter, "L4D Infected", false);
		}
		
		case Engine_HL2DM:
		{
			AddMultiTargetFilter("combine", HL2DMTeamFilter, "HL2DM Combine", false);
			AddMultiTargetFilter("blue", HL2DMTeamFilter, "HL2DM Combine", false);
			AddMultiTargetFilter("rebels", HL2DMTeamFilter, "HL2DM Rebels", false);
			AddMultiTargetFilter("red", HL2DMTeamFilter, "HL2DM Rebels", false);
		}
		
		case Engine_DODS:
		{
			AddMultiTargetFilter("allies", DODSTeamFilter, "DODS Allies", false);
			AddMultiTargetFilter("blue", DODSTeamFilter, "DODS Allies", false);
			AddMultiTargetFilter("axis", DODSTeamFilter, "DODS Axis", false);
			AddMultiTargetFilter("red", DODSTeamFilter, "DODS Axis", false);
		}
		
		case Engine_CSS, Engine_CSGO:
		{
			// @ct and @t are already registered in CS:S and CS:GO
			AddMultiTargetFilter("blue", DODSTeamFilter, "CS Counter-Terrorists", true);
			AddMultiTargetFilter("red", DODSTeamFilter, "CS Terrorists", true);
		}
		
		case Engine_TF2:
		{
			// @red and @blue are already registered in TF2.
			AddMultiTargetFilter("blu", TF2TeamFilter, "TF2 BLU", true);
		}
	}
}

public bool:TF2TeamFilter(const String:pattern[], Handle:clients)
{
	new checkTeam;
	
	// "red" and "blue" are already registered
	if (StrEqual(pattern, "blu", false))
	{
		checkTeam = TF_TEAM_BLU;
	}
	
	if (checkTeam == 0)
		return false;
	
	ProcessTeamTargets(checkTeam, clients);
	return true;
}

public bool:L4DTeamFilter(const String:pattern[], Handle:clients)
{
	new checkTeam;
	
	if (StrEqual(pattern, "survivors", false) || StrEqual(pattern, "blue", false))
	{
		checkTeam = L4D_TEAM_SURVIVOR;
	}
	else if (StrEqual(pattern, "infected", false) || StrEqual(pattern, "red", false))
	{
		checkTeam = L4D_TEAM_INFECTED;
	}
	
	if (checkTeam == 0)
		return false;

	ProcessTeamTargets(checkTeam, clients);
	
	return true;
}

public bool:DODSTeamFilter(const String:pattern[], Handle:clients)
{
	new checkTeam;
	
	if (StrEqual(pattern, "allies", false) || StrEqual(pattern, "blue", false))
	{
		checkTeam = DOD_TEAM_ALLIES;
	}
	else if (StrEqual(pattern, "axis", false) || StrEqual(pattern, "red", false))
	{
		checkTeam = DOD_TEAM_AXIS;
	}
	
	if (checkTeam == 0)
		return false;

	ProcessTeamTargets(checkTeam, clients);
	
	return true;
}

public bool:HL2DMTeamFilter(const String:pattern[], Handle:clients)
{
	new Handle:cvar = FindConVar("mp_teamplay");
	if (!GetConVarBool(cvar))
		return false;
	
	new checkTeam;
	
	if (StrEqual(pattern, "combine", false) || StrEqual(pattern, "blue", false))
	{
		checkTeam = HL2DM_TEAM_COMBINE;
	}
	else if (StrEqual(pattern, "rebels", false) || StrEqual(pattern, "red", false))
	{
		checkTeam = HL2DM_TEAM_REBEL;
	}
	
	if (checkTeam == 0)
		return false;

	ProcessTeamTargets(checkTeam, clients);
	
	return true;
}

public bool:CSTeamFilter(const String:pattern[], Handle:clients)
{
	new checkTeam;
	
	if (StrEqual(pattern, "blue", false))
	{
		checkTeam = CS_TEAM_CT;
	}
	else if (StrEqual(pattern, "red", false))
	{
		checkTeam = CS_TEAM_T;
	}
	
	if (checkTeam == 0)
		return false;

	ProcessTeamTargets(checkTeam, clients);
	
	return true;
}

ProcessTeamTargets(team, Handle:clients)
{
	for (new i = 1; i <= MaxClients; ++i)
	{
		if (IsClientInGame(i) && GetClientTeam(i) == team)
			PushArrayCell(clients, i);
	}
}

