﻿/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
*
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
/**@file tnet_dns_ns.h
 * @brief DNS Name Server record - RR - (RFC 1035).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TNET_DNS_RR_NS_H
#define TNET_DNS_RR_NS_H

#include "tinynet_config.h"

#include "tnet_dns_rr.h"


TNET_BEGIN_DECLS


/** DNS NS Resource Record.
*/
typedef struct tnet_dns_ns_s {
    TNET_DECLARE_DNS_RR;

    /* RFC 1035 - 3.3.11. NS RDATA format
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    /                   NSDNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    */
    char* nsdname;
}
tnet_dns_ns_t;

TINYNET_API tnet_dns_ns_t* tnet_dns_ns_create(const char* name, tnet_dns_qclass_t qclass, uint32_t ttl, uint16_t rdlength, const void* data, tsk_size_t offset);

TINYNET_GEXTERN const tsk_object_def_t *tnet_dns_ns_def_t;

TNET_END_DECLS

#endif /* TNET_DNS_RR_NS_H */

