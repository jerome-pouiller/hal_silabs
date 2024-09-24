/*******************************************************************************
 * @file  sl_platform_wireless.c
* @brief 
*******************************************************************************
* # License
* <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
*******************************************************************************
*
* The licensor of this software is Silicon Laboratories Inc. Your use of this
* software is governed by the terms of Silicon Labs Master Software License
* Agreement (MSLA) available at
* www.silabs.com/about-us/legal/master-software-license-agreement. This
* software is distributed to you in Source Code format and is governed by the
* sections of the MSLA applicable to Source Code.
*
******************************************************************************/

#include "rsi_wisemcu_hardware_setup.h"
#include "rsi_m4.h"
#include "rsi_rom_egpio.h"
#include "rsi_pll.h"
#include "rsi_rom_clks.h"
#include "rsi_rom_ulpss_clk.h"
#include "rsi_rom_power_save.h"
#include "sl_si91x_host_interface.h"
#include "sl_rsi_utility.h"
#include "system_si91x.h"
#include <stdio.h>
#include "cmsis_os2.h"
#include "sl_rsi_utility.h"

extern osEventFlagsId_t si91x_events;
extern osEventFlagsId_t si91x_bus_events;
extern osEventFlagsId_t si91x_async_events;
extern uint32_t frontend_switch_control;
extern osMutexId_t side_band_crypto_mutex;

/** @addtogroup SOC2
* @{
*/

/**
 * @brief	  Configure the default hardware configuration required for 'WiSeMCU' mode.
 * @param[in] None
 * @note  Must be called in main before using any power save related configurations in applications.
 * @return    Void
 */
