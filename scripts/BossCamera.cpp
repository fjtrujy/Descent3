/////////////////////////////////////////////////////////////////////
// D.A.L.L.A.S. Generated Level Script - DLL Source File
// 
// Filename:	BossCamera.cpp
// Version:	3
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "osiris_import.h"
#include "osiris_common.h"
#include "DallasFuncs.cpp"

#ifdef _MSC_VER		//Visual C++ Build
#define STDCALL		__stdcall
#define STDCALLPTR	*STDCALL
#else					//Non-Visual C++ Build
#define STDCALL __attribute__((stdcall))
#define STDCALLPTR	STDCALL*
#endif

#ifdef __cplusplus
extern "C"{
#endif
char		STDCALL InitializeDLL(tOSIRISModuleInit *func_list);
void		STDCALL ShutdownDLL(void);
int		STDCALL GetGOScriptID(char *name,ubyte is_door);
void		STDCALLPTR CreateInstance(int id);
void		STDCALL DestroyInstance(int id,void *ptr);
short	STDCALL CallInstanceEvent(int id,void *ptr,int event,tOSIRISEventInfo *data);
int		STDCALL GetTriggerScriptID(int trigger_room, int trigger_face );
int		STDCALL GetCOScriptList( int **list, int **id_list );
int		STDCALL SaveRestoreState( void *file_ptr, ubyte saving_state );
#ifdef __cplusplus
}
#endif

// =================
// Script ID Numbers
// =================
#define ID_LEVEL_0000			0x000


#define ID_TRIGGER_0000		0x001

// ========================
// Script Class Definitions
// ========================

class BaseScript {
public:
	BaseScript();
	~BaseScript();
	virtual short CallEvent(int event, tOSIRISEventInfo *data);
};

class LevelScript_0000 : public BaseScript {
public:
	short CallEvent(int event, tOSIRISEventInfo *data);
};

class TriggerScript_0000 : public BaseScript {
public:
	short CallEvent(int event, tOSIRISEventInfo *data);
};

// ======================
// Global Action Counters
// ======================

#define MAX_ACTION_CTR_VALUE		100000

int ScriptActionCtr_000 = 0;
 
// ========================================
// Function to Clear Global Action Counters
// ========================================
void ClearGlobalActionCtrs(void)
{
	ScriptActionCtr_000 = 0;
}
 
// ========================================
// Function to Save Global Action Counters
// ========================================
void SaveGlobalActionCtrs(void *file_ptr)
{
	File_WriteInt(ScriptActionCtr_000,file_ptr);
}
 
// ===========================================
// Function to Restore Global Action Counters
// ===========================================
void RestoreGlobalActionCtrs(void *file_ptr)
{
	ScriptActionCtr_000=File_ReadInt(file_ptr);
}
 
 
// ===============================================================
// Start of Custom Script Block - DO NOT EDIT ANYTHING BEFORE THIS
// ===============================================================
/**{CUSTOM_SCRIPT_BLOCK_START}** DO NOT EDIT! **/
 
// Enter your custom script code here
 
/**{CUSTOM_SCRIPT_BLOCK_END}**** DO NOT EDIT! **/
// ============================================================
// End of Custom Script Block - DO NOT EDIT ANYTHING AFTER THIS
// ============================================================
 
 
// =================
// Message File Data
// =================
 
#define MAX_SCRIPT_MESSAGES	256
#define MAX_MSG_FILEBUF_LEN	1024
#define NO_MESSAGE_STRING		"*Message Not Found*"
#define INV_MSGNAME_STRING	"*Message Name Invalid*"
#define WHITESPACE_CHARS	" \t\r\n"
 
// Structure for storing a script message
typedef struct {
	char *name;			// the name of the message
	char *message;		// the actual message text
} tScriptMessage;
 
// Global storage for level script messages
tScriptMessage *message_list[MAX_SCRIPT_MESSAGES];
int num_messages;
 
// ======================
// Message File Functions
// ======================
 
// Initializes the Message List
void InitMessageList(void)
{
	for(int j=0;j<MAX_SCRIPT_MESSAGES;j++)
		message_list[j]=NULL;
	num_messages=0;
}
 
