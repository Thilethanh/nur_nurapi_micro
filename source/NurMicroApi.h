/* 
  Copyright � 2001-2008 Nordic ID 
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

#ifndef _NURMICROAPI_H_
#define _NURMICROAPI_H_ 1

#ifndef _NURAPICONFIG_H_
	#define HAVE_NUR_MEMSET
	#define HAVE_NUR_STRNCPY
	#define HAVE_NUR_MEMCPY
	#define IMPLEMENT_CRC16
	#define CONFIG_GENERIC_READ
	#define CONFIG_GENERIC_WRITE
#endif

#define _UNUSED(_uuVarName)	(void)_uuVarName

#ifdef _WIN32
	#include <Windows.h>
	typedef ULONGLONG QWORD;
#else
	typedef unsigned long long ULONGLONG;
	typedef unsigned long long QWORD;
	typedef unsigned long ULONG;
	typedef unsigned int DWORD;
	typedef unsigned short WORD;
	typedef unsigned char BYTE;
	typedef unsigned int BOOL;
	typedef void * LPVOID;
	#define TRUE 1
	#define FALSE 0
#endif

#ifdef HAVE_NUR_MEMSET
#ifdef __cplusplus 
extern "C" { 
#endif
void *nurMemset(void *destination, int val, int num);
#ifdef __cplusplus 
}
#endif
#else
	#define nurMemset	memset
#endif

#ifdef HAVE_NUR_MEMCPY
#ifdef __cplusplus 
extern "C" { 
#endif
void *nurMemcpy(void *destination, const void *source, int num);
#ifdef __cplusplus 
}
#endif
#else
	#define nurMemcpy	memcpy
#endif

#ifdef HAVE_NUR_STRNCPY
#ifdef __cplusplus 
extern "C" { 
#endif
char *nurStrncpy(char *destination, const char *source, int num);
#ifdef __cplusplus 
}
#endif
#else
#define nurStrncpy	strncpy
#endif

#ifdef __cplusplus 
extern "C" { 
#endif

/** @addtogroup MicroAPI
 *  @{
 */

/** Maximum length of EPC data in bytes. Increased to 64 so that placed inventory read data will fit into EPC buffer too. */
#define NUR_MAX_EPC_LENGTH		(62)

/** Maximum length of EPC data in bytes. Increased to 64 so that placed inventory read data will fit into EPC buffer too. */
#define NUR_MAX_EPC_LENGTH_EX	(64)

/** Maximum length for accompanied inventory read data. */
#define NUR_MAX_IRDATA_LENGTH   (64)

/** Maximum length of the EPC enumeration response's EPC content. */
#define MAX_EE_EPCLEN			16

/** Maximum length of the EPC enumeration response's TID content. */
#define MAX_EE_TIDLEN			16

/** Maximum length of select mask in bytes. */
#define NUR_MAX_SELMASK			(62)

/** Default NUR module baudrate. */
#define NUR_DEFAULT_BAUDRATE	(115200)

/** Maximum number of GPIOs. */
#define NUR_MAX_GPIO			(7)

/** Maximum number of regions. */
#define NUR_MAX_CONFIG_REGIONS  16

/** Maximum number of invetoryex filters. */
#define NUR_MAX_FILTERS			(8)

/** Maximum number of custom frequencies in custom hop table. */
#define NUR_MAX_CUSTOM_FREQS	100

/** Maximum number of TX bits in the custom bit stream. */
#define NUR_MAX_BITS_IN_STREAM	1024

/** Size reserved for NUR device capabilities structure. */
#define SZ_NUR_DEVCAPS			128

/** Number of bands the antenna tuning uses. */
#define NR_TUNEBANDS			6

/**
 * NUR module baudrate indices.
 * @sa NurApiGetBaudrate(), NurApiSetBaudrate()
 */
enum NUR_BAUDRATE
{
	NUR_BR_115200 = 0,	/**< 115200 bps. */
	NUR_BR_230400,		/**< 230400 bps. */
	NUR_BR_500000,		/**< 500000 bps. */
	NUR_BR_1000000,		/**< 1000000 bps. */
	NUR_BR_1500000,		/**< 1500000 bps (use with care). */
	NUR_BR_38400		/**< 38400 bps. */
};
	
/**
 * Log levels
 * @sa NurApiSetLogLevel(), NurApiGetLogLevel()
 */
enum NUR_LOG
{
	NUR_LOG_VERBOSE = (1<<0),	/**< Verbose log. */
	NUR_LOG_ERROR = (1<<1),		/**< Error log. */
	NUR_LOG_USER = (1<<2),		/**< User log. */
	NUR_LOG_DATA = (1<<3),		/**< Data log. */
	NUR_LOG_ALL = (NUR_LOG_VERBOSE | NUR_LOG_ERROR | NUR_LOG_USER | NUR_LOG_DATA) /**< All flags log combined. */
};

/**
  * Filter types for Broadcast messaging
  * @sa NurApiSendBroadcast()
*/
enum NUR_BC_FILTER_TYPE
{
	NUR_BC_FILTER_TYPE_NONE = 0,	/**< No filter */
	NUR_BC_FILTER_TYPE_MAC,			/**< Broadcast filter type MAC address (12 hexstring like: "0021ad0a0007)" */
	NUR_BC_FILTER_TYPE_IP,			/**< IP address (x.x.x.x) */
	NUR_BC_FILTER_TYPE_VERSION,		/**< Version (Numeric) */
	NUR_BC_FILTER_TYPE_SERVERPORT,	/**< Server port(Numeric) */
	NUR_BC_FILTER_TYPE_ADDRTYPE,	/**< Address type (0=DHCP 1=static) */
	NUR_BC_FILTER_TYPE_MODE,		/**< Host mode (0=server 1=client) */
	NUR_BC_FILTER_TYPE_NURVER,		/**< Nur version (Numeric) */
	NUR_BC_FILTER_TYPE_STATUS,		/**< Status (0=Disconnected 1=Connected) */
	NUR_BC_FILTER_TYPE_NAME			/**< Name (part of name string for filtering with device name) */
};

