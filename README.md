# Godot TensorFlow Inference

A lightweight, exportable addon allowing for loading and inferring from TensorFlow SavedModel models from GDScript. Note that this project is still in development and has only been tested on MacOS.

The focus of this project is *not* on creating a TensorFlow model but rather using a model from within Godot. To explore creating a ML model using Godot, please refer to the following projects.
- [CreepyCrawlAI](https://github.com/apockill/CreepyCrawlAI)
- [Godot RL Agents](https://github.com/edbeeching/godot_rl_agents)
- [GodotAIGym](https://github.com/lupoglaz/GodotAIGym)
- [Godot Python](https://github.com/touilleMan/godot-python)

## Installation

Installation involves simply downloading and installing a zip file from Godot's UI. Recompilation of the engine is not required.

1. Ensure you have a model in an uncompressed [SavedModel format](https://www.tensorflow.org/guide/saved_model) in your Godot project. This directory should contain *.pb* files and a *variables* subdirectory. The process of creating a TensorFlow model is beyond the scope of this project.
1. Download the Godot TF Inference addon zip file from the [releases page](https://github.com/ashtonmeuser/godot-tf-inference/releases).
1. In Godot's Asset Library tab, click Import and select the addon zip file.
1. Choose how to expose addon (autoload singleton or preload). The recommended way of exposing an autoload singleton can be accomplished as follows.
    1. In Godot's Project Settings menu, select AutoLoad.
    1. Add a new singleton by selecting *addons/godot-tf-inference/TFInference.gdns* as the path.
    1. Make note of the Node Name is e.g. *TFInference* and click Add.

## Usage

The following assumes you're using an autoload singleton named *TFInference*.

1. Load model via `TFInference.load_model("res://MY_MODEL")`. Note that `MY_MODEL` should be a TensorFlow SavedModel directory (see Installation).
1. Set TensorFlow model signature names via `TFInference.set_names('MY_INPUT', 'MY_OUTPUT')`. For more information on TensorFlow model signatures, see [TensorFlow documentation](https://blog.tensorflow.org/2021/03/a-tour-of-savedmodel-signatures.html). Note that you may have to explore the structure of your model to find such signature names.
1. Infer from your model via `TFInference.infer(['MY_INPUT_0', 'MY_INPUT_1'])`. The array argument of the `infer()` method will be used as a single-dimensional tensor input to your model. A single-dimensional array is returned as a proxy of the model's output tensor.

## Exporting Project

### MacOS

Subject to change after improving dependencies, export settings, etc.

1. Disable library validation in the export template settings.
1. Add your model directory to export filters in export settings e.g. *MY_MODEL/\**.

## Linux

This addon has not been tested on Linux. See [Issue#1](https://github.com/ashtonmeuser/godot-tf-inference/issues/1).

## Windows

This addon has not been tested on Windows. See [Issue#2](https://github.com/ashtonmeuser/godot-tf-inference/issues/2).

## Developing

This section is targeted at folks looking to work on the Godot TF Inference addon itself. To develop a Godot game using this addon, simply installing the addon will suffice.

These instructions are tailored to UNIX machines.

1. Clone repo and submodules via `git clone --recurse-submodules https://github.com/ashtonmeuser/godot-tf-inference.git`.
1. Ensure correct Godot submodules are checked out. Refer to relevant branch of godot-cpp project e.g. `3.x` to verify submodule hashes. At the time of writing, hashes were `836676193031b706a9151f74959de7ae2fc1279b` (godot-cpp) and `0f91de28a593670a9cbea3dd78163a31ddfcbff4` (godot-headers).
1. Download [TensorFlow C](https://www.tensorflow.org/install/lang_c) for your platform and extract into a directory named *libtensorflow2* at the root of the repository. There should be *include* and *lib* subdirectories within the *libtensorflow2* directory.
1. Install SConstruct via `pip install SCons`. SConstruct is what Godot uses to build Godot and generate C++ bindings. For convenience, we'll use the same tool to build the Godot TF Inference addon.
1. Compile Godot C++ bindings. From within the *godot-cpp* directory, run `scons platform=PLATFORM generate_bindings=yes -j4` replacing `PLATFORM` with the relevant platform type e.g. `osx`, `windows`, etc. To expedite this process, you may consider setting the `-j` argument to the number of CPUs that your machine has.
1. Compile the Godot TF Inference addon. From the repository root directory, run `scons platform=PLATFORM` once again replacing `PLATFORM` with your platform. This will create the *addons/godot-tf-inference/bin/PLATFORM* directory where `PLATFORM` is your platform.
1. Copy the TensorFlow C dynamic libraries to relevant platform directory via `cp -RP libtensorflow2/lib/. addons/godot-tf-inference/bin/PLATFORM/` replacing `PLATFORM` with your platform.
1. Zip addons directory via `zip -FSr addons.zip addons`. This allows the addon to be conveniently distributed and imported into Godot.

If iterating on the addon using a Godot project, it may help to pipe the compiled library directly into your Godot project after every build. This can only be done if the addon has previously been installed in your Godot project.

```
scons platform=PLATFORM && cp addons/godot-tf-inference/bin/PLATFORM/libgodot-tf-inference.dylib MY_GODOT_PROJECT/addons/godot-tf-inference/bin/PLATFORM/
```

## Roadmap

Please feel free submit a PR or an [issue](https://github.com/ashtonmeuser/godot-tf-inference/issues).

- [x] Load model from GDScript
- [x] Specify signature names from GDScript
- [x] Infer from model from GDScript
- [x] Simple example Godot project
- [x] Exportable
- [ ] Return errors
- [ ] Arbitrary tensor shape
- [x] Decent documentation
- [x] Starter GitHub issues
- [x] Resolve Godot `res://` paths for model
- [ ] Windows support
- [ ] Linux support
- [ ] Cart-pole example
- [ ] Godot 4.x support (GDExtension)
- [ ] Type checking and handling type errors
- [ ] Extract model signature definitions
- [ ] Use extracted signature names by default
- [ ] Validate input tensor against signature defs
- [ ] ONNX support
