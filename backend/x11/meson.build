x11_libs = []
x11_required = [
	'xcb',
	'xcb-dri3',
	'xcb-present',
	'xcb-render',
	'xcb-renderutil',
	'xcb-shm',
	'xcb-xfixes',
	'xcb-xinput',
]

msg = []
if get_option('x11-backend').enabled()
	msg += 'Install "@0@" or pass "-Dx11-backend=disabled" to disable it.'
endif
if not get_option('x11-backend').disabled()
	msg += 'Required for X11 backend support.'
endif

foreach lib : x11_required
	dep = dependency(lib,
		required: get_option('x11-backend'),
		not_found_message: '\n'.join(msg).format(lib),
	)
	if not dep.found()
		subdir_done()
	endif

	x11_libs += dep
endforeach

wlr_files += files(
	'backend.c',
	'input_device.c',
	'output.c',
)
wlr_deps += x11_libs
features += { 'x11-backend': true }
