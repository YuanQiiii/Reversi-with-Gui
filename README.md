# Reversi-with-Gui
带有图形化界面的黑白棋项目
- 使用了第三方库 `EasyX`
  - 可以在官网上下载
- 构建平台为Visual Studio 2022
- 需要三个格式为`ico`的图标放在与编译文件同一目录下作为源代码中`Expressions()`的读取文件,否则将不显示表情(但是并不会报错)
  - `careful.ico`
  - `sad.ico`
  - `scornful.ico`
- 有关源代码的具体实现细节,我在的`main.cpp`中撰写了相应的注释,编码格式为UTF-8
  - 采用了单文件编程的方式
  - 整体采用的统一的命名风格
  - 压缩代码行数的同时保证了一定的可读性和可维护性
- 有关Botzone的参赛代码我放在了`EricQ`目录下,与原来项目中的ai基本延续相同思路,只是在评估函数和搜索层数上做了相应的改进,加入了计时模块防止超时
- `lastone.cpp`为我改进之前的代码,有很多重复和迷惑的操作,放在这里记录我的成长
