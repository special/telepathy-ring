/*
 * modem/smart-messaging.h - oFono SmartMessaging interface
 *
 * Copyright (C) 2013 Jolla Ltd.
 * Copyright (C) 2011 Nokia Corporation
 *
 * Contact: John Brooks <john.brooks@jollamobile.com>
 *
 * This work is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This work is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _MODEM_SMART_MESSAGING_H_
#define _MODEM_SMART_MESSAGING_H_

#include <glib-object.h>
#include <modem/request.h>
#include <modem/oface.h>

G_BEGIN_DECLS

#define MODEM_OFACE_SMART_MESSAGING "org.ofono.SmartMessaging"

typedef struct _ModemSmartMessaging ModemSmartMessaging;
typedef struct _ModemSmartMessagingClass ModemSmartMessagingClass;
typedef struct _ModemSmartMessagingPrivate ModemSmartMessagingPrivate;

struct _ModemSmartMessagingClass
{
  ModemOfaceClass parent_class;
};

struct _ModemSmartMessaging
{
  ModemOface parent;
  ModemSmartMessagingPrivate *priv;
};

GType modem_smart_messaging_get_type (void);

/* TYPE MACROS */
#define MODEM_TYPE_SMART_MESSAGING                  \
  (modem_smart_messaging_get_type ())
#define MODEM_SMART_MESSAGING(obj)                  \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj),           \
      MODEM_TYPE_SMART_MESSAGING, ModemSmartMessaging))
#define MODEM_SMART_MESSAGING_CLASS(klass)                  \
  (G_TYPE_CHECK_CLASS_CAST ((klass),                    \
      MODEM_TYPE_SMART_MESSAGING, ModemSmartMessagingClass))
#define MODEM_IS_SMART_MESSAGING(obj)                               \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MODEM_TYPE_SMART_MESSAGING))
#define MODEM_IS_SMART_MESSAGING_CLASS(klass)                       \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), MODEM_TYPE_SMART_MESSAGING))
#define MODEM_SMART_MESSAGING_GET_CLASS(obj)                \
  (G_TYPE_INSTANCE_GET_CLASS ((obj),                    \
      MODEM_TYPE_SMART_MESSAGING, ModemSmartMessagingClass))

/* ---------------------------------------------------------------------- */

typedef void ModemSmartMessagingSendReply (ModemSmartMessaging *self,
  ModemRequest *request,
  char const *message_id,
  GError const *error,
  gpointer user_data);

ModemRequest *
modem_smartmsg_request_send_vcard (ModemSmartMessaging *self,
                                   char const *to, char const *vcard, size_t vcard_size,
                                   ModemSmartMessagingSendReply *reply, gpointer user_data);

G_END_DECLS

#endif /* #ifndef _MODEM_SMART_MESSAGING_H_*/
