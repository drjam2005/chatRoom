project(
  'ChatClient',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=3', 'cpp_std=c++14'],
)

serverDeps = [
]

raylib_dep = dependency('raylib', required: true)
clientDeps = [
  raylib_dep
]
includeDirs = include_directories('include')

mainSrc = [
  # 'src/objects.cpp',
]

serverSrc = [
 'src/server.cpp'

]
clientSrc = [
 'src/client.cpp'
]

server = executable(
  'server',
  [mainSrc, serverSrc],
  install : true,
  dependencies : serverDeps,
  include_directories : includeDirs,
)

client = executable(
  'client',
  [mainSrc, clientSrc],
  install : true,
  dependencies : clientDeps,
  include_directories : includeDirs,
)

test('basic', server)
test('basic', client)
