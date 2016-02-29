// 
// 
// 

#include "BMP180.h"

SFE_BMP180 BMP180;

//uint8_t OSS = ;           // maximum pressure resolution

double baseline; // baseline pressure (at launch site)
float temperature, pressure;

double getPressure()
{
	char status;
	double T, P, p0, a;

	// You must first get a temperature measurement to perform a pressure reading.

	// Start a temperature measurement:
	// If request is successful, the number of ms to wait is returned.
	// If request is unsuccessful, 0 is returned.

	status = BMP180.startTemperature();
	if (status != 0)
	{
		// Wait for the measurement to complete:

		delay(status);

		// Retrieve the completed temperature measurement:
		// Note that the measurement is stored in the variable T.
		// Use '&T' to provide the address of T to the function.
		// Function returns 1 if successful, 0 if failure.

		status = BMP180.getTemperature(T);
		if (status != 0)
		{
			// Start a pressure measurement:
			// The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
			// If request is successful, the number of ms to wait is returned.
			// If request is unsuccessful, 0 is returned.

			status = BMP180.startPressure(3);
			if (status != 0)
			{
				// Wait for the measurement to complete:
				delay(status);

				// Retrieve the completed pressure measurement:
				// Note that the measurement is stored in the variable P.
				// Use '&P' to provide the address of P.
				// Note also that the function requires the previous temperature measurement (T).
				// (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
				// Function returns 1 if successful, 0 if failure.

				status = BMP180.getPressure(P, T);
				if (status != 0)
				{
					return(P);
				}
				else Serial.println("error retrieving pressure measurement\n");
			}
			else Serial.println("error starting pressure measurement\n");
		}
		else Serial.println("error retrieving temperature measurement\n");
	}
	else Serial.println("error starting temperature measurement\n");
}


