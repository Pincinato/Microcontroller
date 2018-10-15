#include "tmc260.h"
#include "tmc260_driver.h"


static uint8_t const tmc260_parameterMaximumRaitings[TMC260_NUMBER_OF_PARAMETERS] = {8, 31};
#define IS_IN_PARAMETER_RANGE(parameter, value) ((value) <= tmc260_parameterMaximumRaitings[(parameter)])
#define IS_VALID_PARAMETER(parameter) ((parameter)>=0 && (parameter)<TMC260_NUMBER_OF_PARAMETERS)

static uint8_t registerHasChanged[TMC260_NUMBER_OF_REGISTERS] = {0};
#define TMC260_REGISTER_NOT_CHANGED 0
#define TMC260_REGISTER_CHANGED 1

static int8_t tmc260_setParameter(tmc260_Device * const device, tmc260_parameter parameter, uint32_t value){
    /* validate parameter and value */
    if(IS_VALID_PARAMETER(parameter) == 0)
        return EXIT_FAILURE;
    if(IS_IN_PARAMETER_RANGE(parameter, value) == 0)
        return EXIT_FAILURE;

    /* assign parameter to register */
    tmc260_register currentRegister;
    if(parameter == TMC260_MICROSTEP_RESOLUTION)
        currentRegister = TMC260_DRVCTRL;
    else
        currentRegister = TMC260_SGSCONF;

    /* record changed register */
    registerHasChanged[currentRegister] = TMC260_REGISTER_CHANGED;

    /* assign parameter to bitfield member */
    switch(parameter){
    /* Driver Control Register Step/Dir */
    case TMC260_MICROSTEP_RESOLUTION:    device->registerSet->driverControlRegister.bits_StepDir.microstepResolution = value; break;
    /* StallGuard2 Control Register */
    case TMC260_CURRENT_SCALE:           device->registerSet->stallGuard2ControlRegister.bits.currentScale = value;           break;
    default:    return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

static int8_t tmc260_sendParameter(tmc260_Device * const device){
    for(tmc260_register registerIndex = TMC260_DRVCTRL; registerIndex < TMC260_NUMBER_OF_REGISTERS; registerIndex++){
        if(registerHasChanged[registerIndex]){
            device->registerSet->sendAndReceive(device->registerSet, registerIndex);
            registerHasChanged[registerIndex] = TMC260_REGISTER_NOT_CHANGED;
        }
    }
    return EXIT_SUCCESS;
}

/* One and only registerSet, pseudo SINGELTON */
static tmc260_RegisterSet singeltonRegisterSet = {0};

int8_t tmc260_init(tmc260_Device * const device){
    device->registerSet = &singeltonRegisterSet;
    tmc260_Registers_init(device->registerSet);
    device->setParameter = tmc260_setParameter;
    device->sendParameter = tmc260_sendParameter;
    return EXIT_SUCCESS;
}
