/*
 * This software program is licensed subject to the GNU General Public License
 * (GPL).Version 2,June 1991, available at http://www.fsf.org/copyleft/gpl.html

 * (C) Copyright 2010 Bosch Sensortec GmbH
 * All Rights Reserved
 */

/*
 * bma222.c
 * Sensor Driver for BMA222 Triaxial acceleration sensor
 */

//#include "API.h"
#include "define.h"
#include "bma222.h"

    bma222_t * p_bma222;

int bma222_init(bma222_t * bma222)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	p_bma222 = bma222;	/* assign bma222 ptr */
	p_bma222->dev_addr = BMA222_I2C_ADDR;	/* preset bma222 I2C_addr */
	comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_CHIP_ID__REG, &data, 1);	/* read Chip Id */
	p_bma222->chip_id = data;	/* get bitslice */
	comres += p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_ML_VERSION__REG, &data, 1);	/* read Version reg */
	p_bma222->ml_version = bma222_GET_BITSLICE(data, bma222_ML_VERSION);	/* get ML Version */
	p_bma222->al_version = bma222_GET_BITSLICE(data, bma222_AL_VERSION);	/* get AL Version */
	return comres;
}

int bma222_soft_reset(void)
{
	int comres = C_Zero_U8X;
	unsigned char data = bma222_EN_SOFT_RESET_VALUE;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_SOFT_RESET__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_write_reg(unsigned char addr, unsigned char *data, unsigned char len)
{
	int comres = C_Zero_U8X;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, addr, data, len);
		}
	return comres;
}

int bma222_read_reg(unsigned char addr, unsigned char *data, unsigned char len)
{
	int comres = C_Zero_U8X;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, addr, data, len);
		}
	return comres;
}

int bma222_set_mode(unsigned char Mode)
{
	int comres = C_Zero_U8X;
	unsigned char data1;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		if (Mode < C_Three_U8X)
			{
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_LOW_POWER__REG, &data1,
							   C_One_U8X);
			switch (Mode)
				{
			case bma222_MODE_NORMAL:
				data1 = bma222_SET_BITSLICE(data1, bma222_EN_LOW_POWER, C_Zero_U8X);
				data1 = bma222_SET_BITSLICE(data1, bma222_EN_SUSPEND, C_Zero_U8X);
				break;
			case bma222_MODE_LOWPOWER:
				data1 = bma222_SET_BITSLICE(data1, bma222_EN_LOW_POWER, C_One_U8X);
				data1 = bma222_SET_BITSLICE(data1, bma222_EN_SUSPEND, C_Zero_U8X);
				break;
			case bma222_MODE_SUSPEND:
				data1 = bma222_SET_BITSLICE(data1, bma222_EN_LOW_POWER, C_Zero_U8X);
				data1 = bma222_SET_BITSLICE(data1, bma222_EN_SUSPEND, C_One_U8X);
				break;
			default:
				break;
				}
			comres +=
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_LOW_POWER__REG, &data1,
							    C_One_U8X);
			p_bma222->mode = Mode;
			}
		else
			{
			comres = E_OUT_OF_RANGE;
			}
		}
	return comres;
}

int bma222_get_mode(unsigned char *Mode)
{
	int comres = C_Zero_U8X;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_LOW_POWER__REG, Mode, C_One_U8X);
		*Mode = (*Mode) >> C_Six_U8X;
		    p_bma222->mode = *Mode;
		}
	return comres;
}

unsigned char bma222_set_range(unsigned char Range)
{
	int comres = C_Zero_U8X;
	unsigned char data1;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		if (Range < C_Four_U8X)
			{
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_RANGE_SEL_REG, &data1, C_One_U8X);
			switch (Range)
				{
			case C_Zero_U8X:
				data1 = bma222_SET_BITSLICE(data1, bma222_RANGE_SEL, C_Zero_U8X);
				break;
			case C_One_U8X:
				data1 = bma222_SET_BITSLICE(data1, bma222_RANGE_SEL, C_Five_U8X);
				break;
			case C_Two_U8X:
				data1 = bma222_SET_BITSLICE(data1, bma222_RANGE_SEL, C_Eight_U8X);
				break;
			case C_Three_U8X:
				data1 = bma222_SET_BITSLICE(data1, bma222_RANGE_SEL, C_Twelve_U8X);
				break;
			default:
				break;
				}
			comres +=
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_RANGE_SEL_REG, &data1,
							    C_One_U8X);
			}
		else
			{
			comres = E_OUT_OF_RANGE;
			}
		}
	return comres;
}

int bma222_get_range(unsigned char *Range)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_RANGE_SEL__REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_RANGE_SEL);
		*Range = data;
		}
	return comres;
}

int bma222_set_bandwidth(unsigned char BW)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	int Bandwidth;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		if (BW < C_Eight_U8X)
			{
			switch (BW)
				{
			case C_Zero_U8X:
				Bandwidth = bma222_BW_7_81HZ;
				    /*  7.81 Hz      64000 uS   */
				    break;
			case C_One_U8X:
				Bandwidth = bma222_BW_15_63HZ;
				    /*  15.63 Hz     32000 uS   */
				    break;
			case C_Two_U8X:
				Bandwidth = bma222_BW_31_25HZ;
				    /*  31.25 Hz     16000 uS   */
				    break;
			case C_Three_U8X:
				Bandwidth = bma222_BW_62_50HZ;
				    /*  62.50 Hz     8000 uS   */
				    break;
			case C_Four_U8X:
				Bandwidth = bma222_BW_125HZ;
				    /*  125 Hz       4000 uS   */
				    break;
			case C_Five_U8X:
				Bandwidth = bma222_BW_250HZ;
				    /*  250 Hz       2000 uS   */
				    break;
			case C_Six_U8X:
				Bandwidth = bma222_BW_500HZ;
				    /*  500 Hz       1000 uS   */
				    break;
			case C_Seven_U8X:
				Bandwidth = bma222_BW_1000HZ;
				    /*  1000 Hz      500 uS   */
				    break;
			default:
				break;
				}
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_BANDWIDTH__REG, &data, C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_BANDWIDTH, Bandwidth);
			comres +=
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_BANDWIDTH__REG, &data,
							    C_One_U8X);
			}
		else
			{
			comres = E_OUT_OF_RANGE;
			}
		}
	return comres;
}

