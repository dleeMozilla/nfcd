/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

/*****************************************************************************
**
**  Name:           Pn544Interop.h
**
**  Description:    Implement operations that provide compatibility with NXP
**                  PN544 controller.  Specifically facilitate peer-to-peer
**                  operations with PN544 controller.
**
*****************************************************************************/
#pragma once
#include "NfcUtil.h"


/*******************************************************************************
**
** Function:        pn544InteropStopPolling
**
** Description:     Stop polling to let NXP PN544 controller poll.
**                  PN544 should activate in P2P mode.
**
** Returns:         None
**
*******************************************************************************/
void pn544InteropStopPolling ();


/*******************************************************************************
**
** Function:        pn544InteropIsBusy
**
** Description:     Is the code performing operations?
**
** Returns:         True if the code is busy.
**
*******************************************************************************/
bool pn544InteropIsBusy ();


/*******************************************************************************
**
** Function:        pn544InteropAbortNow
**
** Description:     Request to abort all operations.
**
** Returns:         None.
**
*******************************************************************************/
void pn544InteropAbortNow ();
