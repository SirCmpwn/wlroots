#include <stdlib.h>
#include <unistd.h>
#include <xf86drm.h>
#include <wlr/types/wlr_drm.h>
#include <wlr/util/log.h>
#include "drm-protocol.h"
#include "util/signal.h"

#define WLR_DRM_VERSION 2

static void drm_handle_authenticate(struct wl_client *client,
		struct wl_resource *resource, uint32_t id) {
	// We only use render nodes, which don't need authentication
	wl_drm_send_authenticated(resource);
}

static void drm_handle_create_buffer(struct wl_client *client,
		struct wl_resource *resource, uint32_t id, uint32_t name, int32_t width,
		int32_t height, uint32_t stride, uint32_t format) {
	wl_resource_post_error(resource, WL_DRM_ERROR_INVALID_NAME,
		"Flink handles are not supported, use DMA-BUF instead");
}

static void drm_handle_create_planar_buffer(struct wl_client *client,
		struct wl_resource *resource, uint32_t id, uint32_t name, int32_t width,
		int32_t height, uint32_t format, int32_t offset0, int32_t stride0,
		int32_t offset1, int32_t stride1, int32_t offset2, int32_t stride2) {
	wl_resource_post_error(resource, WL_DRM_ERROR_INVALID_NAME,
		"Flink handles are not supported, use DMA-BUF instead");
}

static void drm_handle_create_prime_buffer(struct wl_client *client,
		struct wl_resource *resource, uint32_t id, int fd, int32_t width,
		int32_t height, uint32_t format, int32_t offset0, int32_t stride0,
		int32_t offset1, int32_t stride1, int32_t offset2, int32_t stride2) {
	close(fd);
	wl_resource_post_error(resource, WL_DRM_ERROR_INVALID_NAME,
		"DMA-BUF imports via wl_drm are not supported, use linux-dmabuf instead");
}

static const struct wl_drm_interface drm_impl = {
	.authenticate = drm_handle_authenticate,
	.create_buffer = drm_handle_create_buffer,
	.create_planar_buffer = drm_handle_create_planar_buffer,
	.create_prime_buffer = drm_handle_create_prime_buffer,
};

static void drm_bind(struct wl_client *client, void *data,
		uint32_t version, uint32_t id) {
	struct wlr_drm *drm = data;

	struct wl_resource *resource = wl_resource_create(client,
		&wl_drm_interface, version, id);
	if (resource == NULL) {
		wl_client_post_no_memory(client);
		return;
	}
	wl_resource_set_implementation(resource, &drm_impl, drm, NULL);

	wl_drm_send_device(resource, drm->node_name);
	wl_drm_send_capabilities(resource, WL_DRM_CAPABILITY_PRIME);
}

static void handle_display_destroy(struct wl_listener *listener, void *data) {
	struct wlr_drm *drm = wl_container_of(listener, drm, display_destroy);

	wlr_signal_emit_safe(&drm->events.destroy, NULL);

	wl_list_remove(&drm->display_destroy.link);

	free(drm->node_name);
	wl_global_destroy(drm->global);
	free(drm);
}

struct wlr_drm *wlr_drm_create(struct wl_display *display, int drm_fd) {
	struct wlr_drm *drm = calloc(1, sizeof(*drm));
	if (drm == NULL) {
		return NULL;
	}

	wl_signal_init(&drm->events.destroy);

	drm->global = wl_global_create(display, &wl_drm_interface, WLR_DRM_VERSION,
		drm, drm_bind);
	if (drm->global == NULL) {
		free(drm);
		return NULL;
	}

	drm->node_name = drmGetRenderDeviceNameFromFd(drm_fd);
	if (drm->node_name == NULL) {
		wlr_log(WLR_ERROR, "drmGetRenderDeviceNameFromFd failed");
		wl_global_destroy(drm->global);
		free(drm);
		return NULL;
	}

	drm->display_destroy.notify = handle_display_destroy;
	wl_display_add_destroy_listener(display, &drm->display_destroy);

	return drm;
}
