project('plot-wasm', 'c',
        version         : '1.4.+',
        license         : 'Apache-2.0',
        meson_version   : '>=1.0.0',
        default_options : ['c_std=c17'])

cc = meson.get_compiler('c')

deps = [
  dependency('sdl2'),
  dependency('sdl2_ttf'),
  dependency('sdl2_image')
]

incdir = include_directories('src')
sources = [
  'src/channel.c',
  'src/channel_relay.c',
  'src/channel_service.c',
  'src/coord_info.c',
  'src/draw.c',
  'src/global.c',
  'src/graphics.c',
  'src/main.c',
  'src/parse.c',
  'src/plot.c',
  'src/text.c',
  'src/vec_channel.c',
  'src/vec_channel.h']

executable('plot', sources,
           include_directories : incdir,
           dependencies: deps,
           link_args : ['-Wall',
                        '-Wextra',
                        '-pedantic',
                        '-O2'])