/**
  * Filter operations for Broadcast messaging
  * @sa NurApiSendBroadcast()
*/
enum NUR_BC_FILTER_OP
{
	NUR_BC_FILTER_OP_EQUAL = 0,	/**< EQUAL */
	NUR_BC_FILTER_OP_HIGHER,	/**< HIGHER */
	NUR_BC_FILTER_OP_LOWER		/**< LOWER */
};

/**
  * Broadcast commands.
  * @sa NurApiSendBroadcast(), NUR_NOTIFICATION_DEVSEARCH, NUR_NETDEV_INFO
*/
enum NUR_BC_CMD
{
	NUR_BC_GET_DEV_INFO	= 1,	/**< Request for device info. Sampo response with notification NUR_NOTIFICATION_DEVSEARCH*/
	NUR_BC_GET_ETHCONFIG,		/**< NOT USED */
	NUR_BC_GET_GPIO,			/**< NOT USED */
	NUR_BC_SET_ETHCONFIG,		/**< Set Ethernet configuration */
	NUR_BC_BEEP					/**< Generates beep */
};

/**
	* Transport method when sending ethernet configuration to device.
	* See Struct NUR_ETHDEV_CONFIG
*/
enum NUR_SETCONFIG_TRANSPORT_METHOD
{
	NUR_TRANSPORT_NUR = 0,		/**< Transport will be via currently connected NUR device */
	NUR_TRANSPORT_BROADCAST		/**< Transport is Broadcast and filtering is set to MAC address */
};



/**
 * EPC memory banks for singulation, read and write operations.
 */
enum NUR_BANK
{
	NUR_BANK_PASSWD = 0,	/**< Password memory bank. */
	NUR_BANK_EPC,			/**< EPC memory bank. */
	NUR_BANK_TID,			/**< TID memory bank. */
	NUR_BANK_USER			/**< User memory bank. */
};

/**
 * Action parameter for NurApiSetLock() and NurApiSetLockByEPC() functions.
 * @sa NurApiSetLock(), NurApiSetLockByEPC()
 */
enum NUR_LOCKACTION
{
	NUR_LOCK_OPEN = 0,		/**< Associated memory bank is readable from open/secured states. */
	NUR_LOCK_PERMAWRITE,	/**< Associated memory bank is permanently writable from open/secured states and may never be locked. */
	NUR_LOCK_SECURED,		/**< Associated memory bank is writable only from secured state. */
	NUR_LOCK_PERMALOCK		/**< Associated memory bank is not writable from any state. If locked memory is AccessPwd or KillPwd memory is not readable from any state. */
};

/**
 * Memory mask parameter for NurApiSetLock() and NurApiSetLockByEPC() functions.
 * @sa NurApiSetLock(), NurApiSetLockByEPC()
 */
enum NUR_LOCKMEM
{
	NUR_LOCK_USERMEM	= (1<<0),	/**< User memory bank lock mask. */
	NUR_LOCK_TIDMEM		= (1<<1),	/**< TID memory bank lock mask. */
	NUR_LOCK_EPCMEM		= (1<<2),	/**< EPC memory bank lock mask. */
	NUR_LOCK_ACCESSPWD	= (1<<3),	/**< Access password memory lock mask. */
	NUR_LOCK_KILLPWD	= (1<<4)	/**< Kill password memory lock mask. */
};

/**
 * Flags parameter for NurApiTraceTag32() function.
 * @sa NurApiTraceTag32(), NUR_NOTIFICATION_TRACETAG
 */
enum NUR_TRACETAG
{
	NUR_TRACETAG_NO_EPC				= (1<<0), /**< Do not transfer EPC back from trace tag function. */
	NUR_TRACETAG_START_CONTINUOUS	= (1<<1), /**< Start continuous tag tracing. */
	NUR_TRACETAG_STOP_CONTINUOUS	= (1<<3), /**< Stop continuous tag tracing. */
};

/**
 * Flags parameter for custom bit stream command.
 * @sa NurApiCustomBitStreamSingulated32(), NurApiCustomBitStreamByEPC, NurApiCustomBitStream32
 */
enum NUR_CUSTXCHGFLAGS
{
	CXF_ASWRITE     = (1<<0),     /**< "Act if this was a write operation" */
	CXF_USEHANDLE   = (1<<1),     /**< RN16 resulting from sinulation shall be appended to the bit stream */
	CXF_XORRN16     = (1<<2),     /**< if CXF_ASWRITE == '1' and TX bit length == 16, XOR the TX data with RN16 received from the tag access. */
	CXF_TXONLY      = (1<<3),     /**< Transmit only. No response is expected. */
	CXF_NOTXCRC     = (1<<4),     /**< No TX CRC. */
	CXF_NORXCRC     = (1<<5),     /**< Do not decode RX CRC, return the response as is. */
	CXF_CRC5        = (1<<6),     /**< TX uses CRC-5 instead of CRC-16. */
	CXF_NORXLEN     = (1<<7),     /**< Unknown RX length. RX length is ignored. */
	CXF_STRIPHND    = (1<<8),		/**< Leave out the bacscattered handle in the response. */
	CXF_SKIPRESEL   = (1<<9),     /**< Skip tag re-selection during the custom exchange. */
};

/**
 * Action field for struct NUR_GPIO_ENTRY or NUR_SENSOR_CONFIG.
 * @sa struct NUR_GPIO_ENTRY, NUR_GPIO_CONFIG, NUR_SENSOR_CONFIG, NurApiSetGPIOConfig(), NurApiGetGPIOConfig(), NUR_NOTIFICATION_IOCHANGE, NUR_NOTIFICATION_TRIGGERREAD
 */
