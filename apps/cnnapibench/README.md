# CNNAPIBench

RV-CVP CNNAPI 正确性和性能测试程序。

## 程序构成

共有三个部分：cnnapi、func、lenet。

cnnapi 是 CNNAPI 框架文件。

func 是功能测试程序，对比 RV-CVP 指令实现 CNN 计算和标准指令实现 CNN 计算，验证 RV-CVP CNNAPI 框架实现的正确性。func 分为两组，分别对应 v1 和 v2 两个版本的 CNNAPI。

lenet 是 LeNet-5 模型测试程序。
其中又分为采用随机输入数据的三组程序 lenet5_func、lenet5_base、lenet5_perf，和采用真实模型以及 MNIST 测试集的两组程序 lenet5_real_int8_func、lenet5_real_int8_perf。

## 使用方法

在 AM 根目录运行以下命令进行编译：
```
source ./env.sh
cd nexus-am/apps/cnnapibench
make ARCH=riscv64-nutshell mainargs=testfunc
```

其中 mainargs 可选：
```
testfunc
testfuncv2
test_lenet_func
test_lenet_base
test_lenet_perf
test_lenet_real_func
test_lenet_real_perf
```
