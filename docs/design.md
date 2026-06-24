# LuminaFrame 设计说明

  ## 1. 项目目标

  LuminaFrame 是一个运行在 IMX6ULL PRO 开发板上的轻量数码相框程序。

  它的核心目标是：
  - 从指定目录读取图片
  - 在 LCD 屏幕上显示图片
  - 支持触摸操作
  - 支持手动浏览和自动播放

  第一版追求稳定、简单、可理解，不追求功能完整。

  ## 2. 运行环境

  目标硬件：
  - IMX6ULL PRO 开发板
  - LCD 显示屏
  - 触摸屏

  目标系统：
  - Linux
  - framebuffer 显示接口
  - Linux input 或 tslib 触摸输入接口

  编译方式：
  - 使用 ARM 交叉编译工具链
  - 工具链后续根据开发板 rootfs 决定

  ## 3. 第一版功能范围

  第一版只实现图片相框核心功能：

  - 程序启动后进入主菜单
  - 主菜单可以进入图片浏览页面
  - 图片浏览页面可以显示目录中的图片文件
  - 用户可以选择图片进入查看页面
  - 图片查看页面可以显示当前图片
  - 图片查看页面支持上一张、下一张、返回
  - 支持从指定目录开始自动播放图片
  - 自动播放支持固定时间间隔切换

  ## 4. 暂不实现的功能

  第一版暂不实现：

  - 音乐播放
  - 文本阅读
  - 网络调试
  - 远程控制
  - 复杂设置页面
  - 图片旋转
  - 图片拖拽移动
  - 多点触控
  - 缩略图缓存
  - 数据库存储
  - 主题皮肤

  这些功能以后可以作为扩展，但不能影响第一版主链路。

  ## 5. 核心页面

  第一版包含四个页面：

  - 主菜单页面
  - 图片浏览页面
  - 图片查看页面
  - 自动播放页面

  每个页面只处理自己的交互，不直接操作其他页面内部状态。

  ## 6. 核心模块

  第一版包含这些模块：

  - app：保存全局应用状态
  - display：负责屏幕初始化、清屏、绘制、刷新
  - input：负责读取触摸输入
  - page：负责页面切换
  - browser：负责目录扫描和图片列表
  - image：负责图片解码和缩放
  - photo：负责单张图片查看
  - slideshow：负责自动播放

  ## 7. 模块边界

  display 模块：
  - 负责 framebuffer 初始化
  - 负责管理屏幕尺寸和像素格式
  - 负责清屏、画矩形、绘制像素数据
  - 不负责页面逻辑
  - 不负责图片文件解析

  input 模块：
  - 负责读取触摸事件
  - 负责把硬件输入转换成统一事件
  - 不负责判断按钮点击了什么功能

  browser 模块：
  - 负责扫描目录
  - 负责保存当前目录、文件列表、当前选中项
  - 不负责显示图片内容
  - 不负责直接操作 framebuffer

  image 模块：
  - 负责判断图片格式
  - 负责解码图片
  - 负责缩放图片
  - 不负责页面跳转
  - 不负责读取触摸事件

  page 模块：
  - 负责页面流转
  - 负责调用当前页面的处理函数
  - 不负责具体业务

  ## 8. 第一版成功标准

  第一版完成时，程序应该可以：

  1. 在 IMX6ULL PRO 上启动
  2. LCD 可以显示主菜单
  3. 触摸可以进入图片浏览页面
  4. 可以从目录中选择图片
  5. 图片可以正确显示在屏幕中间
  6. 可以上一张、下一张
  7. 可以返回主菜单
  8. 可以自动播放指定目录中的图片

  只要这些完成，就认为第一版成功。

  你写需求和边界时，最重要的是这句话：

  第一版只做数码相框核心，不做播放器、不做阅读器、不做复杂系统。

  边界就是保护你不跑偏的。
## 9. 第一阶段：页面状态机

  里面写清楚三件事：

  目标：
  先做一个不依赖硬件的页面状态机，让程序能在终端中模拟页面跳转。

  包含页面：
  - MAIN_MENU_PAGE
  - IMAGE_BROWSING_PAGE
  - IMAGE_VIEWING_PAGE
  - AUTO_PLAY_PAGE

  暂不做：
  - framebuffer
  - 触摸屏
  - 图片解码
  - 目录扫描
  - 自动播放线程

  页面跳转规则：
  - MAIN_MENU_PAGE 可以进入 IMAGE_BROWSING_PAGE
  - MAIN_MENU_PAGE 可以进入 AUTO_PLAY_PAGE
  - IMAGE_BROWSING_PAGE 可以进入 IMAGE_VIEWING_PAGE
  - IMAGE_BROWSING_PAGE 可以返回 MAIN_MENU_PAGE
  - IMAGE_VIEWING_PAGE 可以返回 IMAGE_BROWSING_PAGE
  - IMAGE_VIEWING_PAGE 可以进入 AUTO_PLAY_PAGE
  - AUTO_PLAY_PAGE 可以返回 MAIN_MENU_PAGE

  然后再设计下一批文件，不要急着实现：

  include/app/app.h
  include/page/page.h
  src/app/app.c
  src/page/page.c
  src/pages/main_menu_page.c
  src/pages/image_browsing_page.c
  src/pages/image_viewing_page.c
  src/pages/auto_play_page.c
  main.c

  你要先判断每个文件负责什么：

  app.c：
  负责初始化 AppState，运行主循环，判断程序是否退出。

  page.c：
  负责根据 current_page 调用对应页面。

  main_menu_page.c：
  负责主菜单页面的临时终端交互。

  image_browsing_page.c：
  第一阶段先只打印“这里是浏览页”，然后模拟进入图片查看页或返回。

  image_viewing_page.c：
  第一阶段先只打印“这里是图片查看页”，然后模拟返回或进入自动播放。

  auto_play_page.c：
  第一阶段先只打印“这里是自动播放页”，然后模拟返回主菜单。

  重点是：第一阶段页面可以是假的，但流转必须是真的。

