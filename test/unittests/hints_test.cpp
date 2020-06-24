#include "hints.h"

#include <catch.hpp>

using namespace centurion;
using namespace hint;

namespace {

template <typename Hint, typename Lambda>
void test_hint(Lambda&& lambda)
{
  const auto optPrev = get_hint<Hint>();

  lambda();

  if (optPrev) {
    set_hint<Hint, HintPrio::Default>(*optPrev);
  }
}

template <typename Hint>
void test_bool_hint()
{
  test_hint<Hint>([] {
    CHECK(set_hint<Hint>(true));
    CHECK(get_hint<Hint>().value());

    CHECK(set_hint<Hint>(false));
    CHECK(!get_hint<Hint>().value());
  });
}

}  // namespace

TEST_CASE("HintPrio", "[Hints]")
{
  CHECK(HintPrio::Default == static_cast<HintPrio>(SDL_HINT_DEFAULT));
  CHECK(HintPrio::Normal == static_cast<HintPrio>(SDL_HINT_NORMAL));
  CHECK(HintPrio::Override == static_cast<HintPrio>(SDL_HINT_OVERRIDE));

  CHECK(static_cast<HintPrio>(SDL_HINT_DEFAULT) == HintPrio::Default);
  CHECK(static_cast<HintPrio>(SDL_HINT_NORMAL) == HintPrio::Normal);
  CHECK(static_cast<HintPrio>(SDL_HINT_OVERRIDE) == HintPrio::Override);
}

TEST_CASE("set_hint", "[Hints]")
{
  SECTION("AccelerometerAsJoystick")
  {
    test_bool_hint<AccelerometerAsJoystick>();
  }

  SECTION("AndroidBlockOnPause") { test_bool_hint<AndroidBlockOnPause>(); }

  SECTION("AppleTVControllerUIEvents")
  {
    test_bool_hint<AppleTVControllerUIEvents>();
  }

  SECTION("AppleTVRemoteAllowRotation")
  {
    test_bool_hint<AppleTVRemoteAllowRotation>();
  }

  SECTION("BMPSaveLegacyFormat") { test_bool_hint<BMPSaveLegacyFormat>(); }

  SECTION("GrabKeyboard") { test_bool_hint<GrabKeyboard>(); }

  SECTION("FramebufferAcceleration")
  {
    test_bool_hint<FramebufferAcceleration>();
  }

  SECTION("IdleTimerDisabled") { test_bool_hint<IdleTimerDisabled>(); }

  SECTION("IMEInternalEditing") { test_bool_hint<IMEInternalEditing>(); }

  SECTION("JoystickAllowBackgroundEvents")
  {
    test_bool_hint<JoystickAllowBackgroundEvents>();
  }

  SECTION("MacBackgroundApp") { test_bool_hint<MacBackgroundApp>(); }

  SECTION("MacCTRLClickEmulateRightClick")
  {
    test_bool_hint<MacCTRLClickEmulateRightClick>();
  }

  SECTION("MouseFocusClickthrough")
  {
    test_bool_hint<MouseFocusClickthrough>();
  }

  SECTION("MouseRelativeModeWarp") { test_bool_hint<MouseRelativeModeWarp>(); }

  SECTION("NoSignalHandlers") { test_bool_hint<NoSignalHandlers>(); };

  // TODO remove Render prefix
  SECTION("RenderDirect3D11Debug") { test_bool_hint<RenderDirect3D11Debug>(); };

  SECTION("RenderDirect3DThreadSafe")
  {
    test_bool_hint<RenderDirect3DThreadSafe>();
  };

  SECTION("RenderOpenGLShaders") { test_bool_hint<RenderOpenGLShaders>(); };

  SECTION("RenderVSync") { test_bool_hint<RenderVSync>(); };

  SECTION("AllowScreensaver") { test_bool_hint<AllowScreensaver>(); };

  SECTION("DisableHighDPI") { test_bool_hint<DisableHighDPI>(); };

  SECTION("MacFullscreenSpaces") { test_bool_hint<MacFullscreenSpaces>(); };

  SECTION("MinimizeOnFocusLoss") { test_bool_hint<MinimizeOnFocusLoss>(); };

  SECTION("X11NetWMPing") { test_bool_hint<X11NetWMPing>(); };

  SECTION("X11Xinerama") { test_bool_hint<X11Xinerama>(); };

  SECTION("X11XRandR") { test_bool_hint<X11XRandR>(); };

  SECTION("X11XVidMode") { test_bool_hint<X11XVidMode>(); };

  SECTION("WindowsDisableThreadNaming")
  {
    test_bool_hint<WindowsDisableThreadNaming>();
  };

  SECTION("WindowsEnableMessageLoop")
  {
    test_bool_hint<WindowsEnableMessageLoop>();
  };

  SECTION("WindowsNoCloseOnAltF4") { test_bool_hint<WindowsNoCloseOnAltF4>(); };

  SECTION("WindowFrameUsableWithCursorHidden")
  {
    test_bool_hint<WindowFrameUsableWithCursorHidden>();
  };

  SECTION("XinputEnabled") { test_bool_hint<XinputEnabled>(); };

  SECTION("XinputUseOldJoystickMapping")
  {
    test_bool_hint<XinputUseOldJoystickMapping>();
  };

  SECTION("RenderDriver")
  {
    test_hint<RenderDriver>([] {
      const auto value = OpenGL;
      CHECK(set_hint<RenderDriver>(value));
      CHECK(get_hint<RenderDriver>().value() == value);
    });
  }

  SECTION("AndroidAPKExpansionMainFileVersion")
  {
    //    using AndroidMainFile = AndroidAPKExpansionMainFileVersion;
    //    test_hint<AndroidMainFile>([] {
    //      CHECK(!get_hint<AndroidMainFile>());
    //      CHECK(set_hint<AndroidMainFile>(1));
    //      CHECK(get_hint<AndroidMainFile>() == 1);
    //    });
  }

  SECTION("AndroidAPKExpansionPatchFileVersion")
  {
    //    using AndroidPatchFile = AndroidAPKExpansionPatchFileVersion;
    //    test_hint<AndroidPatchFile>([] {
    //      CHECK(!get_hint<AndroidPatchFile>());
    //      CHECK(set_hint<AndroidPatchFile>(1));
    //      CHECK(get_hint<AndroidPatchFile>() == 1);
    //    });
  }
}