int bma222_get_bandwidth(unsigned char *BW)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_BANDWIDTH__REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_BANDWIDTH);
		if (data <= C_Eight_U8X)
			{
			*BW = C_Zero_U8X;
			}
		else
			{
			if (data >= 0x0F)
				{
				*BW = C_Seven_U8X;
				}
			else
				{
				*BW = data - C_Eight_U8X;
				}
			}
		}
	return comres;
}

int bma222_read_accel_xyz(bma222acc_t * acc)
{
	int comres;
	unsigned char data[6];
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, BMA222_ACC_X8_LSB__REG, data, 6);
		acc->x =
		    bma222_GET_BITSLICE(data[0],
					BMA222_ACC_X8_LSB) | (bma222_GET_BITSLICE(data[1],
										  BMA222_ACC_X_MSB) <<
							      (BMA222_ACC_X8_LSB__LEN));
		acc->x = acc->x << (sizeof(short) * 8 - (BMA222_ACC_X8_LSB__LEN + BMA222_ACC_X_MSB__LEN));
		acc->x = acc->x >> (sizeof(short) * 8 - (BMA222_ACC_X8_LSB__LEN + BMA222_ACC_X_MSB__LEN));

		acc->y =
		    bma222_GET_BITSLICE(data[2],
					BMA222_ACC_Y8_LSB) | (bma222_GET_BITSLICE(data[3],
										  BMA222_ACC_Y_MSB) <<
							      (BMA222_ACC_Y8_LSB__LEN));
		acc->y = acc->y << (sizeof(short) * 8 - (BMA222_ACC_Y8_LSB__LEN + BMA222_ACC_Y_MSB__LEN));
		acc->y = acc->y >> (sizeof(short) * 8 - (BMA222_ACC_Y8_LSB__LEN + BMA222_ACC_Y_MSB__LEN));

		acc->z =
		    bma222_GET_BITSLICE(data[4],
					BMA222_ACC_Z8_LSB) | (bma222_GET_BITSLICE(data[5],
										  BMA222_ACC_Z_MSB) <<
							      (BMA222_ACC_Z8_LSB__LEN));
		acc->z = acc->z << (sizeof(short) * 8 - (BMA222_ACC_Z8_LSB__LEN + BMA222_ACC_Z_MSB__LEN));
		acc->z = acc->z >> (sizeof(short) * 8 - (BMA222_ACC_Z8_LSB__LEN + BMA222_ACC_Z_MSB__LEN));
		}
	return comres;
}

int bma222_read_accel_x(short *a_x)
{
	int comres;
	unsigned char data[2];
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, BMA222_ACC_X8_LSB__REG, data, 2);
		*a_x =
		    bma222_GET_BITSLICE(data[0],
					BMA222_ACC_X8_LSB) | (bma222_GET_BITSLICE(data[1],
										  BMA222_ACC_X_MSB) <<
							      (BMA222_ACC_X8_LSB__LEN));
		*a_x = *a_x << (sizeof(short) * 8 - (BMA222_ACC_X8_LSB__LEN + BMA222_ACC_X_MSB__LEN));
		*a_x = *a_x >> (sizeof(short) * 8 - (BMA222_ACC_X8_LSB__LEN + BMA222_ACC_X_MSB__LEN));
		}
	return comres;
}

int bma222_read_accel_y(short *a_y)
{
	int comres;
	unsigned char data[2];
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, BMA222_ACC_Y8_LSB__REG, data, 2);
		*a_y =
		    bma222_GET_BITSLICE(data[0],
					BMA222_ACC_Y8_LSB) | (bma222_GET_BITSLICE(data[1],
										  BMA222_ACC_Y_MSB) <<
							      (BMA222_ACC_Y8_LSB__LEN));
		*a_y = *a_y << (sizeof(short) * 8 - (BMA222_ACC_Y8_LSB__LEN + BMA222_ACC_Y_MSB__LEN));
		*a_y = *a_y >> (sizeof(short) * 8 - (BMA222_ACC_Y8_LSB__LEN + BMA222_ACC_Y_MSB__LEN));
		}
	return comres;
}

int bma222_read_accel_z(short *a_z)
{
	int comres;
	unsigned char data[2];
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, BMA222_ACC_Z8_LSB__REG, data, 2);
		*a_z =
		    bma222_GET_BITSLICE(data[0],
					BMA222_ACC_Z8_LSB) | (bma222_GET_BITSLICE(data[1],
										  BMA222_ACC_Z_MSB) <<
							      (BMA222_ACC_Z8_LSB__LEN));
		*a_z = *a_z << (sizeof(short) * 8 - (BMA222_ACC_Z8_LSB__LEN + BMA222_ACC_Z_MSB__LEN));
		*a_z = *a_z >> (sizeof(short) * 8 - (BMA222_ACC_Z8_LSB__LEN + BMA222_ACC_Z_MSB__LEN));
		}
	return comres;
}

int bma222_reset_interrupt(void)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_RESET_LATCHED__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_INT_RESET_LATCHED, C_One_U8X);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_INT_RESET_LATCHED__REG, &data,
						    C_One_U8X);
		}
	return comres;
}

