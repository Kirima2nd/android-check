/*
    android-check.inc
    
    About
    -----
    Original implementations by Y_Less and agus, it's just more like complete
    implementations of y_android but standalone include.

    Credits
    -------
      - Y_Less (rconfix hook, and y_android implementations)
      - Kirima (current maintainer)
      - ff-agus44 (previous maintainer)
      - Fairuz-Afdhal (for the original version)

*/
#if !defined _samp_included
    #error "Include a_samp.inc first before including this."
#endif

// Define if not exists
#if defined OnClientChecked
 	forward OnClientChecked(playerid, bool:isAndroid);
#endif

#if !defined SendClientCheck
	native SendClientCheck(playerid, type, memAddr, memOffset, byteCount);
#endif

#if !defined ANDROID_CHECK_INTERVAL
    #define ANDROID_CHECK_INTERVAL 10_000 // 10 seconds, can be changed by define this first before including
#endif

static stock
    bool:g_sPlayerFromAndroid[MAX_PLAYERS] = bool:-1,
    g_sPlayerTimerCheck[MAX_PLAYERS];

stock bool:IsPlayerUsingAndroid(playerid)
{
    return (g_sPlayerFromAndroid[playerid]);
}

stock bool:IsPlayerUsingPC(playerid)
{
    return (!g_sPlayerFromAndroid[playerid]);
}

// Callbacks
public OnGameModeInit()
{
	#if !defined y_rconfix_Load && !defined _INC_SAMP_Community_fixes
		LoadClientCheckFix();
	#endif

    #if defined Ks_OnGameModeInit
        return Ks_OnGameModeInit();
    #else
        return 1;
    #endif
}

