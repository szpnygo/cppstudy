- [X] **Hello World (C++基础和CMake)**
   - [X] 学习基本的C++语法
   - [X] 编译并运行你的第一个C++程序
   - [X] 学习如何使用CMake来管理你的项目，包括如何引用第三方库
   - [X] 掌握基础的C++特性，如类、继承、模板、STL等

- [X] **HTTP Requests and JSON Parser (网络编程和数据处理)**
   - [X] 使用C++发出HTTP请求
   - [X] 处理服务器的响应
   - [X] 利用第三方库处理JSON数据
   - [X] 增加错误处理、重试、超时等特性

- [X] **Memory Management in C++**
   - [X] 学习C++中的内存管理机制，理解栈(stack)和堆(heap)的区别
   - [X] 理解和使用智能指针（如unique_ptr和shared_ptr）进行内存管理
   - [X] 学习如何防止内存泄漏和其他常见的内存问题

- [X] **C++ Advanced Features (C++的高级特性)**
   - [X] 学习和理解C++17中新增的特性，包括但不限于结构化绑定（Structured bindings）、if constexpr、std::optional、std::variant等，并通过实践加深理解。
   - [X] 深入理解左值、右值、左值引用、右值引用的概念，明白C++11引入的移动语义和右值引用，并通过实践了解其优点及适用场景。
   - [X] 探索模板的高级用法，包括理解和应用模板元编程，掌握一些常用的元编程技术，如SFINAE（Substitution Failure Is Not An Error，替换失败并非错误）、类型萃取等。
   - [X] 学习理解完美转发（Perfect Forwarding）的概念和用法，理解std::forward的工作原理，实践在函数模板中使用std::forward进行参数转发。

- [X] **Multithreading (并行和并发)**
   - [X] 学习C++的多线程编程、异步编程以及同步原语（如互斥锁和条件变量）
   - [X] 创建一个简单的项目，如一个并发的web爬虫

- [X] **Web Server (网络编程深化)**
   - [X] 学习套接字编程和HTTP协议，创建一个简单的Web服务器，能够处理基本的GET和POST请求
   - [X] 开始从一个单线程服务器开始，然后添加多线程或异步处理来支持并发连接
   - [X] 学习并处理一些网络编程中的错误情况

- [X] **KeyValue Store (数据库和持久化)**
   - [X] 创建一个简单的键值存储，学习如何在C++中进行文件I/O，以及如何将数据持久化到磁盘
   - [ ] 尝试使用一些数据结构，如B树或哈希表，来优化查询性能
   - [X] 掌握如何处理并发写入和读取

- [X] **Unit Testing in C++**
   - [X] 学习C++的单元测试方法和工具，例如Google Test
   - [X] 学习如何编写可测试的代码，理解单元测试的重要性
   - [X] 为之前的demo添加测试

- [X] **Vector Search with HNSWlib**
   - [X] 学习HNSWlib的使用，创建一个简单的应用程序，如一个图像检索系统，来应用向量搜索
   - [X] 对HNSWlib的数据结构进行持久化，使其能够在程序重启后重新加载

- [ ] **Building Your Vector Database**
   - [ ] 将上述所有的技术整合到一起，创建你的向量数据库
   - [ ] 在这个阶段，进行一些设计和架构的工作，处理一些更复杂的问题，如并发控制和错误恢复
   - [ ] 在项目的开发过程中应用所学的错误处理和内存管理知识，确保代码的健壮性
   - [ ] 利用单元测试方法和工具进行全面的测试，确保项目的可用性

- [ ] **Basic Graphics Programming with OpenGL (OpenGL图形编程基础)**
   - [ ] 学习OpenGL的渲染管线和基础概念
   - [ ] 使用OpenGL创建窗口并渲染简单的2D形状
   - [ ] 使用OpenGL渲染3D几何形状，并应用基本的光照模型

- [ ] **Basics of Filament Library (使用Filament库基础)**
   - [ ] 学习并理解Filament库的基本结构和主要类/函数
   - [ ] 在C++程序中集成并配置Filament库
   - [ ] 使用Filament创建简单的3D场景，例如添加光源、摄像机和基本几何体
   - [ ] 学习和实现Filament的基本材质系统
   - [ ] 使用Filament库加载和渲染3D模型

- [ ] **Physically Based Rendering with Filament (物理基础渲染与Filament)**
   - [ ] 学习物理基础渲染（PBR）的基本理论和技术
   - [ ] 使用Filament库实现PBR渲染
   - [ ] 在一个3D模型上应用PBR材质

