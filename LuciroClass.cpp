// -----------------------------
// LuciroClass_system.cpp

#include "LuciroClass.h"
#include <stdexcept>



// ---- ClassDef implementation ----
ClassDef::ClassDef(const std::string& n, ClassDef* parent_)
    : name(n), parent(parent_)
{
    // If there's a parent, inherit (flatten) its fields and defaults
    if (parent) {
        // copy parent's flattened fields
        field_names = parent->field_names;
        field_defaults = parent->field_defaults;
        // rebuild index
        field_index.clear();
        for (size_t i = 0; i < field_names.size(); ++i)
            field_index[field_names[i]] = i;
    }
}

void ClassDef::add_field(const std::string& fname, const Value& default_value) {
    if (field_index.find(fname) != field_index.end()) {
        // replace default for existing field
        size_t idx = field_index[fname];
        field_defaults[idx] = default_value;
        return;
    }

    size_t idx = field_names.size();
    field_names.push_back(fname);
    field_defaults.push_back(default_value);
    field_index[fname] = idx;
}

void ClassDef::add_native_method(const std::string& mname, NativeMethod m) {
    Method meth;
    meth.native = std::move(m);
    meth.is_native = true;
    methods[mname] = std::move(meth);
}

bool ClassDef::has_field(const std::string& name) const {
    return field_index.find(name) != field_index.end();
}

int ClassDef::get_field_index(const std::string& name) const {
    auto it = field_index.find(name);
    if (it != field_index.end()) return static_cast<int>(it->second);
    return -1;
}

const Method* ClassDef::lookup_method(const std::string& name) const {
    const ClassDef* cur = this;
    while (cur) {
        auto it = cur->methods.find(name);
        if (it != cur->methods.end())
            return &it->second;
        cur = cur->parent;
    }
    return nullptr;
}

// ---- Instance implementation ----
Instance::Instance(ClassDef* k) : klass(k) {
    if (!klass) throw std::invalid_argument("Instance requires non-null ClassDef");
    // copy defaults
    fields = klass->field_defaults;
    // ensure fields vector size matches class layout
    if (fields.size() != klass->field_names.size())
        fields.resize(klass->field_names.size());
}

Value* Instance::get_field(const std::string& name) {
    int idx = klass->get_field_index(name);
    if (idx < 0) return nullptr;
    return &fields[static_cast<size_t>(idx)];
}

const Value* Instance::get_field_const(const std::string& name) const {
    int idx = klass->get_field_index(name);
    if (idx < 0) return nullptr;
    return &fields[static_cast<size_t>(idx)];
}

bool Instance::set_field(const std::string& name, const Value& v) {
    int idx = klass->get_field_index(name);
    if (idx < 0) return false;
    fields[static_cast<size_t>(idx)] = v;
    return true;
}

Value Instance::call_method(const std::string& name, const std::vector<Value>& args) {
    const Method* m = klass->lookup_method(name);
    if (!m) {
        throw std::runtime_error("Method '" + name + "' not found on class '" + klass->name + "'");
    }

    // ensure native exists
    if (!m->is_native || !m->native) {
        throw std::runtime_error("Method '" + name + "' is not a native function or is empty");
    }

    // call native function: signature must be Value(Instance*, const std::vector<Value>&)
    return m->native(this, args);
}



// Utility instantiate
std::unique_ptr<Instance> instantiate(ClassDef* klass) {
    return std::make_unique<Instance>(klass);
}

// -----------------------------
// End of LuciroClass_system.cpp