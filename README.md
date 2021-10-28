# Godot TensorFlow Inference

A lightweight, exportable addon allowing for loading and inferring from TensorFlow [SavedModel format](https://www.tensorflow.org/guide/saved_model) models from GDScript. Note that this project is still in development and has only been tested on MacOS.

The focus of the Godot TF Inference addon is not on creating a TensorFlow model but rather using a previously-created model from within Godot. To explore creating a ML model using Godot, please refer to the following projects.
- [CreepyCrawlAI](https://github.com/apockill/CreepyCrawlAI)
- [Godot RL Agents](https://github.com/edbeeching/godot_rl_agents)
- [GodotAIGym](https://github.com/lupoglaz/GodotAIGym)
- [Godot Python](https://github.com/touilleMan/godot-python)

## Installation

Installation involves simply downloading and installing a zip file from Godot's UI. Recompilation of the engine is *not* required.

1. Ensure you have a model in an uncompressed [SavedModel format](https://www.tensorflow.org/guide/saved_model) in your Godot project. This directory should contain *.pb* files and a *variables* subdirectory. The process of creating a TensorFlow model is beyond the scope of this project.
1. Download the Godot TF Inference addon zip file relevant to your platform from the [releases page](https://github.com/ashtonmeuser/godot-tf-inference/releases).
1. In Godot's Asset Library tab, click Import and select the addon zip file. Follow prompts to complete installation of the addon.

## Usage

The following assumes you're using an autoload singleton named *TFInferenceSingleton*. This name is arbitrary but is recommended to be named to reflect the model(s) it is loading.

1. Choose how to expose the Godot TF Inference addon `TFInference` class (autoload singleton or load). The recommended way of exposing an autoload singleton can be accomplished as follows.
    1. In Godot's Project Settings menu, select AutoLoad.
    1. Add a new singleton by selecting *addons/godot-tf-inference/TFInference.gdns* as the path.
    1. Make note of the Node Name e.g. *TFInferenceSingleton* and click Add.
1. Load your model via `TFInferenceSingleton.load_model("res://MY_MODEL")`. Note that `MY_MODEL` should be a TensorFlow SavedModel directory (see [Installation](https://github.com/ashtonmeuser/godot-tf-inference#installation)).
1. Set TensorFlow model signature names via `TFInferenceSingleton.set_names('MY_INPUT', 'MY_OUTPUT')`. For more information on TensorFlow model signatures, see [TensorFlow documentation](https://blog.tensorflow.org/2021/03/a-tour-of-savedmodel-signatures.html). Note that you may have to explore the structure of your model to find such signature names.
1. Infer from your model via `TFInferenceSingleton.infer(['MY_INPUT_0', 'MY_INPUT_1'])`. The array argument of the `infer()` method will be used as a one-dimensional tensor input to your model. A one-dimensional array is returned representing the model's output tensor.

## Examples

Examples can be found in the [examples directory](https://github.com/ashtonmeuser/godot-tf-inference/tree/master/examples) of this repository. So as to not clutter up the repository, examples do not have the Godot TF Inference addon installed. See [Installation](https://github.com/ashtonmeuser/godot-tf-inference#installation).

## Exporting Project

### MacOS

Subject to change after improving dependencies, export settings, etc.

1. Disable library validation in the export template settings.
1. Under the Resources tab of the export template settings, add your model directory to the non-resource export filters e.g. *MY_MODEL/\**.

### Linux

This addon has not been tested on Linux. See [Issue#1](https://github.com/ashtonmeuser/godot-tf-inference/issues/1).

### Windows

This addon has not been tested on Windows. See [Issue#2](https://github.com/ashtonmeuser/godot-tf-inference/issues/2).

## Developing

This section is targeted at folks looking to work on the Godot TF Inference addon itself. To develop a Godot game using this addon, simply installing the addon will suffice.

These instructions are tailored to UNIX machines.

1. Clone repo and submodules via `git clone --recurse-submodules https://github.com/ashtonmeuser/godot-tf-inference.git`.
1. Ensure the correct Godot submodule commits are checked out. Refer to relevant branch of the [godot-cpp project](https://github.com/godotengine/godot-cpp/tree/3.x) e.g. `3.x` to verify submodule hashes. At the time of this writing, the hashes for the godot-cpp and godot-headers submodules were `836676193031b706a9151f74959de7ae2fc1279b` and `0f91de28a593670a9cbea3dd78163a31ddfcbff4`, respectively.
1. Download the [TensorFlow C](https://www.tensorflow.org/install/lang_c) libraries for your platform and extract into a directory named *libtensorflow2* at the root of this repository. There should be *include* and *lib* subdirectories within the *libtensorflow2* directory.
1. Install [SConstruct](https://scons.org/) via `pip install SCons`. SConstruct is what Godot uses to build Godot and generate C++ bindings. For convenience, we'll use the same tool to build the Godot TF Inference addon.
1. Compile the Godot C++ bindings. From within the *godot-cpp* directory, run `scons platform=PLATFORM generate_bindings=yes -j4` replacing `PLATFORM` with your relevant platform type e.g. `osx`, `linux`, `windows`, etc. To expedite this process, you may consider setting the `-j` argument to the number of CPUs that your machine has.
1. Compile the Godot TF Inference addon. From the repository root directory, run `scons platform=PLATFORM` once again replacing `PLATFORM` with your platform. This will create the *addons/godot-tf-inference/bin/PLATFORM* directory where `PLATFORM` is your platform. You should see a dynamic library (*.dylib*, *.so*, *.dll*, etc.) created within this directory.
1. Copy the TensorFlow C dynamic libraries to the appropriate platform directory via `cp -RP libtensorflow2/lib/. addons/godot-tf-inference/bin/PLATFORM/` replacing `PLATFORM` with your platform.
1. Zip the addons directory via `zip -FSr addons.zip addons`. This allows the addon to be conveniently distributed and imported into Godot. This zip file can be imported directly into Godot (see [Installation](https://github.com/ashtonmeuser/godot-tf-inference#installation)).

If frequently iterating on the addon using a Godot project, it may help to push the compiled dynamic library directly into your Godot project after every build. This can only be done if the addon has previously been installed in your Godot project. From the repository root directory, run `scons platform=PLATFORM && cp addons/godot-tf-inference/bin/PLATFORM/libgodot-tf-inference.dylib MY_GODOT_PROJECT/addons/godot-tf-inference/bin/PLATFORM/` replacing `PLATFORM` with your platform.

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
