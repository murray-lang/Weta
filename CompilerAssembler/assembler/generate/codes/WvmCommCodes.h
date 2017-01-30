#ifndef ____WVMCOMMCODES_H__
#define __WVMCOMMCODES_H__

enum eWvmCommCodes
{
	OP_COMM_SERIAL_CONFIG		= 0,
	OP_COMM_WIFI_CONFIG		= 1,
	OP_COMM_WIFI_CONFIG_IP		= 2,
	OP_COMM_WIFI_CONFIG_DNS		= 3,
	OP_COMM_WIFI_CONFIG_GATEWAY		= 4,
	OP_COMM_WIFI_CONFIG_MASK		= 5,
	OP_COMM_WIFI_SCAN		= 6,
	OP_COMM_WIFI_SSID		= 7,
	OP_COMM_WIFI_RSSI		= 8,
	OP_COMM_WIFI_BSSID		= 9,
	OP_COMM_WIFI_ENCRYPT		= 10,
	OP_COMM_WIFI_CONNECT		= 11,
	OP_COMM_WIFI_DISCONNECT		= 12,
	OP_COMM_WIFI_MAC		= 13,
	OP_COMM_WIFI_IP		= 14,
	OP_COMM_WIFI_MASK		= 15,
	OP_COMM_WIFI_GATEWAY		= 16,
	OP_COMM_WIFI_STATUS		= 17,
	OP_COMM_WIFI_CLIENT_CONNECT		= 18,
	OP_COMM_WIFI_CLIENT_DISCONNECT		= 19,
	OP_COMM_WIFI_CLIENT_CONNECTED		= 20,
	OP_COMM_WIFI_CLIENT_AVAILABLE		= 21,
	OP_COMM_WIFI_CLIENT_READ		= 22,
	OP_COMM_WIFI_CLIENT_WRITE		= 23,
	OP_COMM_WIFI_SERVER_START		= 24,
	OP_COMM_WIFI_SERVER_STOP		= 25,
	OP_COMM_WIFI_SERVER_LISTEN		= 26,
	OP_COMM_WIFI_SERVER_WRITE		= 27,
	OP_COMM_WIFI_SERVER_STATUS		= 28,
};

#endif