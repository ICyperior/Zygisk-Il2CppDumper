//
// Created by Perfare on 2020/7/4.
//

#include "il2cpp_dump.h"
#include <dlfcn.h>
#include <cstdlib>
#include <cstring>
#include <cinttypes>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include "xdl.h"
#include "log.h"
#include "il2cpp-tabledefs.h"
#include "il2cpp-class.h"
#include <link.h>

static uint64_t il2cpp_base = 0;

int (*il2cpp_init)(const char*);
int (*il2cpp_init_utf16)(const Il2CppChar*);
void (*il2cpp_shutdown)();
const char* (*il2cpp_method_get_name)(const MethodInfo*);
const Il2CppType* (*il2cpp_field_get_type)(FieldInfo*);
const char* (*il2cpp_field_get_name)();
void (*il2cpp_field_static_get_value)(FieldInfo*, void*);
Il2CppClass* (*il2cpp_class_from_type)(const Il2CppType*);
const char* (*il2cpp_class_get_namespace)(Il2CppClass*);
int (*il2cpp_class_get_flags)(const Il2CppClass*);
bool (*il2cpp_class_is_valuetype)(const Il2CppClass*);
bool (*il2cpp_class_is_enum)(const Il2CppClass*);
Il2CppClass* (*il2cpp_class_get_parent)(Il2CppClass*);
Il2CppClass* (*il2cpp_class_get_interfaces)(Il2CppClass*, void**);
const Il2CppImage* (*il2cpp_get_corlib)();
Il2CppClass* (*il2cpp_class_from_name)(const Il2CppImage*, const char*, const char*);
const MethodInfo* (*il2cpp_class_get_method_from_name)(Il2CppClass*, const char*, int);
const Il2CppImage* (*il2cpp_assembly_get_image)(const Il2CppAssembly*);
const char* (*il2cpp_image_get_name)(const Il2CppImage*);
size_t (*il2cpp_image_get_class_count)(const Il2CppImage*);
Il2CppString* (*il2cpp_string_new)(const char*);
Il2CppClass* (*il2cpp_class_from_system_type)(Il2CppReflectionType*);
FieldInfo* (*il2cpp_class_get_fields)(Il2CppClass*, void**);
const MethodInfo* (*il2cpp_class_get_methods)(Il2CppClass*, void**);
const Il2CppType* (*il2cpp_method_get_param)(const MethodInfo*, uint32_t);
uint32_t (*il2cpp_method_get_param_count)(const MethodInfo*);
const char* (*il2cpp_method_get_param_name)(const MethodInfo*, uint32_t);
const PropertyInfo* (*il2cpp_class_get_properties)(Il2CppClass*, void**);
const MethodInfo* (*il2cpp_property_get_get_method)(PropertyInfo*);
const MethodInfo* (*il2cpp_property_get_set_method)(PropertyInfo*);
const char* (*il2cpp_property_get_name)(PropertyInfo*);
int (*il2cpp_field_get_flags)(FieldInfo*);
size_t (*il2cpp_field_get_offset)(FieldInfo*);
const char* (*il2cpp_class_get_name)(Il2CppClass*);
bool (*il2cpp_is_vm_thread)(Il2CppThread*);
Il2CppDomain* (*il2cpp_domain_get)();
const Il2CppAssembly** (*il2cpp_domain_get_assemblies)(const Il2CppDomain*, size_t*);
const Il2CppType* (*il2cpp_method_get_return_type)(const MethodInfo*);
uint32_t (*il2cpp_method_get_flags)(const MethodInfo*, uint32_t*);
const Il2CppType* (*il2cpp_class_get_type)(Il2CppClass*);
Il2CppThread* (*il2cpp_thread_attach)(Il2CppDomain*);
bool (*il2cpp_type_is_byref)(const Il2CppType*);

// Exemple de structure pour stocker les informations sur les fonctions
struct ExportedFunction {
    std::string name;
    void* address;
};

std::vector<ExportedFunction> exported_functions;

