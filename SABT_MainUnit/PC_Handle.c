#include "Globals.h"

bool PC_parse_message()
{
	unsigned char MessageType;
	USART_PC_Message_ready=false;
	MessageType=USART_PC_ReceivedPacket[2];
	switch(MessageType)
	{
		case PC_CMD_INIT:
			USART_transmitStringToPCFromFlash(PSTR("SABT-v2.1"));
			TX_NEWLINE_PC;			
			break;
		case PC_CMD_NEWMODES:
			PC_RequestsToModifyModesFile();
			break;
	}
}

void PC_RequestsToModifyModesFile(void)
{
	const char* ModesFile="MODES.DAT";
	unsigned char WritingFileContent[20];
	int iT=0;
	for(iT=0;iT<20;iT++)
	{
		WritingFileContent[iT]=0x00;
	}
	InitSDCard(false);
	iT=0;
	for(iT=3;iT<USART_PC_received_playload_len;iT++)
	{
		WritingFileContent[iT-3]=USART_PC_ReceivedPacket[iT];
	}
	if(ReplaceTheContentOfThisFileWith(ModesFile,WritingFileContent)==0)
	{
		USART_transmitStringToPCFromFlash(PSTR("SABT-OK"));
		TX_NEWLINE_PC;			
	}else{
		USART_transmitStringToPCFromFlash(PSTR("SABT-FAIL"));
		TX_NEWLINE_PC;	
	}
}
