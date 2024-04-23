#ifndef DO_API_NO_RETURN
#define DO_API_NO_RETURN(r, n, p) DO_API(r,n,p)
#endif

// Real name to modified name conversions macros
#define il2cpp_class_get_fields _Znwm2hYwfujqYskrgugh
#define il2cpp_class_get_methods _ZNKSt61__9Iyfwcxjrd2m
#define il2cpp_method_get_param _ZThn73TTr
#define il2cpp_method_get_param_name _ZNKSt627zZLdXcm78cx2
#define il2cpp_class_get_properties _ZNKSt28__LOrUnxvproperties
#define il2cpp_property_get_get_method _Znwm84nzkIg_E
#define il2cpp_property_get_set_method _ZNSt23__xgrJxIqr_btq
#define il2cpp_property_get_name _ZNSt19__EpRk0E
#define il2cpp_field_get_flags _ZThn57__zhInNsR8d
#define il2cpp_field_static_get_value _Znwm10__ziO2E
#define il2cpp_field_get_offset _Znwm84__FJffUr
#define il2cpp_class_get_name _ZSt664ng8BcrLoHD
#define il2cpp_is_vm_thread _ZNSt45zFeGW
#define il2cpp_domain_get _ZSt11hcEXbjEqget
#define il2cpp_domain_get_assemblies _ZNKSt41brqNPbggAzk8RuswumT
#define il2cpp_method_get_return_type _Znwm64nfNbTOJU6wspZ	
#define il2cpp_method_get_param_count _ZThn68__ikU5X0xBpMsrh
#define il2cpp_method_get_flags _ZSt4__h5cgzIj2cj	
#define il2cpp_class_get_type _ZNSt72	
#define il2cpp_thread_attach _Znwm27vHddCzmNzdmn


DO_API(int, _Znwm74__, (const char* domain_name));

DO_API(const Il2CppImage*, _ZThn91__C, ());

// assembly
DO_API(const Il2CppImage*, _ZNKSt37__lKDz3nh1, (const Il2CppAssembly * assembly));

// class
DO_API(Il2CppClass*, _ZSt29wfUfejwdc_, (const Il2CppImage * image, const char* namespaze, const char *name));
DO_API(Il2CppClass*, _Znwm96__9w, (Il2CppReflectionType * type));
DO_API(FieldInfo*, _Znwm2hYwfujqYskrgugh, (Il2CppClass * klass, void* *iter));
DO_API(Il2CppClass*, _ZNKSt65__qB6numrNAomlV9U8, (Il2CppClass * klass, void* *iter));
DO_API(const PropertyInfo*, _ZNKSt28__LOrUnxvproperties, (Il2CppClass * klass, void* *iter));
DO_API(const MethodInfo*,  _ZNKSt61__9Iyfwcxjrd2m, (Il2CppClass * klass, void* *iter));
DO_API(const MethodInfo*, _ZNSt81__v2wrLut2xj2TpYLBHCsaX7o, (Il2CppClass * klass, const char* name, int argsCount));
DO_API(const char*, _ZSt664ng8BcrLoHD, (Il2CppClass * klass));
DO_API(const char*, _ZNSt47NCEMSwSVQwURTvC, (Il2CppClass * klass));
DO_API(Il2CppClass*, _ZThn7hczNSxmGMnngs, (Il2CppClass * klass));
DO_API(bool, _Znwm52r2xFCk7SluUy, (const Il2CppClass * klass));
DO_API(int, _ZNSt694gycfne, (const Il2CppClass * klass));
DO_API(Il2CppClass*, _ZNSt35o1CBlzvh8nfBs, (const Il2CppType * type));
DO_API(bool, _ZNKSt28fP, (const Il2CppClass * klass));

// domain
DO_API(Il2CppDomain*, _ZSt11hcEXbjEqget, ());
DO_API(const Il2CppAssembly**, _ZNKSt41brqNPbggAzk8RuswumT, (const Il2CppDomain * domain, size_t * size));

// field
DO_API(int, _ZThn57__zhInNsR8d, (FieldInfo * field));
DO_API(const char*, _ZNSt62d6RUNHsJs, (FieldInfo * field));
DO_API(size_t, _Znwm84__FJffUr, (FieldInfo * field));
DO_API(const Il2CppType*, _ZThn685RziUVTpXLw2, (FieldInfo * field));
DO_API(void, _Znwm64rDzQ0jyztCdPn, (FieldInfo * field, void *value));
DO_API(void, _Znwm10__ziO2E, (FieldInfo * field, void *value));

// method
DO_API(const char*, _ZThn99rtSvvPAblA5ivH, (const MethodInfo * method));
DO_API(const Il2CppType*, _ZThn73TTr, (const MethodInfo * method, uint32_t index));
DO_API(const char*, _ZNKSt627zZLdXcm78cx2, (const MethodInfo * method, uint32_t index));

// property
DO_API(const MethodInfo*, _Znwm84nzkIg_E, (PropertyInfo * prop));
DO_API(const MethodInfo*, _ZNSt23__xgrJxIqr_btq, (PropertyInfo * prop));
DO_API(const char*, _ZNSt19__EpRk0E, (PropertyInfo * prop));

DO_API(Il2CppString*, _Znwm56Di, (const char* str));
DO_API(bool, _ZNSt45zFeGW, (Il2CppThread * thread));

DO_API(const char*, _ZThn92A, (const Il2CppImage * image));
DO_API(size_t, _ZSt64__8Ckl_khqkcwnnWD, (const Il2CppImage * image));
DO_API(const Il2CppClass*, _ZSt40XtwuwTJdCmfi, (const Il2CppImage * image, size_t index));

// Shit i forgot
DO_API(const Il2CppType*, _Znwm64nfNbTOJU6wspZ, (const MethodInfo * method));
DO_API(uint32_t, _ZThn68__ikU5X0xBpMsrh, (const MethodInfo * method));
DO_API(uint32_t, _ZSt4__h5cgzIj2cj, (const MethodInfo * method, uint32_t * iflags));
DO_API(const Il2CppType*, _ZNSt72, (Il2CppClass * klass));
DO_API(Il2CppThread*, _Znwm27vHddCzmNzdmn, (Il2CppDomain * domain));
