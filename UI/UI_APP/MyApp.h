#pragma GCC diagnostic push
#pragma GCC
#pragma once

#include <AppCore/AppCore.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>
#include "../../core/crypto_app/src/hash.cpp"
#include "../../core/crypto_app/src/encryption.cpp"
#include "../../core/clinterface_app/src/CLInterface.cpp"
using namespace ultralight;

class MyApp : public AppListener,
              public WindowListener,
              public LoadListener,
              public ViewListener {
  bool isLoggedIn;
public:
  MyApp();

  virtual ~MyApp();

  // Start the run loop.
  virtual void Run();

  // This is called continuously from the app's main loop.
  virtual void OnUpdate() override;

  // This is called when the window is closing.
  virtual void OnClose(ultralight::Window* window) override;

  // This is called whenever the window resizes.
  virtual void OnResize(ultralight::Window* window, uint32_t width, uint32_t height) override;

  // This is called when the page finishes a load in one of its frames.
  virtual void OnFinishLoading(ultralight::View* caller,
                               uint64_t frame_id,
                               bool is_main_frame,
                               const String& url) override;

  // This is called when the DOM has loaded in one of its frames.
  virtual void OnDOMReady(ultralight::View* caller,
                          uint64_t frame_id,
                          bool is_main_frame,
                          const String& url) override;

  // This is called when the page requests to change the Cursor.
  virtual void OnChangeCursor(ultralight::View* caller,
    Cursor cursor) override;

  virtual void OnChangeTitle(ultralight::View* caller,
    const String& title) override;


  virtual void OnChangeURL(String url);

  virtual JSValue loginUser(std::string username, std::string password);

protected:
  RefPtr<App> app_;
  RefPtr<Window> window_;
  RefPtr<Overlay> overlay_;
};