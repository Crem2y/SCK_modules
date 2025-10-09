#pragma once

#define I2C_GCA 0x00         // General Call Address
#define I2C_SC (TWSR & 0xF8) // get Status Code

// master transmitter mode
#define I2C_SC_MT_SC   0x08 // A START condition has been transmitted
#define I2C_SC_MT_RSC  0x10 // A repeated START condition has been transmitted
#define I2C_SC_MT_SWA  0x18 // SLA+W has been transmitted, ACK has been received
#define I2C_SC_MT_SWN  0x20 // SLA+W has been transmitted, NOT ACK has been received
#define I2C_SC_MT_DBA  0x28 // Data byte has been transmitted, ACK has been received
#define I2C_SC_MT_DBN  0x30 // Data byte has been transmitted, NOT ACK has been received
#define I2C_SC_MT_AL   0x38 // Arbitration lost in SLA+W or data bytes

// master receiver mode
#define I2C_SC_MR_SC   0x08 // A START condition has been transmitted
#define I2C_SC_MR_RSC  0x10 // A repeated START condition has been transmitted
#define I2C_SC_MR_AL   0x38 // Arbitration lost in SLA+R or NOT ACK bit
#define I2C_SC_MR_SRA  0x40 // SLA+R has been transmitted, ACK has been received
#define I2C_SC_MR_SRN  0x48 // SLA+R has been transmitted, NOT ACK has been received
#define I2C_SC_MR_DBA  0x50 // Data byte has been received, ACK has been returned
#define I2C_SC_MR_DBN  0x58 // Data byte has been received, NOT ACK has been returned

// slave receiver mode
#define I2C_SC_SR_SWA  0x60 // Own SLA+W has been received, ACK has been returned
#define I2C_SC_SR_AWA  0x68 // Arbitration lost in SLA+R/W as Master, own SLA+W has been received, ACK has been returned
#define I2C_SC_SR_GNC  0x70 // General call address has been received, ACK has been returned
#define I2C_SC_SR_AGA  0x78 // Arbitration lost in SLA+R/W as Master, General call address has been received, ACK has been returned
#define I2C_SC_SR_DBA  0x80 // Previously addressed with own SLA+W, data has been received, ACK has been returned
#define I2C_SC_SR_DBN  0x88 // Previously addressed with own SLA+W, data has been received, NOT ACK has been returned
#define I2C_SC_SR_GNA  0x90 // Previously addressed with general call, data has been received, ACK has been returned
#define I2C_SC_SR_GNN  0x98 // Previously addressed with general call, data has been received, NOT ACK has been returned
#define I2C_SC_SR_STO  0xA0 // A STOP condition or repeated START condition has been received while still addressed as Slave

// slave transmitter mode
#define I2C_SC_ST_SRA  0xA8 // Own SLA+R has been received, ACK has been returned
#define I2C_SC_ST_ARA  0xB0 // Arbitration lost in SLA+R/W as Master, own SLA+R has been received, ACK has been returned
#define I2C_SC_ST_DBA  0xB8 // Data byte in TWDR has been transmitted, ACK has been received
#define I2C_SC_ST_DBN  0xC8 // Data byte in TWDR has been transmitted, NOT ACK has been received

// error code
#define I2C_SC_ER_NA   0xF8 // No relevant state information available, TWINT = “0”
#define I2C_SC_ER_ERR  0x00 // Bus error due to an illegal START or STOP condition