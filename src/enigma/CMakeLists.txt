add_executable(enigma
  test.cc
)

target_link_libraries(enigma PRIVATE nlohmann_json::nlohmann_json)
