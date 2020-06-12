/*
 *
 * Copyright (c) 2018-2020 The University of Waikato, Hamilton, New Zealand.
 * All rights reserved.
 *
 * This file is part of OpenLI.
 *
 * This code has been developed by the University of Waikato WAND
 * research group. For further information please see http://www.wand.net.nz/
 *
 * OpenLI is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenLI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

#ifndef OPENLI_MEDIATOR_PROV_H_
#define OPENLI_MEDIATOR_PROV_H_

#include <inttypes.h>
#include "med_epoll.h"
#include "netcomms.h"
#include "openli_tls.h"

typedef struct mediator_provisioner {
    med_epoll_ev_t *provev;
    med_epoll_ev_t *provreconnect;
    int sentinfo;
    net_buffer_t *outgoing;
    net_buffer_t *incoming;
    uint8_t disable_log;
    uint8_t tryconnect;
    SSL *ssl;
    SSL_CTX **sslctxt;
    int epoll_fd;
    int lastsslerror;

    char *provaddr;
    char *provport;
} mediator_prov_t;

void init_provisioner_instance(mediator_prov_t *prov, SSL_CTX **ctx);

void disconnect_provisioner(mediator_prov_t *prov, int enable_reconnect);

/** Releases all memory associated with a provisioner that this mediator
 *  was connected to.
 *
 *  @param prov         The provisioner to be released.
 */
void free_provisioner(mediator_prov_t *prov);

/** Sends any pending messages to the provisioner.
 *
 *  @param prov             The reference to the provisioner.
 *  @param mev              The epoll event for the provisioner socket.
 *
 *  @return -1 if an error occurs, 1 otherwise.
 */
int transmit_provisioner(mediator_prov_t *prov, med_epoll_ev_t *mev);

/** Attempts to connect to the provisioner.
 *
 *  @param prov             The provisioner to connect to
 *  @param provfail         Set to 1 if the most recent connection attempt
 *                          failed, 0 otherwise.
 *
 *  @return 1 if the connection attempt fails, 0 otherwise.
 */
int attempt_provisioner_connect(mediator_prov_t *prov, int provfail);

int send_mediator_details_to_provisioner(mediator_prov_t *prov,
        openli_mediator_t *meddeets, int justcreated);

/** Applies any changes to the provisioner socket configuration following
 *  a user-triggered config reload.
 *
 *  @param currstate            The pre-reload provisioner state
 *  @param newstate             A provisioner instance containing the updated
 *                              configuration.
 *  @return 0 if the configuration is unchanged, 1 if it has changed.
 */
int reload_provisioner_socket_config(mediator_prov_t *currstate,
        mediator_prov_t *newstate);

#endif

// vim: set sw=4 tabstop=4 softtabstop=4 expandtab :