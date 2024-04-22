#include <unordered_map>
#ifndef DO_API_NO_RETURN
#define DO_API_NO_RETURN(r, n, p) DO_API(r,n,p)
#endif

// Tableau de correspondance entre les faux noms et les vrais noms
static std::unordered_map<std::string, std::string> api_name_map = {
    {"il2cpp_init", "_Znwm74__"},
    {"il2cpp_method_get_name", "_ZThn99rtSvvPAblA5ivH"},
    {"il2cpp_field_get_type", "_ZThn685RziUVTpXLw2"},
    {"il2cpp_field_get_name", "_ZNSt62d6RUNHsJs"},
    {"il2cpp_field_static_get_value", "_Znwm64rDzQ0jyztCdPn"},
    {"il2cpp_class_from_type", "_ZNSt35o1CBlzvh8nfBs"},
    {"il2cpp_class_get_namespace", "_ZNSt47NCEMSwSVQwURTvC"},
    {"il2cpp_class_get_flags", "_ZNSt694gycfne"},
    {"il2cpp_class_is_valuetype", "_Znwm52r2xFCk7SluUy"},
    {"il2cpp_class_is_enum", "_ZNKSt28fP"},
    {"il2cpp_class_get_parent", "_ZThn7hczNSxmGMnngs"},
    {"il2cpp_class_get_interfaces", "_ZNKSt65__qB6numrNAomlV9U8"},
    {"il2cpp_get_corlib", "_ZThn91__C"},
    {"il2cpp_class_from_name", "_ZSt29wfUfejwdc_"},
    {"il2cpp_class_get_method_from_name", "_ZNSt81__v2wrLut2xj2TpYLBHCsaX7o"},
    {"il2cpp_assembly_get_image", "_ZNKSt37__lKDz3nh1"},
    {"il2cpp_image_get_name", "_ZThn92A"},
    {"il2cpp_image_get_class_count", "_ZSt64__8Ckl_khqkcwnnWD"},
    {"il2cpp_image_get_class", "_ZSt40XtwuwTJdCmfi"},
    {"il2cpp_string_new", "_Znwm56Di"},
    {"il2cpp_class_from_system_type", "_Znwm96__9w"}
};

DO_API(int, il2cpp_init, (const char* domain_name));
DO_API(void, il2cpp_shutdown, ());
DO_API(void, il2cpp_class_from_type, (const Il2CppType * type));
DO_API(const MethodInfo*, il2cpp_class_get_method_from_name, (Il2CppClass * klass, const char* name, int argsCount));
DO_API(const Il2CppImage*, il2cpp_assembly_get_image, (const Il2CppAssembly * assembly));
DO_API(const char*, il2cpp_image_get_name, (const Il2CppImage * image));
DO_API(uint32_t, il2cpp_image_get_class_count, (const Il2CppImage * image));
DO_API(const Il2CppClass*, il2cpp_image_get_class, (const Il2CppImage * image, size_t index));
DO_API(Il2CppString*, il2cpp_string_new, (const char* str));
DO_API(Il2CppClass*, il2cpp_class_from_system_type, (Il2CppReflectionType * type));
DO_API(const char*, il2cpp_class_get_namespace, (Il2CppClass * klass));
DO_API(Il2CppClass*, il2cpp_class_get_parent, (Il2CppClass * klass));
DO_API(void, il2cpp_get_corlib, ());
DO_API(Il2CppClass*, il2cpp_class_from_name, (const Il2CppImage * image, const char* namespaze, const char *name));
DO_API(int, il2cpp_class_get_flags, (const Il2CppClass * klass));
DO_API(const char*, il2cpp_method_get_name, (const MethodInfo * method));
DO_API(const Il2CppType*, il2cpp_class_get_type, (Il2CppClass * klass));
DO_API(Il2CppClass*, il2cpp_class_from_type, (const Il2CppType * type));
DO_API(const Il2CppType*, il2cpp_method_get_return_type, (const MethodInfo * method));
DO_API(uint32_t, il2cpp_method_get_param_count, (const MethodInfo * method));
DO_API(const Il2CppType*, il2cpp_method_get_param, (const MethodInfo * method, uint32_t index));