enum NUR_GPIO_ACTION
{
	NUR_GPIO_ACT_NONE = 0,		/**< No action. GPIO state can be read manually. */
	NUR_GPIO_ACT_NOTIFY,		/**< Send NUR_NOTIFICATION_IOCHANGE notification on GPIO/sensor change. */
	NUR_GPIO_ACT_SCANTAG,		/**< Start single tag scan on GPIO/sensor change. Tag result is received with NUR_NOTIFICATION_TRIGGERREAD notification. */
	NUR_GPIO_ACT_INVENTORY		/**< Start inventory on GPIO/sensor change. Result is received with NUR_NOTIFICATION_INVENTORYSTREAM notification. */
};

/**
 * Edge field for struct NUR_GPIO_ENTRY
 * @sa struct NUR_GPIO_ENTRY, NUR_GPIO_CONFIG, NurApiSetGPIOConfig(), NurApiGetGPIOConfig()
 */
enum NUR_GPIO_EDGE
{
	NUR_GPIO_EDGE_FALLING = 0,	/**< Trigger IO's configured action on falling edge of IO */
	NUR_GPIO_EDGE_RISING,		/**< Trigger IO's configured action on rising edge of IO */
	NUR_GPIO_EDGE_BOTH			/**< Trigger IO's configured action on both edges of IO */
};

/** Type of the GPIO
 * @sa struct NUR_GPIO_ENTRY, NUR_GPIO_CONFIG, NurApiSetGPIOConfig(), NurApiGetGPIOConfig()
 */
enum NUR_GPIO_TYPE
{
	NUR_GPIO_TYPE_OUTPUT = 0,	/**< GPIO is configured as output pin. */
	NUR_GPIO_TYPE_INPUT,		/**< GPIO is configured as input pin. */
	NUR_GPIO_TYPE_RFIDON,		/**< GPIO will act as a Sampo S1 RFID on led (high active) */
	NUR_GPIO_TYPE_RFIDREAD,		/**< GPIO will act as a Sampo S1 RFID read led (high active) */
	NUR_GPIO_TYPE_BEEPER,		/**< GPIO will act as a beeper (high active) */
	NUR_GPIO_TYPE_ANTCTL1,		/**< GPIO is configured as antenna control 1 (bit0) */
	NUR_GPIO_TYPE_ANTCTL2,		/**< GPIO is configured as antenna control 2 (bit1) */
	NUR_GPIO_TYPE_DCE_RTS
};

/**
 * Module General Purpose IO (GPIO) numbers
 * @sa struct NUR_GPIO_STATUS, NurApiSetGPIOStatus(), NurApiGetGPIOStatus()
 */
enum NUR_GPIO
{
	NUR_GPIO1 = 0,	/**< Module GPIO 1 */
	NUR_GPIO2,		/**< Module GPIO 2 */
	NUR_GPIO3,		/**< Module GPIO 3 */
	NUR_GPIO4,		/**< Module GPIO 4 */
	NUR_GPIO5		/**< Module GPIO 5 */
};

/**
 * Region ID's indices
 * @sa NurApiGetRegionInfo(), struct NUR_REGIONINFO, struct NUR_MODULESETUP
 */
enum NUR_REGIONID
{
	NUR_REGIONID_EU = 0,		/**< Europe */
	NUR_REGIONID_FCC,			/**< North-America */
	NUR_REGIONID_PRC,			/**< People's Republic of China (Upper Band) */
	NUR_REGIONID_MALAYSIA,		/**< Malaysia */
	NUR_REGIONID_BRAZIL,		/**< Brazil */
	NUR_REGIONID_AUSTRALIA,		/**< Australia */
	NUR_REGIONID_NEWZEALAND,	/**< New Zealand */
	NUR_REGIONID_JA250MW,		/**< Japan 250mW LBT (NUR Fw 2.4-A or later) */
	NUR_REGIONID_JA500MW,		/**< Japan 500mW DRM (NUR Fw 2.4-A or later) */
	NUR_REGIONID_KOREA_LBT,		/**< Korea LBT (NUR Fw 2.7-A or later) */
	NUR_REGIONID_INDIA, 		/**< India (NUR Fw 2.7-A or later) */
	NUR_REGIONID_RUSSIA, 		/**< Russia (NUR Fw 3.0-A or later) */
	NUR_REGIONID_VIETNAM, 		/**< Vietnam (NUR Fw 3.0-A or later) */
	NUR_REGIONID_SINGAPORE,		/**< Singapore (NUR Fw 3.0-A or later) */
	NUR_REGIONID_THAILAND,		/**< Thailand (NUR Fw 3.0-A or later) */
	NUR_REGIONID_PHILIPPINES,	/**< Philippines (NUR Fw 3.0-A or later) */
	NUR_REGIONID_CUSTOM = 0xFE,	/**< Custom hop table */
	NUR_REGIONID_LAST
};

/**
 * RX coding indices
 * @sa struct NUR_MODULESETUP
 */
enum NUR_RXDECODING
{
	NUR_RXDECODING_FM0 = 0,		/**< FM-0 */
	NUR_RXDECODING_M2,  		/**< Miller-2 */
	NUR_RXDECODING_M4,  		/**< Miller-4 */
	NUR_RXDECODING_M8,  		/**< Miller-8 */
	NUR_RXDECODING_LAST
};

/**
 * TX modulation indices
 * @sa struct NUR_MODULESETUP
 */
enum NUR_TXMODULATION
{
	NUR_TXMODULATION_ASK = 0,	/**< ASK */
	NUR_TXMODULATION_PRASK, 	/**< PR-ASK */
	NUR_TXMODULATION_LAST
};

