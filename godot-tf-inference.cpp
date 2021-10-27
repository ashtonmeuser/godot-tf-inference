#include "godot-tf-inference.h"
#include "cppflow/cppflow.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

using namespace godot;

void TFInference::_register_methods() {
    register_method("load_model", &TFInference::load_model);
    register_method("set_names", &TFInference::set_names);
    register_method("infer", &TFInference::infer);
}

void TFInference::_init() {
    this->input_name = "serving_default_input_1";
    this->output_name = "StatefulPartitionedCall";
}

Error copy_resource_to_user_dir(String base_path_src) {
    String base_path_dest = "user://" + base_path_src.trim_prefix("res://");
    Ref<Directory> dir = Directory::_new();
    Error error = dir->open(base_path_src);
	if (error != Error::OK) return error;
    if (!dir->dir_exists(base_path_src)) return Error::ERR_FILE_BAD_PATH;
    if (!dir->dir_exists(base_path_dest)) { // Create dir if required
        dir->make_dir_recursive(base_path_dest);
    }
    dir->list_dir_begin();
    String file_name = dir->get_next();
	while (file_name != "") {
        String file_path_src = base_path_src + "/" + file_name;
		String file_path_dest = base_path_dest + "/" + file_name;
		if (dir->current_is_dir()) {
			if (file_name != "." && file_name != "..") {
				error = copy_resource_to_user_dir(file_path_src);
				if (error != Error::OK) return error;
            }
        } else {
			error = dir->copy(file_path_src, file_path_dest);
			if (error != Error::OK) return error;
        }
		file_name = dir->get_next();
    }
    dir->list_dir_end();
    return Error::OK;
}

void TFInference::load_model(String name) {
    name = name.trim_prefix("res://");
    Error error = copy_resource_to_user_dir("res://" + name);
    // if (error != Error::OK) return error;
    if (error != Error::OK) {
        Godot::print("Failed to copy model directory: {0}", name);
        return;
    }
    String path = OS::get_singleton()->get_user_data_dir().plus_file(name);
    try {
        this->model = new cppflow::model(path.alloc_c_string());
    } catch (std::exception& e) {
        Godot::print("Error while loading model: {0}", e.what());
        return;
        // return Error::ERR_FILE_CANT_OPEN;
    }
    // return Error::OK;
}

void TFInference::set_names(String input_name, String output_name) {
    this->input_name = input_name.alloc_c_string();
    this->output_name = output_name.alloc_c_string();
}

Array TFInference::infer(Array array_in) {
    Array array_out = Array::make();
    if (array_in.empty()) return array_out;
    try {
        // Create input tensor
        std::vector<float> vec_in;
        for (int i = 0; i < array_in.size(); i++) vec_in.push_back(array_in[i]);
        std::vector<int64_t> shape = {1, (int64_t)vec_in.size()};
        cppflow::tensor input(vec_in, shape);

        // Infer from model and extract output
        cppflow::model& model = *this->model;
        std::vector<cppflow::tensor> output = model({{this->input_name, input}}, {this->output_name});
        std::vector<float> vec_out = output[0].get_data<float>();

        // Copy to Godot array
        for (float i: vec_out) array_out.push_back(i);
    } catch (const std::exception &e) {
        Godot::print("Error while infering: {0}", e.what());
    }
    return array_out;
}
