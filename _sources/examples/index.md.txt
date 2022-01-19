# examples

`renderer(OpenGL)` => `bullet, simulation`

となっているぽい。

`renderer(OpenGL)` => `scene` <= `bullet, simulation`

のように OpenGL と bullet 双方から切り離したシーン表現がほしいか。

## bullet2 系
`GuiHelper` による描画と

## bullet3 系 ?
`m_app->m_renderer` による２系統の描画がある。
`m_app->m_renderer` の方にシーン表現あるかも。
`RenderingExamples`

## SharedMemory 系

`m_data->m_commandProcessor->renderScene(renderFlags);`

# Interface

```{toctree}
:maxdepth: 1
CommonInterfaces
CommonExampleInterface/index
```
