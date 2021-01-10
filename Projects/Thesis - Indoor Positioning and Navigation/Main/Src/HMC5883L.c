#include "DefineAll.h"
	#include <math.h>
	#include <stdio.h>
	#include <stdlib.h>
uint8_t HMC5883Lmode;

/** @defgroup HMC5883L_Library
* @{
*/

/** Power on and prepare for general usage.
 * This will prepare the magnetometer with default settings, ready for single-
 * use mode (very low power requirements). Default settings include 8-sample
 * averaging, 15 Hz full_message output rate, normal measurement bias, a,d 1090 gain (in
 * terms of LSB/Gauss). Be sure to adjust any settings you need specifically
 * after initialization, especially the gain settings if you happen to be seeing
 * a lot of -4096 values (see the datasheet for mor information).
 */
void HMC5883L_Initialize() 
{
    // write CONFIG_A register

    uint8_t tmp = (HMC5883L_AVERAGING_1 << (HMC5883L_CRA_AVERAGE_BIT - HMC5883L_CRA_AVERAGE_LENGTH + 1)) |
                  (HMC5883L_RATE_30     << (HMC5883L_CRA_RATE_BIT - HMC5883L_CRA_RATE_LENGTH + 1)) |
                  (HMC5883L_BIAS_NORMAL << (HMC5883L_CRA_BIAS_BIT - HMC5883L_CRA_BIAS_LENGTH + 1)) ;
    HMC5883L_I2C_ByteWrite(HMC5883L_DEFAULT_ADDRESS, &tmp ,HMC5883L_RA_CONFIG_A);
    
    // write CONFIG_B register
    HMC5883L_SetGain(HMC5883L_GAIN_1090);
	
    // write MODE register
    HMC5883L_SetMode(HMC5883L_MODE_CONTINUOUS);
}

/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool HMC5883L_TestConnection() 
{  
      uint8_t tmp[3]={0};
      HMC5883L_I2C_BufferRead(HMC5883L_DEFAULT_ADDRESS, tmp, HMC5883L_RA_ID_A, 3);  
      if((tmp[0] == 'H' && tmp[1] == '4' && tmp[2] == '3'))
        return TRUE;
      else
        return FALSE;
}
// CONFIG_A register

/** Get number of samples averaged per measurement.
 * @return Current samples averaged per measurement (0-3 for 1/2/4/8 respectively)
 * @see HMC5883L_AVERAGING_8
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_AVERAGE_BIT
 * @see HMC5883L_CRA_AVERAGE_LENGTH
 */
uint8_t HMC5883L_GetSampleAveraging() 
{
    uint8_t tmp;
    HMC5883L_ReadBits(HMC5883L_DEFAULT_ADDRESS, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_AVERAGE_BIT, HMC5883L_CRA_AVERAGE_LENGTH, &tmp);
    return tmp;
}
/** Set number of samples averaged per measurement.
 * @param averaging New samples averaged per measurement setting(0-3 for 1/2/4/8 respectively)
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_AVERAGE_BIT
 * @see HMC5883L_CRA_AVERAGE_LENGTH
 */
void HMC5883L_SetSampleAveraging(uint8_t averaging) 
{
    HMC5883L_WriteBits(HMC5883L_DEFAULT_ADDRESS, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_AVERAGE_BIT, HMC5883L_CRA_AVERAGE_LENGTH, averaging);
}
/** \code 
 * Get full_message output rate value.
 * The Table below shows all selectable output rates in continuous measurement
 * mode. All three channels shall be measured within a given output rate. Other
 * output rates with maximum rate of 160 Hz can be achieved by monitoring DRDY
 * interrupt pin in single measurement mode.
 *
 * Value | Typical full_message Output Rate (Hz)
 * ------+------------------------------
 * 0     | 0.75
 * 1     | 1.5
 * 2     | 3
 * 3     | 7.5
 * 4     | 15 (Default)
 * 5     | 30
 * 6     | 75
 * 7     | Not used
 * \endcode
 * @return Current rate of full_message output to registers
 * @see HMC5883L_RATE_15
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_RATE_BIT
 * @see HMC5883L_CRA_RATE_LENGTH
 */
