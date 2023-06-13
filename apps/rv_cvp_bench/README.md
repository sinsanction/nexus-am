# CNNBench

RV-CVP 单个算子正确性和性能测试程序。

## 程序构成

共有三组测试程序：func、base、perf。

func 是功能测试程序，对比 RV-CVP 指令实现 CNN 计算和标准指令实现 CNN 计算，验证 RV-CVP 实现的正确性。

base 是基准测试程序，使用标准指令实现 CNN 计算，作为性能测试 baseline。

perf 是 RV-CVP 测试程序，使用 RV-CVP 指令实现 CNN 计算。

每一组程序又分成三组：conv、pool、act。分别测试卷积、池化、激活这三个算子。每个算子又对四种不同精度分别测试。

## 使用方法

在 AM 根目录运行以下命令进行编译：
```
source ./env.sh
cd nexus-am/apps/cnnbench
make ARCH=riscv64-nutshell mainargs=testfunc
```

其中 mainargs 可选：
```
testfunc
testbase
testperf
```