int bma222_get_interruptstatus1(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS1_REG, &data, C_One_U8X);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_interruptstatus2(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS2_REG, &data, C_One_U8X);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_Low_G_interrupt(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS1_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_LOWG_INT_S);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_High_G_Interrupt(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS1_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_HIGHG_INT_S);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_slope_interrupt(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS1_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_SLOPE_INT_S);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_double_tap_interrupt(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS1_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_DOUBLE_TAP_INT_S);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_single_tap_interrupt(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS1_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_SINGLE_TAP_INT_S);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_orient_interrupt(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS1_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_ORIENT_INT_S);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_flat_interrupt(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS1_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_FLAT_INT_S);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_data_interrupt(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS2_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_DATA_INT_S);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_slope_first(unsigned char param, unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (param)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_TAP_SLOPE_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_SLOPE_FIRST_X);
			*intstatus = data;
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_TAP_SLOPE_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_SLOPE_FIRST_Y);
			*intstatus = data;
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_TAP_SLOPE_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_SLOPE_FIRST_Z);
			*intstatus = data;
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_get_slope_sign(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_TAP_SLOPE_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_SLOPE_SIGN_S);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_tap_first(unsigned char param, unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (param)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_TAP_SLOPE_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_TAP_FIRST_X);
			*intstatus = data;
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_TAP_SLOPE_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_TAP_FIRST_Y);
			*intstatus = data;
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_TAP_SLOPE_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_TAP_FIRST_Z);
			*intstatus = data;
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_get_tap_sign(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_TAP_SLOPE_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_TAP_SIGN_S);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_HIGH_first(unsigned char param, unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (param)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_ORIENT_HIGH_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_HIGHG_FIRST_X);
			*intstatus = data;
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_ORIENT_HIGH_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_HIGHG_FIRST_Y);
			*intstatus = data;
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_ORIENT_HIGH_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_HIGHG_FIRST_Z);
			*intstatus = data;
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_get_HIGH_sign(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_ORIENT_HIGH_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_HIGHG_SIGN_S);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_orient_status(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_ORIENT_HIGH_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_ORIENT_S);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_orient_flat_status(unsigned char *intstatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_ORIENT_HIGH_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_FLAT_S);
		*intstatus = data;
		}
	return comres;
}

int bma222_get_sleep_duration(unsigned char *sleep)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_MODE_CTRL_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_SLEEP_DUR);
		*sleep = data;
		}
	return comres;
}

int bma222_set_sleep_duration(unsigned char sleepdur)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_SLEEP_DUR__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_SLEEP_DUR, sleepdur);
		comres = p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_SLEEP_DUR__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_set_suspend(unsigned char state)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_SUSPEND__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_EN_SUSPEND, state);
		comres = p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_SUSPEND__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_suspend(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_MODE_CTRL_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_EN_SUSPEND);
		*status = data;
		}
	return comres;
}

int bma222_set_lowpower(unsigned char state)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_LOW_POWER__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_EN_LOW_POWER, state);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_LOW_POWER__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_lowpower_en(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_MODE_CTRL_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_EN_LOW_POWER);
		*status = data;
		}
	return comres;
}

int bma222_set_low_noise_ctrl(unsigned char state)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_LOW_NOISE__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_EN_LOW_NOISE, state);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_LOW_NOISE__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_low_noise_ctrl(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_LOW_NOISE_CTRL_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_EN_LOW_NOISE);
		*status = data;
		}
	return comres;
}

int bma222_set_shadow_disable(unsigned char state)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_DIS_SHADOW_PROC__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_DIS_SHADOW_PROC, state);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_DIS_SHADOW_PROC__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_shadow_disable(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_DATA_CTRL_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_DIS_SHADOW_PROC);
		*status = data;
		}
	return comres;
}

int bma222_set_unfilt_acc(unsigned char state)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_UNFILT_ACC__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_EN_UNFILT_ACC, state);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_UNFILT_ACC__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_unfilt_acc(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_DATA_CTRL_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_EN_UNFILT_ACC);
		*status = data;
		}
	return comres;
}

int bma222_set_enable_slope_interrupt(unsigned char slope)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	unsigned char state;
	state = 0x01;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (slope)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_SLOPE_X_INT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_SLOPE_X_INT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_SLOPE_X_INT__REG, &data,
							    C_One_U8X);
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_SLOPE_Y_INT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_SLOPE_Y_INT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_SLOPE_Y_INT__REG, &data,
							    C_One_U8X);
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_SLOPE_Z_INT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_SLOPE_Z_INT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_SLOPE_Z_INT__REG, &data,
							    C_One_U8X);
			break;
		case C_Three_U8X:
			state = 0x07;
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_SLOPE_XYZ_INT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_SLOPE_XYZ_INT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_SLOPE_XYZ_INT__REG, &data,
							    C_One_U8X);
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_get_enable_slope_interrupt(unsigned char param, unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (param)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE1_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_SLOPE_X_INT);
			*status = data;
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE1_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_SLOPE_Y_INT);
			*status = data;
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE1_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_SLOPE_Z_INT);
			*status = data;
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_set_enable_tap_interrupt(unsigned char tapinterrupt)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	unsigned char state;
	state = C_One_U8X;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (tapinterrupt)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_DOUBLE_TAP_INT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_DOUBLE_TAP_INT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_DOUBLE_TAP_INT__REG, &data,
							    C_One_U8X);
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_SINGLE_TAP_INT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_SINGLE_TAP_INT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_SINGLE_TAP_INT__REG, &data,
							    C_One_U8X);
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_ORIENT_INT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_ORIENT_INT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_ORIENT_INT__REG, &data,
							    C_One_U8X);
			break;
		case C_Three_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_FLAT_INT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_FLAT_INT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_FLAT_INT__REG, &data,
							    C_One_U8X);
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_get_enable_tap_interrupt(unsigned char param, unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (param)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE1_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_DOUBLE_TAP_INT);
			*status = data;
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE1_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_SINGLE_TAP_INT);
			*status = data;
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE1_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_ORIENT_INT);
			*status = data;
			break;
		case C_Three_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE1_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_FLAT_INT);
			*status = data;
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_set_enable_high_g_interrupt(unsigned char highinterrupt)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	unsigned char state;
	state = 0x01;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (highinterrupt)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_HIGHG_X_INT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_HIGHG_X_INT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_HIGHG_X_INT__REG, &data,
							    C_One_U8X);
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_HIGHG_Y_INT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_HIGHG_Y_INT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_HIGHG_Y_INT__REG, &data,
							    C_One_U8X);
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_HIGHG_Z_INT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_HIGHG_Z_INT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_HIGHG_Z_INT__REG, &data,
							    C_One_U8X);
			break;
		case C_Three_U8X:
			state = 0x07;
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_HIGHG_XYZ_INT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_HIGHG_XYZ_INT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_HIGHG_XYZ_INT__REG, &data,
							    C_One_U8X);
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_get_enable_high_g_interrupt(unsigned char param, unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (param)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE2_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_HIGHG_X_INT);
			*status = data;
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE2_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_HIGHG_Y_INT);
			*status = data;
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE2_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_HIGHG_Z_INT);
			*status = data;
			break;
		case C_Three_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE2_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_HIGHG_XYZ_INT);
			*status = data;
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_set_enable_low_g_interrupt(void)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	unsigned char state;
	state = 0x01;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_LOWG_INT__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_EN_LOWG_INT, state);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_LOWG_INT__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_enable_low_g_interrupt(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE2_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_EN_LOWG_INT);
		*status = data;
		}
	return comres;
}

