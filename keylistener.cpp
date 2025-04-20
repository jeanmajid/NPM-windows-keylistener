#include <nan.h>
#include <windows.h>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>

std::atomic<bool> running(true);
Nan::Persistent<v8::Function> callback;
std::mutex mtx;
std::queue<uint32_t> keyQueue;
uv_async_t async;

void SendKey(uv_async_t *handle)
{
    Nan::HandleScope scope;

    std::queue<uint32_t> localQueue;

    {
        std::lock_guard<std::mutex> lock(mtx);
        std::swap(localQueue, keyQueue);
    }

    while (!localQueue.empty())
    {
        uint32_t keyCode = localQueue.front();
        localQueue.pop();

        v8::Local<v8::Value> argv[] = {
            Nan::New<v8::Uint32>(keyCode)};
        Nan::MakeCallback(Nan::GetCurrentContext()->Global(), Nan::New(callback), 1, argv);
    }
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            {
                std::lock_guard<std::mutex> lock(mtx);
                keyQueue.push(static_cast<uint32_t>(p->vkCode));
            }
            uv_async_send(&async);
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void KeyboardListener()
{
    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    MSG msg;
    while (running)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(10);
    }
    UnhookWindowsHookEx(hook);
}

NAN_METHOD(start)
{
    if (info.Length() < 1 || !info[0]->IsFunction())
    {
        Nan::ThrowTypeError("Callback function required");
        return;
    }
    callback.Reset(info[0].As<v8::Function>());
    running = true;

    uv_async_init(uv_default_loop(), &async, SendKey);

    std::thread(KeyboardListener).detach();
}

NAN_METHOD(stop)
{
    running = false;
    callback.Reset();
}

NAN_METHOD(copyToClipboard)
{
    if (info.Length() < 1 || !info[0]->IsString())
    {
        Nan::ThrowTypeError("String required");
        return;
    }

    v8::Isolate* isolate = info.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::String::Utf8Value str(isolate, info[0]->ToString(context).ToLocalChecked());
    const char* input = *str;

    if (OpenClipboard(NULL))
    {
        EmptyClipboard();

        HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, strlen(input) + 1);
        char* pchData = (char*)GlobalLock(hClipboardData);
        strcpy(pchData, input);
        GlobalUnlock(hClipboardData);

        SetClipboardData(CF_TEXT, hClipboardData);

        CloseClipboard();
    }
}

NAN_MODULE_INIT(Init)
{
    NAN_EXPORT(target, start);
    NAN_EXPORT(target, stop);
    NAN_EXPORT(target, copyToClipboard);
}

NODE_MODULE(keyListener, Init)