// Clear the Message List
void ClearMessageList(void)
{
	for(int j=0;j<num_messages;j++) {
		free(message_list[j]->name);
		free(message_list[j]->message);
		free(message_list[j]);
		message_list[j]=NULL;
	}
	num_messages=0;
}
 
// Adds a message to the list
int AddMessageToList(char *name, char *msg)
{
	int pos;
 
	// Make sure there is room in the list
	if(num_messages>=MAX_SCRIPT_MESSAGES) return false;
 
	// Allocate memory for this message entry
	pos=num_messages;
	message_list[pos]=(tScriptMessage *)malloc(sizeof(tScriptMessage));
	if(message_list[pos]==NULL) return false;
 
	// Allocate memory for the message name
	message_list[pos]->name=(char *)malloc(strlen(name)+1);
	if(message_list[pos]->name==NULL) {
		free(message_list[pos]);
		return false;
	}
	strcpy(message_list[pos]->name,name);
 
	// Allocate memory for the message name
	message_list[pos]->message=(char *)malloc(strlen(msg)+1);
	if(message_list[pos]->message==NULL) {
		free(message_list[pos]->name);
		free(message_list[pos]);
		return false;
	}
	strcpy(message_list[pos]->message,msg);
	num_messages++;
 
	return true;
}
 
// Removes any whitespace padding from the end of a string
void RemoveTrailingWhitespace(char *s)
{
	int last_char_pos;
 
	last_char_pos=strlen(s)-1;
	while(last_char_pos>=0 && isspace(s[last_char_pos])) {
		s[last_char_pos]='\0';
		last_char_pos--;
	}
}
 
// Returns a pointer to the first non-whitespace char in given string
char *SkipInitialWhitespace(char *s)
{
	while((*s)!='\0' && isspace(*s)) 
		s++;
 
	return(s);
}
 
// Read in the Messages
int ReadMessageFile(char *filename)
{
	void *infile;
	char filebuffer[MAX_MSG_FILEBUF_LEN+1];
	char *line, *msg_start;
	int line_num;
	bool next_msgid_found;
 
	// Try to open the file for loading
	infile=File_Open(filename,"rt");
	if (!infile) return false;
 
	line_num=0;
	next_msgid_found=true;
 
	// Clear the message list
	ClearMessageList();
 
	// Read in and parse each line of the file
	while (!File_eof(infile)) {
 
		// Clear the buffer
		strcpy(filebuffer,"");
 
		// Read in a line from the file
		File_ReadString(filebuffer, MAX_MSG_FILEBUF_LEN, infile);
		line_num++;
 
		// Remove whitespace padding at start and end of line
		RemoveTrailingWhitespace(filebuffer);
		line=SkipInitialWhitespace(filebuffer);
 
		// If line is a comment, or empty, discard it
		if(strlen(line)==0 || strncmp(line,"//",2)==0)
			continue;
 
		if(!next_msgid_found) {		// Parse out the last message ID number
 
			// Grab the first keyword, make sure it's valid
			line=strtok(line,WHITESPACE_CHARS);
			if(line==NULL) continue;
 
			// Grab the second keyword, and assign it as the next message ID
			line=strtok(NULL,WHITESPACE_CHARS);
			if(line==NULL) continue;
 
			next_msgid_found=true;
		}
		else {	// Parse line as a message line
			
			// Find the start of message, and mark it
			msg_start=strchr(line,'=');
			if(msg_start==NULL) continue;
			msg_start[0]='\0';
			msg_start++;
			
			// Add the message to the list
			AddMessageToList(line,msg_start);
		}
	}
	File_Close(infile);
 
	return true;
}
 
// Find a message
char *GetMessage(char *name)
{
	// Make sure given name is valid
	if(name==NULL) return INV_MSGNAME_STRING;
 
	// Search message list for name
	for(int j=0;j<num_messages;j++)
		if(strcmp(message_list[j]->name,name)==0) return(message_list[j]->message);
 
	// Couldn't find it
	return NO_MESSAGE_STRING;
}
 
 
//======================
// Name List Arrays     
//======================
 
#define NUM_DOOR_NAMES		0
char **Door_names=NULL;
int *Door_handles=NULL;
 