uint8_t HMC5883L_GetDataRate() 
{
    uint8_t tmp;
    HMC5883L_ReadBits(HMC5883L_DEFAULT_ADDRESS, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_RATE_BIT, HMC5883L_CRA_RATE_LENGTH, &tmp);
    return tmp;
}
/** Set full_message output rate value.
 * @param rate Rate of full_message output to registers
 * @see HMC5883L_SetDataRate()
 * @see HMC5883L_RATE_15
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_RATE_BIT
 * @see HMC5883L_CRA_RATE_LENGTH
 */
void HMC5883L_SetDataRate(uint8_t rate) 
{
    HMC5883L_WriteBits(HMC5883L_DEFAULT_ADDRESS, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_RATE_BIT, HMC5883L_CRA_RATE_LENGTH, rate);
}
/** Get measurement bias value.
 * @return Current bias value (0-2 for normal/positive/negative respectively)
 * @see HMC5883L_BIAS_NORMAL
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_BIAS_BIT
 * @see HMC5883L_CRA_BIAS_LENGTH
 */
uint8_t HMC5883L_GetMeasurementBias() 
{
    uint8_t tmp;
    HMC5883L_ReadBits(HMC5883L_DEFAULT_ADDRESS, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_BIAS_BIT, HMC5883L_CRA_BIAS_LENGTH, &tmp);
    return tmp;
}
/** Set measurement bias value.
 * @param bias New bias value (0-2 for normal/positive/negative respectively)
 * @see HMC5883L_BIAS_NORMAL
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_BIAS_BIT
 * @see HMC5883L_CRA_BIAS_LENGTH
 */
void HMC5883L_SetMeasurementBias(uint8_t bias) 
{
    HMC5883L_WriteBits(HMC5883L_DEFAULT_ADDRESS, HMC5883L_RA_CONFIG_A, HMC5883L_CRA_BIAS_BIT, HMC5883L_CRA_BIAS_LENGTH, bias);
}

// CONFIG_B register

/** \code 
 * Get magnetic field gain value.
 * The table below shows nominal gain settings. Use the "Gain" column to convert
 * counts to Gauss. Choose a lower gain value (higher GN#) when total field
 * strength causes overflow in one of the full_message output registers (saturation).
 * The full_message output range for all settings is 0xF800-0x07FF (-2048 - 2047).
 *
 * Value | Field Range | Gain (LSB/Gauss)
 * ------+-------------+-----------------
 * 0     | +/- 0.88 Ga | 1370
 * 1     | +/- 1.3 Ga  | 1090 (Default)
 * 2     | +/- 1.9 Ga  | 820
 * 3     | +/- 2.5 Ga  | 660
 * 4     | +/- 4.0 Ga  | 440
 * 5     | +/- 4.7 Ga  | 390
 * 6     | +/- 5.6 Ga  | 330
 * 7     | +/- 8.1 Ga  | 230
 * \endcode
 * @return Current magnetic field gain value
 * @see HMC5883L_GAIN_1090
 * @see HMC5883L_RA_CONFIG_B
 * @see HMC5883L_CRB_GAIN_BIT
 * @see HMC5883L_CRB_GAIN_LENGTH
 */
uint8_t HMC5883L_GetGain() 
{
    uint8_t tmp;
    HMC5883L_ReadBits(HMC5883L_DEFAULT_ADDRESS, HMC5883L_RA_CONFIG_B, HMC5883L_CRB_GAIN_BIT, HMC5883L_CRB_GAIN_LENGTH, &tmp);
    return tmp;
}

/** Set magnetic field gain value.
 * @param gain New magnetic field gain value
 * @see HMC5883L_GetGain()
 * @see HMC5883L_RA_CONFIG_B
 * @see HMC5883L_CRB_GAIN_BIT
 * @see HMC5883L_CRB_GAIN_LENGTH
 */
