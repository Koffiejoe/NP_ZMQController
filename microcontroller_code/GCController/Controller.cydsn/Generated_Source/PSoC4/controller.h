/*******************************************************************************
* File Name: controller.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_controller_H) /* Pins controller_H */
#define CY_PINS_controller_H

#include "cytypes.h"
#include "cyfitter.h"
#include "controller_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} controller_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   controller_Read(void);
void    controller_Write(uint8 value);
uint8   controller_ReadDataReg(void);
#if defined(controller__PC) || (CY_PSOC4_4200L) 
    void    controller_SetDriveMode(uint8 mode);
#endif
void    controller_SetInterruptMode(uint16 position, uint16 mode);
uint8   controller_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void controller_Sleep(void); 
void controller_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(controller__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define controller_DRIVE_MODE_BITS        (3)
    #define controller_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - controller_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the controller_SetDriveMode() function.
         *  @{
         */
        #define controller_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define controller_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define controller_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define controller_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define controller_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define controller_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define controller_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define controller_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define controller_MASK               controller__MASK
#define controller_SHIFT              controller__SHIFT
#define controller_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in controller_SetInterruptMode() function.
     *  @{
     */
        #define controller_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define controller_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define controller_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define controller_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(controller__SIO)
    #define controller_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(controller__PC) && (CY_PSOC4_4200L)
    #define controller_USBIO_ENABLE               ((uint32)0x80000000u)
    #define controller_USBIO_DISABLE              ((uint32)(~controller_USBIO_ENABLE))
    #define controller_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define controller_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define controller_USBIO_ENTER_SLEEP          ((uint32)((1u << controller_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << controller_USBIO_SUSPEND_DEL_SHIFT)))
    #define controller_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << controller_USBIO_SUSPEND_SHIFT)))
    #define controller_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << controller_USBIO_SUSPEND_DEL_SHIFT)))
    #define controller_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(controller__PC)
    /* Port Configuration */
    #define controller_PC                 (* (reg32 *) controller__PC)
#endif
/* Pin State */
#define controller_PS                     (* (reg32 *) controller__PS)
/* Data Register */
#define controller_DR                     (* (reg32 *) controller__DR)
/* Input Buffer Disable Override */
#define controller_INP_DIS                (* (reg32 *) controller__PC2)

/* Interrupt configuration Registers */
#define controller_INTCFG                 (* (reg32 *) controller__INTCFG)
#define controller_INTSTAT                (* (reg32 *) controller__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define controller_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(controller__SIO)
    #define controller_SIO_REG            (* (reg32 *) controller__SIO)
#endif /* (controller__SIO_CFG) */

/* USBIO registers */
#if !defined(controller__PC) && (CY_PSOC4_4200L)
    #define controller_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define controller_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define controller_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define controller_DRIVE_MODE_SHIFT       (0x00u)
#define controller_DRIVE_MODE_MASK        (0x07u << controller_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins controller_H */


/* [] END OF FILE */