/**
 * Zero based antenna ID definitions.
 * @sa NurApiGetAntenna(), NurApiSetAntenna(), NurApiGetAntennaMask()
 */
enum NUR_ANTENNAID
{
	NUR_ANTENNAID_AUTOSELECT = -1, /**< Auto switch antenna based on supported antenna count. */
	NUR_ANTENNAID_1 = 0,	/**< Antenna ID 1. */
	NUR_ANTENNAID_2,		/**< Antenna ID 2. */
	NUR_ANTENNAID_3,		/**< Antenna ID 3. */
	NUR_ANTENNAID_4,		/**< Antenna ID 4. */
	NUR_MAX_ANTENNAS		/**< Maximum number of antennas. */
};

/**
 * Bit masks for antenna mask.
 * @sa NurApiGetAntenna(), NurApiSetAntenna(), NurApiGetAntennaMask()
 */
enum NUR_ANTENNAMASK
{
	NUR_ANTENNAMASK_1 = (1<<NUR_ANTENNAID_1),	/**< Mask for antenna ID 1. */
	NUR_ANTENNAMASK_2 = (1<<NUR_ANTENNAID_2),	/**< Mask for antenna ID 2. */
	NUR_ANTENNAMASK_3 = (1<<NUR_ANTENNAID_3),	/**< Mask for antenna ID 3. */
	NUR_ANTENNAMASK_4 = (1<<NUR_ANTENNAID_4),	/**< Mask for antenna ID 4. */
	NUR_ANTENNAMASK_ALL = (NUR_ANTENNAMASK_1 | NUR_ANTENNAMASK_2 | NUR_ANTENNAMASK_3 | NUR_ANTENNAMASK_4) /**< All antenna mask ids combined. */
};

/**
 * NUR_AUTOPERIOD defines hot the periodic auto-inventory is configured.
 */
enum NUR_AUTOPERIOD
{
	NUR_AUTOPER_OFF	= 0,		/**< Autoperiod not in use */
	NUR_AUTOPER_25,			/**< 25% cycle. */
	NUR_AUTOPER_33,			/**< 33% cycle. */
	NUR_AUTOPER_50,			/**< 50/50 cycle. */
	NUR_AUTOPER_LAST = NUR_AUTOPER_50
};

/**
 * Setup flags for NurApiSetModuleSetup() function.
 * @sa NurApiSetModuleSetup(), NurApiGetModuleSetup(), struct NUR_MODULESETUP
 */
enum NUR_MODULESETUP_FLAGS
{
	NUR_SETUP_LINKFREQ		= (1<<0),	/**< linkFreq field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_RXDEC			= (1<<1),	/**< rxDecoding field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_TXLEVEL		= (1<<2),	/**< txLevel field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_TXMOD			= (1<<3),	/**< txModulation field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_REGION		= (1<<4),	/**< regionId field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_INVQ			= (1<<5),	/**< inventoryQ field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_INVSESSION	= (1<<6),	/**< inventorySession field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_INVROUNDS		= (1<<7),	/**< inventoryRounds field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_ANTMASK		= (1<<8),	/**< antennaMask field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_SCANSINGLETO	= (1<<9),	/**< scanSingleTriggerTimeout field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_INVENTORYTO	= (1<<10),	/**< inventoryTriggerTimeout field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_SELECTEDANT	= (1<<11),	/**< selectedAntenna field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_OPFLAGS		= (1<<12),	/**< opFlags field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_INVTARGET		= (1<<13),	/**< inventoryTarget field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_INVEPCLEN		= (1<<14),	/**< inventoryEpcLength field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_READRSSIFILTER	= (1<<15), /**< readRssiFilter field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_WRITERSSIFILTER	= (1<<16), /**< writeRssiFilter field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_INVRSSIFILTER = (1<<17), /**< inventoryRssiFilter field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_READTIMEOUT	= (1<<18), /**< readTO field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_WRITETIMEOUT	= (1<<19), /**< writeTO field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_LOCKTIMEOUT	= (1<<20), /**< lockTO field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_KILLTIMEOUT	= (1<<21), /**< killTO field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_AUTOPERIOD	= (1<<22), /**< stixPeriod field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_PERANTPOWER	= (1<<23), /**< antPower field in struct NUR_MODULESETUP is valid */
	NUR_SETUP_PERANTOFFSET	= (1<<24), /**< powerOffset field in struct NUR_MODULESETUP is valid */

	NUR_SETUP_ALL		= (NUR_SETUP_LINKFREQ | NUR_SETUP_RXDEC | NUR_SETUP_TXLEVEL | NUR_SETUP_TXMOD | NUR_SETUP_REGION |
							NUR_SETUP_INVQ | NUR_SETUP_INVSESSION | NUR_SETUP_INVROUNDS | NUR_SETUP_ANTMASK |
							NUR_SETUP_SCANSINGLETO | NUR_SETUP_INVENTORYTO | NUR_SETUP_SELECTEDANT |
							NUR_SETUP_OPFLAGS | NUR_SETUP_INVTARGET | NUR_SETUP_INVEPCLEN |
							NUR_SETUP_READRSSIFILTER | NUR_SETUP_WRITERSSIFILTER | NUR_SETUP_INVRSSIFILTER |
							NUR_SETUP_READTIMEOUT | NUR_SETUP_WRITETIMEOUT | NUR_SETUP_LOCKTIMEOUT | NUR_SETUP_KILLTIMEOUT |
							NUR_SETUP_AUTOPERIOD | NUR_SETUP_PERANTPOWER | NUR_SETUP_PERANTOFFSET)	/**< All fields in struct NUR_MODULESETUP are valid */
};

/** Possible inventory targets.
 * @sa NurApiSetModuleSetup(), NurApiGetModuleSetup(), struct NUR_MODULESETUP
 */
