# btDefaultSerializer

```c++
btSerializer* s = new btDefaultSerializer;
m_dynamicsWorld->serialize(s);
char resourcePath[1024];
if (b3ResourcePath::findResourcePath("slope.bullet", resourcePath, 1024, 0))
{
    FILE* f = fopen(resourcePath, "wb");
    fwrite(s->getBufferPointer(), s->getCurrentBufferSize(), 1, f);
    fclose(f);
}
```
