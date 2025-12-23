# Simple HTTP Server (C++ Learning Project)

## Overview
A lightweight, asynchronous HTTP server implementation in C++20 for Windows, designed to serve static files from a local directory. Built from scratch using Winsock2 and modern C++ features to understand network programming fundamentals.

## 🎯 Project Goals
This project was created as a learning exercise to understand:
- Low-level network programming with Berkeley sockets (Winsock2)
- Asynchronous I/O using `select()` for handling multiple concurrent connections
- HTTP protocol implementation (request parsing, status codes, MIME types)
- Resource management with RAII pattern in C++
- Security considerations in file serving (path traversal prevention)

## ✨ Features

### Core Functionality
- **Asynchronous I/O**: Non-blocking sockets with `select()` for concurrent client handling
- **HTTP/1.1 Support**: Proper status codes (200, 404, 500) with reason phrases
- **MIME Type Detection**: Automatic content-type headers for HTML, CSS, JavaScript
- **Static File Serving**: Serves files from a configurable project directory
- **Clean Architecture**: Separation of concerns with dedicated parser, server, and logging components

### Technical Highlights
- **RAII Pattern**: Custom `socketRAII` and `wsaRAII` classes for automatic resource cleanup
- **Modern C++**: Uses C++20 features including `std::filesystem`, structured bindings
- **Colored Console Logging**: Timestamped, color-coded logging system for debugging
- **Error Handling**: Proper HTTP error responses with appropriate status codes

## 🏗️ Architecture

### Component Overview
```
├── server.cpp/h          - Core server logic, socket management, select() loop
├── parser.cpp/h          - HTTP request parsing, file resolution, MIME detection
├── socketRAII.cpp        - RAII wrapper for socket cleanup
├── wsaRAII.cpp           - RAII wrapper for Winsock initialization/cleanup
├── consoleManager.cpp/h  - Colored logging system with timestamps
└── serverDataLogger.h    - Data structures for metrics (planned feature)
```

### Key Design Patterns
- **RAII (Resource Acquisition Is Initialization)**: Automatic cleanup of sockets and Winsock resources
- **Separation of Concerns**: Parser handles HTTP logic, server handles network I/O
- **Fail-Fast**: Early returns on errors with proper status codes

## 🚀 Getting Started

### Prerequisites
- Windows 10/11
- CMake 3.x or higher
- MinGW-w64 or MSVC compiler with C++20 support
- Winsock2 library (ws2_32)

### Building
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Running
```bash
# Create your projects directory structure
mkdir projects\project_name

# Place your HTML/CSS/JS files in the project directory
# Example: projects\project_name\index.html

# Run the server
.\untitled.exe
```

The server will start on `localhost:5051`

### Testing
Open your browser and navigate to:
```
http://localhost:5051/index.html
```

## 📝 Current Implementation Status

### ✅ Implemented
- [x] TCP socket creation and binding
- [x] Non-blocking I/O with `select()`
- [x] Multiple concurrent client connections
- [x] HTTP request line parsing
- [x] File serving from configured directory
- [x] HTTP status codes (200, 404, 500)
- [x] MIME type detection (HTML, CSS, JS)
- [x] RAII resource management
- [x] Colored console logging

### 🚧 In Progress
- [ ] Path traversal security validation
- [ ] Complete error response pages (404/500 HTML)
- [ ] Full HTTP header parsing
- [ ] Request size limits
- [ ] Connection timeouts

## 📋 Roadmap / TODO

### High Priority
- [ ] **Security: Path Traversal Prevention**
  - Input validation to reject `../` patterns
  - Canonical path verification
  - Reject absolute paths and encoded attacks
- [ ] **Bug Fixes**
  - Fix HTTP response line spacing (missing space before reason phrase)
  - Fix Content-Type header spacing (missing space after colon)
  - Initialize status code to 200 by default
  - Add missing return statement after file open failures
  - Set MIME type for error responses
  - Fix default MIME type from "undefined" to "application/octet-stream"

### Medium Priority
- [ ] **Enhanced HTTP Support**
  - More MIME types: images (PNG, JPEG, GIF), JSON, fonts, SVG, ICO
  - Proper 404/500 error pages with HTML content
  - Additional status codes: 400 Bad Request, 403 Forbidden, 413 Payload Too Large
  - Full HTTP header parsing (Host, User-Agent, Accept, etc.)
  - Query string parsing (`/page?id=123`)
