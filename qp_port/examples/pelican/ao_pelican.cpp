//////////////////////////////////////////////////////////////////////////////
// Model: pelican.qm
// File:  ./ao_pelican.cpp
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
// @(/2/3) ...................................................................
#include "qp_port.h"
#include "bsp.h"
#include "pelican.h"

//Q_DEFINE_THIS_FILE

enum PelicanTimeouts {                            // various timeouts in ticks
    CARS_GREEN_MIN_TOUT = BSP_TICKS_PER_SEC * 8,         // min green for cars
    CARS_YELLOW_TOUT = BSP_TICKS_PER_SEC * 3,               // yellow for cars
    PEDS_WALK_TOUT   = BSP_TICKS_PER_SEC * 3,         // walking time for peds
    PEDS_FLASH_TOUT  = BSP_TICKS_PER_SEC / 5,     // flashing timeout for peds
    PEDS_FLASH_NUM   = 5*2,                      // number of flashes for peds
    OFF_FLASH_TOUT   = BSP_TICKS_PER_SEC / 2      // flashing timeout when off
};

// Pelican class -------------------------------------------------------------
// @(/1/0) ...................................................................
class Pelican : public QP::QActive {
private:
    QTimeEvt m_timeout;
    uint8_t m_flashCtr;

public:
    Pelican()
      : QActive((QStateHandler)&Pelican::initial),
        m_timeout(TIMEOUT_SIG) 
    {
    }

protected:
    static QP::QState initial(Pelican * const me, QP::QEvt const * const e);
    static QP::QState operational(Pelican * const me, QP::QEvt const * const e);
    static QP::QState carsEnabled(Pelican * const me, QP::QEvt const * const e);
    static QP::QState carsGreen(Pelican * const me, QP::QEvt const * const e);
    static QP::QState carsGreenNoPed(Pelican * const me, QP::QEvt const * const e);
    static QP::QState carsGreenInt(Pelican * const me, QP::QEvt const * const e);
    static QP::QState carsGreenPedWait(Pelican * const me, QP::QEvt const * const e);
    static QP::QState carsYellow(Pelican * const me, QP::QEvt const * const e);
    static QP::QState pedsEnabled(Pelican * const me, QP::QEvt const * const e);
    static QP::QState pedsWalk(Pelican * const me, QP::QEvt const * const e);
    static QP::QState pedsFlash(Pelican * const me, QP::QEvt const * const e);
    static QP::QState offline(Pelican * const me, QP::QEvt const * const e);
};


// Local objects -------------------------------------------------------------
static Pelican l_Pelican;      // the single instance of Pelican active object

// Global objects ------------------------------------------------------------
QActive * const AO_Pelican = &l_Pelican;                 // the opaque pointer

// Pelican class definition --------------------------------------------------
// @(/1/0) ...................................................................