void sl_si91x_hardware_setup(void)
{
  /* Disable OTHER_CLK that was enabled at Start-up*/
  RSI_CLK_PeripheralClkDisable3(M4CLK, M4_SOC_CLK_FOR_OTHER_ENABLE);

#ifndef SL_ULP_TIMER
  /* Disable Timer clock that was enabled in Bootloader*/
  RSI_ULPSS_TimerClkDisable(ULPCLK);
#endif

#if !(defined(SLI_SI917) || defined(SLI_SI917B0))
  /* Disable 40MHz Clocks*/
  RSI_ULPSS_DisableRefClks(MCU_ULP_40MHZ_CLK_EN);
#endif

  /* Power-Down Button Calibration*/
  RSI_PS_BodPwrGateButtonCalibDisable();

  /* Disable PTAT for Analog Peripherals*/
  RSI_PS_AnalogPeriPtatDisable();

  /* Disable PTAT for Brown-Out Detection Clocks*/
  RSI_PS_BodClksPtatDisable();

  /* Power-Down unused Analog(IPMU) Domain peripherals*/
  RSI_IPMU_PowerGateClr(AUXDAC_PG_ENB | AUXADC_PG_ENB | WURX_CORR_PG_ENB | WURX_PG_ENB | ULP_ANG_CLKS_PG_ENB
                        | CMP_NPSS_PG_ENB);

  /* Power-Down unused NPSS Domain peripherals*/
  RSI_PS_NpssPeriPowerDown(SLPSS_PWRGATE_ULP_MCUWDT | SLPSS_PWRGATE_ULP_MCUPS | SLPSS_PWRGATE_ULP_MCUTS
                           | SLPSS_PWRGATE_ULP_MCUSTORE2 | SLPSS_PWRGATE_ULP_MCUSTORE3
#ifndef SL_SLEEP_TIMER
                           | SLPSS_PWRGATE_ULP_MCURTC
#endif
  );

#ifndef DS_BASED_WKP
  RSI_PS_PowerSupplyDisable(POWER_ENABLE_DEEPSLEEP_TIMER);
#endif
  /* Power-Down unused NPSS Domain peripherals*/
  RSI_PS_PowerSupplyDisable(POWER_ENABLE_TIMESTAMPING);

#ifdef CHIP_9118
  /* Power-Down Unused M4SS Domain peripherals  */
  RSI_PS_M4ssPeriPowerDown(

#ifndef DEBUG_UART
    M4SS_PWRGATE_ULP_M4_FPU |
#endif
    M4SS_PWRGATE_ULP_ETHERNET | M4SS_PWRGATE_ULP_EFUSE | M4SS_PWRGATE_ULP_SDIO_SPI | M4SS_PWRGATE_ULP_USB
    | M4SS_PWRGATE_ULP_RPDMA
#ifndef DEBUG_UART
    | M4SS_PWRGATE_ULP_PERI1
#endif
    | M4SS_PWRGATE_ULP_PERI2 | M4SS_PWRGATE_ULP_PERI3 | M4SS_PWRGATE_ULP_CCI | M4SS_PWRGATE_ULP_SD_MEM);
  /* Power-Down unused ULPSS Domain peripherals*/
  RSI_PS_UlpssPeriPowerDown(
#ifndef SL_ULP_TIMER
    ULPSS_PWRGATE_ULP_MISC |
#endif
    ULPSS_PWRGATE_ULP_AUX | ULPSS_PWRGATE_ULP_CAP | ULPSS_PWRGATE_ULP_VAD
#ifndef DEBUG_UART
    | ULPSS_PWRGATE_ULP_UART
#endif
    | ULPSS_PWRGATE_ULP_SSI | ULPSS_PWRGATE_ULP_I2S | ULPSS_PWRGATE_ULP_I2C | ULPSS_PWRGATE_ULP_IR
    | ULPSS_PWRGATE_ULP_UDMA | ULPSS_PWRGATE_ULP_FIM);

  /* Turn off ULPSS SRAM domains*/
  RSI_PS_UlpssRamBanksPowerDown(ULPSS_2K_BANK_0 | ULPSS_2K_BANK_1 | ULPSS_2K_BANK_2 | ULPSS_2K_BANK_3 | ULPSS_2K_BANK_4
                                | ULPSS_2K_BANK_5 | ULPSS_2K_BANK_6 | ULPSS_2K_BANK_7);
  /* Turn off ULPSS SRAM Core/Periphery domains*/
  RSI_PS_UlpssRamBanksPeriPowerDown(ULPSS_2K_BANK_0 | ULPSS_2K_BANK_1 | ULPSS_2K_BANK_2 | ULPSS_2K_BANK_3
                                    | ULPSS_2K_BANK_4 | ULPSS_2K_BANK_5 | ULPSS_2K_BANK_6 | ULPSS_2K_BANK_7);
#endif

#ifdef SLI_SI917
  /* Power-Down Unused M4SS Domains */
  RSI_PS_M4ssPeriPowerDown(
#ifndef SLI_SI91X_MCU_ENABLE_FLASH_BASED_EXECUTION
    M4SS_PWRGATE_ULP_QSPI_ICACHE |
#endif
#ifndef DEBUG_UART
    M4SS_PWRGATE_ULP_EFUSE_PERI |
#endif
    M4SS_PWRGATE_ULP_SDIO_SPI | M4SS_PWRGATE_ULP_RPDMA);

  /* Power-Down Unused ULPSS Domain peripherals  */
  RSI_PS_UlpssPeriPowerDown(
#ifndef SL_ULP_TIMER
    ULPSS_PWRGATE_ULP_MISC |
#endif
    ULPSS_PWRGATE_ULP_AUX | ULPSS_PWRGATE_ULP_CAP
#ifndef DEBUG_UART
    | ULPSS_PWRGATE_ULP_UART
#endif
    | ULPSS_PWRGATE_ULP_SSI | ULPSS_PWRGATE_ULP_I2S | ULPSS_PWRGATE_ULP_I2C | ULPSS_PWRGATE_ULP_IR
    | ULPSS_PWRGATE_ULP_UDMA | ULPSS_PWRGATE_ULP_FIM);
#endif
  /* Power-Down High-Frequency PLL Domain */
  RSI_PS_SocPllSpiDisable();
  /* Power-Down QSPI-DLL Domain */
  RSI_PS_QspiDllDomainDisable();
  /* Configure PMU Start-up Time to be used on Wake-up*/
  RSI_PS_PmuGoodTimeDurationConfig(PMU_GOOD_TIME);
  /* Configure XTAL Start-up Time to be used on Wake-up*/
  RSI_PS_XtalGoodTimeDurationConfig(XTAL_GOOD_TIME);
  /*Enable first boot up*/
  RSI_PS_EnableFirstBootUp(1);
}

