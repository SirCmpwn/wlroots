#ifndef WLR_RENDER_RENDERER_H
#define WLR_RENDER_RENDERER_H

#include <wayland-server.h>

struct wlr_allocator;
struct wlr_backend;
struct wlr_image;
struct wlr_renderer_impl_2;

struct wlr_renderer_2 {
	const struct wlr_renderer_impl_2 *impl;

	struct {
		struct wl_signal destroy;
	} events;
};

struct wlr_renderer_2 *wlr_renderer_autocreate_2(struct wlr_backend *backend);

void wlr_renderer_destroy_2(struct wlr_renderer_2 *renderer);

struct wlr_allocator *wlr_renderer_get_allocator_2(struct wlr_renderer_2 *renderer);

void wlr_renderer_bind_image_2(struct wlr_renderer_2 *renderer, struct wlr_image *img);

void wlr_renderer_flush_2(struct wlr_renderer_2 *renderer, int *fence_out);

void wlr_renderer_clear_2(struct wlr_renderer_2 *renderer, const float color[static 4]);

#endif
