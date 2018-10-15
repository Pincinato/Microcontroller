#include "tmc260_driver.h"

static int8_t tmc260_Registers_sendAndReceive(tmc260_RegisterSet * const _self, tmc260_register registerSelect){
    uint8_t * selectedRegister = NULL;

    switch(registerSelect){
    case TMC260_DRVCTRL:    selectedRegister = _self->driverControlRegister.bytes;          break;
    case TMC260_CHOPCONF:   selectedRegister = _self->chopperControlRegister.bytes;         break;
    case TMC260_SMARTEN:    selectedRegister = _self->coolStepControlRegister.bytes;        break;
    case TMC260_SGSCONF:    selectedRegister = _self->stallGuard2ControlRegister.bytes;     break;
    case TMC260_DRVCONF:    selectedRegister = _self->driverConfigurationRegister.bytes;    break;
    default:                return EXIT_FAILURE;
    }

    HAL_GPIO_WritePin(TMC260_CHIP_SELECT_GPIO_Port, TMC260_CHIP_SELECT_Pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(&hspi2, &selectedRegister[TMC260_BYTE_2], &_self->readResponse.bytes[TMC260_BYTE_2], NUMBER_OF_BYTES_PER_TRANSMISSION, TIMEOUT_FOR_TRANSMUSSION);
    HAL_SPI_TransmitReceive(&hspi2, &selectedRegister[TMC260_BYTE_1], &_self->readResponse.bytes[TMC260_BYTE_1], NUMBER_OF_BYTES_PER_TRANSMISSION, TIMEOUT_FOR_TRANSMUSSION);
    HAL_SPI_TransmitReceive(&hspi2, &selectedRegister[TMC260_BYTE_0], &_self->readResponse.bytes[TMC260_BYTE_0], NUMBER_OF_BYTES_PER_TRANSMISSION, TIMEOUT_FOR_TRANSMUSSION);
    HAL_GPIO_WritePin(TMC260_CHIP_SELECT_GPIO_Port, TMC260_CHIP_SELECT_Pin, GPIO_PIN_SET);
    return EXIT_SUCCESS;
};

static tmc260_RegisterSet  defaultConfig =
{
    .driverControlRegister =           {.bits_StepDir =
                                        {.microstepResolution = 0b110,
                                         .reserved_1 = 0b0,
                                         .enableDoubleEdge = 0b1,
                                         .enableInterpolation = 0b0,
                                         .reserved_2 = 0b0,
                                         .registerAdressBit_1 = 0b0,
                                         .registerAdressBit_2 = 0b0}},

    .chopperControlRegister =           {.bits =
                                         {.offTimeOrMosfetDisable = 0b01,
                                          .hysteresisStartOrFastDecayTime = 0b111,
                                          .hysteresisEndOrSineWaveOffset = 0b110,
                                          .hysteresisDecrementIntervalOrFastDecayMode = 0b10,
                                          .randomToffTime = 0b01,
                                          .chopperMode = 0b0,
                                          .blankingTime = 0b10,
                                          .registerAdressBit_1 = 0b0,
                                          .registerAdressBit_2 = 0b0,
                                          .registerAdressBit_3 = 0b1}},

    .coolStepControlRegister =          {.bits =
                                         {.lowerCoolStepThresholdOrCoolStepDisable = 0b0,
                                          .reserved_1 = 0b0,
                                          .currentIncrementSize = 0b00,
                                          .reserved_2 = 0b0,
                                          .upperCoolStepThresholdAsOffsetFromLower = 0b0,
                                          .reserved_3 = 0b0,
                                          .currentDecrementSpeed = 0b00,
                                          .minimumCoolStepCurrent = 0b0,
                                          .reserved_4 = 0b0,
                                          .registerAdressBit_1 = 0b1,
                                          .registerAdressBit_2 = 0b0,
                                          .registerAdressBit_3 = 0b1}},

    .stallGuard2ControlRegister =       {.bits =
                                         {.currentScale = 0b11,
                                          .reserved_1 = 0b0,
                                          .reserved_2 = 0b0,
                                          .reserved_3 = 0b0,
                                          .stallGuard2ThresholdValue = 0b101,
                                          .reserved_4 = 0b0,
                                          .stallGuard2FilterEnable = 0b0,
                                          .registerAdressBit_1 = 0b0,
                                          .registerAdressBit_2 = 0b1,
                                          .registerAdressBit_3 = 0b1}},

    .driverConfigurationRegister =       {.bits =
                                          {.reserved_1 = 0b0,
                                           .reserved_2 = 0b0,
                                           .reserved_3 = 0b0,
                                           .reserved_4 = 0b0,
                                           .selectValueforReadOut = 0b01,
                                           .senseResistorVoltageBasedCurrentScallung = 0b0,
                                           .stepDirInterfaceDisable = 0b0,
                                           .shortToGndDetectionTimer = 0b0,
                                           .shortToGndProtectionDisable = 0b0,
                                           .reserved_5 = 0b0,
                                           .slopeControlLowSide = 0b0,
                                           .slopeControlHighSide = 0b0,
                                           .reservedTestMode = 0b0,
                                           .registerAdressBit_1 = 0b01,
                                           .registerAdressBit_2 = 0b01,
                                           .registerAdressBit_3 = 0b01}},
    .readResponse =                      {.bytes = {0}},
    .sendAndReceive =                    tmc260_Registers_sendAndReceive
};

int8_t tmc260_Registers_init(tmc260_RegisterSet * device){
    *device = defaultConfig;
    device->sendAndReceive(device, TMC260_DRVCTRL);
    device->sendAndReceive(device, TMC260_CHOPCONF);
    device->sendAndReceive(device, TMC260_SMARTEN);
    device->sendAndReceive(device, TMC260_SGSCONF);
    device->sendAndReceive(device, TMC260_DRVCONF);
    return EXIT_SUCCESS;
}
