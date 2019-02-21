//
// Created by Shinan on 2019/2/20.
//此测试程序将在当前目录生成随机数并写入20个文件,以字符形式存储，以" "分隔，每个文件60M左右,共1亿多个数
//然后分别模拟1个客户端以本地单线程和向服务器请求并行计算（map-reduce），最终寻找最大的数并返回，算法一致。
//打印最大的数并打印运行时间（注意log打印时间戳并不准确，但是足以说明问题）
//此sample可以说明并发(同时执行多个实例)但是不能说明高并发，因为并行计算的场景也不适宜高并发.

#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/stat.h>
using namespace std;

int main()
{
  cout << "test begin! please wait..." << std::endl;
  //生成数据集
  default_random_engine e;
  stringstream ss;
  vector<string> vecPath;
  for (int i = 0; i < 20; i++)
  {
    ss.str("");
    ss << "test_data_part_" << i;
    vecPath.emplace_back(ss.str());

    //如果文件已存在则跳过
    if (access(ss.str().c_str(), F_OK) == 0) continue;
    ofstream file(ss.str());
    //字符形式存储每个文件60M左右,6M个数,共1亿多个数
    for (int j = 0; j < 1024 * 1024 * 6; j++)
      file << e() << " ";
  }

  //本地串行计算

  std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
  size_t maxNum = 0;
  int tmp;
  bool bFirst = true;
  for (auto &path : vecPath)
  {
    ifstream file(path);
    if (bFirst)
    {
      file >> maxNum;
      bFirst = false;
    }
    while (file >> tmp)
    {
      if (tmp > maxNum) maxNum = tmp;
    }
  }
  std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();

  cout << "serial-calculate on local result: the max num is " << maxNum << " duration "
       << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << endl;

  //请求服务器并行计算
  struct sockaddr_in addr_in;
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  addr_in.sin_family = AF_INET;
  addr_in.sin_addr.s_addr = inet_addr("0.0.0.0");
  addr_in.sin_port = htons(10087);
  connect(fd, (struct sockaddr *) &addr_in, sizeof(addr_in));
  ss.str("");
  for (int i = 0; i < vecPath.size(); i++)
  {
    if (i == vecPath.size() - 1)
    {
      ss << vecPath[i];
      break;
    }
    ss << vecPath[i] << " ";
  }
  size_t len = ss.str().size();

  std::chrono::system_clock::time_point t3 = std::chrono::system_clock::now();

  send(fd, &len, sizeof(len), 0);
  send(fd, ss.str().c_str(), len, 0);
  maxNum = 0;
  recv(fd, &maxNum, sizeof(maxNum), 0);

  std::chrono::system_clock::time_point t4 = std::chrono::system_clock::now();

  cout << "parallel-calculate from server result: the max num is " << maxNum << " duration "
       << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count() << " ms" << endl;
  cout << "test finished!" << std::endl;

  return 0;
}