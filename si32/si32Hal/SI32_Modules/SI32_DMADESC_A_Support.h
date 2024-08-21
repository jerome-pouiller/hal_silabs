//------------------------------------------------------------------------------
// Copyright 2012 (c) Silicon Laboratories Inc.
//
// SPDX-License-Identifier: Zlib
//
// This siHAL software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//------------------------------------------------------------------------------

#ifndef __SI32_DMADESC_A_SUPPORT_H__
#define __SI32_DMADESC_A_SUPPORT_H__

// register definitions
#include "SI32_DMADESC_A_Registers.h"


//-----------------------------------------------------------------------------
// Define constants for aligning the primary and alternate descriptor arrays.

#define SI32_DMADESC_ALT_STRIDE 16

#define SI32_DMADESC_PRI_ALIGN  (2 * SI32_DMADESC_ALT_STRIDE * sizeof(SI32_DMADESC_A_Type))
#define SI32_DMADESC_ALT_ALIGN  (SI32_DMADESC_ALT_STRIDE * sizeof(SI32_DMADESC_A_Type))


//-----------------------------------------------------------------------------
// Define constants for the DMA descriptor config parameter.

#define SI32_DMADESC_A_CONFIG_RPOWER(rpower) \
    (((rpower) << SI32_DMADESC_A_CONFIG_RPOWER_SHIFT) & SI32_DMADESC_A_CONFIG_RPOWER_MASK)

#define SI32_DMADESC_A_CONFIG_NCOUNT(ncount) \
    (((ncount) << SI32_DMADESC_A_CONFIG_NCOUNT_SHIFT) & SI32_DMADESC_A_CONFIG_NCOUNT_MASK)