int bma222_set_enable_data_interrupt(void)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	unsigned char state;
	state = 0x01;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_NEW_DATA_INT__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_EN_NEW_DATA_INT, state);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_NEW_DATA_INT__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_enable_data_interrupt(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE2_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_EN_NEW_DATA_INT);
		*status = data;
		}
	return comres;
}

int bma222_set_int1_pad_sel(unsigned char int1sel)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	unsigned char state;
	state = 0x01;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (int1sel)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_LOWG__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT1_PAD_LOWG, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_LOWG__REG, &data,
							    C_One_U8X);
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_HIGHG__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT1_PAD_HIGHG, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_HIGHG__REG, &data,
							    C_One_U8X);
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_SLOPE__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT1_PAD_SLOPE, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_SLOPE__REG, &data,
							    C_One_U8X);
			break;
		case C_Three_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_DB_TAP__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT1_PAD_DB_TAP, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_DB_TAP__REG, &data,
							    C_One_U8X);
			break;
		case C_Four_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_SNG_TAP__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT1_PAD_SNG_TAP, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_SNG_TAP__REG, &data,
							    C_One_U8X);
			break;
		case C_Five_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_ORIENT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT1_PAD_ORIENT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_ORIENT__REG, &data,
							    C_One_U8X);
			break;
		case C_Six_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_FLAT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT1_PAD_FLAT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_FLAT__REG, &data,
							    C_One_U8X);
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_get_int1_pad_sel(unsigned char param, unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (param)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT1_PAD_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT1_PAD_LOWG);
			*status = data;
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT1_PAD_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT1_PAD_HIGHG);
			*status = data;
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT1_PAD_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT1_PAD_SLOPE);
			*status = data;
			break;
		case C_Three_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT1_PAD_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT1_PAD_DB_TAP);
			*status = data;
			break;
		case C_Four_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT1_PAD_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT1_PAD_SNG_TAP);
			*status = data;
			break;
		case C_Five_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT1_PAD_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT1_PAD_ORIENT);
			*status = data;
			break;
		case C_Six_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT1_PAD_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT1_PAD_FLAT);
			*status = data;
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_set_int_data_sel(unsigned char intsel)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	unsigned char state;
	state = C_One_U8X;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (intsel)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_NEWDATA__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT1_PAD_NEWDATA, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT1_PAD_NEWDATA__REG, &data,
							    C_One_U8X);
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_NEWDATA__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT2_PAD_NEWDATA, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_NEWDATA__REG, &data,
							    C_One_U8X);
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_get_int_data_sel(unsigned char param, unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (param)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_DATA_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT1_PAD_NEWDATA);
			*status = data;
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_DATA_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT2_PAD_NEWDATA);
			*status = data;
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_set_int2_pad_sel(unsigned char int2sel)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	unsigned char state;
	state = C_One_U8X;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (int2sel)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_LOWG__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT2_PAD_LOWG, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_LOWG__REG, &data,
							    C_One_U8X);
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_HIGHG__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT2_PAD_HIGHG, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_HIGHG__REG, &data,
							    C_One_U8X);
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_SLOPE__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT2_PAD_SLOPE, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_SLOPE__REG, &data,
							    C_One_U8X);
			break;
		case C_Three_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_DB_TAP__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT2_PAD_DB_TAP, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_DB_TAP__REG, &data,
							    C_One_U8X);
			break;
		case C_Four_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_SNG_TAP__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT2_PAD_SNG_TAP, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_SNG_TAP__REG, &data,
							    C_One_U8X);
			break;
		case C_Five_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_ORIENT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT2_PAD_ORIENT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_ORIENT__REG, &data,
							    C_One_U8X);
			break;
		case C_Six_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_FLAT__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_INT2_PAD_FLAT, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_INT2_PAD_FLAT__REG, &data,
							    C_One_U8X);
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_get_int2_pad_sel(unsigned char param, unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (param)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT2_PAD_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT2_PAD_LOWG);
			*status = data;
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT2_PAD_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT2_PAD_HIGHG);
			*status = data;
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT2_PAD_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT2_PAD_SLOPE);
			*status = data;
			break;
		case C_Three_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT2_PAD_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT2_PAD_DB_TAP);
			*status = data;
			break;
		case C_Four_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT2_PAD_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT2_PAD_SNG_TAP);
			*status = data;
			break;
		case C_Five_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT2_PAD_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT2_PAD_ORIENT);
			*status = data;
			break;
		case C_Six_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT2_PAD_SEL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_INT2_PAD_FLAT);
			*status = data;
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_set_int_src(unsigned char intsrc)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	unsigned char state;
	state = C_One_U8X;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (intsrc)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_UNFILT_INT_SRC_LOWG__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_UNFILT_INT_SRC_LOWG, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_UNFILT_INT_SRC_LOWG__REG, &data,
							    C_One_U8X);
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_UNFILT_INT_SRC_HIGHG__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_UNFILT_INT_SRC_HIGHG, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_UNFILT_INT_SRC_HIGHG__REG, &data,
							    C_One_U8X);
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_UNFILT_INT_SRC_SLOPE__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_UNFILT_INT_SRC_SLOPE, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_UNFILT_INT_SRC_SLOPE__REG, &data,
							    C_One_U8X);
			break;
		case C_Three_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_UNFILT_INT_SRC_TAP__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_UNFILT_INT_SRC_TAP, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_UNFILT_INT_SRC_TAP__REG, &data,
							    C_One_U8X);
			break;
		case C_Four_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_UNFILT_INT_SRC_DATA__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_UNFILT_INT_SRC_DATA, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_UNFILT_INT_SRC_DATA__REG, &data,
							    C_One_U8X);
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_get_int_src(unsigned char param, unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (param)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_SRC_REG, &data, C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_UNFILT_INT_SRC_LOWG);
			*status = data;
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_SRC_REG, &data, C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_UNFILT_INT_SRC_HIGHG);
			*status = data;
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_SRC_REG, &data, C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_UNFILT_INT_SRC_SLOPE);
			*status = data;
			break;
		case C_Three_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_SRC_REG, &data, C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_UNFILT_INT_SRC_TAP);
			*status = data;
			break;
		case C_Four_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_SRC_REG, &data, C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_UNFILT_INT_SRC_DATA);
			*status = data;
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_set_int_set(unsigned char intset, unsigned char state)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (intset)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT1_PAD_ACTIVE_LEVEL__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_INT1_PAD_ACTIVE_LEVEL, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_INT1_PAD_ACTIVE_LEVEL__REG,
							    &data, C_One_U8X);
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT1_PAD_OUTPUT_TYPE__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_INT1_PAD_OUTPUT_TYPE, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_INT1_PAD_OUTPUT_TYPE__REG, &data,
							    C_One_U8X);
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT2_PAD_ACTIVE_LEVEL__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_INT2_PAD_ACTIVE_LEVEL, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_INT2_PAD_ACTIVE_LEVEL__REG,
							    &data, C_One_U8X);
			break;
		case C_Three_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT2_PAD_OUTPUT_TYPE__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_INT2_PAD_OUTPUT_TYPE, state);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_INT2_PAD_OUTPUT_TYPE__REG, &data,
							    C_One_U8X);
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_get_int_set(unsigned char param, unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (param)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_SET_REG, &data, C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_INT1_PAD_ACTIVE_LEVEL);
			*status = data;
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_SET_REG, &data, C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_INT1_PAD_OUTPUT_TYPE);
			*status = data;
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_SET_REG, &data, C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_INT2_PAD_ACTIVE_LEVEL);
			*status = data;
			break;
		case C_Three_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_SET_REG, &data, C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_INT2_PAD_OUTPUT_TYPE);
			*status = data;
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_get_mode_ctrl(unsigned char *mode)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_MODE_CTRL_REG, &data, C_One_U8X);
		*mode = data;
		}
	return comres;
}