/**
 * @brief  This API is used to configure wireless GPIO front end controls from TA to M4
 * @return none
 */
void sli_si91x_configure_wireless_frontend_controls(uint32_t switch_sel)
{
#if SLI_SI91X_MCU_INTERFACE
  switch (switch_sel) {
    case FRONT_END_SWITCH_SEL0:
      //!GPIO 46,47,48
      break;
    case FRONT_END_SWITCH_SEL1:
#ifdef SLI_SI917B0
    {
      //!Program GPIO mode6 in ULP for ULP4,ULP5,ULP0 GPIOS
      RSI_EGPIO_SetPinMux(EGPIO1, 0, GPIO4, 6);
      RSI_EGPIO_SetPinMux(EGPIO1, 0, GPIO5, 6);
      RSI_EGPIO_SetPinMux(EGPIO1, 0, GPIO0, 6);
    }
#else
    {
      //!GPIO 46,47,48
    }
#endif
    break;
    case FRONT_END_SWITCH_SEL2:
#ifndef SLI_SI917B0
      //!Program GPIO mode6 in ULP for ULP4,ULP5,ULP0 GPIOS
      RSI_EGPIO_SetPinMux(EGPIO1, 0, GPIO4, 6);
      RSI_EGPIO_SetPinMux(EGPIO1, 0, GPIO5, 6);
      RSI_EGPIO_SetPinMux(EGPIO1, 0, GPIO0, 6);
#endif
      break;
    case FRONT_END_SWITCH_SEL3:
#ifndef SLI_SI917B0
      //!Program GPIO mode6 in ULP for ULP4,ULP5,ULP7 GPIOS
      RSI_EGPIO_SetPinMux(EGPIO1, 0, GPIO4, 6);
      RSI_EGPIO_SetPinMux(EGPIO1, 0, GPIO5, 6);
      RSI_EGPIO_SetPinMux(EGPIO1, 0, GPIO7, 6);
#endif
      break;
  }
#endif
}

/**
 * @brief	  Configure the default hardware configuration required for 'WiSeMCU' mode.
 * @param[in] rams_in_use                 - RAMs to be powered functionally (the rest of the RAM banks will be power gates)
 *                                          \n Macros used for this parameter:
 *                                          \n WISEMCU_0KB_RAM_IN_USE   : None of the RAMs will be powered , i.e., all RAM banks will be power gates
 *                                          \n WISEMCU_16KB_RAM_IN_USE  : Only 16KB  RAM will be retained
 *                                          \n WISEMCU_48KB_RAM_IN_USE  : Only 48KB  RAM will be retained
 *                                          \n WISEMCU_112KB_RAM_IN_USE : Only 112KB RAM will be retained
 *                                          \n WISEMCU_128KB_RAM_IN_USE : Only 128KB RAM will be retained
 *                                          \n WISEMCU_144KB_RAM_IN_USE : Only 114KB RAM will be retained
 *                                          \n WISEMCU_176KB_RAM_IN_USE : Only 176KB RAM will be retained
 *                                          \n WISEMCU_192KB_RAM_IN_USE : Only 192KB RAM will be retained
 *                                          \n WISEMCU_208KB_RAM_IN_USE : Only 208KB RAM will be retained
 *                                          \n WISEMCU_240KB_RAM_IN_USE : Only 240KB RAM will be retained
 *                                          \n WISEMCU_320KB_RAM_IN_USE : Only 320KB RAM will be retained
 *                                          \n WISEMCU_384KB_RAM_IN_USE : Only 384KB RAM will be retained
 *
 *                                          \n Macros used for 9117:
 *                                          \n WISEMCU_64KB_RAM_IN_USE     : 320KB  RAM will be retained
 *                                          \n WISEMCU_128KB_RAM_IN_USE    : 320KB  RAM will be retained
 *                                          \n WISEMCU_192KB_RAM_IN_USE    : 320KB  RAM will be retained
 *                                          \n WISEMCU_256KB_RAM_IN_USE    : 320KB  RAM will be retained
 *                                          
 * @param[in] rams_retention_during_sleep - Configure RAM retentions to the hardware so that particular RAM banks are retained during sleep
 *                                          \n Macros used for this parameter:
 *                                          \n WISEMCU_RETAIN_DEFAULT_RAM_DURING_SLEEP : Select the RAM Retention controls automatically by API based on 'rams_power_gate' value passed by user
 *                                          \n WISEMCU_RETAIN_16K_RAM_DURING_SLEEP     : Retain 16KB M4-ULP RAM
 *                                          \n WISEMCU_RETAIN_128K_RAM_DURING_SLEEP    : Retain 16KB M4-ULP RAM and 112KB M4-ULP RAM
 *                                          \n WISEMCU_RETAIN_192K_RAM_DURING_SLEEP    : Retain 16KB M4-ULP RAM and 112KB M4-ULP RAM and 64KB M4SS RAM
 *                                          \n WISEMCU_RETAIN_384K_RAM_DURING_SLEEP    : Retain 16KB M4-ULP RAM and 112KB M4-ULP RAM and 64KB M4SS RAM and TASS 192KB RAM
 *                                          \n WISEMCU_RETAIN_M4SS_RAM_DURING_SLEEP    : Retain Only 64KB M4SS RAM
 *                                          \n WISEMCU_RETAIN_ULPSS_RAM_DURING_SLEEP   : Retain Only 16KB ULPSS RAM
 *                                          \n WISEMCU_RETAIN_TASS_RAM_DURING_SLEEP    : Retain Only 192KB TASS RAM
 *                                          \n WISEMCU_RETAIN_M4ULP_RAM_DURING_SLEEP   : Retain Only 112KB M4-ULP RAM
 * @return    void
 * @note  Must be called in main before using any power save related configurations in applications.
 */
