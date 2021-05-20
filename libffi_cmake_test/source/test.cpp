#include <stdio.h>
#include <iostream>
#include <ffi.h>
#include <cstdlib>
#include <string>

char* testFunc(const char* m, const char* n) {
    std::string res = std::string(m) + std::string(n);
    auto ret = (char*)res.c_str();
    std::cout << ret<< std::endl;
    return ret;
}

//int testFunc(int m, int n) {
//    auto res = m + n;
//    return res;
//}

int main() {
    testFunc("test", "func");
    //testFunc(1, 2);


    //拿函数指针
    void* functionPtr = (void*)&testFunc;
    int argCount = 2;

    //参数类型数组
    ffi_type** ffiArgTypes = (ffi_type**)alloca(sizeof(ffi_type*) * argCount);
    //ffiArgTypes[0] = &ffi_type_sint32;
    //ffiArgTypes[1] = &ffi_type_sint32;
    ffiArgTypes[0] = &ffi_type_pointer;
    ffiArgTypes[1] = &ffi_type_pointer;

    //参数数据数组
    void** ffiArgs = (void**)alloca(sizeof(void*) * argCount);
    void* ffiArgPtr = (void*)alloca(ffiArgTypes[0]->size);
    //int* argPtr = (int*)ffiArgPtr;
    //*argPtr = 1;
    char** argPtr = (char**)ffiArgPtr;
    *argPtr = (char*)"hello";

    ffiArgs[0] = ffiArgPtr;

    void* ffiArgPtr2 = (void*)alloca(ffiArgTypes[1]->size);
    //int* argPtr2 = (int*)ffiArgPtr2;
    //*argPtr2 = 2;
    char** argPtr2 = (char**)ffiArgPtr2;
    *argPtr2 = (char*)"world";
    ffiArgs[1] = ffiArgPtr2;

    //生成函数原型 ffi_cfi 对象
    ffi_cif cif;
    //ffi_type* returnFfiType = &ffi_type_sint32;
    ffi_type* returnFfiType = &ffi_type_pointer;
    ffi_status ffiPrepStatus = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, (unsigned int)argCount, returnFfiType, ffiArgTypes);

    if (ffiPrepStatus == FFI_OK) {
        //生成用于保存返回值的内存
        //void* returnPtr = nullptr;
        void** returnPtr = nullptr;
        if (returnFfiType->size) {
            //returnPtr = (void*)alloca(returnFfiType->size);
            returnPtr =(void**)alloca(returnFfiType->size);
        }
        //根据cif函数原型，函数指针，返回值内存指针，函数参数数据调用这个函数
        ffi_call(&cif, (void(*)())functionPtr, returnPtr, ffiArgs);

        //拿到返回值
        //int* returnValue = (int*)returnPtr;
        printf("ret: %s ", (char*)*returnPtr);
    }

    return  0;
}