int bma222_set_low_g_duration(unsigned char duration)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_LOWG_DUR__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_LOWG_DUR, duration);
		comres = p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_LOWG_DUR__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_low_g_duration(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_LOW_DURN_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_LOWG_DUR);
		*status = data;
		}
	return comres;
}

int bma222_set_low_g_threshold(unsigned char threshold)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_LOWG_THRES__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_LOWG_THRES, threshold);
		comres = p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_LOWG_THRES__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_low_g_threshold(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_LOW_THRES_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_LOWG_THRES);
		*status = data;
		}
	return comres;
}

int bma222_set_high_g_duration(unsigned char duration)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_HIGHG_DUR__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_HIGHG_DUR, duration);
		comres = p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_HIGHG_DUR__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_high_g_duration(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_HIGH_DURN_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_HIGHG_DUR);
		*status = data;
		}
	return comres;
}

int bma222_set_high_g_threshold(unsigned char threshold)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_HIGHG_THRES__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_HIGHG_THRES, threshold);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_HIGHG_THRES__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_high_g_threshold(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_HIGH_THRES_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_HIGHG_THRES);
		*status = data;
		}
	return comres;
}

int bma222_set_slope_duration(unsigned char duration)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_SLOPE_DUR__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_SLOPE_DUR, duration);
		comres = p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_SLOPE_DUR__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_slope_duration(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_SLOPE_DURN_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_SLOPE_DUR);
		*status = data;
		}
	return comres;
}

int bma222_set_slope_threshold(unsigned char threshold)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_SLOPE_THRES__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_SLOPE_THRES, threshold);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_SLOPE_THRES__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_slope_threshold(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_SLOPE_THRES_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_SLOPE_THRES);
		*status = data;
		}
	return comres;
}

int bma222_set_tap_duration(unsigned char duration)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_TAP_DUR__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_TAP_DUR, duration);
		comres = p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_TAP_DUR__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_tap_duration(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_TAP_PARAM_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_TAP_DUR);
		*status = data;
		}
	return comres;
}

int bma222_set_tap_shock(unsigned char setval)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_TAP_SHOCK_DURN__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_TAP_SHOCK_DURN, setval);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_TAP_SHOCK_DURN__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_tap_shock(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_TAP_PARAM_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_TAP_SHOCK_DURN);
		*status = data;
		}
	return comres;
}

