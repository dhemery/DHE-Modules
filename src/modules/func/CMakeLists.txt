add_library(func INTERFACE)

target_sources(func INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/Func1Panel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Func6Panel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/FuncControls.h
        ${CMAKE_CURRENT_SOURCE_DIR}/FuncEngine.h
        ${CMAKE_CURRENT_SOURCE_DIR}/FuncModule.h
        ${CMAKE_CURRENT_SOURCE_DIR}/OperandParamQuantity.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Operation.h
        ${CMAKE_CURRENT_SOURCE_DIR}/OperatorSwitch.h
        )

target_include_directories(func INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
        $<INSTALL_INTERFACE:.>
        )

target_link_libraries(func
        INTERFACE
        dhe::components
        dhe::controls
        dhe::config
        dhe::widgets
        Rack::SDK
        )

add_library(dhe::func ALIAS func)
