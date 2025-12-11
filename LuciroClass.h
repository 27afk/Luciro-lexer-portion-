// LuciroClass_system.h

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include "token.h" // Value is defined here

// --- Forward declarations ---
// Note: Value is defined in token.h (via UniversalArray.h)
struct UniversalArray; // Forward declare if token.h/Value depends on it (it does)
struct Value;          // Forward declaration for Value (defined in UniversalArray.h via token.h)
class Instance;        // <-- add this forward declaration


// A method representation - either native C++ function or an AST/callable
using NativeMethod = std::function<Value(Instance*, const std::vector<Value>&)>;

struct Method {
    // For simplicity we only implement native methods here.
    // If you later want language-level methods, store an AST node or bytecode pointer.
    NativeMethod native;
    bool is_native = true;
};

// Class definition (type information)
class ClassDef {
public:
    std::string name;

    // field names and their default values (fixed layout)
    std::vector<std::string> field_names;
    std::vector<Value> field_defaults; // same length as field_names
    std::unordered_map<std::string, size_t> field_index; // name -> index

    // methods (name -> Method)
    std::unordered_map<std::string, Method> methods;

    // optional parent for single inheritance
    ClassDef* parent = nullptr; // non-owning

    ClassDef() = default;
    explicit ClassDef(const std::string& n, ClassDef* parent_ = nullptr);

    // add field (must be called while building class)
    void add_field(const std::string& name, const Value& default_value);

    // add/replace method
    void add_native_method(const std::string& name, NativeMethod m);

    // lookup helpers
    bool has_field(const std::string& name) const;
    // returns index OR -1 if not found
    int get_field_index(const std::string& name) const;

    // lookup method including parent chain. returns nullptr if not found
    const Method* lookup_method(const std::string& name) const;
};


// Instance (object) with fixed fields defined by ClassDef
class Instance {
public:
    ClassDef* klass; // non-owning
    std::vector<Value> fields; // sized to klass->field_names.size()

    explicit Instance(ClassDef* k);

    // field access
    // returns pointer to Value or nullptr if field doesn't exist on this instance's class
    Value* get_field(const std::string& name);
    const Value* get_field_const(const std::string& name) const;

    // set field (returns false if field doesn't exist)
    bool set_field(const std::string& name, const Value& v);

    // call method by name
    // returns optional Value (you can design Value::nil for void)
    Value call_method(const std::string& name, const std::vector<Value>& args);
};


// Utility: create an instance and copy defaults
std::unique_ptr<Instance> instantiate(ClassDef* klass);