int bma222_set_tap_quiet_duration(unsigned char duration)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_TAP_QUIET_DURN__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_TAP_QUIET_DURN, duration);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_TAP_QUIET_DURN__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_tap_quiet(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_TAP_PARAM_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_TAP_QUIET_DURN);
		*status = data;
		}
	return comres;
}

int bma222_set_tap_threshold(unsigned char threshold)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_TAP_THRES__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_TAP_THRES, threshold);
		comres = p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_TAP_THRES__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_tap_threshold(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_TAP_THRES_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_TAP_THRES);
		*status = data;
		}
	return comres;
}

int bma222_set_tap_samp(unsigned char samp)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_TAP_SAMPLES__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_TAP_SAMPLES, samp);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_TAP_SAMPLES__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_tap_samp(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_TAP_THRES_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_TAP_SAMPLES);
		*status = data;
		}
	return comres;
}

int bma222_set_orient_mode(unsigned char mode)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_ORIENT_MODE__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_ORIENT_MODE, mode);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_ORIENT_MODE__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_orient_mode(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_ORIENT_PARAM_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_ORIENT_MODE);
		*status = data;
		}
	return comres;
}

int bma222_set_orient_blocking(unsigned char orientblk)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_ORIENT_BLOCK__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_ORIENT_BLOCK, orientblk);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_ORIENT_BLOCK__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_orient_blocking(unsigned char *orientblk)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_ORIENT_PARAM_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_ORIENT_BLOCK);
		*orientblk = data;
		}
	return comres;
}

int bma222_set_orient_hyst(unsigned char orienthyst)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_ORIENT_HYST__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_ORIENT_HYST, orienthyst);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_ORIENT_HYST__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_orient_hyst(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_ORIENT_PARAM_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_ORIENT_HYST);
		*status = data;
		}
	return comres;
}

int bma222_set_theta_blocking(unsigned char thetablk)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_THETA_BLOCK__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_THETA_BLOCK, thetablk);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_THETA_BLOCK__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_theta_blocking(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_THETA_BLOCK_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_THETA_BLOCK);
		*status = data;
		}
	return comres;
}

int bma222_set_orient_ex(unsigned char orientex)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_ORIENT_AXIS__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_ORIENT_AXIS, orientex);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_ORIENT_AXIS__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_orient_ex(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_THETA_BLOCK_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_ORIENT_AXIS);
		*status = data;
		}
	return comres;
}

int bma222_set_theta_flat(unsigned char thetaflat)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_THETA_FLAT__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_THETA_FLAT, thetaflat);
		comres = p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_THETA_FLAT__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_theta_flat(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_THETA_FLAT_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_THETA_FLAT);
		*status = data;
		}
	return comres;
}

int bma222_set_flat_hold_time(unsigned char holdtime)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_FLAT_HOLD_TIME__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_FLAT_HOLD_TIME, holdtime);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_FLAT_HOLD_TIME__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_flat_hold_time(unsigned char *holdtime)
{
	int comres = C_Zero_U8X;
	unsigned char data1;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_FLAT_HOLD_TIME_REG, &data1, C_One_U8X);
		data1 = bma222_GET_BITSLICE(data1, bma222_FLAT_HOLD_TIME);
		*holdtime = data1;
		}
	return comres;
}

int bma222_get_low_power_state(unsigned char *Lowpower)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_STATUS_LOW_POWER_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_LOW_POWER_MODE_S);
		*Lowpower = data;
		}
	return comres;
}

int bma222_set_selftest_st(unsigned char selftest)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_SELF_TEST__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_EN_SELF_TEST, selftest);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_SELF_TEST__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_selftest_st(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_SELF_TEST_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_EN_SELF_TEST);
		*status = data;
		}
	return comres;
}

int bma222_set_selftest_stn(unsigned char stn)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_NEG_SELF_TEST__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_NEG_SELF_TEST, stn);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_NEG_SELF_TEST__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_selftest_stn(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_SELF_TEST_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_NEG_SELF_TEST);
		*status = data;
		}
	return comres;
}

int bma222_set_selftest_st_amp(unsigned char stamp)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_SELF_TEST_AMP__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_SELF_TEST_AMP, stamp);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_SELF_TEST_AMP__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_selftest_st_amp(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_SELF_TEST_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_SELF_TEST_AMP);
		*status = data;
		}
	return comres;
}

int bma222_set_ee_w(unsigned char eew)
{
	unsigned char data;
	int comres;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_UNLOCK_EE_WRITE_SETTING__REG, &data,
						   C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_UNLOCK_EE_WRITE_SETTING, eew);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_UNLOCK_EE_WRITE_SETTING__REG, &data,
						    C_One_U8X);
		}
	return comres;
}

int bma222_get_ee_w(unsigned char *eew)
{
	int comres;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EE_WRITE_SETTING_S__REG, eew, C_One_U8X);
		*eew = bma222_GET_BITSLICE(*eew, bma222_EE_WRITE_SETTING_S);
		}
	return comres;
}

int bma222_set_ee_prog_trig(void)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	unsigned char eeprog;
	eeprog = 0x01;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_START_EE_WRITE_SETTING__REG, &data,
						   C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_START_EE_WRITE_SETTING, eeprog);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_START_EE_WRITE_SETTING__REG, &data,
						    C_One_U8X);
		}
	return comres;
}

int bma222_get_eeprom_writing_status(unsigned char *eewrite)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EEPROM_CTRL_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_EE_WRITE_SETTING_S);
		*eewrite = data;
		}
	return comres;
}

int bma222_set_update_image(void)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	unsigned char update;
	update = C_One_U8X;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_UPDATE_IMAGE__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_UPDATE_IMAGE, update);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_UPDATE_IMAGE__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_set_3wire_spi(void)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	unsigned char update;
	update = 0x01;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_SPI_MODE_3__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_EN_SPI_MODE_3, update);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_SPI_MODE_3__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_3wire_spi(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_SERIAL_CTRL_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_EN_SPI_MODE_3);
		*status = data;
		}
	return comres;
}