// Callback pour `dl_iterate_phdr` pour remplir la liste des fonctions exportées
int callback(struct dl_phdr_info *info, size_t size, void *data) {
    auto *lib_handle = static_cast<void**>(data);

    if (*lib_handle == nullptr) {
        *lib_handle = dlopen(info->dlpi_name, RTLD_NOW);
    }

    if (*lib_handle) {
        // Essayez d'énumérer les symboles exportés
        for (size_t i = 0; i < info->dlpi_phnum; ++i) {
            const ElfW(Phdr) &phdr = info->dlpi_phdr[i];

            if (phdr.p_type == PT_DYNAMIC) {
                ElfW(Dyn) *dyn = (ElfW(Dyn) *)(info->dlpi_addr + phdr.p_vaddr);
                for (; dyn->d_tag != DT_NULL; ++dyn) {
                    if (dyn->d_tag == DT_SYMTAB) {
                        ElfW(Sym) *symtab = (ElfW(Sym) *)(info->dlpi_addr + dyn->d_un.d_ptr);
                        const char *strtab = (const char *)(info->dlpi_addr + dyn->d_un.d_ptr);

                        for (int j = 0; symtab[j].st_name != 0; ++j) {
                            if (ELF32_ST_TYPE(symtab[j].st_info) == STT_FUNC) {
                                ExportedFunction func;
                                func.name = &strtab[symtab[j].st_name];
                                func.address = (void*)(info->dlpi_addr + symtab[j].st_value);
                                exported_functions.push_back(func);
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void dyn_init_il2cpp_api_functions() {
    // Mettre dl_iterate_phdr ou xdl_iterate_phdr avec le callback
    // Supposons que vous ayez déjà la liste des fonctions exportées
    // Vous pouvez les trier ou les filtrer si nécessaire.
    void* handle = dlopen("libil2cpp.so", RTLD_NOW);
    if (!handle) {
        LOGE("Error handle");
    }

    dl_iterate_phdr(callback, &handle);

    // Localiser UnityAdsEngineSetDidFinishCallback
    auto it = std::find_if(exported_functions.begin(), exported_functions.end(),
                           [](const ExportedFunction& func) {
                               return func.name == "UnityAdsEngineSetDidFinishCallback";
                           });

    if (it != exported_functions.end()) {
        // Associer les fonctions selon l'ordre que vous avez fourni
        auto func_it = it; // Le suivant est il2cpp_init

        il2cpp_init = (int (*)(const char*)) (func_it + 1)->address;
        il2cpp_init_utf16 = (int (*)(const Il2CppChar*)) (func_it + 2)->address;
        il2cpp_shutdown = (void (*)()) (func_it + 3)->address;
        il2cpp_method_get_name = (const char* (*) (const MethodInfo * method)) (func_it + 136)->address;
        il2cpp_field_get_type = (const Il2CppType * (*) (FieldInfo *)) (func_it + 88)->address;
        il2cpp_field_get_name = (const char* (*) ()) (func_it + 84)->address;
        il2cpp_field_static_get_value = (void (*)(FieldInfo *, void *)) (func_it + 94)->address;
        il2cpp_class_from_type = (Il2CppClass* (*)(const Il2CppType *)) (func_it + 58)->address;
        il2cpp_class_get_namespace = (const char* (*)(Il2CppClass *)) (func_it + 47)->address;
        il2cpp_class_get_flags = (int (*)(const Il2CppClass *)) (func_it + 59)->address;
        il2cpp_class_is_valuetype = (bool (*)(const Il2CppClass *)) (func_it + 52)->address;
        il2cpp_class_is_enum = (bool (*)(const Il2CppClass *)) (func_it + 64)->address;
        il2cpp_class_get_parent = (Il2CppClass* (*)(Il2CppClass *)) (func_it + 48)->address;
        il2cpp_class_get_interfaces = (Il2CppClass* (*)(Il2CppClass *, void* *)) (func_it + 40)->address;
        il2cpp_get_corlib = (const Il2CppImage* (*)()) (func_it + 12)->address;
        il2cpp_class_from_name = (Il2CppClass* (*)(const Il2CppImage *, const char*, const char *)) (func_it + 35)->address;
        il2cpp_class_get_method_from_name = (const MethodInfo* (*)(Il2CppClass *, const char*, int)) (func_it + 45)->address;
        il2cpp_assembly_get_image = (const Il2CppImage* (*)(const Il2CppAssembly *)) (func_it + 25)->address;
        il2cpp_image_get_name = (const char* (*)(const Il2CppImage *)) (func_it + 214)->address;
        il2cpp_image_get_class_count = (size_t (*)(const Il2CppImage *)) (func_it + 217)->address;
        il2cpp_string_new = (Il2CppString* (*)(const char*)) (func_it + 181)->address;
        il2cpp_class_from_system_type = (Il2CppClass* (*)(Il2CppReflectionType *)) (func_it + 27)->address;
        il2cpp_class_get_fields = (FieldInfo* (*)(Il2CppClass *, void* *)) (func_it + 38)->address;
        il2cpp_class_get_methods = (const MethodInfo* (*)(Il2CppClass *, void* *)) (func_it + 44)->address;
        il2cpp_method_get_param = (const Il2CppType* (*)(const MethodInfo *, uint32_t)) (func_it + 141)->address;
        il2cpp_method_get_param_count = (uint32_t (*)(const MethodInfo *)) (func_it + 140)->address;
        il2cpp_method_get_param_name = (const char* (*)(const MethodInfo *, uint32_t)) (func_it + 147)->address;
        il2cpp_class_get_properties = (const PropertyInfo* (*)(Il2CppClass *, void* *)) (func_it + 41)->address;
        il2cpp_property_get_get_method = (const MethodInfo* (*)(PropertyInfo *)) (func_it + 156)->address;
        il2cpp_property_get_set_method = (const MethodInfo* (*)(PropertyInfo *)) (func_it + 157)->address;
        il2cpp_property_get_name = (const char* (*)(PropertyInfo *)) (func_it + 155)->address;
        il2cpp_field_get_flags = (int (*)(FieldInfo *)) (func_it + 85)->address;
        il2cpp_field_get_offset = (size_t (*)(FieldInfo *)) (func_it + 87)->address;
        il2cpp_class_get_name = (const char* (*)(Il2CppClass *)) (func_it + 46)->address;
        il2cpp_is_vm_thread = (bool (*)(Il2CppThread *)) (func_it + 191)->address;
        il2cpp_domain_get = (Il2CppDomain* (*)()) (func_it + 74)->address;
        il2cpp_domain_get_assemblies = (const Il2CppAssembly** (*)(const Il2CppDomain *, size_t *)) (func_it + 76)->address;
        il2cpp_method_get_return_type = (const Il2CppType* (*)(const MethodInfo *)) (func_it + 133)->address;
        il2cpp_method_get_flags = (uint32_t (*)(const MethodInfo *, uint32_t *)) (func_it + 145)->address;
        il2cpp_class_get_type = (const Il2CppType* (*)(Il2CppClass *)) (func_it + 60)->address;
        il2cpp_thread_attach = (Il2CppThread* (*)(Il2CppDomain *)) (func_it + 188)->address;
        il2cpp_type_is_byref = (bool (*)(const Il2CppType *)) (func_it + 208)->address;

    } else {
        // TODO: Add error here
    }
}

std::string get_method_modifier(uint32_t flags) {
    std::stringstream outPut;
    auto access = flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK;
    switch (access) {
        case METHOD_ATTRIBUTE_PRIVATE:
            outPut << "private ";
            break;
        case METHOD_ATTRIBUTE_PUBLIC:
            outPut << "public ";
            break;
        case METHOD_ATTRIBUTE_FAMILY:
            outPut << "protected ";
            break;
        case METHOD_ATTRIBUTE_ASSEM:
        case METHOD_ATTRIBUTE_FAM_AND_ASSEM:
            outPut << "internal ";
            break;
        case METHOD_ATTRIBUTE_FAM_OR_ASSEM:
            outPut << "protected internal ";
            break;
    }
    if (flags & METHOD_ATTRIBUTE_STATIC) {
        outPut << "static ";
    }
    if (flags & METHOD_ATTRIBUTE_ABSTRACT) {
        outPut << "abstract ";
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            outPut << "override ";
        }
    } else if (flags & METHOD_ATTRIBUTE_FINAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            outPut << "sealed override ";
        }
    } else if (flags & METHOD_ATTRIBUTE_VIRTUAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_NEW_SLOT) {
            outPut << "virtual ";
        } else {
            outPut << "override ";
        }
    }
    if (flags & METHOD_ATTRIBUTE_PINVOKE_IMPL) {
        outPut << "extern ";
    }
    return outPut.str();
}

bool _il2cpp_type_is_byref(const Il2CppType *type) {
    auto byref = type->byref;
    if (il2cpp_type_is_byref) {
        byref = il2cpp_type_is_byref(type);
    }
    return byref;
}

std::string dump_method(Il2CppClass *klass) {
    std::stringstream outPut;
    outPut << "\n\t// Methods\n";
    void *iter = nullptr;
    while (auto method = il2cpp_class_get_methods(klass, &iter)) {
        //TODO attribute
        if (method->methodPointer) {
            outPut << "\t// RVA: 0x";
            outPut << std::hex << (uint64_t) method->methodPointer - il2cpp_base;
            outPut << " VA: 0x";
            outPut << std::hex << (uint64_t) method->methodPointer;
        } else {
            outPut << "\t// RVA: 0x VA: 0x0";
        }
        /*if (method->slot != 65535) {
            outPut << " Slot: " << std::dec << method->slot;
        }*/
        outPut << "\n\t";
        uint32_t iflags = 0;
        auto flags = il2cpp_method_get_flags(method, &iflags);
        outPut << get_method_modifier(flags);
        //TODO genericContainerIndex
        auto return_type = il2cpp_method_get_return_type(method);
        if (_il2cpp_type_is_byref(return_type)) {
            outPut << "ref ";
        }
        auto return_class = il2cpp_class_from_type(return_type);
        outPut << il2cpp_class_get_name(return_class) << " " << il2cpp_method_get_name(method)
               << "(";
        auto param_count = il2cpp_method_get_param_count(method);
        for (int i = 0; i < param_count; ++i) {
            auto param = il2cpp_method_get_param(method, i);
            auto attrs = param->attrs;
            if (_il2cpp_type_is_byref(param)) {
                if (attrs & PARAM_ATTRIBUTE_OUT && !(attrs & PARAM_ATTRIBUTE_IN)) {
                    outPut << "out ";
                } else if (attrs & PARAM_ATTRIBUTE_IN && !(attrs & PARAM_ATTRIBUTE_OUT)) {
                    outPut << "in ";
                } else {
                    outPut << "ref ";
                }
            } else {
                if (attrs & PARAM_ATTRIBUTE_IN) {
                    outPut << "[In] ";
                }
                if (attrs & PARAM_ATTRIBUTE_OUT) {
                    outPut << "[Out] ";
                }
            }
            auto parameter_class = il2cpp_class_from_type(param);
            outPut << il2cpp_class_get_name(parameter_class) << " "
                   << il2cpp_method_get_param_name(method, i);
            outPut << ", ";
        }
        if (param_count > 0) {
            outPut.seekp(-2, outPut.cur);
        }
        outPut << ") { }\n";
        //TODO GenericInstMethod
    }
    return outPut.str();
}

std::string dump_property(Il2CppClass *klass) {
    std::stringstream outPut;
    outPut << "\n\t// Properties\n";
    void *iter = nullptr;
    while (auto prop_const = il2cpp_class_get_properties(klass, &iter)) {
        //TODO attribute
        auto prop = const_cast<PropertyInfo *>(prop_const);
        auto get = il2cpp_property_get_get_method(prop);
        auto set = il2cpp_property_get_set_method(prop);
        auto prop_name = il2cpp_property_get_name(prop);
        outPut << "\t";
        Il2CppClass *prop_class = nullptr;
        uint32_t iflags = 0;
        if (get) {
            outPut << get_method_modifier(il2cpp_method_get_flags(get, &iflags));
            prop_class = il2cpp_class_from_type(il2cpp_method_get_return_type(get));
        } else if (set) {
            outPut << get_method_modifier(il2cpp_method_get_flags(set, &iflags));
            auto param = il2cpp_method_get_param(set, 0);
            prop_class = il2cpp_class_from_type(param);
        }
        if (prop_class) {
            outPut << il2cpp_class_get_name(prop_class) << " " << prop_name << " { ";
            if (get) {
                outPut << "get; ";
            }
            if (set) {
                outPut << "set; ";
            }
            outPut << "}\n";
        } else {
            if (prop_name) {
                outPut << " // unknown property " << prop_name;
            }
        }
    }
    return outPut.str();
}

std::string dump_field(Il2CppClass *klass) {
    std::stringstream outPut;
    outPut << "\n\t// Fields\n";
    auto is_enum = il2cpp_class_is_enum(klass);
    void *iter = nullptr;
    while (auto field = il2cpp_class_get_fields(klass, &iter)) {
        //TODO attribute
        outPut << "\t";
        auto attrs = il2cpp_field_get_flags(field);
        auto access = attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;
        switch (access) {
            case FIELD_ATTRIBUTE_PRIVATE:
                outPut << "private ";
                break;
            case FIELD_ATTRIBUTE_PUBLIC:
                outPut << "public ";
                break;
            case FIELD_ATTRIBUTE_FAMILY:
                outPut << "protected ";
                break;
            case FIELD_ATTRIBUTE_ASSEMBLY:
            case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
                outPut << "internal ";
                break;
            case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
                outPut << "protected internal ";
                break;
        }
        if (attrs & FIELD_ATTRIBUTE_LITERAL) {
            outPut << "const ";
        } else {
            if (attrs & FIELD_ATTRIBUTE_STATIC) {
                outPut << "static ";
            }
            if (attrs & FIELD_ATTRIBUTE_INIT_ONLY) {
                outPut << "readonly ";
            }
        }
        auto field_type = il2cpp_field_get_type(field);
        auto field_class = il2cpp_class_from_type(field_type);
        outPut << il2cpp_class_get_name(field_class) << " " << il2cpp_field_get_name(field);
        //TODO 获取构造函数初始化后的字段值
        if (attrs & FIELD_ATTRIBUTE_LITERAL && is_enum) {
            uint64_t val = 0;
            il2cpp_field_static_get_value(field, &val);
            outPut << " = " << std::dec << val;
        }
        outPut << "; // 0x" << std::hex << il2cpp_field_get_offset(field) << "\n";
    }
    return outPut.str();
}

std::string dump_type(const Il2CppType *type) {
    std::stringstream outPut;
    auto *klass = il2cpp_class_from_type(type);
    outPut << "\n// Namespace: " << il2cpp_class_get_namespace(klass) << "\n";
    auto flags = il2cpp_class_get_flags(klass);
    if (flags & TYPE_ATTRIBUTE_SERIALIZABLE) {
        outPut << "[Serializable]\n";
    }
    //TODO attribute
    auto is_valuetype = il2cpp_class_is_valuetype(klass);
    auto is_enum = il2cpp_class_is_enum(klass);
    auto visibility = flags & TYPE_ATTRIBUTE_VISIBILITY_MASK;
    switch (visibility) {
        case TYPE_ATTRIBUTE_PUBLIC:
        case TYPE_ATTRIBUTE_NESTED_PUBLIC:
            outPut << "public ";
            break;
        case TYPE_ATTRIBUTE_NOT_PUBLIC:
        case TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM:
        case TYPE_ATTRIBUTE_NESTED_ASSEMBLY:
            outPut << "internal ";
            break;
        case TYPE_ATTRIBUTE_NESTED_PRIVATE:
            outPut << "private ";
            break;
        case TYPE_ATTRIBUTE_NESTED_FAMILY:
            outPut << "protected ";
            break;
        case TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM:
            outPut << "protected internal ";
            break;
    }
    if (flags & TYPE_ATTRIBUTE_ABSTRACT && flags & TYPE_ATTRIBUTE_SEALED) {
        outPut << "static ";
    } else if (!(flags & TYPE_ATTRIBUTE_INTERFACE) && flags & TYPE_ATTRIBUTE_ABSTRACT) {
        outPut << "abstract ";
    } else if (!is_valuetype && !is_enum && flags & TYPE_ATTRIBUTE_SEALED) {
        outPut << "sealed ";
    }
    if (flags & TYPE_ATTRIBUTE_INTERFACE) {
        outPut << "interface ";
    } else if (is_enum) {
        outPut << "enum ";
    } else if (is_valuetype) {
        outPut << "struct ";
    } else {
        outPut << "class ";
    }
    outPut << il2cpp_class_get_name(klass); //TODO genericContainerIndex
    std::vector<std::string> extends;
    auto parent = il2cpp_class_get_parent(klass);
    if (!is_valuetype && !is_enum && parent) {
        auto parent_type = il2cpp_class_get_type(parent);
        if (parent_type->type != IL2CPP_TYPE_OBJECT) {
            extends.emplace_back(il2cpp_class_get_name(parent));
        }
    }
    void *iter = nullptr;
    while (auto itf = il2cpp_class_get_interfaces(klass, &iter)) {
        extends.emplace_back(il2cpp_class_get_name(itf));
    }
    if (!extends.empty()) {
        outPut << " : " << extends[0];
        for (int i = 1; i < extends.size(); ++i) {
            outPut << ", " << extends[i];
        }
    }
    outPut << "\n{";
    outPut << dump_field(klass);
    outPut << dump_property(klass);
    outPut << dump_method(klass);
    //TODO EventInfo
    outPut << "}\n";
    return outPut.str();
}

void il2cpp_api_init(void *handle) {
    
    dyn_init_il2cpp_api_functions();
    
    LOGI("il2cpp_handle: %p", handle);
    init_il2cpp_api(handle);
    if (il2cpp_domain_get_assemblies) {
        Dl_info dlInfo;
        if (dladdr((void *) il2cpp_domain_get_assemblies, &dlInfo)) {
            il2cpp_base = reinterpret_cast<uint64_t>(dlInfo.dli_fbase);
        }
        LOGI("il2cpp_base: %" PRIx64"", il2cpp_base);
    } else {
        LOGE("Failed to initialize il2cpp api.");
        return;
    }
    while (!il2cpp_is_vm_thread(nullptr)) {
        LOGI("Waiting for il2cpp_init...");
        sleep(1);
    }
    auto domain = il2cpp_domain_get();
    il2cpp_thread_attach(domain);
}

void il2cpp_dump(const char *outDir) {
    LOGI("dumping...");

    size_t size;
    auto domain = il2cpp_domain_get();
    auto assemblies = il2cpp_domain_get_assemblies(domain, &size);
    std::stringstream imageOutput;
    for (int i = 0; i < size; ++i) {
        auto image = il2cpp_assembly_get_image(assemblies[i]);
        imageOutput << "// Image " << i << ": " << il2cpp_image_get_name(image) << "\n";
    }
    std::vector<std::string> outPuts;
    if (il2cpp_image_get_class) {
        LOGI("Version greater than 2018.3");
        //使用il2cpp_image_get_class
        for (int i = 0; i < size; ++i) {
            auto image = il2cpp_assembly_get_image(assemblies[i]);
            std::stringstream imageStr;
            imageStr << "\n// Dll : " << il2cpp_image_get_name(image);
            auto classCount = il2cpp_image_get_class_count(image);
            for (int j = 0; j < classCount; ++j) {
                auto klass = il2cpp_image_get_class(image, j);
                auto type = il2cpp_class_get_type(const_cast<Il2CppClass *>(klass));
                //LOGD("type name : %s", il2cpp_type_get_name(type));
                auto outPut = imageStr.str() + dump_type(type);
                outPuts.push_back(outPut);
            }
        }
    } else {
        LOGI("Version less than 2018.3");
        //使用反射
        auto corlib = il2cpp_get_corlib();
        auto assemblyClass = il2cpp_class_from_name(corlib, "System.Reflection", "Assembly");
        auto assemblyLoad = il2cpp_class_get_method_from_name(assemblyClass, "Load", 1);
        auto assemblyGetTypes = il2cpp_class_get_method_from_name(assemblyClass, "GetTypes", 0);
        if (assemblyLoad && assemblyLoad->methodPointer) {
            LOGI("Assembly::Load: %p", assemblyLoad->methodPointer);
        } else {
            LOGI("miss Assembly::Load");
            return;
        }
        if (assemblyGetTypes && assemblyGetTypes->methodPointer) {
            LOGI("Assembly::GetTypes: %p", assemblyGetTypes->methodPointer);
        } else {
            LOGI("miss Assembly::GetTypes");
            return;
        }
        typedef void *(*Assembly_Load_ftn)(void *, Il2CppString *, void *);
        typedef Il2CppArray *(*Assembly_GetTypes_ftn)(void *, void *);
        for (int i = 0; i < size; ++i) {
            auto image = il2cpp_assembly_get_image(assemblies[i]);
            std::stringstream imageStr;
            auto image_name = il2cpp_image_get_name(image);
            imageStr << "\n// Dll : " << image_name;
            //LOGD("image name : %s", image->name);
            auto imageName = std::string(image_name);
            auto pos = imageName.rfind('.');
            auto imageNameNoExt = imageName.substr(0, pos);
            auto assemblyFileName = il2cpp_string_new(imageNameNoExt.data());
            auto reflectionAssembly = ((Assembly_Load_ftn) assemblyLoad->methodPointer)(nullptr,
                                                                                        assemblyFileName,
                                                                                        nullptr);
            auto reflectionTypes = ((Assembly_GetTypes_ftn) assemblyGetTypes->methodPointer)(
                    reflectionAssembly, nullptr);
            auto items = reflectionTypes->vector;
            for (int j = 0; j < reflectionTypes->max_length; ++j) {
                auto klass = il2cpp_class_from_system_type((Il2CppReflectionType *) items[j]);
                auto type = il2cpp_class_get_type(klass);
                //LOGD("type name : %s", il2cpp_type_get_name(type));
                auto outPut = imageStr.str() + dump_type(type);
                outPuts.push_back(outPut);
            }
        }
    }
    LOGI("write dump file");
    auto outPath = std::string(outDir).append("/files/dump.cs");
    std::ofstream outStream(outPath);
    outStream << imageOutput.str();
    auto count = outPuts.size();
    for (int i = 0; i < count; ++i) {
        outStream << outPuts[i];
    }
    outStream.close();
    LOGI("dump done!");
}
