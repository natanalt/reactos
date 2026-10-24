
list(APPEND SOURCE
    sdport.c)

add_library(sdport MODULE ${SOURCE} sdport.rc ${CMAKE_CURRENT_BINARY_DIR}/sdport.def)
#add_pch(sdport pch.h)
spec2def(sdport.sys sdport.spec ADD_IMPORTLIB)
set_module_type(sdport kernelmodedriver)
add_importlibs(sdport ntoskrnl hal)
add_cd_file(TARGET sdport DESTINATION reactos/system32/drivers NO_CAB FOR all)