#define NUM_OBJECT_NAMES		1
char *Object_names[NUM_OBJECT_NAMES] = {
	"Boss"
};
int Object_handles[NUM_OBJECT_NAMES];
 
#define NUM_ROOM_NAMES		0
char **Room_names=NULL;
int *Room_indexes=NULL;
 
#define NUM_TRIGGER_NAMES		1
char *Trigger_names[NUM_TRIGGER_NAMES] = {
	"EnterBigRoom"
};
int Trigger_indexes[NUM_TRIGGER_NAMES];
int Trigger_faces[NUM_TRIGGER_NAMES];
int Trigger_rooms[NUM_TRIGGER_NAMES];
 
#define NUM_SOUND_NAMES		0
char **Sound_names=NULL;
int *Sound_indexes=NULL;
 
#define NUM_TEXTURE_NAMES		0
char **Texture_names=NULL;
int *Texture_indexes=NULL;
 
#define NUM_PATH_NAMES		1
char *Path_names[NUM_PATH_NAMES] = {
	"BossPath"
};
int Path_indexes[NUM_PATH_NAMES];
 
#define NUM_MATCEN_NAMES		0
char **Matcen_names=NULL;
int *Matcen_indexes=NULL;
 
#define NUM_GOAL_NAMES		0
char **Goal_names=NULL;
int *Goal_indexes=NULL;
 
#define NUM_MESSAGE_NAMES		1
char *Message_names[NUM_MESSAGE_NAMES] = {
	"Message2"
};
char *Message_strings[NUM_MESSAGE_NAMES];
 
// ===============
// InitializeDLL()
// ===============
char STDCALL InitializeDLL(tOSIRISModuleInit *func_list)
{
	osicommon_Initialize((tOSIRISModuleInit *)func_list);
	if(func_list->game_checksum!=CHECKSUM)
	{ 
		mprintf(0,"Game-Checksum FAIL!!! (%ul!=%ul)\n",func_list->game_checksum,CHECKSUM);
		mprintf(0,"RECOMPILE YOUR SCRIPTS!!!\n");
		return 0;
	}
 
	ClearGlobalActionCtrs();
	dfInit();
	InitMessageList();
 
	// Build the filename of the message file
	char filename[_MAX_PATH+1];
	int lang_type;
	if(func_list->script_identifier!=NULL) {
		_splitpath(func_list->script_identifier,NULL,NULL,filename,NULL);
		lang_type=Game_GetLanguage();
		if(lang_type==LANGUAGE_FRENCH) strcat(filename,"_FRN");
		else if(lang_type==LANGUAGE_GERMAN) strcat(filename,"_GER");
		else if(lang_type==LANGUAGE_ITALIAN) strcat(filename,"_ITN");
		else if(lang_type==LANGUAGE_SPANISH) strcat(filename,"_SPN");
		strcat(filename,".msg");
	}
	else {
		strcpy(filename,"BossCamera.msg");
		lang_type=LANGUAGE_ENGLISH;
	}
	if(!ReadMessageFile(filename)) {
		mprintf(0,"ERROR: Could not load message file - %s\n",filename);
	}
 
	int j;
 
	// Do Door Index lookups
	for(j=0;j<NUM_DOOR_NAMES;j++) 
		Door_handles[j]=Scrpt_FindDoorName(Door_names[j]);
 
	// Do Object Index lookups
	for(j=0;j<NUM_OBJECT_NAMES;j++) 
		Object_handles[j]=Scrpt_FindObjectName(Object_names[j]);
 
	// Do Room Index lookups
	for(j=0;j<NUM_ROOM_NAMES;j++) 
		Room_indexes[j]=Scrpt_FindRoomName(Room_names[j]);
 
	// Do Trigger Index lookups
	for(j=0;j<NUM_TRIGGER_NAMES;j++) {
		Trigger_indexes[j]=Scrpt_FindTriggerName(Trigger_names[j]);
		Trigger_faces[j]=Scrpt_GetTriggerFace(Trigger_indexes[j]);
		Trigger_rooms[j]=Scrpt_GetTriggerRoom(Trigger_indexes[j]);
	}
 
	// Do Sound Index lookups
	for(j=0;j<NUM_SOUND_NAMES;j++) 
		Sound_indexes[j]=Scrpt_FindSoundName(Sound_names[j]);
 
	// Do Texture Index lookups
	for(j=0;j<NUM_TEXTURE_NAMES;j++) 
		Texture_indexes[j]=Scrpt_FindTextureName(Texture_names[j]);
 
	// Do Path Index lookups
	for(j=0;j<NUM_PATH_NAMES;j++) 
		Path_indexes[j]=Scrpt_FindPathName(Path_names[j]);
 
	// Do Matcen Index lookups
	for(j=0;j<NUM_MATCEN_NAMES;j++) 
		Matcen_indexes[j]=Scrpt_FindMatcenName(Matcen_names[j]);
 
	// Do Goal Index lookups
	for(j=0;j<NUM_GOAL_NAMES;j++) 
		Goal_indexes[j]=Scrpt_FindLevelGoalName(Goal_names[j]);
 
	// Do Message Name lookups
	for(j=0;j<NUM_MESSAGE_NAMES;j++) 
		Message_strings[j]=GetMessage(Message_names[j]);
 
	return 1;
}

