/* 
  Copyright � 2014 Nordic ID 
  NORDIC ID DEMO SOFTWARE DISCLAIMER

  You are about to use Nordic ID Demo Software ("Software"). 
  It is explicitly stated that Nordic ID does not give any kind of warranties, 
  expressed or implied, for this Software. Software is provided "as is" and with 
  all faults. Under no circumstances is Nordic ID liable for any direct, special, 
  incidental or indirect damages or for any economic consequential damages to you 
  or to any third party.

  The use of this software indicates your complete and unconditional understanding 
  of the terms of this disclaimer. 
  
  IF YOU DO NOT AGREE OF THE TERMS OF THIS DISCLAIMER, DO NOT USE THE SOFTWARE.  
*/

#define _CRT_SECURE_NO_WARNINGS	1

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "NurMicroApi.h"

BOOL open_serial(struct NUR_API_HANDLE *hApi, int number, DWORD baudrate);
void close_serial();

static BOOL gConnected = FALSE;

// The API's communication buffers.
static BYTE gRxBuffer[NUR_MAX_RCV_SZ];
static BYTE gTxBuffer[NUR_MAX_SEND_SZ];

/*
	In this approach we are using a static API structure.
*/
static struct NUR_API_HANDLE gApi =
{
	NULL,	// void *UserData;
	NULL,	// TransportReadDataFunction
	NULL,	// TransportWriteDataFunction
	NULL,	// UnsolEventHandler;
	
	NULL,	// BYTE *TxBuffer;
	0,		// DWORD TxBufferLen;

	NULL,	//BYTE *RxBuffer;
	0,		// DWORD RxBufferLen;
	0,		// DWORD RxBufferUsed;

	0,		// DWORD respLen;
	NULL	// struct NUR_CMD_RESP *resp;
};

static struct NUR_API_HANDLE *hApi = &gApi;

int InitNurApiHandle(struct NUR_API_HANDLE *hApi)
{
	// Init RX buffer
	hApi->RxBuffer = gRxBuffer;
	hApi->RxBufferLen = sizeof(gRxBuffer);

	// Init TX buffer
	hApi->TxBuffer = gTxBuffer;
	hApi->TxBufferLen = sizeof(gTxBuffer);

	return hApi->RxBufferLen;
}

void wait_key()
{
	printf("Hit any key...");
	_getch();
	printf("\n");
}

void cls()
{
	system("cls");
}

static void handle_connection()
{
	if (gConnected) {
		close_serial();
		gConnected = FALSE;
	}
	else 
	{
		int num = -1;
		printf("Enter COM number (baud 115200): " );
		if (scanf("%d", &num) == 1) {
			if (open_serial(hApi, num, 115200)) {
				gConnected = TRUE;
				printf("Connected.\n");
			}
			else {
				printf("Connection error\n");
				wait_key();
			}
		}
		else {
			printf("Canceled.\n");
			wait_key();
		}
	}
}

static void handle_versions()
{
	struct NUR_CMD_VERSION_RESP *vr;
	int rc;

	if (!gConnected)
		return;
	cls();

	rc = NurApiGetVersions(hApi);
	vr = &hApi->resp->versions;

	if (rc == NUR_SUCCESS) {
		printf("Versions, mode = '%c':\n", vr->mode);
		printf(" - primary   : %d.%d-%c\n", vr->vMajor, vr->vMinor, vr->vBuild);
		printf(" - secondary : %d.%d-%c\n", vr->otherMajor, vr->otherMinor, vr->otherBuild);
	}
	else {
		printf("Version error. Code = %d.\n", rc);
	}

	wait_key();
}

static void handle_readerinfo()
{
	struct NUR_CMD_READERINFO_RESP *ri;
	char tmpStr[50];
	int rc;

	if (!gConnected)
		return;
	cls();

	rc = NurApiGetReaderInfo(hApi);
	
	if (rc == NUR_SUCCESS) { 		
		ri = &hApi->resp->readerinfo;
		printf("* Reader information *\n");

		// Name
		memcpy(tmpStr, ri->name, ri->nameLen);
		tmpStr[ri->nameLen] = 0;
		printf("Name: %s\n", tmpStr);
		// Serial
		memcpy(tmpStr, ri->serial, ri->serialLen);
		tmpStr[ri->serialLen] = 0;
		printf("Serial: %s\n", tmpStr);
		// FCC ID
		memcpy(tmpStr, ri->fccId, ri->fccIdLen);
		tmpStr[ri->fccIdLen] = 0;
		printf("FCC ID: %s\n", tmpStr);
		// Antennas
		printf("# antennas: %d\n", ri->numAntennas);
		// Regions
		printf("# regions: %d\n", ri->numRegions);
		// GPIO
		printf("# GPIOs: %d\n", ri->numGpio);
		// Sensors
		printf("# sensors: %d\n", ri->numSensors);
	}
	else {
		printf("NUR reader info error. Code = %d.\n", rc);
	}

	wait_key();
}

static void handle_ping()
{
	int rc;
	if (!gConnected)
		return;

	cls();
	printf("* Ping *\n");
	rc = NurApiPing(hApi);
	if (rc == NUR_SUCCESS)
		printf("Ping was OK.\n");
	else
		printf("Ping error. Code = %d.\n", rc);

	wait_key();
}

static char *RxDecToStr(int rxd)
{
	static char buf[50];

	switch (rxd)
	{
		case 0: return "FM0";
		case 1:
		case 2:
		case 3: 
		sprintf(buf, "M-%d", (1<<rxd));
		break;
		default:
		sprintf(buf, "unknown RX decoding (%d)", rxd);
		break;
	}

	return buf;
}

