# Need further fine tunes

set(THIRDPARTY_SOURCES)
set(THIRDPARTY_INCLUDE_DIRS)
set(THIRDPARTY_COMPILE_DEFINIES)
file(GLOB_RECURSE THIRDPARTY_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.h**)


if(SVG_ENABLED)
    set(THORVG_SRCS  
        # common
        "src/common/tvgCompressor.cpp" 
        "src/common/tvgMath.cpp" 
        "src/common/tvgStr.cpp" 
        # SVG parser
        "src/loaders/svg/tvgSvgCssStyle.cpp" 
        "src/loaders/svg/tvgSvgLoader.cpp" 
        "src/loaders/svg/tvgSvgPath.cpp" 
        "src/loaders/svg/tvgSvgSceneBuilder.cpp" 
        "src/loaders/svg/tvgSvgUtil.cpp" 
        "src/loaders/svg/tvgXmlParser.cpp" 
        "src/loaders/raw/tvgRawLoader.cpp" 
        # image loaders
        "src/loaders/jpg/tvgJpgd.cpp" 
        "src/loaders/jpg/tvgJpgLoader.cpp" 
        # renderer common
        "src/renderer/tvgAccessor.cpp" 
        "src/renderer/tvgAnimation.cpp" 
        "src/renderer/tvgCanvas.cpp" 
        "src/renderer/tvgFill.cpp" 
        # "src/renderer/tvgGlCanvas.cpp" 
        "src/renderer/tvgInitializer.cpp" 
        "src/renderer/tvgLoader.cpp" 
        "src/renderer/tvgPaint.cpp" 
        "src/renderer/tvgPicture.cpp" 
        "src/renderer/tvgRender.cpp" 
        # "src/renderer/tvgSaver.cpp" 
        "src/renderer/tvgScene.cpp" 
        "src/renderer/tvgShape.cpp" 
        "src/renderer/tvgSwCanvas.cpp" 
        "src/renderer/tvgTaskScheduler.cpp" 
        "src/renderer/tvgText.cpp" 
        # "src/renderer/tvgWgCanvas.cpp" 
        # renderer sw_engine
        "src/renderer/sw_engine/tvgSwFill.cpp" 
        "src/renderer/sw_engine/tvgSwImage.cpp" 
        "src/renderer/sw_engine/tvgSwMath.cpp" 
        "src/renderer/sw_engine/tvgSwMemPool.cpp" 
        "src/renderer/sw_engine/tvgSwPostEffect.cpp" 
        "src/renderer/sw_engine/tvgSwRaster.cpp" 
        "src/renderer/sw_engine/tvgSwRenderer.cpp" 
        "src/renderer/sw_engine/tvgSwRle.cpp" 
        "src/renderer/sw_engine/tvgSwShape.cpp" 
        "src/renderer/sw_engine/tvgSwStroke.cpp")
    set(THORVG_INCLUDES 
        "inc" 
        "src/common" 
        "src/loaders/svg" 
        "src/renderer" 
        "src/renderer/sw_engine" 
        "src/loaders/raw" 
        "src/loaders/external_png" 
        "src/loaders/jpg")
    list(TRANSFORM THORVG_SRCS PREPEND ${CMAKE_CURRENT_LIST_DIR}/thorvg/)
    list(TRANSFORM THORVG_INCLUDES PREPEND ${CMAKE_CURRENT_LIST_DIR}/thorvg/)

    list(APPEND THIRDPARTY_SOURCES ${THORVG_SRCS})
    list(APPEND THIRDPARTY_INCLUDE_DIRS ${THORVG_INCLUDES})

    list(APPEND THIRDPARTY_COMPILE_DEFINIES -D TVG_STATIC)
    
    if(LOTTIE_ENABLED)
        set(THORVG_LOTTIE_INCLUDES
            "src/loaders/lottie"
            "src/loaders/lottie/jerryscript"
            "src/loaders/lottie/rapidjson"
            "src/loaders/lottie/jerryscript/jerry-core"
            "src/loaders/lottie/jerryscript/jerry-core/api"
            "src/loaders/lottie/jerryscript/jerry-core/ecma"
            "src/loaders/lottie/jerryscript/jerry-core/include"
            "src/loaders/lottie/jerryscript/jerry-core/jcontext"
            "src/loaders/lottie/jerryscript/jerry-core/jmem"
            "src/loaders/lottie/jerryscript/jerry-core/jrt"
            "src/loaders/lottie/jerryscript/jerry-core/lit"
            "src/loaders/lottie/jerryscript/jerry-core/parser"
            "src/loaders/lottie/jerryscript/jerry-core/vm"
            "src/loaders/lottie/jerryscript/jerry-core/ecma/base"
            "src/loaders/lottie/jerryscript/jerry-core/ecma/builtin-objects"
            "src/loaders/lottie/jerryscript/jerry-core/ecma/operations"
            "src/loaders/lottie/jerryscript/jerry-core/ecma/builtin-objects/typedarray"
            "src/loaders/lottie/jerryscript/jerry-core/parser/js"
            "src/loaders/lottie/jerryscript/jerry-core/parser/regexp")
        list(TRANSFORM THORVG_LOTTIE_INCLUDES PREPEND ${CMAKE_CURRENT_LIST_DIR}/thorvg/)

        file(GLOB_RECURSE THORVG_LOTTIE_SRCS CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/thorvg/src/loaders/lottie/*.c**)

        list(APPEND THIRDPARTY_SOURCES ${THORVG_LOTTIE_SRCS})
        list(APPEND THIRDPARTY_INCLUDE_DIRS ${THORVG_LOTTIE_INCLUDES})
    endif()
endif()