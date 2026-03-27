# freertos-cpp

A header-only C++ wrapper library for FreeRTOS, providing RAII-friendly interfaces to FreeRTOS kernel objects.

## Integration

Add this project as a CMake dependency in your parent project's `CMakeLists.txt`:

```cmake
FetchContent_Declare(
  freertos_cpp
  GIT_REPOSITORY https://github.com/lterzic/freertos-cpp.git
  GIT_TAG master
)
FetchContent_MakeAvailable(freertos_cpp)

target_link_libraries(your_target freertos_cpp)
```

You'll also need to provide a `FreeRTOSConfig.h` file. Either create the `freertos_config` target with the path
to your config before fetching this repository, since FreeRTOS expects that a target with that name exists, or
an empty target with default settings will be created, and just add your include directory:

```cmake
target_include_directories(freertos_config INTERFACE path/to/your/config)
```

## Example

```cpp
#include <freertos/task.hpp>
#include <freertos/scheduler.hpp>

class my_task : public freertos::task {
public:
    my_task() : freertos::task("my_task", 1, m_stack) {}

protected:
    void run() noexcept override
    {
        while (true) {
            // Your task logic here
            sleep(std::chrono::seconds(1));
        }
    }

private:
    freertos::task_bstack<512> m_stack;
};

int main() {
    my_task task;
    freertos::scheduler::start();
    
    // Should never be reached
    return 1;
}
```