// =============
// ShutdownDLL()
// =============
void STDCALL ShutdownDLL(void)
{
	ClearMessageList();
}

// ===============
// GetGOScriptID()
// ===============
int STDCALL GetGOScriptID(char *name,ubyte isdoor)
{
	return -1;
}

// ================
// CreateInstance()
// ================
void STDCALLPTR CreateInstance(int id)
{
	switch(id) {
		case ID_LEVEL_0000:
			return new LevelScript_0000;
			break;
		case ID_TRIGGER_0000:
			return new TriggerScript_0000;
			break;
		default:
			mprintf(0,"SCRIPT: Illegal ID (%d)\n",id);
			break;
	}
	return NULL;
}

// =================
// DestroyInstance()
// =================
void STDCALL DestroyInstance(int id,void *ptr)
{
	switch(id) {
		case ID_LEVEL_0000:
			delete ((LevelScript_0000 *)ptr);
		case ID_TRIGGER_0000:
			delete ((TriggerScript_0000 *)ptr);
			break;
		default:
			mprintf(0,"SCRIPT: Illegal ID (%d)\n",id);
			break;
	}
}

// ===================
// CallInstanceEvent()
// ===================
short STDCALL CallInstanceEvent(int id,void *ptr,int event,tOSIRISEventInfo *data)
{
	switch(id) {
		case ID_LEVEL_0000:
		case ID_TRIGGER_0000:
			return ((BaseScript *)ptr)->CallEvent(event,data);
			break;
		default:
			mprintf(0,"SCRIPT: Illegal ID (%d)\n",id);
			break;
	}
	return CONTINUE_CHAIN|CONTINUE_DEFAULT;
}

// ==================
// SaveRestoreState()
// ==================
int STDCALL SaveRestoreState( void *file_ptr, ubyte saving_state )
{
	return 0;
}

// ====================
// GetTriggerScriptID()
// ====================
int	STDCALL GetTriggerScriptID(int trigger_room,int trigger_face)
{
	if(trigger_room==Trigger_rooms[0] && trigger_face==Trigger_faces[0])
		return ID_TRIGGER_0000;
 
	return -1;
}

// =================
// GetCOScriptList()
// =================
int	STDCALL GetCOScriptList( int **list, int **id_list )
{
	static int *cust_handle_list=NULL;
	static int *cust_id_list=NULL;
 

	*list = cust_handle_list;
	*id_list = cust_id_list;

	return 0;
}

//=======================
// Script Implementation 
//=======================

BaseScript::BaseScript()
{
}

BaseScript::~BaseScript()
{
}

short BaseScript::CallEvent(int event,tOSIRISEventInfo *data)
{
	mprintf(0,"BaseScript::CallEvent()\n");
	return CONTINUE_CHAIN|CONTINUE_DEFAULT;
}