- [ ] **Rendering Performance Optimization (渲染性能优化)**
   - [ ] 学习关于3D渲染性能优化的常见技术，如LOD、空间分区等
   - [ ] 在Filament渲染中应用至少一种优化技术
   - [ ] 通过工具进行性能分析，对比优化前后的效果

- [ ] **Video Streaming with WebRTC (使用WebRTC库进行视频流传输)**
   - [ ] 学习WebRTC的基本结构和API
   - [ ] 使用WebRTC实现视频流的捕获
   - [ ] 实现通过WebRTC进行视频流的发送和接收

- [ ] **Combining WebRTC with 3D Rendering (将WebRTC和3D渲染结合)**
   - [ ] 将3D渲染的结果作为WebRTC视频流的输入
   - [ ] 实现WebRTC视频流的发送和在Web前端的接收
   - [ ] 在Web前端实现视频流的播放

- [ ] **CUDA Environment Setup and Hello World (CUDA环境配置与Hello World )**
   - [ ] 学习如何在你的开发环境中配置CUDA
   - [ ] 使用C++编写并运行你的第一个CUDA程序，例如一个简单的"Hello, World!"程序
   - [ ] 使用CUDA并行执行一个简单的向量加法操作

- [ ] **CUDA Memory Management and Parallel Programming (CUDA内存管理与并行编程 )**
   - [ ] 使用CUDA进行内存管理，包括设备内存和主机内存的分配和释放
   - [ ] 学习并行编程基础，并实现并行化的向量运算
   - [ ] 利用CUDA对向量运算进行优化，并评估和CPU实现的性能差异

- [ ] **CUDA Concurrency and Streams (CUDA并发处理与流 )**
   - [ ] 学习并实践使用CUDA流来并行执行多个内核或内存拷贝操作
   - [ ] 使用CUDA并行处理多个向量运算请求
   - [ ] 将并发处理技术集成到向量数据库中，进行性能评估和优化

- [ ] **Advanced CUDA Optimization Techniques (CUDA高级优化技术 )**
   - [ ] 学习并应用共享内存、常量内存和纹理内存等CUDA内存优化技术
   - [ ] 对向量数据库进行高级优化，提高查询和计算性能
   - [ ] 进一步优化并发处理和内存使用，确保数据库的稳定性和效率

- [ ] **CUDA Error Handling and Debugging (CUDA错误处理与调试 )**
   - [ ] 学习如何在C++中处理和检测CUDA程序中的错误
   - [ ] 优化和调试向量数据库的CUDA实现
   - [ ] 对整个项目进行全面的测试和优化，确保其可靠性和效率

- [ ] **WebRTC屏幕捕捉及数据通道 (WebRTC Screen Capture and Data Channels)**
   - [ ] 学习WebRTC库的基本使用和结构
   - [ ] 实现使用WebRTC捕获屏幕的视频流
   - [ ] 学习WebRTC的数据通道，用于传输非音视频数据
   - [ ] 实现使用WebRTC的数据通道发送和接收简单消息

- [ ] **WebRTC视频编解码 (WebRTC Video Codec)**
   - [ ] 深入学习WebRTC的媒体流处理，包括音频视频编解码
   - [ ] 实现H264编解码操作，理解编解码对于视频质量和网络传输的影响
   - [ ] 使用WebRTC进行视频流的发送和接收，实现基于WebRTC的点对点视频通话
   - [ ] 测试并优化视频流的传输效率，学习如何处理网络波动情况

- [ ] **操作捕捉与反馈 (Operation Capture and Feedback)**
   - [ ] 学习并实现对鼠标操作的捕捉，理解操作系统的事件机制
   - [ ] 学习并实现对键盘操作的捕捉，理解如何处理不同类型的键盘输入
   - [ ] 通过WebRTC的数据通道发送捕捉到的操作，理解如何将操作数据进行序列化和反序列化
   - [ ] 在远程端接收和处理操作数据，实现对远程系统的基本操作

- [ ] **远程桌面系统整合 (Remote Desktop System Integration)**
   - [ ] 将前面所有的技术整合起来，设计并创建你的远程桌面系统
   - [ ] 实现并发控制和错误恢复，理解在实际项目中如何处理这些复杂问题
   - [ ] 理解并处理可能出现的性能问题，如内存管理，CPU利用率等
   - [ ] 设计并实施对项目的全面测试，包括单元测试，功能测试，以及性能测试
