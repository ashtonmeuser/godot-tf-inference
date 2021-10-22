#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <iostream>
#include <Godot.hpp>
#include <Node.hpp>
#include "cppflow/cppflow.h"

namespace godot {
class TFInference: public Node {
    GODOT_CLASS(TFInference, Node)

private:
    cppflow::model *model;
    std::string input_name;
    std::string output_name;

public:
    static void _register_methods();
    void _init();
    void load_model(String name);
    void set_names(String input_name, String output_name);
    Array infer(Array array_in);
};
}

#endif
