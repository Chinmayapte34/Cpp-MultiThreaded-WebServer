# Multi-threaded C++ Web Server

A high-performance HTTP/1.1 web server built from scratch using C++17 and Linux Socket APIs.

## 🚀 Key Features
* **Custom Thread Pool:** Built a thread-safe task queue using `std::mutex` and `std::condition_variable` to handle concurrent connections efficiently.
* **HTTP/1.1 Support:** Manual parsing of request headers and routing logic.
* **Static File Serving:** Serves HTML and CSS content with proper MIME type handling.
* **Architecture:** Clean separation of concerns (Header/Implementation) following industrial standards.

## 🛠️ Tech Stack
* **Language:** C++17
* **Platform:** Linux (WSL2 / Ubuntu)
* **Build System:** CMake
* **Concurrency:** POSIX Threads (pthreads) / C++ Standard Threads

## 📦 How to Run
1. Clone the repo: `git clone <your-link>`
2. Create build folder: `mkdir build && cd build`
3. Build: `cmake .. && make`
4. Run: `../build/WebServer`
