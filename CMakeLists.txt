cmake_minimum_required(VERSION 3.14)
project(ed1_linked_list_list07_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opcional: detecta vazamentos de memória e comportamento indefinido.
# cmake -S . -B build -DENABLE_SANITIZERS=ON
option(ENABLE_SANITIZERS "Compila com AddressSanitizer e UBSan" OFF)

# Biblioteca somente de cabeçalhos (include/LinkedList.h e include/List.h)
add_library(linkedlist_headers INTERFACE)
target_include_directories(linkedlist_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(ENABLE_SANITIZERS)
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

# Demonstração dos exemplos do enunciado
add_executable(list07 main.cpp)
target_link_libraries(list07 PRIVATE linkedlist_headers)

# Testes automatizados
add_executable(list07_tests src/LinkedListTest.cpp)
target_link_libraries(list07_tests PRIVATE linkedlist_headers)

enable_testing()
add_test(NAME list07_tests COMMAND list07_tests)
