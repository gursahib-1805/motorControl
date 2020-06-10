//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : HWDEFS.H
// FILE VERSION : 1.0
// PROGRAMMER   : Gursahib Singh
//----------------------------------------------------------------------------
// REVISION HISTORY
//----------------------------------------------------------------------------
//
// 1.0, 2019-NOV-01, Gursahib Singh
//   - Initial release
//
//----------------------------------------------------------------------------
// INCLUSION LOCK
//----------------------------------------------------------------------------

#ifndef HWDEFS_H_
#define HWDEFS_H_

//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------

#define SYSCTL_RCGCGPIO             0x400FE608 // hw_sysctl
#define SYSCTL_RCGCUART             0x400FE618 // hw_sysctl
#define SYSCTL_RESC                 0x400FE05C // hw_sysctl
#define SYSCTL_RCGCADC              0x400FE638 // hw_sysctl
#define SYSCTL_RCGCI2C              0x400FE620 // hw_sysctl
#define SYSCTL_RCGCPWM              0x400FE640 // hw_sysctl
#define SYSCTL_RCGCQEI              0x400FE644 // hw_sysctl
#define SYSCTL_SRQEI                0x400FE544 // hw_sysctl
#define SYSCTL_RCGCWD               0x400FE600 // hw_sysctl


#define SYSCTL_RCC                  0x400FE060 // hw_sysctl
#define SYSCTL_RCC2                 0x400FE070 // hw_sysctl
#define SYSCTL_PLLSTAT              0x400FE168 // hw_sysctl

#define GPIO_PORTA_BASE             0x40004000 // hw_memmap
#define GPIO_PORTB_BASE             0x40005000 // hw_memmap
#define GPIO_PORTD_BASE             0x40007000 // hw_memmap
#define GPIO_PORTE_BASE             0x40024000 // hw_memmap
#define GPIO_PORTF_BASE             0x40025000 // hw_memmap

#define GPIO_O_DATA                 0x00000000 // hw_gpio
#define GPIO_O_DIR                  0x00000400 // hw_gpio
#define GPIO_O_AFSEL                0x00000420 // hw_gpio
#define GPIO_O_ODR                  0x0000050C // hw_gpio
#define GPIO_O_DEN                  0x0000051C // hw_gpio
#define GPIO_O_AMSEL                0x00000528 // hw_gpio
#define GPIO_O_PCTL                 0x0000052C // hw_gpio
#define GPIO_O_CR                   0x00000524 // hw_gpio
#define GPIO_O_LOCK                 0x00000520 // hw_gpio
#define GPIO_O_IS                   0x00000404 // hw_gpio
#define GPIO_O_IEV                  0x0000040C // hw_gpio
#define GPIO_O_IM                   0x00000410 // hw_gpio
#define GPIO_O_ICR                  0x0000041C // hw_gpio

#define UART0_BASE                  0x4000C000 // hw_memmap

#define UART_O_DR                   0x00000000 // hw_uart
#define UART_O_FR                   0x00000018 // hw_uart
#define UART_O_IBRD                 0x00000024 // hw_uart
#define UART_O_FBRD                 0x00000028 // hw_uart
#define UART_O_CTL                  0x00000030 // hw_uart
#define UART_O_IM                   0x00000038 // hw_uart
#define UART_O_RIS                  0x0000003C // hw_uart
#define UART_O_MIS                  0x00000040 // hw_uart
#define UART_O_ICR                  0x00000044 // hw_uart
#define UART_O_LCRH                 0x0000002C // hw_uart

#define ADC0_BASE                   0x40038000 // hw_memmap

#define ADC_O_ACTSS                 0x00000000 // hw_adc
#define ADC_O_IM                    0x00000008 // hw_adc
#define ADC_O_ISC                   0x0000000C // hw_adc
#define ADC_O_EMUX                  0x00000014 // hw_adc
#define ADC_O_SSPRI                 0x00000020 // hw_adc
#define ADC_O_PSSI                  0x00000028 // hw_adc
#define ADC_O_SAC                   0x00000030 // hw_adc
#define ADC_O_SSMUX0                0x00000040 // hw_adc
#define ADC_O_SSCTL0                0x00000044 // hw_adc
#define ADC_O_SSFIFO0               0x00000048 // hw_adc
#define ADC_O_SSFSTAT0              0x0000004C // hw_adc

#define I2C0_BASE                   0x40020000 // hw_memmap

#define I2C_O_MSA                   0x00000000 // hw_i2c
#define I2C_O_MCS                   0x00000004 // hw_i2c
#define I2C_O_MDR                   0x00000008 // hw_i2c
#define I2C_O_MTPR                  0x0000000C // hw_i2c
#define I2C_O_MIMR                  0x00000010 // hw_i2c
#define I2C_O_MRIS                  0x00000014 // hw_i2c
#define I2C_O_MMIS                  0x00000018 // hw_i2c
#define I2C_O_MICR                  0x0000001C // hw_i2c
#define I2C_O_MCR                   0x00000020 // hw_i2c
#define I2C_O_MCLKOCNT              0x00000024 // hw_i2c
#define I2C_O_MBMON                 0x0000002C // hw_i2c
#define I2C_O_MCR2                  0x00000038 // hw_i2c

#define PWM0_BASE                   0x40028000 // hw_memmap

#define PWM_O_ENABLE                0x00000008 // hw_pwm
#define PWM_O_0_CTL                 0x00000040 // hw_pwm
#define PWM_O_0_GENA                0x00000060 // hw_pwm
#define PWM_O_0_GENB                0x00000064 // hw_pwm
#define PWM_O_0_LOAD                0x00000050 // hw_pwm
#define PWM_O_0_CMPA                0x00000058 // hw_pwm
#define PWM_O_0_CMPB                0x0000005C // hw_pwm

#define QEI0_BASE                   0x4002C000 // hw_pwm

#define QEI_O_CTL                   0x00000000 // hw_pwm
#define QEI_O_LOAD                  0x00000010 // hw_pwm
#define QEI_O_SPEED                 0x0000001C // hw_pwm

#define WDT0_BASE                   0x40000000 // hw_wdt

#define WDT_O_WDTLOAD               0x00000000 // hw_wdt
#define WDT_O_WDTCTL                0x00000008 // hw_wdt
#define WDT_O_WDTLOCK               0x00000C00 // hw_wdt

#define NVIC_ST_CTRL                0xE000E010 // hw_nvic
#define NVIC_ST_RELOAD              0xE000E014 // hw_nvic
#define NVIC_ST_CURRENT             0xE000E018 // hw_nvic
#define NVIC_APINT                  0xE000ED0C // hw_nvic
#define NVIC_APINT_VECTKEY          0x05FA0000 // hw_nvic
#define NVIC_APINT_SYSRESREQ        0x00000004 // hw_nvic

#define NVIC_EN0                    0xE000E100 // hw_nvic
#define NVIC_DIS0                   0xE000E180 // hw_nvic

#define HWREG( x ) ( *( ( volatile uint32_t * )( x ) ) )
#endif // HWDEFS_H_

//----------------------------------------------------------------------------
// END HWDEFS.H
//----------------------------------------------------------------------------
