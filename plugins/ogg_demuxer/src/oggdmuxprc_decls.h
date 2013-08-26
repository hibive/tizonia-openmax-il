/**
 * Copyright (C) 2011-2013 Aratelia Limited - Juan A. Rubio
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
 * @file   oggdmuxprc_decls.h
 * @author Juan A. Rubio <juan.rubio@aratelia.com>
 * 
 * @brief  Tizonia OpenMAX IL - Ogg demuxer processor class decls
 * 
 * 
 */

#ifndef OGGDMUXPRC_DECLS_H
#define OGGDMUXPRC_DECLS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "oggdmuxprc.h"
#include "tizprc_decls.h"

#include "OMX_Core.h"

#include <stdbool.h>
#include <oggz/oggz.h>

  typedef struct oggdmux_prc oggdmux_prc_t;
  struct oggdmux_prc
  {
    /* Object */
    const tiz_prc_t _;
    FILE *p_file_;
    OMX_PARAM_CONTENTURITYPE *p_uri_param_;
    OGGZ *p_oggz_;
    OggzTable *p_tracks_;
    OMX_BUFFERHEADERTYPE *p_aud_hdr_;
    OMX_BUFFERHEADERTYPE *p_vid_hdr_;
    bool eos_;
    bool awaiting_buffers_;
    bool port_disabled_;
  };

#ifdef __cplusplus
}
#endif

#endif                          /* OGGDMUXPRC_DECLS_H */
