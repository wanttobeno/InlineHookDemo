<p align="center">
  <h3 align="center">InlineHookDemo</h3>
  <p align="center">
    InlineHookDemo -- Hook CreateProcessW as example.
</p>



原函数CreateProcessW的Call入口 

![ScreenShot](/ScreenShot1.png)

执行InlineHook WriteProcessMemory

![ScreenShot](/ScreenShot2.png)

在Call的第一句，jmp到dll代码中去处理

![ScreenShot](/ScreenShot3.png)