int bma222_set_i2c_wdt_timer(unsigned char timedly)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_I2C_WATCHDOG_PERIOD__REG, &data,
						   C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_I2C_WATCHDOG_PERIOD, timedly);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_I2C_WATCHDOG_PERIOD__REG, &data,
						    C_One_U8X);
		}
	return comres;
}

int bma222_get_i2c_wdt_timer(unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_SERIAL_CTRL_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_I2C_WATCHDOG_PERIOD);
		*status = data;
		}
	return comres;
}

int bma222_set_hp_en(unsigned char param, unsigned char hpval)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		 {
		switch (param)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_SLOW_COMP_X__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_SLOW_COMP_X, hpval);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_SLOW_COMP_X__REG, &data,
							    C_One_U8X);
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_SLOW_COMP_Y__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_SLOW_COMP_Y, hpval);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_SLOW_COMP_Y__REG, &data,
							    C_One_U8X);
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_SLOW_COMP_Z__REG, &data,
							   C_One_U8X);
			data = bma222_SET_BITSLICE(data, bma222_EN_SLOW_COMP_Z, hpval);
			comres =
			    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_SLOW_COMP_Z__REG, &data,
							    C_One_U8X);
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_get_hp_en(unsigned char param, unsigned char *status)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		switch (param)
			{
		case C_Zero_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_OFFSET_CTRL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_SLOW_COMP_X);
			*status = data;
			break;
		case C_One_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_OFFSET_CTRL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_SLOW_COMP_Y);
			*status = data;
			break;
		case C_Two_U8X:
			comres =
			    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_OFFSET_CTRL_REG, &data,
							   C_One_U8X);
			data = bma222_GET_BITSLICE(data, bma222_EN_SLOW_COMP_Z);
			*status = data;
			break;
		default:
			break;
			}
		}
	return comres;
}

int bma222_get_cal_ready(unsigned char *calrdy)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_OFFSET_CTRL_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_FAST_COMP_RDY_S);
		*calrdy = data;
		}
	return comres;
}

int bma222_set_cal_trigger(unsigned char caltrigger)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EN_FAST_COMP__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_EN_FAST_COMP, caltrigger);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_EN_FAST_COMP__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_set_offset_reset(void)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	unsigned char offsetreset;
	offsetreset = C_One_U8X;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_RESET_OFFSET_REGS__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_RESET_OFFSET_REGS, offsetreset);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_RESET_OFFSET_REGS__REG, &data,
						    C_One_U8X);
		}
	return comres;
}

int bma222_set_offset_cutoff(unsigned char offsetcutoff)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_COMP_CUTOFF__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_COMP_CUTOFF, offsetcutoff);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_COMP_CUTOFF__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_offset_cutoff(unsigned char *cutoff)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_OFFSET_PARAMS_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_COMP_CUTOFF);
		*cutoff = data;
		}
	return comres;
}

int bma222_set_offset_target_x(unsigned char offsettarget)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_COMP_TARGET_OFFSET_X__REG, &data,
						   C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_COMP_TARGET_OFFSET_X, offsettarget);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_COMP_TARGET_OFFSET_X__REG, &data,
						    C_One_U8X);
		}
	return comres;
}

int bma222_get_offset_target_x(unsigned char *offsettarget)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_OFFSET_PARAMS_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_COMP_TARGET_OFFSET_X);
		*offsettarget = data;
		}
	return comres;
}

int bma222_set_offset_target_y(unsigned char offsettarget)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_COMP_TARGET_OFFSET_Y__REG, &data,
						   C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_COMP_TARGET_OFFSET_Y, offsettarget);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_COMP_TARGET_OFFSET_Y__REG, &data,
						    C_One_U8X);
		}
	return comres;
}

int bma222_get_offset_target_y(unsigned char *offsettarget)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_OFFSET_PARAMS_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_COMP_TARGET_OFFSET_Y);
		*offsettarget = data;
		}
	return comres;
}

int bma222_set_offset_target_z(unsigned char offsettarget)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_COMP_TARGET_OFFSET_Z__REG, &data,
						   C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_COMP_TARGET_OFFSET_Z, offsettarget);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_COMP_TARGET_OFFSET_Z__REG, &data,
						    C_One_U8X);
		}
	return comres;
}

int bma222_get_offset_target_z(unsigned char *offsettarget)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_OFFSET_PARAMS_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_COMP_TARGET_OFFSET_Z);
		*offsettarget = data;
		}
	return comres;
}

int bma222_set_offset_filt_x(unsigned char offsetfilt)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		data = offsetfilt;
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_OFFSET_FILT_X_REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_offset_filt_x(unsigned char *offsetfilt)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_OFFSET_FILT_X_REG, &data, C_One_U8X);
		*offsetfilt = data;
		}
	return comres;
}

int bma222_set_offset_filt_y(unsigned char offsetfilt)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		data = offsetfilt;
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_OFFSET_FILT_Y_REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_offset_filt_y(unsigned char *offsetfilt)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_OFFSET_FILT_Y_REG, &data, C_One_U8X);
		*offsetfilt = data;
		}
	return comres;
}

int bma222_set_offset_filt_z(unsigned char offsetfilt)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		data = offsetfilt;
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_OFFSET_FILT_Z_REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_offset_filt_z(unsigned char *offsetfilt)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_OFFSET_FILT_Z_REG, &data, C_One_U8X);
		*offsetfilt = data;
		}
	return comres;
}

int bma222_set_offset_unfilt_x(unsigned char offsetfilt)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		data = offsetfilt;
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_OFFSET_UNFILT_X_REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_offset_unfilt_x(unsigned char *offsetfilt)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_OFFSET_UNFILT_X_REG, &data, C_One_U8X);
		*offsetfilt = data;
		}
	return comres;
}

