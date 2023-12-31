/**************************************************************************
	Souliss 
    Copyright (C) 2013  Veseo

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
	Originally developed by Dario Di Maio
	
***************************************************************************/
/*!
    \file 
    \ingroup
*/
/**************************************************************************/
#ifndef QUICKCONF_H
#define QUICKCONF_H

// Quick Configuration
//
//	The quick configuration allow a one step configuration for the whole
//	framework, is available for the supported board and their configurations.
// 	Using the quick configuation no other configuration is required, and
//	all the other configuration files will be ignored.
//
//	Is always possible to disable the quick configuration and define manually
//	the settings contained in the other configuration files.
//
//	By default quick configuration is disabled.

/**************************************************************************/
/*!
	Enable quick configuration, if enabled discard configuration parameters
	from other files and build the configuration only accordingly to details
	inserted in this file.
	
	If disabled, the configuration goes in Detailed Configuration mode
	        
		Value       
        0x00        Disabled (Default)
		0x01		Enabled
		
*/
/**************************************************************************/
#define	QC_ENABLE			0x01

/**************************************************************************/
/*!
	Select the used board type and the associated media interface (wireless,
	ethernet or WiFi), using this option no other configuration is required
	for standard nodes.
	
	Standard configuration is always possible
	        
		Value      
        0x00        No selection (Default)
		0x01		Freaklabs Chibiduino (2.4 GHz Wireless)
		0x02		Freaklabs Chibiduino with Ethernet Shield (W5100)
        0x03        Arduino Ethernet (W5100) 
		0x04		Arduino with Ethernet Shield (W5100)
		0x05		Arduino with ENC28J60 Ethernet Shield
		0x06		KMTronic DINo v1	
		0x07		Olimex AVR-T32U4 		with MOD-ENC28J60 (UEXT)
		0x08		Olimex OLIMEXINO-32U4 	with MOD-ENC28J60 (UEXT)
		0x09		Olimex OLIMEXINO-328 	with MOD-ENC28J60 (UEXT)
		0x0A		Olimex AVR-T32U4 		with MOD-WIFI	  (UEXT)	
		0x0B		Olimex OLIMEXINO-32U4 	with MOD-WIFI	  (UEXT)
		0x0C		Olimex OLIMEXINO-328 	with MOD-WIFI	  (UEXT)
		0x0D		Olimex AVR-T32U4 		with MOD-ENC28J60 and MOD-IO (UEXT)
		0x0E		Olimex OLIMEXINO-32U4 	with MOD-ENC28J60 and MOD-IO (UEXT)
		0x0F		Olimex OLIMEXINO-328 	with MOD-ENC28J60 and MOD-IO (UEXT)
		0x10		Olimex AVR-T32U4 		with MOD-WIFI	  and MOD-IO (UEXT)
		0x11		Olimex OLIMEXINO-32U4 	with MOD-WIFI	  and MOD-IO (UEXT)
		0x12		Olimex OLIMEXINO-328 	with MOD-WIFI	  and MOD-IO (UEXT)		
		0x13		Olimex AVR-T32U4 		with MOD-ENC28J60 and MOD-IO 2(UEXT)
		0x14		Olimex OLIMEXINO-32U4 	with MOD-ENC28J60 and MOD-IO 2(UEXT)
		0x15		Olimex OLIMEXINO-328 	with MOD-ENC28J60 and MOD-IO 2(UEXT)
		0x16		Olimex AVR-T32U4 		with MOD-WIFI	  and MOD-IO 2(UEXT)
		0x17		Olimex OLIMEXINO-32U4 	with MOD-WIFI	  and MOD-IO 2(UEXT)
		0x18		Olimex OLIMEXINO-328 	with MOD-WIFI	  and MOD-IO 2(UEXT)			
		0x19		Olimex AVR-T32U4 		with MOD-ENC28J60 and MOD-RGB(UEXT)
		0x1A		Olimex OLIMEXINO-32U4 	with MOD-ENC28J60 and MOD-RGB(UEXT)
		0x1B		Olimex OLIMEXINO-328 	with MOD-ENC28J60 and MOD-RGB(UEXT)
		0x1C		Olimex AVR-T32U4 		with MOD-WIFI	  and MOD-RGB(UEXT)
		0x1D		Olimex OLIMEXINO-32U4 	with MOD-WIFI	  and MOD-RGB(UEXT)
		0x1E		Olimex OLIMEXINO-328 	with MOD-WIFI	  and MOD-RGB(UEXT)	
		0x20		KMP Electronics DINo v2
		0x30		DFRobots XBoard Relay
		0x31		DFRobots XBoard
		0x40		Freaklabs Chibiduino with ENC28J60 Ethernet Shield
		
*/
/**************************************************************************/
#if(QC_ENABLE)
#	define	QC_BOARDTYPE			0x00
#endif
/**************************************************************************/
/*!
	Select the requested gateway type (if needed), a gateway node collects
	data from all the other in the network and make the same available to
	external interfaces (like Android, Modbus or HTTP/JSON).
	        
		Value       
        0x00        No selection (Default)
		0x01		Gateway
		0x02		Gateway with additional Modbus TCP
		0x03		Gateway with additional Modbus RTU
		0x04		Gateway with additional HTTP Commander
		
*/
/**************************************************************************/
#if(QC_ENABLE)
#	define	QC_GATEWAYTYPE			0x00
#endif