enum NUR_INVENTORY_TARGET
{
	NUR_INVTARGET_A = 0,	/**< Query tags with inventoried flag set to A */
	NUR_INVTARGET_B,		/**< Query tags with inventoried flag set to B */
	NUR_INVTARGET_AB		/**< Query tags with inventoried flag set to A or B */
};

/** Possible G2 query command sel states. See UHF G2 standard section 6.3.2.11.2.1
 * @sa NurApiInventoryEx(), NurApiStartInventoryEx(), struct NUR_INVEX_PARAMS
 */
enum NUR_INVENTORY_SELSTATE
{
	NUR_SELSTATE_ALL = 0,		/**< All tags respond, ignoring SL flag */
	NUR_SELSTATE_NOTSL = 2,		/**< Only tags with SL deasserted responds */
	NUR_SELSTATE_SL = 3			/**< Only tags with SL asserted responds */
};

/** Possible target sessions for inventory or inventory filters. */
enum NUR_INVENTORY_SESSION
{
	NUR_SESSION_S0 = 0,	/**< Session 0 */
	NUR_SESSION_S1,		/**< Session 1 */
	NUR_SESSION_S2,		/**< Session 2 */
	NUR_SESSION_S3,		/**< Session 3 */
	NUR_SESSION_SL,		/**< SL flag */
};

/** Possible filter actions for extended inventory. */
enum NUR_FILTER_ACTION
{
	NUR_FACTION_0 = 0,	/**< Matching tags: assert SL or inventoried session flag -> A. Non-matching: deassert SL or inventoried session flag -> B. */
	NUR_FACTION_1,		/**< Matching tags: assert SL or inventoried session flag -> A. Non-matching: do nothing. */
	NUR_FACTION_2,		/**< Matching tags: do nothing. Non-matching: deassert SL or inventoried session -> B. */
	NUR_FACTION_3,		/**< Matching tags: negate SL or invert inventoried session flag (A->B, B->A). Non-matching: do nothing. */
	NUR_FACTION_4,		/**< Matching tags: deassert SL or inventoried session flag -> B. Non-matching: assert SL or inventoried session flag -> A. */
	NUR_FACTION_5,		/**< Matching tags: deassert SL or inventoried session flag -> B. Non-matching: do nothing. */
	NUR_FACTION_6,		/**< Matching tags: do nothing. Non-matching: assert SL or inventoried session flag -> A. */
	NUR_FACTION_7,		/**< Matching tags: do nothing. Non-matching: negate SL or invert inventoried session flag (A->B, B->A). */
};

/**
 * Defines inventory read type.
 * @sa NurApiConfigInventoryRead()
 */
enum NUR_IRTYPE
{
	NUR_IR_EPCDATA = 0,     /**< EPC + data combined, strip by EPC */
	NUR_IR_DATAONLY,			/**< Duplicates are stripped based on data */
	NUR_IR_LAST = NUR_IR_DATAONLY
};

/** Operation flags.
 * @sa NurApiSetModuleSetup(), NurApiGetModuleSetup(), struct NUR_MODULESETUP
 */
enum NUR_OPFLAGS
{
	NUR_OPFLAGS_EN_HOPEVENTS = (1<<0),		/**< Notification NUR_NOTIFICATION_HOPEVENT is enabled. */
	NUR_OPFLAGS_INVSTREAM_ZEROS = (1<<1),	/**< Inventory stream frunction will report zero count inventory rounds also. */
	NUR_OPFLAGS_INVENTORY_TID = (1<<2),
	NUR_OPFLAGS_INVENTORY_READ = (1<<3),
	/* Keyboard : scan single -> key presses */
	NUR_OPFLAGS_SCANSINGLE_KBD	= (1<<4),
	NUR_OPFLAGS_STANDALONE_APP1	= (1<<5),
	NUR_OPFLAGS_STANDALONE_APP2	= (1<<6),
	NUR_OPFLAGS_EXTIN_EVENTS 	= (1<<7),
	// Ext out lines 0-3 can be set to predefined state after boot.
	NUR_OPFLAGS_STATE_EXTOUT_0 	= (1<<8),
	NUR_OPFLAGS_STATE_EXTOUT_1 	= (1<<9),
	NUR_OPFLAGS_STATE_EXTOUT_2 	= (1<<10),
	NUR_OPFLAGS_STATE_EXTOUT_3 	= (1<<11)
};

/**
 * Defines which settings to store module internal non-volatile memory.
 * @sa NurApiStoreCurrentSetupEx()
 */
enum NUR_STORE_FLAGS
{
	NUR_STORE_RF = (1<<0),			/**< Store RF settings */
	NUR_STORE_GPIO = (1<<1),		/**< Store GPIO/Sensor settings */
	NUR_STORE_BAUDRATE = (1<<2),	/**< Store Baudrate setting */
	NUR_STORE_OPFLAGS = (1<<3),		/**< Store OpFlags setting */
	NUR_STORE_ALL = (NUR_STORE_RF | NUR_STORE_GPIO | NUR_STORE_BAUDRATE | NUR_STORE_OPFLAGS) /**< Store all settings */
};


/**
 * Defines error flags in the custom hoptable set command.
 * @sa NurApiBuildCustomHoptable(), NurApiSetCustomHoptable(), NurApiSetCustomHoptableEx(), NurApiLoadHopTable()
 */
enum NUR_CUSTHOP_ERROR
{
	NUR_CHERR_COUNT = (1<<0),
	NUR_CHERR_CHTIME = (1<<1),
	NUR_CHERR_PAUSETIME = (1<<2),
	NUR_CHERR_LF = (1<<3),
	NUR_CHERR_TARI = (1<<4),
	NUR_CHERR_SIZE = (1<<5),
	NUR_CHERR_FREQ = (1<<6),
	NUR_CHERR_TXLIMIT = (1<<7)
};

