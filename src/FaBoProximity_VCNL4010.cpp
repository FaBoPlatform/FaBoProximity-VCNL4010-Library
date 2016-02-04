#include "FaBoProximity_VCNL4010.h"

FaBoProximity_VCNL4010::FaBoProximity_VCNL4010(uint8_t addr) {
  _i2caddr = addr;
  Wire.begin();
}

bool FaBoProximity_VCNL4010::checkDevice(void) {
  if ( readId() == 0x21 ) {
    return true;
  } else {
    return false;
  }
}

uint8_t FaBoProximity_VCNL4010::readId(void) {
  return readI2c(VCNL4010_ID);
}

void FaBoProximity_VCNL4010::setupDevice(void) {
  setCurrent(20);
  setCommand(VCNL4010_COMMAND_ALL_DISABLE);
  setProximityRate(VCNL4010_PROX_MEASUREMENT_RATE_31);
  setCommand(VCNL4010_COMMAND_PROX_ENABLE |
             VCNL4010_COMMAND_AMBI_ENABLE |
             VCNL4010_COMMAND_SELFTIMED_MODE_ENABLE);
  setAmbiConfiguration(VCNL4010_AMBI_PARA_AVERAGE_32 |
                       VCNL4010_AMBI_PARA_AUTO_OFFSET_ENABLE |
                       VCNL4010_AMBI_PARA_MEAS_RATE_2);
}

void FaBoProximity_VCNL4010::setCurrent(uint8_t current) {
  if ( current > 20 ) {
    current = 20;
  }
  writeI2c(VCNL4010_PROX_CURRENT, current);
}

uint8_t FaBoProximity_VCNL4010::readCurrent(void) {
  return readI2c(VCNL4010_PROX_CURRENT);
}

void FaBoProximity_VCNL4010::setCommand(uint8_t command) {
  writeI2c(VCNL4010_COMMAND, command);
}

uint8_t FaBoProximity_VCNL4010::readCommandRegister(void) {
  return readI2c(VCNL4010_COMMAND);
}

void FaBoProximity_VCNL4010::setProximityRate(uint8_t rate) {
  writeI2c(VCNL4010_PROX_RATE, rate);
}

void FaBoProximity_VCNL4010::setAmbiConfiguration(uint8_t config) {
  writeI2c(VCNL4010_AMBI_PARAMETER, config);
}

bool FaBoProximity_VCNL4010::isProxDataReady(void) {
  if (readCommandRegister() & VCNL4010_COMMAND_MASK_PROX_DATA_READY) {
    return true;
  }
  return false;
}

bool FaBoProximity_VCNL4010::isAmbiDataReady(void) {
  if (readCommandRegister() & VCNL4010_COMMAND_MASK_AMBI_DATA_READY) {
    return true;
  }
  return false;
}

uint16_t FaBoProximity_VCNL4010::readProxValue(void) {
  uint16_t value;
  value = readI2c(VCNL4010_PROX_VALUE);
  value <<= 8;
  value |= readI2c(VCNL4010_PROX_VALUE+1);
  return value;
}

uint16_t FaBoProximity_VCNL4010::readAmbiValue(void) {
  uint16_t value;
  value = readI2c(VCNL4010_AMBI_VALUE);
  value <<= 8;
  value |= readI2c(VCNL4010_AMBI_VALUE+1);
  return value;
}

////////////////////////////////////////////////////////////////

void FaBoProximity_VCNL4010::setInterruptControl(uint8_t ctrl) {
  writeI2c(VCNL4010_INTERRUPT_CONTROL, ctrl);
}

void FaBoProximity_VCNL4010::setHighThreshold(uint8_t highThreshold) {
  unsigned char LoByte=0, HiByte=0;
  LoByte = (unsigned char)(highThreshold & 0x00ff);
  HiByte = (unsigned char)((highThreshold & 0xff00)>>8);

  writeI2c(VCNL4010_INTERRUPT_HIGH_THRES, HiByte);
  writeI2c(VCNL4010_INTERRUPT_HIGH_THRES+1, LoByte);
}

uint8_t FaBoProximity_VCNL4010::readInterruptStatus(void) {
  return readI2c(VCNL4010_INTERRUPT_STATUS);
}

void FaBoProximity_VCNL4010::setInterruptStatus(uint8_t status) {
  writeI2c(VCNL4010_INTERRUPT_STATUS, status);
}

////////////////////////////////////////////////////////////////

uint8_t FaBoProximity_VCNL4010::readI2c(uint8_t address) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(_i2caddr, (uint8_t)1);
  while(!Wire.available());
  return Wire.read();
}

void FaBoProximity_VCNL4010::writeI2c(uint8_t address, uint8_t data) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}