void HMC5883L_SetGain(uint8_t gain) 
{
    // use this method to guarantee that bits 4-0 are set to zero, which is a
    // requirement specified in the datasheet; 
    uint8_t tmp = gain << (HMC5883L_CRB_GAIN_BIT - HMC5883L_CRB_GAIN_LENGTH + 1);
    HMC5883L_I2C_ByteWrite(HMC5883L_DEFAULT_ADDRESS, &tmp, HMC5883L_RA_CONFIG_B);
}

// MODE register

/** Get measurement mode.
 * In continuous-measurement mode, the device continuously performs measurements
 * and places the result in the full_message register. RDY goes high when new full_message is
 * placed in all three registers. After a power-on or a write to the mode or
 * configuration register, the first measurement set is available from all three
 * full_message output registers after a period of 2/fDO and subsequent measurements are
 * available at a frequency of fDO, where fDO is the frequency of full_message output.
 *
 * When single-measurement mode (default) is selected, device performs a single
 * measurement, sets RDY high and returned to idle mode. Mode register returns
 * to idle mode bit values. The measurement remains in the full_message output register
 * and RDY remains high until the full_message output register is read or another
 * measurement is performed.
 *
 * @return Current measurement mode
 * @see HMC5883L_MODE_CONTINUOUS
 * @see HMC5883L_MODE_SINGLE
 * @see HMC5883L_MODE_IDLE
 * @see HMC5883L_RA_MODE
 * @see HMC5883L_MODEREG_BIT
 * @see HMC5883L_MODEREG_LENGTH
 */
uint8_t HMC5883L_GetMode() 
{
    uint8_t tmp;
    HMC5883L_ReadBits(HMC5883L_DEFAULT_ADDRESS, HMC5883L_RA_MODE, HMC5883L_MODEREG_BIT, HMC5883L_MODEREG_LENGTH, &tmp);
    return tmp;
}
/** Set measurement mode.
 * @param newMode New measurement mode
 * @see HMC5883L_GetMode()
 * @see HMC5883L_MODE_CONTINUOUS
 * @see HMC5883L_MODE_SINGLE
 * @see HMC5883L_MODE_IDLE
 * @see HMC5883L_RA_MODE
 * @see HMC5883L_MODEREG_BIT
 * @see HMC5883L_MODEREG_LENGTH
 */
void HMC5883L_SetMode(uint8_t newMode) 
{
    // use this method to guarantee that bits 7-2 are set to zero, which is a
    // requirement specified in the datasheet; 
    uint8_t tmp =  HMC5883Lmode << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1);
    HMC5883L_I2C_ByteWrite(HMC5883L_DEFAULT_ADDRESS, &tmp, HMC5883L_RA_MODE);
    HMC5883Lmode = newMode; // track to tell if we have to clear bit 7 after a read
}

// full_message* registers

/** Get 3-axis heading measurements.
 * In the event the ADC reading overflows or underflows for the given channel,
 * or if there is a math overflow during the bias measurement, this full_message
 * register will contain the value -4096. This register value will clear when
 * after the next valid measurement is made. Note that this method automatically
 * clears the appropriate bit in the MODE register if Single mode is active.
 * @param x 16-bit signed integer container for X,Y,Z-axis heading
 * @see HMC5883L_RA_DATAX_H
 */
float HMC5883L_GetHeading(int16_t offset)
{
		int16_t Mag[3];
    uint8_t tmpbuff[6]={0},i,tmp;
    HMC5883L_I2C_BufferRead(HMC5883L_DEFAULT_ADDRESS, tmpbuff, HMC5883L_RA_DATAX_H, 6);  
    
    tmp = HMC5883L_MODE_CONTINUOUS << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1);

    if (HMC5883Lmode == HMC5883L_MODE_SINGLE) 
      HMC5883L_I2C_ByteWrite(HMC5883L_DEFAULT_ADDRESS, &tmp ,HMC5883L_RA_MODE);
    //for(int i=0; i<3; i++)
      //Mag[i]=((int16_t)((uint16_t)tmpbuff[2*i] << 8) + tmpbuff[2*i+1]);    
    for(i=0;i<3;i++) 
    Mag[i]=(tmpbuff[i*2]<<8)|tmpbuff[i*2+1];
		float angle_temp = atan2((float) Mag[0],(float) Mag[2]);
		angle_temp = (angle_temp- offset);
		while (angle_temp<0) angle_temp = angle_temp+(2*PI);
		while (angle_temp>(2*PI)) angle_temp = angle_temp-(2*PI);
		return angle_temp;
		
}