/**
 * How a NUR binary file was recognized.
 * @sa NurApiGetImageInfo, struct NUR_IMAGE_INFO
 */
enum NUR_BINFILEFORMAT
{
	NUR_FILE_NONE = 0,		/**< Not recognized at all */
	NUR_FILE_NURLDR,		/**< NUR05W L1 bootloader. */
	NUR_FILE_NURAPP,		/**< NUR05W L1 application. */
	NUR_FILE_XNURLDR,		/**< XNUR bootloader. */
	NUR_FILE_XNURAPP,		/**< XNUR application. */
	NUR_FILE_NUR05WL2LDR,	/**< NUR05WL2 bootloader. */
	NUR_FILE_NUR05WL2APP	/**< NUR05WL2 application. */
};

/**
 * How the device capabilities are presented in the flag field 1.
 * @sa NurApiGetDeviceCaps, struct NUR_DEVICECAPS
 */
enum NUR_DEVCAPS_F1
{
	NUR_DC_RXDECFM0		= (1<<0),	/**< RX modulation FM0. */
	NUR_DC_RXDECM2		= (1<<1), 	/**< RX modulation Miller-2. */
	NUR_DC_RXDECM4		= (1<<2), 	/**< RX modulation Miller-4. */
	NUR_DC_RXDECM8		= (1<<3), 	/**< RX modulation Miller-8. */

	NUR_DC_RXLF40k		= (1<<4), 	/**< Backscatter LF of 40kHz. */
	NUR_DC_RXLF80k		= (1<<5), 	/**< Backscatter LF of 80kHz. */
	NUR_DC_RXLF160k		= (1<<6), 	/**< Backscatter LF of 160kHz. */
	NUR_DC_RXLF256k		= (1<<7), 	/**< Backscatter LF of 256kHz. */
	NUR_DC_RXLF320k		= (1<<8), 	/**< Backscatter LF of 320kHz. */
	NUR_DC_RXLF640k		= (1<<9), 	/**< Backscatter LF of 640kHz. */
	NUR_DC_RXLFres1		= (1<<10), 	/**< Reserved LF 1. */
	NUR_DC_RXLFres2		= (1<<11), 	/**< Reserved LF 2. */

	NUR_DC_HASBEEP		= (1<<12), 	/**< The device has beeper available. */
	NUR_DC_HASLIGHT		= (1<<13), 	/**< The device has light sensor available. */
	NUR_DC_HASTAP		= (1<<14), 	/**< The device has tap sensor available. */
	NUR_DC_ANTTUNE		= (1<<15), 	/**< The antenna or antennas in this device can be tuned. */
	NUR_DC_CHSCANNER	= (1<<16),	/**< This module can run channel scan. */
	NUR_DC_INVREAD		= (1<<17),	/**< This module can run inventory + read. */
	NUR_DC_LASTBITF1	= (1<<18)	/**< Next available bi for future extensions. */
};

/** Flag field 1 'all device caps' bitmask. */
#define NUR_DC_F1MASK	(NUR_DC_LASTBITF1-1)
/** Flag field 1 RX decoding mask. */
#define NUR_DC_RXDECMASK	(0x0F)
/** Flag field 1 'all RX decodings' mask. */
#define NUR_DC_RXDECALL		NUR_DC_RXDECMASK
/** Flag field 1 link frequency mask. */
#define NUR_DC_RXLFMASK		(0xFF << 4)
/** Flag field 1 sensor mask. */
#define NUR_DC_SENSOR_MASK	(0x03 << 13)

/**
 * RFID Chip version.
 * @sa NurApiGetDeviceCaps, struct NUR_DEVICECAPS
 */
enum NUR_CHIPVER
{
	/** Chip version AS3992 */
	NUR_CHIPVER_AS3992 = 1,
	/** Chip version AS3993 */
	NUR_CHIPVER_AS3993 = 2
};

/**
 * Module type.
 * @sa NurApiGetDeviceCaps, struct NUR_DEVICECAPS
 */
enum NUR_MODULETYPE
{
	/** Module type NUR05W */
	NUR_MODULETYPE_NUR05W = 1,
	/** Module type NUR05WL */
	NUR_MODULETYPE_NUR05WL = 2,
	/** Module type NUR05WL2 */
	NUR_MODULETYPE_NUR05WL2 = 3
};


/**
 * NurApi error codes
 */
enum NUR_ERRORCODES
{
	/** Call succeeded; Same as NUR_NO_ERROR  */
	NUR_SUCCESS = 0,
	/** Call succeeded; Same as NUR_SUCCESS */
	NUR_NO_ERROR = NUR_SUCCESS,
	/** Invalid command sent to module */
	NUR_ERROR_INVALID_COMMAND,
	/** Invalid packet length sent to module */
	NUR_ERROR_INVALID_LENGTH,
	/** Command parametr(s) out of range */
	NUR_ERROR_PARAMETER_OUT_OF_RANGE,
	/** Data receive timeout */
	NUR_ERROR_RECEIVE_TIMEOUT,
	/** Invalid command parameter(s); Invalid function parameter(s) */
	NUR_ERROR_INVALID_PARAMETER,
	/** Programming failure */
	NUR_ERROR_PROGRAM_FAILED,
	/** Parameter mismatch */
	NUR_ERROR_PARAMETER_MISMATCH,
	/** HW mismatch */
	NUR_ERROR_HW_MISMATCH,
	NUR_ERROR_RESERVED1,
	/** Page programming failure */
	NUR_ERROR_PAGE_PROGRAM,
	/** Memory check failed */
	NUR_ERROR_CRC_CHECK,
	/** CRC mismatch in parameter */
	NUR_ERROR_CRC_MISMATCH,
	/** Device not ready or region that is being programmed is not unlocked */
	NUR_ERROR_NOT_READY,
	/** Module application not present */
	NUR_ERROR_APP_NOT_PRESENT,