// @(/1/0/3) .................................................................
// @(/1/0/3/0)
QP::QState Pelican::initial(Pelican * const me, QP::QEvt const * const e) {
    me->subscribe(PEDS_WAITING_SIG);
    return Q_TRAN(&Pelican::operational);
}
// @(/1/0/3/1) ...............................................................
QP::QState Pelican::operational(Pelican * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // @(/1/0/3/1)
        case Q_ENTRY_SIG: {
            BSP_signalCars(CARS_RED);
            BSP_signalPeds(PEDS_DONT_WALK);
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/1/0)
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&Pelican::carsEnabled);
            break;
        }
        // @(/1/0/3/1/1)
        case OFF_SIG: {
            status_ = Q_TRAN(&Pelican::offline);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm::top);
            break;
        }
    }
    return status_;
}
// @(/1/0/3/1/2) .............................................................
QP::QState Pelican::carsEnabled(Pelican * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // @(/1/0/3/1/2)
        case Q_EXIT_SIG: {
            BSP_signalCars(CARS_RED);
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/1/2/0)
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&Pelican::carsGreen);
            break;
        }
        default: {
            status_ = Q_SUPER(&Pelican::operational);
            break;
        }
    }
    return status_;
}
// @(/1/0/3/1/2/1) ...........................................................
QP::QState Pelican::carsGreen(Pelican * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // @(/1/0/3/1/2/1)
        case Q_ENTRY_SIG: {
            BSP_signalCars(CARS_GREEN);
            me->m_timeout.postIn(me, CARS_GREEN_MIN_TOUT);
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/1/2/1)
        case Q_EXIT_SIG: {
            me->m_timeout.disarm();
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/1/2/1/0)
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&Pelican::carsGreenNoPed);
            break;
        }
        default: {
            status_ = Q_SUPER(&Pelican::carsEnabled);
            break;
        }
    }
    return status_;
}
// @(/1/0/3/1/2/1/1) .........................................................
QP::QState Pelican::carsGreenNoPed(Pelican * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // @(/1/0/3/1/2/1/1)
        case Q_ENTRY_SIG: {
            BSP_showState("carsGreenNoPed");
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/1/2/1/1/0)
        case PEDS_WAITING_SIG: {
            status_ = Q_TRAN(&Pelican::carsGreenPedWait);
            break;
        }
        // @(/1/0/3/1/2/1/1/1)
        case TIMEOUT_SIG: {
            status_ = Q_TRAN(&Pelican::carsGreenInt);
            break;
        }
        default: {
            status_ = Q_SUPER(&Pelican::carsGreen);
            break;
        }
    }
    return status_;
}
// @(/1/0/3/1/2/1/2) .........................................................
QP::QState Pelican::carsGreenInt(Pelican * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // @(/1/0/3/1/2/1/2)
        case Q_ENTRY_SIG: {
            BSP_showState("carsGreenInt");
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/1/2/1/2/0)
        case PEDS_WAITING_SIG: {
            status_ = Q_TRAN(&Pelican::carsYellow);
            break;
        }
        default: {
            status_ = Q_SUPER(&Pelican::carsGreen);
            break;
        }
    }
    return status_;
}
// @(/1/0/3/1/2/1/3) .........................................................
QP::QState Pelican::carsGreenPedWait(Pelican * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // @(/1/0/3/1/2/1/3)
        case Q_ENTRY_SIG: {
            BSP_showState("carsGreenPedWait");
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/1/2/1/3/0)
        case TIMEOUT_SIG: {
            status_ = Q_TRAN(&Pelican::carsYellow);
            break;
        }
        default: {
            status_ = Q_SUPER(&Pelican::carsGreen);
            break;
        }
    }
    return status_;
}
// @(/1/0/3/1/2/2) ...........................................................
QP::QState Pelican::carsYellow(Pelican * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // @(/1/0/3/1/2/2)
        case Q_ENTRY_SIG: {
            BSP_showState("carsYellow");
            BSP_signalCars(CARS_YELLOW);
            me->m_timeout.postIn(me, CARS_YELLOW_TOUT);
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/1/2/2)
        case Q_EXIT_SIG: {
            me->m_timeout.disarm();
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/1/2/2/0)
        case TIMEOUT_SIG: {
            status_ = Q_TRAN(&Pelican::pedsEnabled);
            break;
        }
        default: {
            status_ = Q_SUPER(&Pelican::carsEnabled);
            break;
        }
    }
    return status_;
}
// @(/1/0/3/1/3) .............................................................
QP::QState Pelican::pedsEnabled(Pelican * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // @(/1/0/3/1/3)
        case Q_EXIT_SIG: {
            BSP_signalPeds(PEDS_DONT_WALK);
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/1/3/0)
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&Pelican::pedsWalk);
            break;
        }
        default: {
            status_ = Q_SUPER(&Pelican::operational);
            break;
        }
    }
    return status_;
}
// @(/1/0/3/1/3/1) ...........................................................
QP::QState Pelican::pedsWalk(Pelican * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // @(/1/0/3/1/3/1)
        case Q_ENTRY_SIG: {
            BSP_showState("pedsWalk");
            BSP_signalPeds(PEDS_WALK);
            me->m_timeout.postIn(me, PEDS_WALK_TOUT);
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/1/3/1)
        case Q_EXIT_SIG: {
            me->m_timeout.disarm();
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/1/3/1/0)
        case TIMEOUT_SIG: {
            status_ = Q_TRAN(&Pelican::pedsFlash);
            break;
        }
        default: {
            status_ = Q_SUPER(&Pelican::pedsEnabled);
            break;
        }
    }
    return status_;
}
// @(/1/0/3/1/3/2) ...........................................................
QP::QState Pelican::pedsFlash(Pelican * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // @(/1/0/3/1/3/2)
        case Q_ENTRY_SIG: {
            BSP_showState("pedsWalk");
            me->m_timeout.postEvery(me, PEDS_FLASH_TOUT);
            me->m_flashCtr = PEDS_FLASH_NUM*2 + 1;
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/1/3/2)
        case Q_EXIT_SIG: {
            me->m_timeout.disarm();
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/1/3/2/0)
        case TIMEOUT_SIG: {
            // @(/1/0/3/1/3/2/0/0)
            if (me->m_flashCtr != 0) {
                --me->m_flashCtr;
                // @(/1/0/3/1/3/2/0/0/0)
                if ((me->m_flashCtr & 1) == 0) {
                    BSP_signalPeds(PEDS_DONT_WALK);
                    status_ = Q_HANDLED();
                }
                // @(/1/0/3/1/3/2/0/0/1)
                else {
                    BSP_signalPeds(PEDS_BLANK);
                    status_ = Q_HANDLED();
                }
            }
            // @(/1/0/3/1/3/2/0/1)
            else {
                status_ = Q_TRAN(&Pelican::carsEnabled);
            }
            break;
        }
        default: {
            status_ = Q_SUPER(&Pelican::pedsEnabled);
            break;
        }
    }
    return status_;
}
// @(/1/0/3/2) ...............................................................
QP::QState Pelican::offline(Pelican * const me, QP::QEvt const * const e) {
    QP::QState status_;
    switch (e->sig) {
        // @(/1/0/3/2)
        case Q_ENTRY_SIG: {
            BSP_showState("offline");
            me->m_timeout.postEvery(me, OFF_FLASH_TOUT);
            me->m_flashCtr = 0;
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/2)
        case Q_EXIT_SIG: {
            me->m_timeout.disarm();
            status_ = Q_HANDLED();
            break;
        }
        // @(/1/0/3/2/0)
        case TIMEOUT_SIG: {
            me->m_flashCtr ^= 1;
            // @(/1/0/3/2/0/0)
            if ((me->m_flashCtr & 1) == 0) {
                BSP_signalCars(CARS_RED);
                BSP_signalPeds(PEDS_DONT_WALK);
                status_ = Q_HANDLED();
            }
            // @(/1/0/3/2/0/1)
            else {
                BSP_signalCars(CARS_BLANK);
                BSP_signalPeds(PEDS_BLANK);
                status_ = Q_HANDLED();
            }
            break;
        }
        // @(/1/0/3/2/1)
        case ON_SIG: {
            status_ = Q_TRAN(&Pelican::operational);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm::top);
            break;
        }
    }
    return status_;
}