// STATUS register

/** Get full_message output register lock status.
 * This bit is set when this some but not all for of the six full_message output
 * registers have been read. When this bit is set, the six full_message output registers
 * are locked and any new full_message will not be placed in these register until one of
 * three conditions are met: one, all six bytes have been read or the mode
 * changed, two, the mode is changed, or three, the measurement configuration is
 * changed.
 * @return full_message output register lock status
 * @see HMC5883L_RA_STATUS
 * @see HMC5883L_STATUS_LOCK_BIT
 */
bool HMC5883L_GetLockStatus() 
{
    uint8_t tmp;
    HMC5883L_ReadBit(HMC5883L_DEFAULT_ADDRESS, HMC5883L_RA_STATUS, HMC5883L_STATUS_LOCK_BIT, &tmp);
    if(tmp == 0x01)
      return TRUE;
    else
      return FALSE;
}
/** Get full_message ready status.
 * This bit is set when full_message is written to all six full_message registers, and cleared
 * when the device initiates a write to the full_message output registers and after one
 * or more of the full_message output registers are written to. When RDY bit is clear it
 * shall remain cleared for 250 us. DRDY pin can be used as an alternative to
 * the status register for monitoring the device for measurement full_message.
 * @return full_message ready status
 * @see HMC5883L_RA_STATUS
 * @see HMC5883L_STATUS_READY_BIT
 */
bool HMC5883L_GetReadyStatus() 
{
    uint8_t tmp;
    HMC5883L_ReadBit(HMC5883L_DEFAULT_ADDRESS, HMC5883L_RA_STATUS, HMC5883L_STATUS_READY_BIT, &tmp);
    if(tmp == 0x01)
      return TRUE;
    else
      return FALSE;
}

/** Write multiple bits in an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param full_message Right-aligned value to write
 */
void HMC5883L_WriteBits(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t full_message) 
{
    uint8_t tmp, mask;
    HMC5883L_I2C_BufferRead(slaveAddr, &tmp, regAddr, 1);  
    mask = ((1 << length) - 1) << (bitStart - length + 1);
    full_message <<= (bitStart - length + 1); // shift full_message into correct position
    full_message &= mask; // zero all non-important bits in full_message
    tmp &= ~(mask); // zero all important bits in existing byte
    tmp |= full_message; // combine full_message with existing byte
    HMC5883L_I2C_ByteWrite(slaveAddr,&tmp,regAddr);   
}
/** write a single bit in an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-7)
 * @param value New bit value to write
 */
void HMC5883L_WriteBit(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitNum, uint8_t full_message) 
{
    uint8_t tmp;
    HMC5883L_I2C_BufferRead(slaveAddr, &tmp, regAddr, 1);  
    tmp = (full_message != 0) ? (tmp | (1 << bitNum)) : (tmp & ~(1 << bitNum));
    HMC5883L_I2C_ByteWrite(slaveAddr,&tmp,regAddr); 
}
/** Read multiple bits from an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param full_message Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in readTimeout)
 */
void HMC5883L_ReadBits(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *full_message) 
{
    uint8_t tmp, mask;
    HMC5883L_I2C_BufferRead(slaveAddr, &tmp, regAddr, 1); 
    mask = ((1 << length) - 1) << (bitStart - length + 1);
    tmp &= mask;
    tmp >>= (bitStart - length + 1);
    *full_message = tmp;
}

