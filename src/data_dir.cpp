#include "data_dir.h"

#include <filesystem>
#include <iostream>
#include <source_location>

std::string GetDataDir() {
  namespace fs = std::filesystem;
  auto current_file_path = fs::path(__FILE__);
  auto project_root = current_file_path.parent_path().parent_path();
  std::cout << "project_root: " << project_root.string() << std::endl;
  std::cout << "__FILE__: " << __FILE__ << std::endl;
  auto data_dir = project_root / "data";
  return data_dir.string();
}
