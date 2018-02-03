#ifndef WLR_BACKEND_LIBINPUT_H
#define WLR_BACKEND_LIBINPUT_H

#include <libinput.h>
#include <wayland-server.h>
#include <wlr/backend/session/session.h>
#include <wlr/backend/backend.h>
#include <wlr/types/wlr_input_device.h>

struct wlr_backend *wlr_libinput_backend_create(struct wl_display *display,
		struct wlr_session *session);
struct libinput_device *wlr_libinput_get_device_handle(struct wlr_input_device *dev);

bool wlr_backend_is_libinput(struct wlr_backend *backend);
bool wlr_input_device_is_libinput(struct wlr_input_device *device);

#endif