	/** Generic, non-interpreted / unexpected error */
	NUR_ERROR_GENERAL = 0x10,

	/** No tag(s) found */
	NUR_ERROR_NO_TAG = 0x20,
	/** Air error */
	NUR_ERROR_RESP_AIR,
	/** G2 select error */
	NUR_ERROR_G2_SELECT,
	/** G2 select data missing */
	NUR_ERROR_MISSING_SELDATA,
	/** G2 access error */
	NUR_ERROR_G2_ACCESS,

	/** G2 Read error, unspecified */
	NUR_ERROR_G2_READ = 0x30,
	/** G2 Partially successful read */
	NUR_ERROR_G2_RD_PART,
	/** G2 Write error, unspecified */
	NUR_ERROR_G2_WRITE = 0x40,
	/** G2 Partially successful write */
	NUR_ERROR_G2_WR_PART,
	/** G2 Tag read responded w/ error */
	NUR_ERROR_G2_TAG_RESP,

	/** Special error; Some additional debug data is returned with this error */
	NUR_ERROR_G2_SPECIAL = 0x50,

	/** HW error */
	NUR_ERROR_READER_HW = 0x60,

	/** Invalid handle passed to function */
	NUR_ERROR_INVALID_HANDLE = 0x1000,
	/** Transport error */
	NUR_ERROR_TRANSPORT,
	/** Transport not connected */
	NUR_ERROR_TR_NOT_CONNECTED,
	/** Transport timeout */
	NUR_ERROR_TR_TIMEOUT,
	/** Buffer too small */
	NUR_ERROR_BUFFER_TOO_SMALL,
	/** Functionality not supported */
	NUR_ERROR_NOT_SUPPORTED,
	/** Packet contains no payload */
	NUR_ERROR_NO_PAYLOAD,
	/** Packet is invalid */
	NUR_ERROR_INVALID_PACKET,
	/** Packet too long */
	NUR_ERROR_PACKET_TOO_LONG,
	/** Packet Checksum failure */
	NUR_ERROR_PACKET_CS_ERROR,
	/** Data not in WORD boundary */
	NUR_ERROR_NOT_WORD_BOUNDARY,
	/** File not found */
	NUR_ERROR_FILE_NOT_FOUND,
	/** File error; not in NUR format */
	NUR_ERROR_FILE_INVALID,
	/** NUR file and module's MCU architecture mismatch  */
	NUR_ERROR_MCU_ARCH,

	/** The specified memory location does not exists or the EPC length field is not supported by the tag */
	NUR_ERROR_G2_TAG_MEM_OVERRUN,
	/** The specified memory location is locked and/or permalocked and is either not writeable or not readable */
	NUR_ERROR_G2_TAG_MEM_LOCKED,
	/** The tag has insufficient power to perform the memory-write operation */
	NUR_ERROR_G2_TAG_INSUF_POWER,
	/** The tag does not support error-specific codes */
	NUR_ERROR_G2_TAG_NON_SPECIFIC,
	/** Transport suspended error */
	NUR_ERROR_TR_SUSPENDED,
	/** TCP/IP Server error */
	NUR_ERROR_SERVER,
};

/** @} */ // end of API

#define NUR_API
#define NURAPICONV

#include "NurProtocol.h"

#define NurLog do { } while(0)
//#define LOGIFERROR(x) if ((x) != NUR_NO_ERROR) printf("ERROR %d @ %s %d\n", (x), __FILE__, __LINE__);
//#define RETLOGERROR(x) { printf("ERROR %d @ %s %d\n", (x), __FILE__, __LINE__); return (x); }
#define LOGIFERROR(x) do { } while(0)
#define RETLOGERROR(x) do { } while(0)


struct NUR_API_HANDLE;

typedef int (*pTransportReadDataFunction)(struct NUR_API_HANDLE *hNurApi, BYTE *buffer, DWORD bufferLen, DWORD *bytesRead);
typedef int (*pTransportWriteDataFunction)(struct NUR_API_HANDLE *hNurApi, BYTE *buffer, DWORD bufferLen, DWORD *bytesWritten);

typedef void (*pUnsolEventHandler)(struct NUR_API_HANDLE *hNurApi);

typedef int (*pFetchTagsFunction)(struct NUR_API_HANDLE *hNurApi, struct NUR_IDBUFFER_ENTRY *tag);

struct NUR_API_HANDLE
{
	void *UserData;

	/* 
		Transport: 
		Implement the read & write functionality.
		Physical transport can be anything that Sampo or NUR module currently supports: UART, USB or Ethernet.
	*/
	pTransportReadDataFunction TransportReadDataFunction;
	pTransportWriteDataFunction TransportWriteDataFunction;
	
	/*
		Packet handler can call this funtion to pass events to an application.
	*/
	pUnsolEventHandler UnsolEventHandler;
	
	BYTE *TxBuffer;
	DWORD TxBufferLen;

	BYTE *RxBuffer;
	DWORD RxBufferLen;
	DWORD RxBufferUsed;

	DWORD respLen;
	struct NUR_CMD_RESP *resp;
};

#ifdef HAVE_ERROR_MESSAGES
NUR_API const char * NURAPICONV NurApiGetErrorMessage(int error);
#endif

NUR_API int NURAPICONV NurApiSetupPacket(struct NUR_API_HANDLE *hNurApi, BYTE cmd, WORD payloadLen, WORD flags, WORD *packetLen);
NUR_API int NURAPICONV NurApiXchPacket(struct NUR_API_HANDLE *hNurApi, BYTE cmd, WORD payloadLen, int timeout);