static void handle_setup_get()
{
	struct NUR_CMD_LOADSETUP_PARAMS *setup;
	int flags, rc;
	
	if (!gConnected)
		return;

	flags = NUR_SETUP_TXLEVEL | NUR_SETUP_LINKFREQ | NUR_SETUP_RXDEC;

	cls();
	printf("* Get basic setup *\n");
	rc = NurApiGetModuleSetup(hApi, flags);
	if (rc == NUR_SUCCESS) {
		setup = &hApi->resp->loadsetup;
		printf("Setup:\n");
		printf("TX level: %d\n", setup->txLevel);
		printf("Link frequency: %d\n", GET_DWORD(setup->linkFreq));
		printf("RX decoding: %s\n", RxDecToStr(setup->rxDecoding));
	}
	else
		printf("GetModuleSetup error. Code = %d.\n", rc);

	wait_key();
}

static void handle_setup_set_txlevel()
{
	struct NUR_CMD_LOADSETUP_PARAMS params;
	int txlevel, rc;
	
	if (!gConnected)
		return;

	cls();
	printf("Set TX level (0 - 19): ");
	if (scanf("%d", &txlevel) == 1) 
	{
		params.flags = NUR_SETUP_TXLEVEL;
		params.txLevel = txlevel;

		rc = NurApiSetModuleSetup(hApi, &params);

		if (rc == NUR_SUCCESS) {
			printf("OK\n");
		}
		else
		{
			printf("SetModuleSetup error. Code = %d.\n", rc);
		}
	}
	else 
	{
		printf("Invalid input\n");
	}

	wait_key();
}

int FetchTagsFunction(struct NUR_API_HANDLE *hNurApi, struct NUR_IDBUFFER_ENTRY *tag)
{
	int n;
	printf("Antenna %d, RSSI = %d (%d%%) ", tag->antennaId, tag->rssi, tag->scaledRssi);
	printf("EPC[%d]: ", tag->epcLen);

	for (n=0; n<tag->epcLen; n++) {
		printf("%02X", tag->epcData[n]);
	}
	printf("\n");

	return NUR_SUCCESS; // non-zero terminates tag buffer parsing
}

static void handle_inventory()
{
	int rc;

	if (!gConnected)
		return;

	cls();
	printf("* Inventory *\n");

	// Clear tag buffer
	rc = NurApiClearTags(hApi);
	if (rc == NUR_SUCCESS) 
	{
		rc = NurApiInventory(hApi, NULL); // Pass NULL as params, uses default inventory settings from module setup
		if (rc == NUR_SUCCESS) 
		{
			int tagCount;
#if 1
			// Fetch tags one by one (needs less memory)
			int n;
			tagCount = hApi->resp->inventory.numTagsMem;
			for (n=0; n<tagCount; n++)
			{
				rc = NurApiFetchTagAt(hApi, TRUE, n, FetchTagsFunction);
				if (rc != NUR_SUCCESS) {
					break;
				}
			}
#else
			// Fetch all tags at once (needs more memory)
			rc = NurApiFetchTags(hApi, TRUE, TRUE, &tagCount, FetchTagsFunction);
#endif
			printf("\nInventory done; total %d tags\n", tagCount);
		}
		else {
			printf("Inventory error. Code = %d.\n", rc);
		}
	}
	else
	{
		printf("ClearTags error. Code = %d.\n", rc);
	}

	wait_key();
}

static void handle_tune()
{
	int rc, ant, i;
	int res[NR_TUNEBANDS];

	if (!gConnected)
		return;

	cls();
	printf("* Antenna Tune *\n");
	printf(" NOTE: Make sure antenna is in open space\n\n");

	printf("Enter Antenna ID (0 = first): " );
	if (scanf("%d", &ant) == 1) 
	{
		printf("Tune in progress, may take up to 25 secs\n");
		rc = NurApiTuneAntenna(hApi, ant, TRUE, TRUE, res);

		if (rc == NUR_SUCCESS) {
			printf("Tune OK\n");
			for (i=0; i<NR_TUNEBANDS; i++)
			{
				printf(" Band[%d] = %.2f dBm\n", i, (float)res[i]/1000.0f);
			}
			printf("\n");
		}
		else
		{
			printf("Tune error. Code = %d.\n", rc);
		}
	}
	else 
	{
		printf("Invalid input\n");
	}

	wait_key();
}

static void options()
{
	if (gConnected)
		printf("[1]\tDisconnect\n");
	else
		printf("[1]\tConnect\n");

	if (gConnected)
	{
		printf("[2]\tPing\n");
		printf("[3]\tVersions\n");
		printf("[4]\tReader info\n");		
		printf("[5]\tGet setup\n");
		printf("[6]\tInventory\n");
		printf("[7]\tTune\n");
		printf("[8]\tSet TX Level\n");
	}
	printf("\nESC\tExit\n");

	printf("\nSelection: ");
}

static BOOL do_command()
{
	int key;
	options();
	key = _getch();
	printf("\n");

	if (key == 27)
		return FALSE;

	switch (key)
	{
		case '1': handle_connection(); break;
		case '2': handle_ping(); break;
		case '3': handle_versions(); break;
		case '4': handle_readerinfo(); break;		
		case '5': handle_setup_get(); break;
		case '6': handle_inventory(); break;
		case '7': handle_tune(); break;
		case '8': handle_setup_set_txlevel(); break;			

		default: break;
	}

	cls();
	return TRUE;
}

int main(int argc, char *argv[])
{
	InitNurApiHandle(hApi);

	while (do_command());

	close_serial();

	return 0;
}