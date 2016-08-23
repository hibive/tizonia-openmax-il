/**
 * Copyright (C) 2011-2016 Aratelia Limited - Juan A. Rubio
 *
 * This file is part of Tizonia
 *
 * Tizonia is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Tizonia is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Tizonia.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file   webmdmux.c
 * @author Juan A. Rubio <juan.rubio@aratelia.com>
 *
 * @brief  Tizonia - WebM Demuxer component
 *
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <string.h>

#include <OMX_Core.h>
#include <OMX_Component.h>
#include <OMX_Types.h>

#include <tizplatform.h>

#include <tizport.h>
#include <tizscheduler.h>

#include "webmdmuxprc.h"
#include "webmdmux.h"

#ifdef TIZ_LOG_CATEGORY_NAME
#undef TIZ_LOG_CATEGORY_NAME
#define TIZ_LOG_CATEGORY_NAME "tiz.webm_demuxer"
#endif

/**
 *@defgroup libtizfr 'libtizfr' : OpenMAX IL WebM Demuxer
 *
 * - Component name : "OMX.Aratelia.container_demuxer.webm"
 * - Implements role: "container_demuxer.webm"
 *
 *@ingroup plugins
 */

static OMX_VERSIONTYPE webm_demuxer_version = { {1, 0, 0, 0} };

static OMX_PTR
instantiate_binary_port (OMX_HANDLETYPE ap_hdl)
{
  tiz_port_options_t port_opts = {
    OMX_PortDomainAudio,
    OMX_DirOutput,
    ARATELIA_WEBM_DEMUXER_PORT_MIN_BUF_COUNT,
    ARATELIA_WEBM_DEMUXER_PORT_MIN_BUF_SIZE,
    ARATELIA_WEBM_DEMUXER_PORT_NONCONTIGUOUS,
    ARATELIA_WEBM_DEMUXER_PORT_ALIGNMENT,
    ARATELIA_WEBM_DEMUXER_PORT_SUPPLIERPREF,
    {ARATELIA_WEBM_DEMUXER_PORT_INDEX, NULL, NULL, NULL},
    -1                          /* slave port's index, use -1 for now */
  };

  return factory_new (tiz_get_type (ap_hdl, "tizbinaryport"), &port_opts);
}

static OMX_PTR
instantiate_config_port (OMX_HANDLETYPE ap_hdl)
{
  return factory_new (tiz_get_type (ap_hdl, "tizconfigport"),
                      NULL,       /* this port does not take options */
                      ARATELIA_WEBM_DEMUXER_COMPONENT_NAME,
                      webm_demuxer_version);
}

static OMX_PTR
instantiate_processor (OMX_HANDLETYPE ap_hdl)
{
  return factory_new (tiz_get_type (ap_hdl, "webmdmuxprc"));
}

OMX_ERRORTYPE
OMX_ComponentInit (OMX_HANDLETYPE ap_hdl)
{
  tiz_role_factory_t role_factory;
  const tiz_role_factory_t *rf_list[] = { &role_factory };
  tiz_type_factory_t webmdmuxprc_type;
  const tiz_type_factory_t *tf_list[] = { &webmdmuxprc_type };

  strcpy ((OMX_STRING) role_factory.role, ARATELIA_WEBM_DEMUXER_DEFAULT_ROLE);
  role_factory.pf_cport   = instantiate_config_port;
  role_factory.pf_port[0] = instantiate_binary_port;
  role_factory.nports     = 1;
  role_factory.pf_proc    = instantiate_processor;

  strcpy ((OMX_STRING) webmdmuxprc_type.class_name, "webmdmuxprc_class");
  webmdmuxprc_type.pf_class_init = webmdmux_prc_class_init;
  strcpy ((OMX_STRING) webmdmuxprc_type.object_name, "webmdmuxprc");
  webmdmuxprc_type.pf_object_init = webmdmux_prc_init;

  /* Initialize the component infrastructure */
  tiz_check_omx_err (tiz_comp_init (ap_hdl, ARATELIA_WEBM_DEMUXER_COMPONENT_NAME));

  /* Register the "webmdmuxprc" class */
  tiz_check_omx_err (tiz_comp_register_types (ap_hdl, tf_list, 1));

  /* Register the component role(s) */
  tiz_check_omx_err (tiz_comp_register_roles (ap_hdl, rf_list, 1));

  return OMX_ErrorNone;
}