#define SI32_DMADESC_A_CONFIG_MOVE(SIZE, MODE) \
    ( SI32_DMADESC_A_CONFIG_DSTAIMD_## SIZE ##_U32 | SI32_DMADESC_A_CONFIG_DSTSIZE_## SIZE ##_U32 \
    | SI32_DMADESC_A_CONFIG_SRCAIMD_## SIZE ##_U32 | SI32_DMADESC_A_CONFIG_SRCSIZE_## SIZE ##_U32 \
    | SI32_DMADESC_A_CONFIG_TMD_## MODE ##_U32 )

#define SI32_DMADESC_A_CONFIG_RX(SIZE, MODE) \
    ( SI32_DMADESC_A_CONFIG_DSTAIMD_## SIZE ##_U32 | SI32_DMADESC_A_CONFIG_DSTSIZE_## SIZE ##_U32 \
    | SI32_DMADESC_A_CONFIG_SRCAIMD_NO_INCREMENT_U32 | SI32_DMADESC_A_CONFIG_SRCSIZE_## SIZE ##_U32 \
    | SI32_DMADESC_A_CONFIG_TMD_## MODE ##_U32 )

#define SI32_DMADESC_A_CONFIG_TX(SIZE, MODE) \
    ( SI32_DMADESC_A_CONFIG_DSTAIMD_NO_INCREMENT_U32 | SI32_DMADESC_A_CONFIG_DSTSIZE_## SIZE ##_U32 \
    | SI32_DMADESC_A_CONFIG_SRCAIMD_## SIZE ##_U32 | SI32_DMADESC_A_CONFIG_SRCSIZE_## SIZE ##_U32 \
    | SI32_DMADESC_A_CONFIG_TMD_## MODE ##_U32 )

#define SI32_DMADESC_A_CONFIG_PIPE(SIZE, MODE) \
    ( SI32_DMADESC_A_CONFIG_DSTAIMD_NO_INCREMENT_U32 | SI32_DMADESC_A_CONFIG_DSTSIZE_## SIZE ##_U32 \
    | SI32_DMADESC_A_CONFIG_SRCAIMD_NO_INCREMENT_U32 | SI32_DMADESC_A_CONFIG_SRCSIZE_## SIZE ##_U32 \
    | SI32_DMADESC_A_CONFIG_TMD_## MODE ##_U32 )

#define SI32_DMADESC_A_CONFIG_BYTE_MOVE_AUTO    SI32_DMADESC_A_CONFIG_MOVE(BYTE, AUTO_REQUEST)
#define SI32_DMADESC_A_CONFIG_BYTE_MOVE_AUTO_SG SI32_DMADESC_A_CONFIG_MOVE(BYTE, MEMORY_SCATTER_GATHER_A)
#define SI32_DMADESC_A_CONFIG_BYTE_RX           SI32_DMADESC_A_CONFIG_RX(BYTE, BASIC)
#define SI32_DMADESC_A_CONFIG_BYTE_RX_PP        SI32_DMADESC_A_CONFIG_RX(BYTE, PING_PONG)
#define SI32_DMADESC_A_CONFIG_BYTE_RX_SG        SI32_DMADESC_A_CONFIG_RX(BYTE, PERIPHERAL_SCATTER_GATHER_A)
#define SI32_DMADESC_A_CONFIG_BYTE_TX           SI32_DMADESC_A_CONFIG_TX(BYTE, BASIC)
#define SI32_DMADESC_A_CONFIG_BYTE_TX_AUTO      SI32_DMADESC_A_CONFIG_TX(BYTE, AUTO_REQUEST)
#define SI32_DMADESC_A_CONFIG_BYTE_TX_PP        SI32_DMADESC_A_CONFIG_TX(BYTE, PING_PONG)
#define SI32_DMADESC_A_CONFIG_BYTE_TX_SG        SI32_DMADESC_A_CONFIG_TX(BYTE, PERIPHERAL_SCATTER_GATHER_A)
#define SI32_DMADESC_A_CONFIG_BYTE_PIPE         SI32_DMADESC_A_CONFIG_PIPE(BYTE, BASIC)
#define SI32_DMADESC_A_CONFIG_BYTE_PIPE_SG      SI32_DMADESC_A_CONFIG_PIPE(BYTE, PERIPHERAL_SCATTER_GATHER_A)

#define SI32_DMADESC_A_CONFIG_HWORD_MOVE_AUTO   SI32_DMADESC_A_CONFIG_MOVE(HALF_WORD, AUTO_REQUEST)
#define SI32_DMADESC_A_CONFIG_HWORD_MOVE_AUTO_SG SI32_DMADESC_A_CONFIG_MOVE(HALF_WORD, MEMORY_SCATTER_GATHER_A)
#define SI32_DMADESC_A_CONFIG_HWORD_RX          SI32_DMADESC_A_CONFIG_RX(HALF_WORD, BASIC)
#define SI32_DMADESC_A_CONFIG_HWORD_RX_PP       SI32_DMADESC_A_CONFIG_RX(HALF_WORD, PING_PONG)
#define SI32_DMADESC_A_CONFIG_HWORD_RX_SG       SI32_DMADESC_A_CONFIG_RX(HALF_WORD, PERIPHERAL_SCATTER_GATHER_A)
#define SI32_DMADESC_A_CONFIG_HWORD_TX          SI32_DMADESC_A_CONFIG_TX(HALF_WORD, BASIC)
#define SI32_DMADESC_A_CONFIG_HWORD_TX_AUTO     SI32_DMADESC_A_CONFIG_TX(HALF_WORD, AUTO_REQUEST)
#define SI32_DMADESC_A_CONFIG_HWORD_TX_PP       SI32_DMADESC_A_CONFIG_TX(HALF_WORD, PING_PONG)
#define SI32_DMADESC_A_CONFIG_HWORD_TX_SG       SI32_DMADESC_A_CONFIG_TX(HALF_WORD, PERIPHERAL_SCATTER_GATHER_A)
#define SI32_DMADESC_A_CONFIG_HWORD_PIPE        SI32_DMADESC_A_CONFIG_PIPE(HALF_WORD, BASIC)
#define SI32_DMADESC_A_CONFIG_HWORD_PIPE_SG     SI32_DMADESC_A_CONFIG_PIPE(HALF_WORD, PERIPHERAL_SCATTER_GATHER_A)

#define SI32_DMADESC_A_CONFIG_WORD_MOVE_AUTO    SI32_DMADESC_A_CONFIG_MOVE(WORD, AUTO_REQUEST)
#define SI32_DMADESC_A_CONFIG_WORD_MOVE_AUTO_SG SI32_DMADESC_A_CONFIG_MOVE(WORD, MEMORY_SCATTER_GATHER_A)
#define SI32_DMADESC_A_CONFIG_WORD_RX           SI32_DMADESC_A_CONFIG_RX(WORD, BASIC)
#define SI32_DMADESC_A_CONFIG_WORD_RX_PP        SI32_DMADESC_A_CONFIG_RX(WORD, PING_PONG)
#define SI32_DMADESC_A_CONFIG_WORD_RX_SG        SI32_DMADESC_A_CONFIG_RX(WORD, PERIPHERAL_SCATTER_GATHER_A)
#define SI32_DMADESC_A_CONFIG_WORD_TX           SI32_DMADESC_A_CONFIG_TX(WORD, BASIC)
#define SI32_DMADESC_A_CONFIG_WORD_TX_AUTO      SI32_DMADESC_A_CONFIG_TX(WORD, AUTO_REQUEST)
#define SI32_DMADESC_A_CONFIG_WORD_TX_PP        SI32_DMADESC_A_CONFIG_TX(WORD, PING_PONG)
#define SI32_DMADESC_A_CONFIG_WORD_TX_SG        SI32_DMADESC_A_CONFIG_TX(WORD, PERIPHERAL_SCATTER_GATHER_A)
#define SI32_DMADESC_A_CONFIG_WORD_PIPE         SI32_DMADESC_A_CONFIG_PIPE(WORD, BASIC)
#define SI32_DMADESC_A_CONFIG_WORD_PIPE_SG      SI32_DMADESC_A_CONFIG_PIPE(WORD, PERIPHERAL_SCATTER_GATHER_A)

#define SI32_DMADESC_A_CONFIG_MEMORY_SG \
    (SI32_DMADESC_A_CONFIG_MOVE(WORD, MEMORY_SCATTER_GATHER_P) | SI32_DMADESC_A_CONFIG_RPOWER(2))

#define SI32_DMADESC_A_CONFIG_PERIPHERAL_SG \
    (SI32_DMADESC_A_CONFIG_MOVE(WORD, PERIPHERAL_SCATTER_GATHER_P) | SI32_DMADESC_A_CONFIG_RPOWER(2))


#endif // __SI32_DMADESC_A_SUPPORT_H__

//-eof--------------------------------------------------------------------------