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

void TFInference::load_model(String name) {
    try {
        this->model = new cppflow::model(name.alloc_c_string());
    } catch (std::exception& e) {
        Godot::print("Error while loading model: {0}", e.what());
    }
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
        return array_out;
    } catch (std::exception& e) {
        Godot::print("Error while infering: {0}", e.what());
        return array_out;
    }
}