public OnPlayerConnect(playerid)
{
	SendClientCheck(playerid, 0x48, 0, 0, 2);
	g_sPlayerTimerCheck[playerid] = SetTimerEx(#Android_InternalTimer, ANDROID_CHECK_INTERVAL, true, "i", playerid);

    #if defined Ks_OnPlayerConnect
        return Ks_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

forward Android_InternalTimer(playerid);
public Android_InternalTimer(playerid)
{
    if (g_sPlayerFromAndroid[playerid] != true)
    {
        g_sPlayerFromAndroid[playerid] = true;
        CallLocalFunction(#OnClientChecked, "ii", playerid, true);
    }
}

// Taken from y_android_entry.inc from YSI by Y_Less
// it's matter because of FIXES_OnClientCheckResponse
#if !defined _INC_y_rconfix
	forward OnClientCheckResponse(playerid, actionid, memaddr, retndata);

	public OnClientCheckResponse(playerid, actionid, memaddr, retndata)
	{
		// Enable, then disable, the call in to the real version (if it exists).
		state OnClientCheckResponse : OnClientCheckResponse_GM;
		new
			ret = FIXES_OnClientCheckResponse(playerid, actionid, memaddr, retndata);
		state OnClientCheckResponse : OnClientCheckResponse_FS;
		return ret;
	}

	forward FIXES_OnClientCheckResponse(playerid, actionid, memaddr, retndata);
	public FIXES_OnClientCheckResponse(playerid, actionid, memaddr, retndata) <OnClientCheckResponse : OnClientCheckResponse_FS>
	{
		return 0;
	}

	public FIXES_OnClientCheckResponse(playerid, actionid, memaddr, retndata) <>
	{
		return 0;
	}

	static stock _FIXES_OnClientCheckResponse() <OnClientCheckResponse : OnClientCheckResponse_GM, OnClientCheckResponse : OnClientCheckResponse_FS>
	{
		// Define states only.
	}

	#if defined _ALS_OnClientCheckResponse
		#error _ALS_OnClientCheckResponse defined
	#endif
	#define _ALS_OnClientCheckResponse
	#define OnClientCheckResponse(%0) FIXES_OnClientCheckResponse(%0) <OnClientCheckResponse : OnClientCheckResponse_GM>
#endif

public OnClientCheckResponse(playerid, actionid, memaddr, retndata)
{
	if (actionid == 0x48) 
	{
        if (g_sPlayerTimerCheck[playerid])
        {
            KillTimer(g_sPlayerTimerCheck[playerid]);
            g_sPlayerTimerCheck[playerid] = 0;

            if (g_sPlayerFromAndroid[playerid] != false)
            {
		        g_sPlayerFromAndroid[playerid] = false;
                CallLocalFunction(#OnClientChecked, "ii", playerid, false);
            }
        }
	}

    #if defined Ks_OnClientCheckResponse
        return Ks_OnClientCheckResponse(playerid, actionid, memaddr, retndata);
    #else
        return 1;
    #endif
}

public OnPlayerDisconnect(playerid, reason)
{
    if (g_sPlayerTimerCheck[playerid])
    {
        KillTimer(g_sPlayerTimerCheck[playerid]);
        g_sPlayerTimerCheck[playerid] = 0;
    }

    g_sPlayerFromAndroid[playerid] = bool:-1;

    #if defined Ks_OnPlayerDisconnect
        return Ks_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}


/* 
	functions to fix OnClientCheckResponse, originally made by Y_Less.

	Source:
  		- https://github.com/pawn-lang/YSI-Includes/blob/aaeb2cc8200bfc64c0133002e7ce6398ea6c5959/YSI_Core/y_master/y_rconfix.inc
  		- https://github.com/pawn-lang/sa-mp-fixes/issues/80#issuecomment-338402500
*/

static LoadClientCheckFix()
{
	// Test if the fix has already been loaded.
	setproperty(27, #FIXES_OnClientCheckResponse, 0);
	SendRconCommand(#FIXES_OnClientCheckResponse);

	if (getproperty(27, #FIXES_OnClientCheckResponse) == 27)
	{
		return;
	}
    
	// If it is not exists, create the file first.
	if (!fexist("callbackfix.amx"))
	{
		static
			lRCFixCode[] = {
				0x00000120, 0x0808F1E0, 0x00080014, 0x000000AC, 0x00000190, 0x000001C0, 0x000041C0, 0xFFFFFFFF,
				0x00000038, 0x00000048, 0x00000060, 0x00000060, 0x00000060, 0x00000060, 0x00000094, 0x00000062,
				0x00000008, 0x00000078, 0x00000000, 0x00000086, 0x00000000, 0x0000008D, 0x00000000, 0x00000099,
				0x6E4F001F, 0x65696C43, 0x6843746E, 0x526B6365, 0x6F707365, 0x0065736E, 0x63526E4F, 0x6F436E6F,
				0x6E616D6D, 0x74730064, 0x706D6372, 0x74657300, 0x706F7270, 0x79747265, 0x6C614300, 0x6D65526C,
				0x4665746F, 0x74636E75, 0x006E6F69, 0x2E007880, 0xFFFF8727, 0x01277FFF, 0x0C290027, 0x7B801027,
				0x36142C00, 0x1C270C81, 0x00271B27, 0x10271B27, 0x2C017B80, 0x30548014, 0x2E305980, 0x81180581,
				0x05811405, 0x0C058110, 0x00272827, 0x7B801827, 0x301C2C02, 0xB0A5B284, 0xFD9A8545, 0x9B846E9E,
				0x8665D2B1, 0x6886D1F3, 0xD68DAB86, 0xCDAB8652, 0xF3866FE0, 0x0000CACD, 0xCB860000, 0x0069D2A5
			};
		new
			File:fs = fopen("callbackfix.amx", io_write);

		if (fs)
		{
			fblockwrite(fs, lRCFixCode);
			fclose(fs);
		}
	}

	/* 
		after that we check if the caller is actually exist, then send the rcon command
		the reason why the original code using this instead of else is because when the file is not exists
		using else here will not be called.
	*/
	if (fexist("callbackfix.amx"))
	{
		SendRconCommand("loadfs ../scriptfiles/callbackfix");
        state OnClientCheckResponse : OnClientCheckResponse_GM;
	}
    else
    {
        printf("Cannot load callbackfix.amx");
        printf("Ignore if YSI or fixes.inc already load this.");
    }
}

// ALS stuff
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif

#define OnGameModeInit( Ks_OnGameModeInit(
#if defined Ks_OnGameModeInit
    forward Ks_OnGameModeInit();
#endif

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect( Ks_OnPlayerConnect(
#if defined Ks_OnPlayerConnect
    forward Ks_OnPlayerConnect(playerid);
#endif

#if defined _ALS_OnClientCheckResponse
	#undef OnClientCheckResponse
#else
	#define _ALS_OnClientCheckResponse
#endif
#define OnClientCheckResponse( Ks_OnClientCheckResponse(
#if defined Ks_OnClientCheckResponse
	forward Ks_OnClientCheckResponse(playerid, actionid, memaddr, retndata);
#endif

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect( Ks_OnPlayerDisconnect(
#if defined Ks_OnPlayerDisconnect
    forward Ks_OnPlayerDisconnect(playerid, reason);
#endif