- [ ] **Performance & Reliability**
  - Connection limits (max concurrent clients)
  - Request size validation and limits
  - Connection timeouts using select() timeout parameter
  - Socket option improvements (SO_REUSEADDR, TCP_NODELAY)

### Low Priority
- [ ] **Monitoring & Debugging**
  - Complete serverDataLogger implementation
  - Request/response metrics (total requests, bytes sent, error counts)
  - Performance timing (response time tracking)
  - Access logging to file
- [ ] **Advanced Features**
  - Keep-alive connection support (HTTP/1.1)
  - Configuration file support (port, directories, limits)
  - Graceful shutdown signal handling
  - Directory index listing
- [ ] **Cross-Platform Support**
  - Linux port using Berkeley sockets
  - Platform abstraction layer
  - CMake cross-platform build configuration
- [ ] **OS-Level Security**
  - Windows: Integrity Levels sandboxing
  - Linux: chroot jail implementation

## 🐛 Known Bugs

### Critical
- ⚠️ **Uninitialized status code**: `statusCode` variable not initialized, may contain garbage values
- ⚠️ **Reading from closed file**: Missing early return after file open failure causes undefined behavior
- ⚠️ **Security: No path traversal protection**: Can access files outside project directory with `../` attacks

### High Priority
- HTTP response line missing space: `"HTTP/1.1 404Not Found"` instead of `"HTTP/1.1 404 Not Found"`
- Content-Type header missing space: `"Content-Type:text/html"` instead of `"Content-Type: text/html"`
- MIME type not set for error responses (empty string in header)
- Invalid default MIME type: Returns "undefined" instead of valid type

### Medium Priority
- No request size limits (potential buffer overflow)
- No connection limits (potential DoS)
- select() error handling incomplete (just continues on error)
- No timeout handling (connections can hang indefinitely)

### Low Priority
- serverDataLogger class empty (defined but not implemented)
- File type detection incomplete (missing many common types)
- No logging to file (console only)

## 🔒 Security Considerations

### Current Security Measures
- HTTP status codes for error conditions
- File existence validation before serving
- Separate error handling for different failure modes

### Known Security Limitations
⚠️ **This is a learning project and NOT production-ready!**

**Missing security features:**
- No path traversal protection (can access files outside project directory)
- No request size limits (potential DoS)
- No connection limits (potential resource exhaustion)
- No authentication or authorization
- No HTTPS/TLS support
- No rate limiting
- No input sanitization for special characters

**For production use, you would need:**
- Input validation to prevent `../` path traversal
- Canonical path verification using `std::filesystem::canonical()`
- Rate limiting and connection pooling
- OS-level sandboxing (chroot on Linux, Integrity Levels on Windows)
- Request timeouts and size limits
- Proper error handling for all edge cases

## 🛠️ Technical Details

### Network I/O Model
- **select()**: Monitors multiple socket file descriptors for readiness
- **Non-blocking sockets**: Uses `ioctlsocket(FIONBIO)` to prevent blocking
- **fd_set management**: Maintains master set of all client connections
- **HTTP/1.0 style**: Closes connection after each response (no keep-alive yet)

### HTTP Implementation
```
Request:  GET /index.html HTTP/1.1
Response: HTTP/1.1 200 OK
          Content-Type: text/html
          Content-Length: 1234
          Connection: close
          
          [file contents]
```

### Supported MIME Types
| Extension | MIME Type              |
|-----------|------------------------|
| .html     | text/html              |
| .css      | text/css               |
| .js       | application/javascript |

## 📚 Learning Resources Used
- Beej's Guide to Network Programming
- Microsoft Winsock2 Documentation
- HTTP/1.1 RFC 2616 specification
- C++17/20 filesystem library documentation

## 🤝 Contributing
This is a personal learning project, but feedback and suggestions are welcome! Feel free to:
- Open issues for bugs or suggestions
- Share similar learning projects
- Suggest improvements to code structure or design

## 📄 License
MIT License - Feel free to use this for your own learning!

## 🙏 Acknowledgments
- Console logging system inspired by modern logging frameworks
- RAII pattern examples from C++ Core Guidelines
- Socket examples from Winsock2 documentation
- HTTP status codes library from [j-ulrich/http-status-codes-cpp](https://github.com/j-ulrich/http-status-codes-cpp)
- AI assistance (Claude) for code review and learning guidance

---

**Note**: This is an educational project created to learn network programming and HTTP protocol fundamentals. It is not intended for production use without significant security hardening and additional features.
