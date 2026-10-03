add_library(csv STATIC
        CSVColumn.h
        CSVTable.h
        CSVTableModel.h
        CSVTableView.h
)

target_include_directories(csv
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(csv
        PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)