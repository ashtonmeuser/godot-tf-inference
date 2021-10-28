extends Control

const TFInference = preload("res://addons/godot-tf-inference/TFInference.gdns")
var input = [0, 0] # Simple input X, Y to XOR model
var output = [0] # Will hold result of model inference
var inference_time := 0 # Time taken to infer XOR result

func _ready():
	assert(!$LabelBit0.connect("pressed", self, "_toggle_bit", [0]))
	assert(!$LabelBit1.connect("pressed", self, "_toggle_bit", [1]))
	assert(!$LabelDescription/LinkButton.connect("pressed", self, "_open_repo"))

	# Load model and set signature names
	TFInferenceSingleton.load_model("res://tf_xor_model")
	TFInferenceSingleton.set_names("serving_default_dense_input:0", "StatefulPartitionedCall:0")

	_update_labels()
	_quick_example()

func _update_labels():
	$LabelBit0.text = String(input[0])
	$LabelBit1.text = String(input[1])
	$LabelBit2.text = String(int(round(output[0])))
	$LabelSubtitle2.text = "Raw value: %1.4f" % output[0]
	$LabelSubtitle3.text = "Inference time: %03dus" % inference_time

func _toggle_bit(i: int):
	input[i] = (input[i] + 1) % 2
	var t := OS.get_ticks_usec()
	output = TFInferenceSingleton.infer(input)
	inference_time = OS.get_ticks_usec() - t
	_update_labels()

func _open_repo():
	var _err := OS.shell_open("https://github.com/ashtonmeuser/godot-tf-inference")

func _quick_example():
	# Here are a few ways to use TFInference class from GDScript
	# Note that the addon is in a very early stage
	# Please refer to the repo at github.com/ashtonmeuser/godot-tf-inference
	# Feel free to open an issue or pull request

	# Using autoloaded TFInference singleton via Project Settings (recommended)
	print("XOR(0, 0) = %s" % [TFInferenceSingleton.infer([0, 0])])

	# Alternatively, a local instance of TFInference class
	var tf = TFInference.new()
	tf.load_model("res://tf_xor_model")
	tf.set_names("serving_default_dense_input:0", "StatefulPartitionedCall:0")
	print("XOR(1, 0) = %s" % [tf.infer([1, 0])])

# P.S. No, I did not want to draw the XOR gate
