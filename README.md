# **Banana**
Banana is a Modern C++ Service FrameWork with high performance. It's easy to use to build server program with high concurrence and excute parallel-calculating tasks like MapReduce, the difference is Banana aims high performance with muti-cores not muti-machines. Once again, it's easy to use just with several steps.
## How to use

### step1. Define your own Connection Inherited From Base Connection
- this is the context each socket fd will bind one , and you'll do any related async tasks within this space
- overrride "startReadOrWriteInService" function to async read or write within loop
```
MyConnection::MyConnection(const SpStream &spStream, const WpNetWorkService &wpNetWorkService)
    : Connection(spStream, wpNetWorkService)
{}
void MyConnection::startReadOrWriteInService()
{
    spStream_->asyncRecvBytes(/*num bytes*/, recvCompleteCallback_);

    spStream_->asyncSendBytes(/*vector<char> data*/, sendCompleteCallback_);
}

```
### step2. Define your own Server Inherited From TcpServer
- override "onConnection" function to handle and create new "MyConnection" context defined in step1.
- add it to be managed by binded service thread.
- start async read or write
```
class MyServer : public TcpServer
{
private:
  void onConnection(const SpStream &spStream, const WpNetWorkService &wpNetWorkService)
  {
    auto spConnection = make_shared<MyConnection>(spStream, wpNetWorkService);
    auto spService = wpNetWorkService.lock();
    spService->addNewConnection(spConnection);
    spConnection->startReadOrWriteInService();
  }
};
```
### step3. Enjoy Your Server !
- "REACTOR_NUM" --  num ioloop thread
- "LISTEN_PORT" -- listen port
- "THREAD_POOL_NUM" -- num threads in pool
```
int main()
{
  MyServer myServer(REACTOR_NUM, LISTEN_PORT, THREAD_POOL_NUM);

  myServer.start();

  return 0;
}
```

## Examples
there is also a "map-reduce"-like example to show several clients concurrently perform parallel-calculating task
#### build
- please clone glog and libevent repo and build ahead according to CMakeLists.txt
```
- cd Banana/cmake
- mkdir build; cd build
- cmake ..; make
```
#### run
```
./MapReduceServer
```
#### run test
```
./BenchmarkTest
```
#### result like below
```
root@ubuntu:/code/ServiceFramework/cmake/build# ./BenchmarkTest

test begin! please wait...

serial-calculate on local result: the max num is 2147483632 duration 14837 ms

parallel-calculate from server result: the max num is 2147483632 duration 4903 ms

test finished!
```