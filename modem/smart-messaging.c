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

#include "config.h"

#define MODEM_DEBUG_FLAG MODEM_LOG_SMS

#include "debug.h"

#include "modem/smart-messaging.h"
#include "modem/request-private.h"
#include "modem/errors.h"
#include "modem/ofono.h"

#include <dbus/dbus-glib.h>

#include "signals-marshal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

/* ------------------------------------------------------------------------ */

G_DEFINE_TYPE (ModemSmartMessaging, modem_smart_messaging, MODEM_TYPE_OFACE);

/* private data */
struct _ModemSmartMessagingPrivate
{
  unsigned dispose_has_run:1, connected:1, signals:1, disconnected:1;
  unsigned connection_error:1;
  unsigned :0;
};

/* ------------------------------------------------------------------------ */

static void
modem_smart_messaging_init (ModemSmartMessaging *self)
{
  DEBUG ("enter");

  self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
      MODEM_TYPE_SMART_MESSAGING, ModemSmartMessagingPrivate);
}

static void
modem_smart_messaging_get_property (GObject *object,
                                guint property_id,
                                GValue *value,
                                GParamSpec *pspec)
{
  ModemSmartMessaging *self = MODEM_SMART_MESSAGING (object);
  ModemSmartMessagingPrivate *priv = self->priv;

  switch (property_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
modem_smart_messaging_set_property (GObject *object,
                                   guint property_id,
                                   const GValue *value,
                                   GParamSpec *pspec)
{
  ModemSmartMessaging *self = MODEM_SMART_MESSAGING (object);
  ModemSmartMessagingPrivate *priv = self->priv;

  switch (property_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
modem_smart_messaging_constructed (GObject *object)
{
  if (G_OBJECT_CLASS (modem_smart_messaging_parent_class)->constructed)
    G_OBJECT_CLASS (modem_smart_messaging_parent_class)->constructed (object);
}

static void
modem_smart_messaging_dispose (GObject *object)
{
  if (G_OBJECT_CLASS (modem_smart_messaging_parent_class)->dispose)
    G_OBJECT_CLASS (modem_smart_messaging_parent_class)->dispose (object);
}

static void
modem_smart_messaging_finalize (GObject *object)
{
  ModemSmartMessaging *self = MODEM_SMART_MESSAGING (object);
  ModemSmartMessagingPrivate *priv = self->priv;

  DEBUG ("enter");

  G_OBJECT_CLASS (modem_smart_messaging_parent_class)->finalize (object);
}

/* ------------------------------------------------------------------------- */
/* ModemOface interface */

static char const *
modem_smart_messaging_property_mapper (char const *name)
{
  return NULL;
}

static void
modem_smart_messaging_connect (ModemOface *_self)
{
  DEBUG ("(%p): enter", _self);
}

static void
modem_smart_messaging_connected (ModemOface *_self)
{
  DEBUG ("(%p): enter", _self);
}

static void
modem_smart_messaging_disconnect (ModemOface *_self)
{
  DEBUG ("(%p): enter", _self);
}

static void
modem_smart_messaging_class_init (ModemSmartMessagingClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  ModemOfaceClass *oface_class = MODEM_OFACE_CLASS (klass);

  DEBUG ("enter");

  object_class->get_property = modem_smart_messaging_get_property;
  object_class->set_property = modem_smart_messaging_set_property;
  object_class->constructed = modem_smart_messaging_constructed;
  object_class->dispose = modem_smart_messaging_dispose;
  object_class->finalize = modem_smart_messaging_finalize;

  oface_class->ofono_interface = MODEM_OFACE_SMART_MESSAGING;
  oface_class->property_mapper = modem_smart_messaging_property_mapper;
  oface_class->connect = modem_smart_messaging_connect;
  oface_class->connected = modem_smart_messaging_connected;
  oface_class->disconnect = modem_smart_messaging_disconnect;

  g_type_class_add_private (klass, sizeof (ModemSmartMessagingPrivate));
}

static void
reply_to_send_message (DBusGProxy *proxy,
                       DBusGProxyCall *call,
                       void *_request)
{
  ModemRequest *request = _request;
  ModemSmartMessaging *self = modem_request_object (request);
  ModemSmartMessagingSendReply *callback = modem_request_callback (request);
  gpointer user_data = modem_request_user_data (request);
  char const *message_path = NULL;

  GError *error = NULL;

  if (dbus_g_proxy_end_call (proxy, call, &error,
          DBUS_TYPE_G_OBJECT_PATH, &message_path,
          G_TYPE_INVALID))
    {
      char const *destination;

      destination = modem_request_get_data (request, "destination");
    }

  callback (self, request, message_path, error, user_data);

  g_clear_error (&error);
}

ModemRequest *
modem_smartmsg_request_send_vcard (ModemSmartMessaging *self,
                                   char const *to, char const *vcard, size_t vcard_size,
                                   ModemSmartMessagingSendReply *reply, gpointer user_data)
{
  ModemRequest *request;
  GArray *data = g_array_new (TRUE, TRUE, 1);
  g_array_append_vals (data, vcard, vcard_size);

  DEBUG (MODEM_OFACE_SMART_MESSAGING ".SendBusinessCard (%s,%u bytes)", to, (unsigned)vcard_size);

  request = modem_request (self,
      modem_oface_dbus_proxy (MODEM_OFACE (self)),
      "SendBusinessCard", reply_to_send_message,
      G_CALLBACK (reply), user_data,
      G_TYPE_STRING, to,
      DBUS_TYPE_G_UCHAR_ARRAY, data,
      G_TYPE_INVALID);

  if (request)
    modem_request_add_data_full (request, "destination", g_strdup (to), g_free);

  g_array_unref(data);
  return request;
}

