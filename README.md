# Godot TensorFlow Inference

Load and infer from TensorFlow SavedModel models from GDScript.

Focus is not on training but rather using a model.

## Installation

Ensure you have a model in SavedModel format
Download zip
In Asset Library, import zip
Include model in Godot project
Choose how to expose addon (singleton or preload)

## Usage

Load model (do not specify res://)
Set signature names
Use infer()

## Exporting Project

### MacOS

Subject to change after improving dependencies, export settings, etc.

1. Disable library validation in export template settings
1. Ensure model is included (added to .app/Contents/MacOS/)

## Developing

Clone repo
Ensure correct submodules are checked out (refer to godot-cpp)
Download TensorFlow C into libtensorflow2 directory
Compile Godot C++ bindings
Compile addon
Zip addons directory

## Roadmap

- [x] Load model from GDScript
- [x] Specify signature names from GDScript
- [x] Infer from model from GDScript
- [x] Simple example Godot project
- [x] Exportable (with extra steps)
- [ ] Custom export template
- [ ] Arbitrary tensor shape
- [ ] Resolve Godot `res://` paths for model
- [ ] Windows support
- [ ] Linux support
- [ ] Cart-pole example
- [ ] Godot 4.x support (GDExtension)
- [ ] Type checking and handing type errors
- [ ] Extract model signature definitions
- [ ] Use extracted signature names by default
- [ ] Validate input tensor against signature defs
- [ ] ONNX support
