#pragma once

#include "sdk/g_pos.hpp"
#include "sdk/cmd.hpp"

namespace sdk {

typedef int HSPRITE_T;	// handle to a graphic

typedef struct client_sprite_s {
	char szName[64];
	char szSprite[64];
	int hspr;
	int iRes;
	s_rect rc;
} client_sprite_t;

typedef struct hud_player_info_s {
	char* name;
	short ping;
	byte thisplayer;  // TRUE if this is the calling player

  // stuff that's unused at the moment,  but should be done
	byte spectator;
	byte packetloss;

	char* model;
	short topcolor;
	short bottomcolor;

} hud_player_info_t;

typedef struct SCREENINFO_s {
	int		iSize;
	int		iWidth;
	int		iHeight;
	int		iFlags;
	int		iCharHeight;
	short	charWidths[256];
} SCREENINFO;

typedef struct client_textmessage_s {
	int		effect;
	byte	r1, g1, b1, a1;		// 2 colors for effects
	byte	r2, g2, b2, a2;
	float	x;
	float	y;
	float	fadein;
	float	fadeout;
	float	holdtime;
	float	fxtime;
	const char* pName;
	const char* pMessage;
} client_textmessage_t;

typedef struct cl_enginefunc_s {
	// sprite handlers
	HSPRITE_T(*pfnSPR_Load)					(const char* szPicName);
	int					(*pfnSPR_Frames)				(HSPRITE_T hPic);
	int					(*pfnSPR_Height)				(HSPRITE_T hPic, int frame);
	int					(*pfnSPR_Width)					(HSPRITE_T hPic, int frame);
	void				(*pfnSPR_Set)					(HSPRITE_T hPic, int r, int g, int b);
	void				(*pfnSPR_Draw)					(int frame, int x, int y, const s_rect* prc);
	void				(*pfnSPR_DrawHoles)				(int frame, int x, int y, const s_rect* prc);
	void				(*pfnSPR_DrawAdditive)			(int frame, int x, int y, const s_rect* prc);
	void				(*pfnSPR_EnableScissor)			(int x, int y, int width, int height);
	void				(*pfnSPR_DisableScissor)		(void);
	client_sprite_t* (*pfnSPR_GetList)				(char* psz, int* piCount);

	// screen handlers
	void				(*pfnFillRGBA)					(int x, int y, int width, int height, int r, int g, int b, int a);
	int					(*pfnGetScreenInfo)				(SCREENINFO* pscrinfo);
	void				(*pfnSetCrosshair)				(HSPRITE_T hspr, s_rect rc, int r, int g, int b);

	// cvar handlers
	struct cvar_s* (*pfnRegisterVariable)			(const char* szName, const char* szValue, int flags);
	float				(*pfnGetCvarFloat)				(const char* szName);
	char* (*pfnGetCvarString)				(const char* szName);

	// command handlers
	int					(*pfnAddCommand)				(const char* cmd_name, void (*function)(void));
	int					(*pfnHookUserMsg)				(const char* szMsgName, pfnUserMsgHook pfn);
	int					(*pfnServerCmd)					(const char* szCmdString);
	int					(*pfnClientCmd)					(const char* szCmdString);

	void				(*pfnGetPlayerInfo)				(int ent_num, hud_player_info_t* pinfo);

	// sound handlers
	void				(*pfnPlaySoundByName)			(char* szSound, float volume);
	void				(*pfnPlaySoundByIndex)			(int iSound, float volume);

	// vector helpers
	void				(*pfnAngleVectors)				(const float* vecAngles, float* forward, float* right, float* up);

	// text message system
	client_textmessage_t* (*pfnTextMessageGet)			(const char* pName);
	int					(*pfnDrawCharacter)				(int x, int y, int number, int r, int g, int b);
	int					(*pfnDrawConsoleString)			(int x, int y, char* string);
	void				(*pfnDrawSetTextColor)			(float r, float g, float b);
	void				(*pfnDrawConsoleStringLen)		(const char* string, int* length, int* height);

	void				(*pfnConsolePrint)				(const char* string);
	void				(*pfnCenterPrint)				(const char* string);

	// Added for user input processing
	int					(*GetWindowCenterX)				(void);
	int					(*GetWindowCenterY)				(void);
	void				(*GetViewAngles)				(float*);
	void				(*SetViewAngles)				(float*);
	int					(*GetMaxClients)				(void);
	void				(*Cvar_SetValue)				(const char* cvar, float value);

	int					(*Cmd_Argc)						(void);
	char* (*Cmd_Argv)						(int arg);
	void				(*Con_Printf)					(const char* fmt, ...);
	void				(*Con_DPrintf)					(const char* fmt, ...);
	void				(*Con_NPrintf)					(int pos, const char* fmt, ...);
	void				(*Con_NXPrintf)					(struct con_nprint_s* info, const char* fmt, ...);

	const char* (*PhysInfo_ValueForKey)			(const char* key);
	const char* (*ServerInfo_ValueForKey)		(const char* key);
	float				(*GetClientMaxspeed)			(void);
	int					(*CheckParm)					(char* parm, char** ppnext);

	void				(*Key_Event)					(int key, int down);
	void				(*GetMousePosition)				(int* mx, int* my);
	int					(*IsNoClipping)					(void);

	struct cl_entity_s* (*GetLocalPlayer)				(void);
	struct cl_entity_s* (*GetViewModel)					(void);
	struct cl_entity_s* (*GetEntityByIndex)				(int idx);

	float				(*GetClientTime)				(void);
	void				(*V_CalcShake)					(void);
	void				(*V_ApplyShake)					(float* origin, float* angles, float factor);

	int					(*PM_PointContents)				(float* point, int* truecontents);
	int					(*PM_WaterEntity)				(float* p);
	struct pmtrace_s* (*PM_TraceLine)					(float* start, float* end, int flags, int usehull, int ignore_pe);

	struct model_s* (*CL_LoadModel)					(const char* modelname, int* index);
	int					(*CL_CreateVisibleEntity)		(int type, struct cl_entity_s* ent);

	const struct model_s* (*GetSpritePointer)			(HSPRITE_T HSPRITE_T);
	void				(*pfnPlaySoundByNameAtLocation)	(char* szSound, float volume, float* origin);

	unsigned short		(*pfnPrecacheEvent)				(int type, const char* psz);
	void				(*pfnPlaybackEvent)				(int flags, const struct edict_s* pInvoker, unsigned short eventindex, float delay, float* origin, float* angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
	void				(*pfnWeaponAnim)				(int iAnim, int body);
	float				(*pfnRandomFloat)				(float flLow, float flHigh);
	int					(*pfnRandomLong)				(int lLow, int lHigh);
	void				(*pfnHookEvent)					(char* name, void (*pfnEvent)(struct event_args_s* args));
	int					(*Con_IsVisible)				(void);
	const char* (*pfnGetGameDirectory)			(void);
	struct cvar_s* (*pfnGetCvarPointer)			(const char* szName);
	const char* (*Key_LookupBinding)			(const char* pBinding);
	const char* (*pfnGetLevelName)				(void);
	void				(*pfnGetScreenFade)				(struct screenfade_s* fade);
	void				(*pfnSetScreenFade)				(struct screenfade_s* fade);
	void* (*VGui_GetPanel)				(void);
	void				(*VGui_ViewportPaintBackground)	(int extents[4]);

	byte* (*COM_LoadFile)					(char* path, int usehunk, int* pLength);
	char* (*COM_ParseFile)				(char* data, char* token);
	void				(*COM_FreeFile)					(void* buffer);

	struct triangleapi_s* pTriAPI;
	struct efx_api_s* pEfxAPI;
	struct event_api_s* pEventAPI;
	struct demo_api_s* pDemoAPI;
	struct net_api_s* pNetAPI;
	struct IVoiceTweak_s* pVoiceTweak;

	// returns 1 if the client is a spectator only (connected to a proxy), 0 otherwise or 2 if in dev_overview mode
	int					(*IsSpectateOnly)				(void);
	struct model_s* (*LoadMapSprite)				(const char* filename);

	// file search functions
	void				(*COM_AddAppDirectoryToSearchPath) (const char* pszBaseDir, const char* appName);
	int					(*COM_ExpandFilename)			(const char* fileName, char* nameOutBuffer, int nameOutBufferSize);

	// User info
	// playerNum is in the range (1, MaxClients)
	// returns NULL if player doesn't exit
	// returns "" if no value is set
	const char* (*PlayerInfo_ValueForKey)		(int playerNum, const char* key);
	void				(*PlayerInfo_SetValueForKey)	(const char* key, const char* value);

	// Gets a unique ID for the specified player. This is the same even if you see the player on a different server.
	// iPlayer is an entity index, so client 0 would use iPlayer=1.
	// Returns false if there is no player on the server in the specified slot.
	int(*GetPlayerUniqueID)			(int iPlayer, char playerID[16]);

	// TrackerID access
	int					(*GetTrackerIDForPlayer)		(int playerSlot);
	int					(*GetPlayerForTrackerID)		(int trackerID);

	// Same as pfnServerCmd, but the message goes in the unreliable stream so it can't clog the net stream
	// (but it might not get there).
	int					(*pfnServerCmdUnreliable)		(char* szCmdString);

	void				(*pfnGetMousePos)				(struct tagPOINT* ppt);
	void				(*pfnSetMousePos)				(int x, int y);
	void				(*pfnSetMouseEnable)			(int fEnable);

	// undocumented interface starts here
	struct cvar_s* (*pfnGetCvarList)				(void);
	struct cmd_s* (*pfnGetCmdList)				(void);

	char* (*pfnGetCvarName)				(struct cvar_s* cvar);
	char* (*pfnGetCmdName)				(struct cmd_s* cmd);

	float				(*pfnGetClientOldTime)			(void);
	float				(*pfnGetGravity)				(void);
	struct model_s* (*pfnGetModelByIndex)			(int index);
	void				(*pfnSetFilterMode)				(int mode); // same as gl_texsort in original Quake
	void				(*pfnSetFilterColor)			(float red, float green, float blue);
	void				(*pfnSetFilterBrightness)		(float brightness);

	//this will always fail with the current engine
	void* (*pfnSequenceGet)				(const char* fileName, const char* entryName);

	void				(*pfnSPR_DrawGeneric)			(int frame, int x, int y, const s_rect* prc, int blendsrc, int blenddst, int width, int height);

	//this will always fail with engine, don't call
	//it actually has paramenters but i dunno what they do
	void* (*pfnSequencePickSentence)		(const char* groupName, int pickMethod, int* entryPicked);

	//localizes hud string, uses Legacy font from skin def
	// also supports unicode strings
	int					(*pfnDrawString)				(int x, int y, const char* str, int r, int g, int b);

	//i can't get this to work for some reason, don't use this
	int					(*pfnDrawStringReverse)			(int x, int y, const char* str, int r, int g, int b);

	//gets keyvalue for local player, useful for querying vgui menus or autohelp
	const char* (*LocalPlayerInfo_ValueForKey)	(const char* key);

	//another vgui2 text drawing function, i dunno how it works
	//it doesn't localize though
	int					(*pfnVGUI2DrawCharacter)		(int x, int y, int ch, unsigned int font);
	int					(*pfnVGUI2DrawCharacterAdditive) (int x, int y, int ch, int r, int g, int b, unsigned int font);

	//checks sound header of a sound file, determines if its a supported type
	unsigned int		(*pfnGetApproxWavePlayLen)		(char* filename);

	//for condition zero, returns interface from GameUI
	void* (*GetCareerGameUI)				(void);	// g-cont. !!!! potential crash-point!

	void				(*Cvar_Set)						(char* name, char* value);

	//this actually checks for if the CareerGameInterface is found
	//and if a server is being run
	int					(*pfnIsPlayingCareerMatch)		(void);

	void				(*pfnPlaySoundVoiceByName)		(char* szSound, float volume, int pitch);

	void				(*pfnPrimeMusicStream)			(char* filename, int looping);

	//get the systems current time as a float
	double				(*pfnSys_FloatTime)				(void);

	void				(*pfnProcessTutorMessageDecayBuffer) (int* buffer, int buflen);
	void				(*pfnConstructTutorMessageDecayBuffer) (int* buffer, int buflen);
	void				(*pfnResetTutorMessageDecayData) (void);

	void				(*pfnPlaySoundByNameAtPitch)	(char* szSound, float volume, int pitch);

	void				(*pfnFillRGBABlend)				(int x, int y, int width, int height, int r, int g, int b, int a);
	int					(*pfnGetAppID)					(void);
	cmdalias_t* (*pfnGetAliases)				(void);
	void				(*pfnVguiWrap2_GetMouseDelta)	(int* x, int* y);

} cl_enginefunc_t;

} // namespace sdk