NUR_API int NURAPICONV NurApiPing(struct NUR_API_HANDLE *hNurApi);
NUR_API int NURAPICONV NurApiWaitEvent(struct NUR_API_HANDLE *hNurApi, int timeout);
NUR_API int NURAPICONV NurApiGetReaderInfo(struct NUR_API_HANDLE *hNurApi);
NUR_API int NURAPICONV NurApiGetVersions(struct NUR_API_HANDLE *hApi);
NUR_API int NURAPICONV NurApiGetRegionInfo(struct NUR_API_HANDLE *hNurApi, BYTE regionId);
NUR_API int NURAPICONV NurApiStoreCurrentSetup(struct NUR_API_HANDLE *hNurApi, BYTE flags);
NUR_API int NURAPICONV NurApiSetBaudrate(struct NUR_API_HANDLE *hNurApi, BYTE setting);
NUR_API int NURAPICONV NurApiGetBaudrate(struct NUR_API_HANDLE *hNurApi);
NUR_API int NURAPICONV NurApiGetFWINFO(struct NUR_API_HANDLE *hNurApi, char *buf, WORD buflen);
NUR_API int NURAPICONV NurApiSetModuleSetup(struct NUR_API_HANDLE *hNurApi, struct NUR_CMD_LOADSETUP_PARAMS *params);
NUR_API int NURAPICONV NurApiGetModuleSetup(struct NUR_API_HANDLE *hNurApi, DWORD setupFlags);
NUR_API int NURAPICONV NurApiGetDeviceCaps(struct NUR_API_HANDLE *hNurApi);

NUR_API int NURAPICONV NurApiInventory(struct NUR_API_HANDLE *hNurApi,
							   struct NUR_CMD_INVENTORY_PARAMS *params);
NUR_API int NURAPICONV NurApiInventoryEx(struct NUR_API_HANDLE *hNurApi,
										struct NUR_CMD_INVENTORYEX_PARAMS *params);
NUR_API int NURAPICONV NurApiFetchTags(struct NUR_API_HANDLE *hNurApi, BOOL includeMeta, BOOL clearModuleTags, int *tagsReceived, pFetchTagsFunction tagFunc);
NUR_API int NURAPICONV NurApiFetchTagAt(struct NUR_API_HANDLE *hNurApi, BOOL includeMeta, int tagNum, pFetchTagsFunction tagFunc);
NUR_API int NURAPICONV NurApiClearTags(struct NUR_API_HANDLE *hNurApi);

int NURAPICONV NurApiTraceTag(struct NUR_API_HANDLE *hNurApi, struct NUR_CMD_TRACETAG_PARAMS *params);

#ifdef CONFIG_GENERIC_READ
NUR_API int NURAPICONV NurApiReadTag(struct NUR_API_HANDLE *hNurApi, struct NUR_CMD_READ_PARAMS *params,  BYTE *rdBuffer);
#endif

#ifdef CONFIG_GENERIC_WRITE
NUR_API int NURAPICONV NurApiWriteTag(struct NUR_API_HANDLE *hNurApi, struct NUR_CMD_WRITE_PARAMS *params);
#endif

int NURAPICONV NurApiScanSingle(struct NUR_API_HANDLE *hNurApi, WORD timeout);

int NURAPICONV NurApiTuneAntenna(struct NUR_API_HANDLE *hNurApi, int antenna, BOOL wideTune, BOOL bSaveResults, int *dBmResults);

int NURAPICONV NurApiSetGPIOConfig(struct NUR_API_HANDLE *hNurApi, struct NUR_CMD_CONFIGGPIO_PARAMS *gpioParams);
int NURAPICONV NurApiGetGPIOConfig(struct NUR_API_HANDLE *hNurApi);
int NURAPICONV NurApiSetGPIOStatus(struct NUR_API_HANDLE *hNurApi, int gpio, BOOL state);
int NURAPICONV NurApiGetGPIOStatus(struct NUR_API_HANDLE *hNurApi, int gpio);

#ifndef IMPLEMENT_CRC16
extern WORD NurCRC16(WORD crc, BYTE *buf, DWORD len);
#endif

#ifdef __cplusplus 
}
#endif

/*
	How the unaligned structure members should be accessed.
*/
#ifdef CONFIG_MEMBER_ACCESS

#ifdef __cplusplus 
extern "C" { 
#endif

WORD GET_U16(void *wSrc);
DWORD GET_U32(void *dwSrc);
QWORD GET_U64(void *qwSrc);
void SET_U16(void *wDst, void *wSrc);
void SET_U32(void *dwDst, void *dwSrc);
void SET_U64(void *qwDst, void *qwSrc);

#ifdef __cplusplus 
extern }
#endif

#define GET_WORD(_wSrc)				GET_U16((void*)&(_wSrc))
#define GET_DWORD(_dwSrc)			GET_U32((void*)&(_dwSrc))
#define GET_QWORD(_qwSrc)			GET_U64((void*)&(_qwSrc))
#define SET_WORD(_wDst, _wSrc)		SET_U16((void *)&(_wDst), (void *)&(_wSrc))
#define SET_DWORD(_dwDst, _dwSrc)	SET_U32((void *)&(_dwDst), (void *)&(_dwSrc))
#define SET_QWORD(_qwDst, _qwSrc)	SET_U64((void *)&(_qwDst), (void *)&(_qwSrc))

#else	// Use member access

// Specific member access not used.
#define GET_WORD(_wSrc)		_wSrc
#define GET_DWORD(_dwSrc)	_dwSrc
#define GET_QWORD(_qwSrc)	_qwSrc
#define SET_WORD(_wDst, _wSrc)		wDst = _wSrc
#define SET_DWORD(_dwDst, _dwSrc)	_dwDst = _dwSrc
#define SET_QWORD(_qwDst, _qwSrc)	_qwDst = _qwSrc

#endif	

#endif