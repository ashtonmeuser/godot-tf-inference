#!python
import os

opts = Variables([], ARGUMENTS)

# Standard flags CC, CCX, etc.
env = DefaultEnvironment()

# Define options
opts.Add(EnumVariable('target', 'Compilation target', 'debug', ['d', 'debug', 'r', 'release']))
opts.Add(EnumVariable('platform', 'Compilation platform', '', ['', 'windows', 'x11', 'linux', 'osx']))
opts.Add(EnumVariable('p', 'Compilation target, alias for platform', '', ['', 'windows', 'x11', 'linux', 'osx']))
opts.Add(BoolVariable('use_llvm', 'Use the LLVM / Clang compiler', 'no'))

# Local dependency paths, adapt them to your setup
cppflow_path = 'cppflow/'
libtensorflow_path = 'libtensorflow2/'
godot_headers_path = 'godot-cpp/godot-headers/'
cpp_bindings_path = 'godot-cpp/'
cpp_library = 'libgodot-cpp'
target_path = 'addons/godot-tf-inference/'
target_name = 'godot-tf-inference'
bits = 64

# Updates the environment with the option variables.
opts.Update(env)

# Process some arguments
if env['use_llvm']:
    env['CC'] = 'clang'
    env['CXX'] = 'clang++'

if env['p'] != '':
    env['platform'] = env['p']

if env['platform'] == '':
    print('No valid target platform selected.')
    quit();

# Fix needed on OSX
def rpath_fix(target, source, env):
    os.system('install_name_tool -change @rpath/libtensorflow.2.dylib @loader_path/libtensorflow.2.dylib {0}'.format(target[0]))

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# Check platform specifics
if env['platform'] == 'osx':
    target_path += 'osx/'
    cpp_library += '.osx'
    env.Append(CCFLAGS=['-arch', 'x86_64'])
    env.Append(CXXFLAGS=['-std=c++17'])
    env.Append(LINKFLAGS=['-arch', 'x86_64'])
    if env['target'] in ('debug', 'd'):
        env.Append(CCFLAGS=['-g', '-O2'])
    else:
        env.Append(CCFLAGS=['-g', '-O3'])

elif env['platform'] in ('x11', 'linux'):
    target_path += 'x11/'
    cpp_library += '.linux'
    env.Append(CCFLAGS=['-fPIC'])
    env.Append(CXXFLAGS=['-std=c++17'])
    if env['target'] in ('debug', 'd'):
        env.Append(CCFLAGS=['-g3', '-Og'])
    else:
        env.Append(CCFLAGS=['-g', '-O3'])

elif env['platform'] == 'windows':
    target_path += 'win64/'
    cpp_library += '.windows'
    # This makes sure to keep the session environment variables on windows,
    # that way you can run scons in a vs 2017 prompt and it will find all the required tools
    env.Append(ENV=os.environ)

    env.Append(CPPDEFINES=['WIN32', '_WIN32', '_WINDOWS', '_CRT_SECURE_NO_WARNINGS'])
    env.Append(CCFLAGS=['-W3', '-GR'])
    env.Append(CXXFLAGS='/std:c++17')
    if env['target'] in ('debug', 'd'):
        env.Append(CPPDEFINES=['_DEBUG'])
        env.Append(CCFLAGS=['-EHsc', '-MDd', '-ZI'])
        env.Append(LINKFLAGS=['-DEBUG'])
    else:
        env.Append(CPPDEFINES=['NDEBUG'])
        env.Append(CCFLAGS=['-O2', '-EHsc', '-MD'])

if env['target'] in ('debug', 'd'):
    cpp_library += '.debug'
else:
    cpp_library += '.release'

cpp_library += '.' + str(bits)

env.Append(CPPPATH=['.', godot_headers_path, cpp_bindings_path + 'include/', cppflow_path + 'include/', libtensorflow_path + 'include/', cpp_bindings_path + 'include/core/', cpp_bindings_path + 'include/gen/'])
env.Append(LIBPATH=[cpp_bindings_path + 'bin/', 'libtensorflow2/lib/'])
env.Append(LIBS=[cpp_library, 'libtensorflow'])

sources = Glob('*.cpp')

library = env.SharedLibrary(target=target_path + target_name , source=sources)

if env['platform'] == 'osx':
    env.AddPostAction(library, rpath_fix)

Default(library)

Help(opts.GenerateHelpText(env))