/**************************************************************************/
/*!
	The dynamic addressing node act as server for Souliss addresses over
	the network. It works for single media and bridged networks with a single
	bridge acting as addressing server.
	
        Value       DYNAMICADDRESSING
        0x0         Disable (Default)
        0x1         Enable	
*/
/**************************************************************************/
#if(QC_ENABLE)						// Define manually only in Detailed Configuration Mode
#	define DYNAMICADDRESSING  		0x00
#endif

/**************************************************************************/
/*!
	In case of a network with multiple Ethernet based boards, the use of 
	MAC RAW communication remove the need of a dedicated IP address for each
	board.
	
	This settings has effect only on Ethernet boards with DYNAMICADDRESSING
	option set, following scenario are forseen:
		- Gateway : Is configured with either IP and MACRAW, bridges data from
					IP to MACRAW and viceversa.
		- Peer (not Gateway) : Is configured only as MACRAW, doesn't need an IP
					address.
	
        Value       ETHERNETMACRAW
        0x0         Disable (Default)
        0x1         Enable	
*/
/**************************************************************************/
#if(QC_ENABLE)						// Define manually only in Detailed Configuration Mode
#	define ETHERNETMACRAW  		0x00
#endif

/**************************************************************************/
/*!
	Select the sensor used to include automatically proper drivers, multiple
	selection is allowed.
	        
		Value       
        0x00        No selection (Default)
		0x01		Selected
 
	List of available sensors and relevant description,
		
		Name		Description
		DHT_SENSOR	Temperature and humidity DHT11, DHT21, DHT22
 
*/
/**************************************************************************/
#if(QC_ENABLE)				
#	define	DHT_SENSOR				0x00
#endif

/**************************************************************************/
/*!
    IP Base Configuration 

	The IP address of Ethernet boards is defined as merge of a IP Base Address
	and the vNet address, to get this the DEFAULT_BASEIPADDRESS[] shall always
	have the last byte at zero, that byte is used to define the vNet address.
	
	Below are listed some example of valid and not valid configurations, the
	default configuration match the one used for most of home networks routers.
	
	Example of valid configuration are:
		- IP 192.168. 0.0  / SUBNETMASK 255.255.255.0
		- IP 192.168. 1.0  / SUBNETMASK 255.255.255.0
		- IP 192.168.10.0  / SUBNETMASK 255.255.255.0
	Example of wrong configuration are:	
		- IP 192.168. 0.12  / SUBNETMASK 255.255.255.0	(WRONG)

*/
/**************************************************************************/
#if(QC_ENABLE)
const uint8_t DEFAULT_BASEIPADDRESS[] = {192, 168, 1, 0};
const uint8_t DEFAULT_SUBMASK[] = {255, 255, 255, 0};
const uint8_t DEFAULT_GATEWAY[] = {192, 168, 1, 1};
#endif
/**************************************************************************/
/*!
	WiFi Configuration
	
	Define the configuration parameters for your wireless network connection,
	these are additional to the standard IP/MAC configuration.
	
	Firstly specify the connection mode using following values, never select 
	more than one driver per time
	        
		Value       Action
        0x0         Disable (Default)
        0x1         Enable
	
		Mode						Description
		Infrastracture				WiFi module connect to a router
		AdHoc						Peer-to-peer connection between two modules
		
	The network protection type suggested is WPA2, if connecting to a router
	refer to router configuration for used protection type. If using an AdHoc
	mode, the configuration parameters set here shall be used by the other
	peer to perform the connection.
	
	The network name and password shall be the one configured into the router
	if using an Infrastracture connection, otherwise can be freely defined
	and then used by the other peer in case of AdHoc connection.
	
*/
/**************************************************************************/
#if(QC_ENABLE)
// Connection mode
#	define	WiFi_Infrastracture		1
#	define	WiFi_AdHoc				0

// Network protection type
#	define	WiFi_OpenNetwork		0	
#	define	WiFi_WEP				0					// Please avoid the use of WEP
#	define	WiFi_WPA				0
#	define	WiFi_WPA2				1

// Network name and password
#	define	WiFi_SSID				"mywifi"
#	define	WiFi_Password			"mypassword"	
#endif


#endif