int bma222_set_offset_unfilt_y(unsigned char offsetfilt)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		data = offsetfilt;
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_OFFSET_UNFILT_Y_REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_offset_unfilt_y(unsigned char *offsetfilt)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_OFFSET_UNFILT_Y_REG, &data, C_One_U8X);
		*offsetfilt = data;
		}
	return comres;
}

int bma222_set_offset_unfilt_z(unsigned char offsetfilt)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		data = offsetfilt;
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_OFFSET_UNFILT_Z_REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_offset_unfilt_z(unsigned char *offsetfilt)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_OFFSET_UNFILT_Z_REG, &data, C_One_U8X);
		*offsetfilt = data;
		}
	return comres;
}

int bma222_set_Int_Mode(unsigned char Mode)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_MODE_SEL__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_INT_MODE_SEL, Mode);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_INT_MODE_SEL__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_Int_Mode(unsigned char *Mode)
{
	int comres = C_Zero_U8X;
	unsigned char data1;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_MODE_SEL__REG, &data1, C_One_U8X);
		data1 = bma222_GET_BITSLICE(data1, bma222_INT_MODE_SEL);
		*Mode = data1;
		}
	return comres;
}

int bma222_set_Int_Enable(unsigned char InterruptType, unsigned char value)
{
	int comres = C_Zero_U8X;
	unsigned char data1, data2;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE1_REG, &data1, C_One_U8X);
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE2_REG, &data2, C_One_U8X);

value = value & C_One_U8X;
		switch (InterruptType)
			{
		case C_Zero_U8X:
			    /* Low G Interrupt  */
			    data2 = bma222_SET_BITSLICE(data2, bma222_EN_LOWG_INT, value);
			break;
		case C_One_U8X:
			    /* High G X Interrupt */
			    data2 = bma222_SET_BITSLICE(data2, bma222_EN_HIGHG_X_INT, value);
			break;
		case C_Two_U8X:
			    /* High G Y Interrupt */
			    data2 = bma222_SET_BITSLICE(data2, bma222_EN_HIGHG_Y_INT, value);
			break;
		case C_Three_U8X:
			    /* High G Z Interrupt */
			    data2 = bma222_SET_BITSLICE(data2, bma222_EN_HIGHG_Z_INT, value);
			break;
		case C_Four_U8X:
			    /* New Data Interrupt  */
			    data2 = bma222_SET_BITSLICE(data2, bma222_EN_NEW_DATA_INT, value);
			break;
		case C_Five_U8X:
			    /* Slope X Interrupt */
			    data1 = bma222_SET_BITSLICE(data1, bma222_EN_SLOPE_X_INT, value);
			break;
		case C_Six_U8X:
			    /* Slope Y Interrupt */
			    data1 = bma222_SET_BITSLICE(data1, bma222_EN_SLOPE_Y_INT, value);
			break;
		case C_Seven_U8X:
			    /* Slope Z Interrupt */
			    data1 = bma222_SET_BITSLICE(data1, bma222_EN_SLOPE_Z_INT, value);
			break;
		case C_Eight_U8X:
			    /* Single Tap Interrupt */
			    data1 = bma222_SET_BITSLICE(data1, bma222_EN_SINGLE_TAP_INT, value);
			break;
		case C_Nine_U8X:
			    /* Double Tap Interrupt */
			    data1 = bma222_SET_BITSLICE(data1, bma222_EN_DOUBLE_TAP_INT, value);
			break;
		case C_Ten_U8X:
			    /* Orient Interrupt  */
			    data1 = bma222_SET_BITSLICE(data1, bma222_EN_ORIENT_INT, value);
			break;
		case C_Eleven_U8X:
			    /* Flat Interrupt */
			    data1 = bma222_SET_BITSLICE(data1, bma222_EN_FLAT_INT, value);
			break;
		default:
			break;
			}
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE1_REG, &data1, C_One_U8X);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_INT_ENABLE2_REG, &data2, C_One_U8X);
		}
	return comres;
}

int bma222_write_ee(unsigned char addr, unsigned char data)
{
	int comres;
	unsigned char ee_busy;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		if (p_bma222->delay_msec == C_Zero_U8X)
			{
			comres = E_SMB_NULL_PTR;
			}
		else
			{
			comres = bma222_get_ee_w(&ee_busy);
			if (ee_busy)
				{
				comres = E_EEPROM_BUSY;
				}
			else
				{
				comres += p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, addr, &data, C_One_U8X);
				addr += bma222_EEP_OFFSET;
				comres += p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, addr, &data, C_One_U8X);
				p_bma222->delay_msec(bma222_EE_W_DELAY);
				}
			}
		}
	return comres;
}

int bma222_set_low_hy(unsigned char hysval)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_LOWG_HYST__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_LOWG_HYST, hysval);
		comres = p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_LOWG_HYST__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_set_high_hy(unsigned char hysval)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_HIGHG_HYST__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_HIGHG_HYST, hysval);
		comres = p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_HIGHG_HYST__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_set_low_mode(unsigned char state)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres =
		    p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_LOWG_INT_MODE__REG, &data, C_One_U8X);
		data = bma222_SET_BITSLICE(data, bma222_LOWG_INT_MODE, state);
		comres =
		    p_bma222->bma222_BUS_WRITE_FUNC(p_bma222->dev_addr, bma222_LOWG_INT_MODE__REG, &data, C_One_U8X);
		}
	return comres;
}

int bma222_get_update_image_status(unsigned char *imagestatus)
{
	int comres = C_Zero_U8X;
	unsigned char data;
	if (p_bma222 == C_Zero_U8X)
		{
		comres = E_SMB_NULL_PTR;
		}
	else
		{
		comres = p_bma222->bma222_BUS_READ_FUNC(p_bma222->dev_addr, bma222_EEPROM_CTRL_REG, &data, C_One_U8X);
		data = bma222_GET_BITSLICE(data, bma222_UPDATE_IMAGE);
		*imagestatus = data;
		}
	return comres;
}
