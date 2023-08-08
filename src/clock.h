#pragma once

#include <chrono>
#include <iostream>
#include <string>

class Clock {
public:
  explicit Clock(const std::string &name)
      : m_birth_time(std::chrono::high_resolution_clock::now()), m_name(name) {}

  ~Clock() {
    auto now = std::chrono::high_resolution_clock::now();
    auto life_time = std::chrono::duration_cast<std::chrono::microseconds>(
        now - m_birth_time);
    std::cout << m_name << " costs " << life_time.count() / 1000.f << " ms."
              << std::endl;
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_birth_time;
  std::string m_name;
};
