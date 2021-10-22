extends Control

const TFInference = preload("res://addons/godot-tf-inference/TFInference.gdns")

func _ready():
	var input = [1, 0] # Simple input X, Y to XOR model
	var output # Will hold result of model inference

	# New local instance of TFInference class
	var tf = TFInference.new()
	tf.load_model("./tf_xor_model")
	tf.set_names("serving_default_dense_input:0", "StatefulPartitionedCall:0")
	output = tf.infer(input)
	print("Input: %s\tOutput: %s" % [input, output])

	# Alternatively, use a singleton
	TfInferenceSingleton.load_model("./tf_xor_model")
	TfInferenceSingleton.set_names("serving_default_dense_input:0", "StatefulPartitionedCall:0")
	output = TfInferenceSingleton.infer(input)
	print("Input: %s\tOutput: %s" % [input, output])