short LevelScript_0000::CallEvent(int event,tOSIRISEventInfo *data)
{
	switch(event) { 
		case EVT_SAVESTATE:
			{
				tOSIRISEVTSAVESTATE *event_data=&data->evt_savestate;
	
				SaveGlobalActionCtrs(event_data->fileptr);
				dfSave(event_data->fileptr);
			}
			break;
		case EVT_RESTORESTATE:
			{
				tOSIRISEVTRESTORESTATE *event_data=&data->evt_restorestate;
	
				RestoreGlobalActionCtrs(event_data->fileptr);
				dfRestore(event_data->fileptr);
			}
			break;
		case EVT_LEVELSTART:
			{
				tOSIRISEVTLEVELSTART *event_data=&data->evt_levelstart;
 
				ClearGlobalActionCtrs();
				dfInit();
			}
			break;
	}
	return CONTINUE_CHAIN|CONTINUE_DEFAULT;
}

short TriggerScript_0000::CallEvent(int event,tOSIRISEventInfo *data)
{
	switch(event) { 
		case EVT_COLLIDE:
			{
				tOSIRISEVTCOLLIDE *event_data=&data->evt_collide;
 
				// Script 000: Script Description Goes Here
				if(qObjIsPlayer(event_data->it_handle)==true)
				{
					aCinematicSimple(Path_indexes[0], Message_strings[0], Object_handles[0], 10.000000f, 1);
					
					// Increment the script action counter
					if(ScriptActionCtr_000 < MAX_ACTION_CTR_VALUE) ScriptActionCtr_000++;
				}
			}
			break;
	}
	return CONTINUE_CHAIN|CONTINUE_DEFAULT;
}

 
 
/*********************************************************
Script Save Block: DO NOT TOUCH ANYTHING IN THIS BLOCK!!! 
**********************************************************
$$SCRIPT_BLOCK_START
 
VERSION		3
NEXT_ID		1
 
// UserType value blocks
$$UTYPE_VALS_START UserVar
$$UTYPE_VALS_END
 
$$UTYPE_VALS_START UserFlag
$$UTYPE_VALS_END
 
$$UTYPE_VALS_START SpewHandle
$$UTYPE_VALS_END
 
$$UTYPE_VALS_START TimerID
$$UTYPE_VALS_END
 
$$UTYPE_VALS_START SavedObjectSlot
$$UTYPE_VALS_END
 
$$UTYPE_VALS_START GoalID
$$UTYPE_VALS_END
 
 
// Name Lists
$$DOOR_LIST_START
$$DOOR_LIST_END
 
$$OBJECT_LIST_START
Boss
$$OBJECT_LIST_END
 
$$ROOM_LIST_START
$$ROOM_LIST_END
 
$$TRIGGER_LIST_START
EnterBigRoom
$$TRIGGER_LIST_END
 
$$SOUND_LIST_START
$$SOUND_LIST_END
 
$$TEXTURE_LIST_START
$$TEXTURE_LIST_END
 
$$SPECNAME_LIST_START
$$SPECNAME_LIST_END
 
$$PATH_LIST_START
BossPath
$$PATH_LIST_END
 
$$MATCEN_LIST_START
$$MATCEN_LIST_END
 
$$GOAL_LIST_START
$$GOAL_LIST_END
 
$$STRM_AUDIO_LIST_START
$$STRM_AUDIO_LIST_END
 
 
// Script Tree Dump
00:0:Script Description Goes Here 
	$$CHILD_BLOCK_START
	01:0:0 
	02:1 
	03:0 
		$$CHILD_BLOCK_START
		05:1 
			$$CHILD_BLOCK_START
			06:qObjIsPlayer:Bool 
				$$CHILD_BLOCK_START
				10:1:1:-1:Object 
				$$CHILD_BLOCK_END
			07:0:6 
			$$CHILD_BLOCK_END
		$$CHILD_BLOCK_END
	04:0:0:0 
		$$CHILD_BLOCK_START
		08:aCinematicSimple 
			$$CHILD_BLOCK_START
			10:16:0:CameraPath 
			10:8:Message2:Text 
			10:1:0:0:Target 
			10:6:10.000000:Seconds 
			10:5:1:FadeIn 
			$$CHILD_BLOCK_END
		$$CHILD_BLOCK_END
	$$CHILD_BLOCK_END
 
$$SCRIPT_BLOCK_END
*********************************************************/
