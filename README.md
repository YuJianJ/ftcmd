# ftcmd
高通 博通平台 用户空间的应用程序，主要用于测试各类硬件性能及参数调整。  
需要搭配各类module内核模块(开放API，不开源)。  
GPL v3 谢谢。  
# 环境搭建
开发环境默认为ubuntu1804，  
交叉编译链需要修改Makefile toolchain路径(toolchain需要自己下载)，  
QCA与BRCM平台均可执行，亲测。  
# 编译  
make clean  
make all