void sl_si91x_configure_ram_retention(uint32_t rams_in_use, uint32_t rams_retention_during_sleep)
{

  uint32_t rams_to_be_powered_down = rams_in_use;

#if (SL_SI91X_SI917_RAM_MEM_CONFIG == 1) // SL_SI91X_RAM_LEVEL_NWP_ADV_MCU_BASIC
  rams_to_be_powered_down &= ~(RAM_BANK_8 | RAM_BANK_9);
#elif (SL_SI91X_SI917_RAM_MEM_CONFIG == 2) // SL_SI91X_RAM_LEVEL_NWP_MEDIUM_MCU_MEDIUM
  rams_to_be_powered_down &= ~(RAM_BANK_9);
#endif

  /* Turn off Unused SRAMs*/
  RSI_PS_M4ssRamBanksPowerDown(rams_to_be_powered_down);

  /* Turn off Unused SRAM Core/Periphery domains*/
  RSI_PS_M4ssRamBanksPeriPowerDown(rams_in_use);

  /* Clear all RAM retention control before configuring the user RAM retentions*/
  RSI_PS_ClrRamRetention(M4ULP_RAM16K_RETENTION_MODE_EN | TA_RAM_RETENTION_MODE_EN | M4ULP_RAM_RETENTION_MODE_EN);

  /* If user selects the default RAM retentions, then select the RAM retentions based on RAM power gates*/
  if (rams_retention_during_sleep & WISEMCU_RETAIN_DEFAULT_RAM_DURING_SLEEP) {
    /* If none of the banks are powered on, clear all retention controls*/
    if (rams_in_use & WISEMCU_0KB_RAM_IN_USE) {
      RSI_PS_ClrRamRetention(M4ULP_RAM16K_RETENTION_MODE_EN | TA_RAM_RETENTION_MODE_EN | M4ULP_RAM_RETENTION_MODE_EN);
    }
    /* Set the 16KB SRAM memory retention */
    if (rams_in_use == WISEMCU_16KB_RAM_IN_USE) {
      RSI_PS_SetRamRetention(M4ULP_RAM16K_RETENTION_MODE_EN);
    }
    /* Set the full SRAM memory retention if the SRAM memory usage is greater than 16KB */
    /* For different SRAM retention modes, respective unused SRAM banks (both SRAM power and core/periphery domains) are powered down as part of the initial configuration above */
    else {
      RSI_PS_SetRamRetention(M4ULP_RAM16K_RETENTION_MODE_EN | M4ULP_RAM_RETENTION_MODE_EN);
    }
  } else {
    /* Program user configuration*/
    RSI_PS_SetRamRetention(rams_retention_during_sleep);
  }
}

/** @} */