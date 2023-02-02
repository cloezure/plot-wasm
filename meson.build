project('plot-wasm', 'c',
        version         : '1.8b',
        license         : 'Apache-2.0',
        meson_version   : '>=0.5.2',
        default_options : ['c_std=c11'])

cc = meson.get_compiler('c')

deps = [
  dependency('sdl2'),
  dependency('sdl2_ttf'),
  dependency('sdl2_image'),
  dependency('freetype2')
]

incdir = include_directories('src')
plot_src = []
subdir('src')

executable('plot', plot_src,
           include_directories : incdir,
           dependencies: deps,
           link_args :
             ['-Wall',
              '-Wextra',
              '-pedantic',
              '-O2'])

#executable('plot.mjs', plot_src,
#           include_directories : incdir,
#           link_args :
#             ['-O2',
#              '-sUSE_SDL_TTF=2',
#              '-sNO_EXIT_RUNTIME=1',
#              '-sENVIRONMENT="web"',
#              '-sEXPORT_ES6=1',
#              '-sSDL2_IMAGE_FORMATS=["png"]',
#              '-sEXPORTED_RUNTIME_METHODS=[\'ccall\',\'cwrap\']',
#              '-sUSE_SDL=2',
#              '-sSINGLE_FILE=1',
#              '-sMODULARIZE=1',
#              '-sALLOW_MEMORY_GROWTH',
#              '-sEXPORT_NAME="GraphicsPlot"',
#              '--preload-file ./res'])