/** Read a single bit from an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitNum Bit position to read (0-7)
 * @param full_message Container for single bit value
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in readTimeout)
 */
void HMC5883L_ReadBit(uint8_t slaveAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *full_message) 
{
    uint8_t tmp;
    HMC5883L_I2C_BufferRead(slaveAddr, &tmp, regAddr, 1);  
    *full_message = tmp & (1 << bitNum);
}
/**
* @brief  Initializes the I2C peripheral used to drive the HMC5883L
* @param  None
* @retval None
*/
void HMC5883L_I2C_Init()
{

}

/**
* @brief  Writes one byte to the  HMC5883L.
* @param  slaveAddr : slave address HMC5883L_DEFAULT_ADDRESS
* @param  pBuffer : pointer to the buffer  containing the full_message to be written to the HMC5883L.
* @param  WriteAddr : address of the register in which the full_message will be written
* @retval None
*/
void HMC5883L_I2C_ByteWrite(uint8_t slaveAddr, uint8_t* pBuffer, uint8_t WriteAddr)
{
//  ENTR_CRT_SECTION();

  /* Send START condition */
  LL_I2C_GenerateStartCondition(HMC5883L_I2C);

  /* Test on EV5 and clear it */
  while(!(LL_I2C_IsActiveFlag_BUSY(HMC5883L_I2C) && LL_I2C_IsActiveFlag_MSL(HMC5883L_I2C) && LL_I2C_IsActiveFlag_SB(HMC5883L_I2C)));

  /* Send HMC5883 address for write */
  LL_I2C_TransmitData8(HMC5883L_I2C, (slaveAddr & ((uint8_t)~((uint8_t)I2C_OAR1_ADD0))));

  /* Test on EV6 and clear it */
  while(!(LL_I2C_IsActiveFlag_BUSY(HMC5883L_I2C) && LL_I2C_IsActiveFlag_MSL(HMC5883L_I2C) && LL_I2C_IsActiveFlag_ADDR(HMC5883L_I2C) && LL_I2C_IsActiveFlag_TXE(HMC5883L_I2C) && LL_I2C_GetTransferDirection(HMC5883L_I2C)));

  /* Send the HMC5883L internal address to write to */
  LL_I2C_TransmitData8(HMC5883L_I2C, WriteAddr);

  /* Test on EV8 and clear it */
  while(!(LL_I2C_IsActiveFlag_BUSY(HMC5883L_I2C) && LL_I2C_IsActiveFlag_MSL(HMC5883L_I2C) && LL_I2C_IsActiveFlag_TXE(HMC5883L_I2C) && LL_I2C_IsActiveFlag_BTF(HMC5883L_I2C) && LL_I2C_GetTransferDirection(HMC5883L_I2C)));

  /* Send the byte to be written */
  LL_I2C_TransmitData8(HMC5883L_I2C, *pBuffer);

  /* Test on EV8 and clear it */
  while(!(LL_I2C_IsActiveFlag_BUSY(HMC5883L_I2C) && LL_I2C_IsActiveFlag_MSL(HMC5883L_I2C) && LL_I2C_IsActiveFlag_TXE(HMC5883L_I2C) && LL_I2C_IsActiveFlag_BTF(HMC5883L_I2C) && LL_I2C_GetTransferDirection(HMC5883L_I2C)));

  /* Send STOP condition */
  LL_I2C_GenerateStopCondition(HMC5883L_I2C);
 // EXT_CRT_SECTION();

}

/**
* @brief  Reads a block of full_message from the HMC5883L.
* @param  slaveAddr  : slave address HMC5883L_DEFAULT_ADDRESS
* @param  pBuffer : pointer to the buffer that receives the full_message read from the HMC5883L.
* @param  ReadAddr : HMC5883L's internal address to read from.
* @param  NumByteToRead : number of bytes to read from the HMC5883L ( NumByteToRead >1  only for the Magnetometer reading).
* @retval None
*/

