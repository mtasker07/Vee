include_guard()

function(vee_add_unit_tests MODULE_NAME)
    set(test_name "${MODULE_NAME}_tests")
    set(link_target "${MODULE_NAME}")

    if(TARGET "${MODULE_NAME}_lib")
        set(link_target "${MODULE_NAME}_lib")
    endif()

    add_executable(${test_name} ${ARGN})

    target_link_libraries(${test_name}
    PRIVATE
        vee_core
        ${link_target}
        GTest::gtest_main
    )

    gtest_discover_tests(${test_name})
endfunction()
