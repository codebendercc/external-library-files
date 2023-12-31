//////////////////////////////////////////////////////////////////////////////
// Model: pelican.qm
// File:  ./pelican.ino
//
// This code has been generated by QM tool (see state-machine.com/qm).
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//////////////////////////////////////////////////////////////////////////////
// @(/2/4) ...................................................................
#include "qp_port.h"
#include "pelican.h"
#include "bsp.h"

#include "Arduino.h"

// Local-scope objects -------------------------------------------------------
static QEvt const *l_pelicanQueueSto[2];
static QEvt const *l_pedQueueSto[3];
static QSubscrList l_subscrSto[MAX_PUB_SIG];

static QF_MPOOL_EL(QEvt) l_smlPoolSto[10]; // storage for the small event pool

//............................................................................
void setup() {
    BSP_init();                                          // initialize the BSP

    QF::init();       // initialize the framework and the underlying RT kernel

                                                  // initialize event pools...
    QF::poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

    QF::psInit(l_subscrSto, Q_DIM(l_subscrSto));     // init publish-subscribe

                                                // start the active objects...
    AO_Pelican->start(1U, l_pelicanQueueSto, Q_DIM(l_pelicanQueueSto),
                      (void *)0, 0U);
}

//////////////////////////////////////////////////////////////////////////////
// NOTE: Do not define the loop() function, because this function is
// already defined in the QP port to Arduino