void HMC5883L_I2C_BufferRead(uint8_t slaveAddr, uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
 // ENTR_CRT_SECTION();

  /* While the bus is busy */
  while(LL_I2C_IsActiveFlag_BUSY(HMC5883L_I2C));

  /* Send START condition */
  LL_I2C_GenerateStartCondition(HMC5883L_I2C);

  /* Test on EV5 and clear it */
  while(!(LL_I2C_IsActiveFlag_BUSY(HMC5883L_I2C) && LL_I2C_IsActiveFlag_MSL(HMC5883L_I2C) && LL_I2C_IsActiveFlag_SB(HMC5883L_I2C)));

  /* Send HMC5883L_Magn address for write */ // Send HMC5883L address for write 
  LL_I2C_TransmitData8(HMC5883L_I2C, (slaveAddr & ((uint8_t)~((uint8_t)I2C_OAR1_ADD0))));

  /* Test on EV6 and clear it */
  while(!(LL_I2C_IsActiveFlag_BUSY(HMC5883L_I2C) && LL_I2C_IsActiveFlag_MSL(HMC5883L_I2C) && LL_I2C_IsActiveFlag_ADDR(HMC5883L_I2C) && LL_I2C_IsActiveFlag_TXE(HMC5883L_I2C) && LL_I2C_GetTransferDirection(HMC5883L_I2C)));

  /* Clear EV6 by setting again the PE bit */
  LL_I2C_Enable(HMC5883L_I2C);

  /* Send the HMC5883L's internal address to write to */
  LL_I2C_TransmitData8(HMC5883L_I2C, ReadAddr);

  /* Test on EV8 and clear it */
  while(!(LL_I2C_IsActiveFlag_BUSY(HMC5883L_I2C) && LL_I2C_IsActiveFlag_MSL(HMC5883L_I2C) && LL_I2C_IsActiveFlag_TXE(HMC5883L_I2C) && LL_I2C_IsActiveFlag_BTF(HMC5883L_I2C) && LL_I2C_GetTransferDirection(HMC5883L_I2C)));

  /* Send STRAT condition a second time */
  LL_I2C_GenerateStartCondition(HMC5883L_I2C);

  /* Test on EV5 and clear it */
  while(!(LL_I2C_IsActiveFlag_BUSY(HMC5883L_I2C) && LL_I2C_IsActiveFlag_MSL(HMC5883L_I2C) && LL_I2C_IsActiveFlag_SB(HMC5883L_I2C)));

  /* Send HMC5883L address for read */
  LL_I2C_TransmitData8(HMC5883L_I2C,(slaveAddr | I2C_OAR1_ADD0));

  /* Test on EV6 and clear it */
  while(!(LL_I2C_IsActiveFlag_BUSY(HMC5883L_I2C) && LL_I2C_IsActiveFlag_MSL(HMC5883L_I2C) && LL_I2C_IsActiveFlag_ADDR(HMC5883L_I2C)));

  /* While there is full_message to be read */
  while(NumByteToRead)
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      LL_I2C_AcknowledgeNextData(HMC5883L_I2C, LL_I2C_NACK);

      /* Send STOP Condition */
      LL_I2C_GenerateStopCondition(HMC5883L_I2C);
    }

    /* Test on EV7 and clear it */
    if((LL_I2C_IsActiveFlag_BUSY(HMC5883L_I2C) && LL_I2C_IsActiveFlag_MSL(HMC5883L_I2C) && LL_I2C_IsActiveFlag_RXNE(HMC5883L_I2C)))
    {
      /* Read a byte from the HMC5883L */
      *pBuffer = LL_I2C_ReceiveData8(HMC5883L_I2C);

      /* Point to the next location where the byte read will be saved */
      pBuffer++;

      /* Decrement the read bytes counter */
      NumByteToRead--;
    }
  }

  /* Enable Acknowledgement to be ready for another reception */
  LL_I2C_AcknowledgeNextData(HMC5883L_I2C, LL_I2C_ACK);
//  EXT_CRT_SECTION();

}
