//#define  FTD2XX_STATIC	// for static library usage
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>		// for sleep() function
#include <windows.h>	// for windows specific keywords in ftd2xx.h
#include "../Header/ftd2xx.h"	// Header file for ftd2xx.lib

int main()
{
	/*
	 * These two lines solve the eclipse IDE bug that its console output window can not display strings by printf()
	 * After adding these two lines, the console output window can display string infos.
	 */
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	FT_HANDLE ft_handle;  // handle to the USB2Serial IC FT232RQ
	FT_STATUS ft_status;  // for status code (error, IO status etc)

	BYTE i = 0;
	BYTE tx_buffer[55] = "This is a program for testing MM232R module. 20180613";
	DWORD tx_buffer_size = sizeof(tx_buffer);
	DWORD num_of_bytes_written = 0;		// The number of bytes written to the MM232R module by the FT_Write()

	BYTE rx_buffer[55] = {0};
	DWORD rx_buffer_size = sizeof(rx_buffer);
	DWORD num_of_bytes_read = 0;	// The number of bytes read from the MM232R module by the FT_Read()

	DWORD BytesInRxQueue = 0;
	DWORD BytesInTxQueue = 0;
	DWORD EventStatus = 0;

	ft_status = FT_Open(0, &ft_handle); //open a connection with MM232R module
	if ( !FT_SUCCESS(ft_status) )	//error checking
	{
		printf("\n\tConnection with FT232RQ failed with error code: %lu", ft_status);
		printf("\n\tCheck device connection !");
		printf("\n\tPress enter key to EXIT!");
		getchar();
		return EXIT_FAILURE;
	}
	printf("\n\tConnection with FT232RQ successful!");
	printf("\n\tPress enter key to CONFIGURE the MM232R module!");
	getchar();

	ft_status = FT_SetBaudRate(ft_handle, FT_BAUD_9600);
	if ( !FT_SUCCESS(ft_status) )
	{
		printf("\n\tSetting baud rate failed with error code: %lu", ft_status);
		printf("\n\tPress enter key to EXIT!");
		getchar();
		return EXIT_FAILURE;
	}

	ft_status = FT_SetDataCharacteristics(ft_handle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
	if ( !FT_SUCCESS(ft_status) )
	{
		printf("\n\tSetting data characteristics failed with error code: %lu", ft_status);
		printf("\n\tPress enter key to EXIT!");
		getchar();
		return EXIT_FAILURE;
	}

	ft_status = FT_SetFlowControl(ft_handle, FT_FLOW_NONE, 0x11, 0x13);
	if ( !FT_SUCCESS(ft_status) )
	{
		printf("\n\tSetting flow control failed with error code: %lu", ft_status);
		printf("\n\tPress enter key to EXIT!");
		getchar();
		return EXIT_FAILURE;
	}
	printf("\n\tSetting FT232RQ successful!");

	printf("\n\tPress enter key to DISPLAY the status in MM232R!");
	getchar();
	ft_status = FT_GetStatus(ft_handle, &BytesInRxQueue, &BytesInTxQueue, &EventStatus);
	if ( !FT_SUCCESS(ft_status) )
	{
		printf("\n\tGetting the device status failed with error code: %lu", ft_status);
	}
	printf("\n\tBefore TX:");
	printf("\n\tNum of bytes in RX queue = %ld", BytesInRxQueue);
	printf("\n\tNum of bytes in TX queue = %ld", BytesInTxQueue);
	printf("\n\tEvent Status = %ld", EventStatus);

	printf("\n");
	printf("\n\tPress enter key to TRANSMIT data!");
	getchar();
	ft_status = FT_Write(ft_handle, tx_buffer, tx_buffer_size, &num_of_bytes_written);
	if ( !FT_SUCCESS(ft_status) )
	{
		printf("\n\tTransmitting data failed with error code: %lu", ft_status);
	}

	ft_status = FT_GetStatus(ft_handle, &BytesInRxQueue, &BytesInTxQueue, &EventStatus);
	if ( !FT_SUCCESS(ft_status) )
	{
		printf("\n\tGetting the device status failed with error code: %lu", ft_status);
	}
	printf("\n\tPress enter key to DISPLAY the status in MM232R!");
	getchar();
	printf("\n\tAfter TX Before RX:");
	printf("\n\tNum of bytes in RX queue = %ld", BytesInRxQueue);
	printf("\n\tNum of bytes in TX queue = %ld", BytesInTxQueue);
	printf("\n\tEvent Status = %ld", EventStatus);

	printf("\n");
	printf("\n\tPress enter key to RECEIVE data!");
	getchar();
	ft_status = FT_Read(ft_handle, rx_buffer, rx_buffer_size, &num_of_bytes_read);
	if ( !FT_SUCCESS(ft_status) )
	{
		printf("\n\tReceiving data failed with error code: %lu", ft_status);
	}

	ft_status = FT_GetStatus(ft_handle, &BytesInRxQueue, &BytesInTxQueue, &EventStatus);
	if ( !FT_SUCCESS(ft_status) )
	{
		printf("\n\tGetting the device status failed with error code: %lu", ft_status);
	}
	printf("\n\tPress enter key to DISPLAY the status in MM232R!");
	getchar();
	printf("\n\tAfter RX:");
	printf("\n\tNum of bytes in RX queue = %ld", BytesInRxQueue);
	printf("\n\tNum of bytes in TX queue = %ld", BytesInTxQueue);
	printf("\n\tEvent Status = %ld", EventStatus);

	printf("\n");
	printf("\n\tPress enter key to PRINT the received data!");
	getchar();
	printf("\n\tReceived data: %s", rx_buffer);

	printf("\n");
	printf("\n\tPress enter key to SET the RTS pin to active LOW ---> RTS LED off");
	getchar();
	FT_SetRts(ft_handle);

	printf("\n\tPress enter key to CLEAR the RTS pin to not active HIGH ---> RTS LED on");
	getchar();
	FT_ClrRts(ft_handle);

	printf("\n\tPress enter key to SET the RTS pin to active LOW ---> RTS LED off");
	getchar();
	FT_SetRts(ft_handle);

	UCHAR CBusMask = 0x00;						// set CBUS3..0 pins as inputs (0) or outputs (1);
	UCHAR UpperNibble_PinDirection = 0xF0;		// The direction nibble sets CBUS3..0 pins as outputs
	UCHAR LowerNibble_PinStatus =  0x0F;		// The status nibble set CBus3..0 status
	UCHAR BitMask = 0x00;						// determine the D7......0 status
	UCHAR BitMode = FT_BITMODE_CBUS_BITBANG;	// set the chip mode as CBUS Bit Bang mode

	// open CBUS bit bang mode
	printf("\n\tPress enter key to set CBUS3..0 as outputs and all ones ---> CLK12M LED on");
	getchar();
	CBusMask =  UpperNibble_PinDirection | LowerNibble_PinStatus;
	ft_status = FT_SetBitMode(ft_handle, CBusMask, BitMode);

	printf("\n\tPress enter key to set CBUS3 (PWREN) = 1 and CBUS2 (CLK12M) = 0 ---> CLK12M LED off");
	getchar();
	CBusMask =  UpperNibble_PinDirection | 0x08;
	ft_status = FT_SetBitMode(ft_handle, CBusMask, BitMode);

	printf("\n\tPress enter key to set CBUS3 (PWREN) = 0 and CBUS2 (CLK12M) = 1 ---> CLK12M LED on");
	getchar();
	CBusMask =  UpperNibble_PinDirection | 0x04;
	ft_status = FT_SetBitMode(ft_handle, CBusMask, BitMode);

	printf("\n\tPress enter key to set CBUS3 (PWREN) = 0 and CBUS2 (CLK12M) = 0 ---> CLK12M LED off");
	getchar();
	CBusMask =  UpperNibble_PinDirection | 0x00;
	ft_status = FT_SetBitMode(ft_handle, CBusMask, BitMode);

	printf("\n\tPress enter key to set CBUS3 (PWREN) = 1 and CBUS2 (CLK12M) = 1 ---> CLK12M LED on");
	getchar();
	CBusMask =  UpperNibble_PinDirection | 0x0C;
	ft_status = FT_SetBitMode(ft_handle, CBusMask, BitMode);

	printf("\n\tPress enter key to set CBUS3 (PWREN) = 0 and CBUS2 (CLK12M) = 0 ---> CLK12M LED off");
	getchar();
	CBusMask =  UpperNibble_PinDirection | 0x00;
	ft_status = FT_SetBitMode(ft_handle, CBusMask, BitMode);

	printf("\n\tPress enter key to READ CBUS3..0 status");
	getchar();
	UCHAR BitStatus = 0x00;		// hold CBUS3..0 pin status
	CBusMask = 0x00;	// set CBUS3..0 pins as inputs
	ft_status = FT_SetBitMode(ft_handle, CBusMask, BitMode);
	ft_status = FT_GetBitMode(ft_handle, &BitStatus);
	printf("\n\tCBUS3..0 = 0x%X", BitStatus);

	printf("\n\n\tPress enter key to BLINK RTS LED and CLK12M LED in 5 times");
	getchar();
	// switch off all LEDs
	ft_status = FT_SetRts(ft_handle);	// Set RTS active LOW ---> RTS LED off
	CBusMask = 0xF0;
	ft_status = FT_SetBitMode(ft_handle, CBusMask, FT_BITMODE_CBUS_BITBANG);	// CLK12M (CBUS2) LED off
	for(i = 0; i < 5; i++)
	{
		FT_ClrRts(ft_handle);	// Clear RTS not active HIGH ---> RTS LED on
		sleep(1);
		FT_SetRts(ft_handle);	// Set RTS active LOW ---> RTS LED off
		sleep(1);
		CBusMask = 0xF4;
		ft_status = FT_SetBitMode(ft_handle, CBusMask, FT_BITMODE_CBUS_BITBANG);	// CLK12M LED on (CBUS2 = 1)
		sleep(1);
		CBusMask = 0xF0;
		ft_status = FT_SetBitMode(ft_handle, CBusMask, FT_BITMODE_CBUS_BITBANG);	// CLK12M LED off (CBUS2 = 0)
		sleep(1);
	}

	/*
	 * D7 ---> RI#
	 * D6 ---> DCD#
	 * D5 ---> DSR#
	 * D4 ---> DTR#
	 * D3 ---> CTS#
	 * D2 ---> RTS# ---> RST LED
	 * D1 ---> RXD
	 * D0 ---> TXD ---> TXD LED
	 *
	 */
	// set D7......0 all outputs
	BitMask = 0xFF;		// all outputs
	BitMode = FT_BITMODE_ASYNC_BITBANG;
	ft_status = FT_SetBitMode(ft_handle, BitMask, BitMode);
	// switch off TXD LED and RST LED
	/*
	 * D2 = 0 (RST LOW) ---> RST LED off
	 * D0 = 1 (TXD HIGH) ---> Inverter ---> TXD LED off
	 */
	BitStatus = 0x01;
	ft_status = FT_Write(ft_handle, &BitStatus, sizeof(BitStatus), &num_of_bytes_written);
	printf("\n\tPress enter key to BLINK TXD LED and RST LED in 5 times");
	getchar();
	for(i = 0; i < 5; i++)
	{
		/*
		 * D2 = 0 (RST LOW) ---> RST LED off
		 * D0 = 0 (TXD LOW) ---> Inverter ---> TXD LED on
		 */
		BitStatus = 0x00;
		ft_status = FT_Write(ft_handle, &BitStatus, sizeof(BitStatus), &num_of_bytes_written);
		sleep(1);
//		getchar();
		/*
		 * D2 = 1 (RST HIGH) ---> RST LED on
		 * D0 = 1 (TXD HIGH) ---> Inverter ---> TXD LED off
		 */
		BitStatus = 0x05;
		ft_status = FT_Write(ft_handle, &BitStatus, sizeof(BitStatus), &num_of_bytes_written);
		sleep(1);
//		getchar();
		/*
		 * D2 = 1 (RST HIGH) ---> RST LED on
		 * D0 = 0 (TXD LOW) ---> Inverter ---> TXD LED on
		 */
		BitStatus = 0x04;
		ft_status = FT_Write(ft_handle, &BitStatus, sizeof(BitStatus), &num_of_bytes_written);
		sleep(1);
//		getchar();
		/*
		 * D2 = 0 (RST LOW) ---> RST LED off
		 * D0 = 1 (TXD HIGH) ---> Inverter ---> TXD LED off
		 */
		BitStatus = 0x01;
		ft_status = FT_Write(ft_handle, &BitStatus, sizeof(BitStatus), &num_of_bytes_written);
		sleep(1);
//		getchar();
	}

	printf("\n\tPress enter key to EXIT");
	getchar();
	ft_status = FT_SetBitMode(ft_handle, 0x00, FT_BITMODE_RESET);	// reset the chip to default mode
	ft_status = FT_Close(ft_handle);    //Close the connection
	return EXIT_SUCCESS;
}
