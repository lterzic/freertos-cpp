# freertos-cpp

A header-only C++ wrapper library for FreeRTOS, providing RAII-friendly interfaces to FreeRTOS kernel objects.

## Integration

Add this project as a CMake dependency in your parent project's `CMakeLists.txt`:

```cmake
FetchContent_Declare(
  freertos_cpp
  GIT_REPOSITORY https://github.com/lterzic/freertos-cpp.git
  GIT_TAG main
)
FetchContent_MakeAvailable(freertos_cpp)

target_link_libraries(your_target freertos_cpp)
```

You'll also need to provide a `FreeRTOSConfig.h` file. Create the `freertos_config` target with the path to your config:

```cmake
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config INTERFACE path/to/your/config)
```

## Example

```cpp
#include <freertos/task.hpp>
#include <freertos/mutex.hpp>
#include <freertos/scheduler.hpp>

class MyTask : public freertos::task<512> {
protected:
  void run() override {
    while (true) {
      // Your task logic here
      sleep(std::chrono::seconds(1));
    }
  }
};

int main() {
  MyTask task("myTask", 1);  // name, priority
  freertos::scheduler::start();
  return 0;
}
```

The library wraps FreeRTOS's core objects—tasks, mutexes, semaphores, queues, and scheduler control—using static allocation and `std::chrono` for time durations.
