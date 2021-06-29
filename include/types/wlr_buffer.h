#ifndef TYPES_WLR_BUFFER
#define TYPES_WLR_BUFFER

#include <wlr/types/wlr_buffer.h>

struct wlr_shm_client_buffer {
	struct wlr_buffer base;

	uint32_t format;
	size_t stride;

	// The following fields are NULL if the client has destroyed the wl_buffer
	struct wl_resource *resource;
	struct wl_shm_buffer *shm_buffer;

	// This is used to keep the backing storage alive after the client has
	// destroyed the wl_buffer
	struct wl_shm_pool *saved_shm_pool;
	void *saved_data;

	struct wl_listener resource_destroy;
	struct wl_listener release;
};

struct wlr_shm_client_buffer *shm_client_buffer_create(
	struct wl_resource *resource);

/**
 * Buffer capabilities.
 *
 * These bits indicate the features supported by a wlr_buffer. There is one bit
 * per function in wlr_buffer_impl.
 */
enum wlr_buffer_cap {
	WLR_BUFFER_CAP_DATA_PTR = 1 << 0,
	WLR_BUFFER_CAP_DMABUF = 1 << 1,
	WLR_BUFFER_CAP_SHM = 1 << 2,
};

/**
 * Buffer data pointer access flags.
 */
enum wlr_buffer_data_ptr_access_flag {
	/**
	 * The buffer contents can be read back.
	 */
	WLR_BUFFER_DATA_PTR_ACCESS_READ = 1 << 0,
	/**
	 * The buffer contents can be written to.
	 */
	WLR_BUFFER_DATA_PTR_ACCESS_WRITE = 1 << 1,
};

/**
 * Get a pointer to a region of memory referring to the buffer's underlying
 * storage. The format and stride can be used to interpret the memory region
 * contents.
 *
 * The returned pointer should be pointing to a valid memory region for the
 * operations specified in the flags. The returned pointer is only valid up to
 * the next buffer_end_data_ptr_access call.
 */
bool buffer_begin_data_ptr_access(struct wlr_buffer *buffer, uint32_t flags,
	void **data, uint32_t *format, size_t *stride);
void buffer_end_data_ptr_access(struct wlr_buffer *buffer);

#endif
