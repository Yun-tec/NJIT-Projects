# NJIT-Projects
在这里，我将尽可能存放相关的课设或设计代码，给未来的同学做参考

# 网络程序设计
课设打开方式：
用VC6.0绿色版，打开工程文件。10.6为服务端，10.8为客户端。具体方式为点击左上角打开工程，打开dsp格式文件。
第一次编辑时间：2024年6月5日21:20:13

不会就大大方方承认，老师对这门课包容度很高
主要围绕用了什么模型
本课设用的是重叠IO模型。
具体可参考下面链接
https://blog.csdn.net/weixin_39303983/article/details/124076183
https://blog.csdn.net/weixin_39303983/category_11724744.html
本人对于这些并不了解，请自习学习

在ClientManager.c文件里了解如下：
这段代码使用了 **I/O Completion Ports (IOCP)** 模型，这是一个用于高性能网络编程的Windows专用网络模型。以下是一些关键点和支持这一结论的代码段：

1. **重叠（Overlapped）I/O**：
   代码中的 `ProcessIO` 函数接收 `LPOVERLAPPED` 类型的参数，这是一个标准的重叠 I/O 操作，用于非阻塞 I/O 操作。

   ```cpp
   void CClientManager::ProcessIO( CClientContext* pClient, 
                                   LPOVERLAPPED pOverlapped, DWORD dwIOSize)
   ```

2. **重叠结构体**：
   代码使用了 `CONTAINING_RECORD` 宏来获取包含 `OVERLAPPED` 结构的自定义结构 `PIO_OPERATION_DATA`。

   ```cpp
   PIO_OPERATION_DATA pIO = CONTAINING_RECORD(pOverlapped,	
                                               IO_OPERATION_DATA,
                                               overlapped);
   ```

3. **I/O 操作类型**：
   代码根据不同的 I/O 操作类型执行不同的处理逻辑，例如 `IOReadHead`, `IOReadBody`, `IOWriteUnLogin` 等。

   ```cpp
   switch(pIO->type) //重叠操作类型
   {
       case IOReadHead://读包头
           pClient->OnRecvHeadCompleted(dwIOSize);
           break;
       case IOReadBody://读包体
           pClient->OnRecvBodyCompleted(dwIOSize);
           break;
       // 其他操作类型...
   }
   ```

4. **线程安全的客户端管理**：
   代码使用了 `CCriticalSection` 来保护对客户端列表的访问，确保在多线程环境下的安全性。

   ```cpp
   m_cs.Lock();
   // 操作客户端列表
   m_cs.Unlock();
   ```

5. **单例模式的使用**：
   `CClientManager` 使用了单例模式，这对于管理全局资源（如IOCP句柄）是常见的做法。

   ```cpp
   CClientManager * CClientManager::GetClientManager( void )
   {
       return (m_pClientMgr == NULL)? m_pClientMgr = new CClientManager:m_pClientMgr;
   }
   ```

总结，这段代码展示了一个典型的使用 IOCP 的服务器端程序的结构，利用 IOCP 提供高效的异步 I/O 操作来处理大量并发的客户端请求。这种模型在处理高负载、高并发的网络服务器时表现尤为出色。


对比select模型，WsaEventSelect事件选择模型等其他网络模型，在项目，在线考试系统里IO的优势在哪

在项目中选择网络模型时，不同的网络模型在性能、复杂性和适用性上各有优劣。下面对比一下IOCP、select模型和WSAEventSelect模型，尤其是在在线考试系统这种应用场景下的优势。

### IOCP (I/O Completion Ports)
**优点：**
1. **高性能和高扩展性**：
   - IOCP是专为高性能和高并发设计的，能够有效处理大量并发连接。它利用完成端口和工作线程池，确保系统资源的最佳利用。
2. **低CPU占用**：
   - 使用IOCP可以减少CPU的上下文切换，因为它能够将多个I/O操作合并到一个线程池中进行处理。
3. **异步操作**：
   - 提供真正的异步I/O操作，使服务器能够在等待I/O操作完成时执行其他任务，提高资源利用率。
4. **线程池管理**：
   - IOCP自动管理线程池，可以根据负载动态调整工作线程数量，进一步优化性能。

**缺点：**
1. **复杂性**：
   - IOCP的实现比较复杂，需要开发人员对异步I/O和多线程编程有深入的了解。
2. **平台限制**：
   - IOCP是Windows特有的模型，不适用于跨平台开发。

### Select模型
**优点：**
1. **简单易用**：
   - 使用select模型的代码相对简单，适合入门和学习。
2. **跨平台支持**：
   - Select模型在大多数操作系统上都可以使用，具有良好的可移植性。

**缺点：**
1. **性能瓶颈**：
   - Select模型在处理大量并发连接时性能较差。每次调用select函数都需要遍历所有的文件描述符，效率低下。
2. **有限的文件描述符数量**：
   - Select模型在一些系统中存在文件描述符数量的限制，不适合大规模的并发连接场景。
3. **同步操作**：
   - Select模型本质上是同步操作，容易导致服务器在处理I/O操作时阻塞。

### WSAEventSelect模型
**优点：**
1. **事件驱动**：
   - WSAEventSelect使用事件通知机制，适合处理I/O事件而不需要轮询，提高了性能。
2. **Windows平台优化**：
   - 该模型是为Windows平台优化的，利用了Windows的事件对象和WaitForMultipleObjects函数。

**缺点：**
1. **复杂性**：
   - 实现相对复杂，需要管理多个事件对象，并且处理事件信号的逻辑较为复杂。
2. **性能限制**：
   - 虽然比select模型性能有所提升，但在处理非常大量的并发连接时，性能还是不如IOCP。

### 在在线考试系统中的优势分析
在线考试系统通常需要处理大量并发用户的连接请求，涉及实时的数据传输、答题信息的实时提交和反馈等操作。对于这种场景，网络模型的选择至关重要。

**选择IOCP的优势：**
1. **高并发支持**：
   - 在线考试系统需要支持大量考生同时在线答题，IOCP的高并发处理能力使其非常适合这种场景。
2. **低延迟**：
   - IOCP的异步I/O操作减少了I/O等待时间，提高了系统响应速度，确保考试系统能够实时响应考生的操作。
3. **资源利用率**：
   - IOCP自动管理线程池，能够根据系统负载动态调整工作线程数量，优化资源利用，避免资源浪费。

**选择Select或WSAEventSelect的劣势**：
1. **性能瓶颈**：
   - Select模型在高并发场景下效率低下，容易成为系统瓶颈。WSAEventSelect虽然有改进，但在大规模并发连接下仍可能出现性能问题。
2. **复杂性和管理成本**：
   - WSAEventSelect模型需要处理大量的事件对象和复杂的事件管理逻辑，增加了系统的实现复杂度和维护成本。

综上所述，在在线考试系统这种高并发、高实时性要求的场景中，IOCP模型因其高性能、高扩展性和异步操作的优势，通常是最佳选择。虽然实现复杂，但其在性能和资源利用率上的显著提升，可以确保系统在大规模并发用户访问时仍能保持稳定和高效运行。
