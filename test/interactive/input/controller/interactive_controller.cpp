#include <array>    // array
#include <cstddef>  // size_t

#include "centurion.hpp"

namespace {

inline constexpr cen::i16 dead_zone = 15'000;

inline constexpr float step_size = 0.0002f;

inline constexpr std::array colors{cen::colors::pink,
                                   cen::colors::steel_blue,
                                   cen::colors::red};

using event_dispatcher = cen::event_dispatcher<cen::quit_event,
                                               cen::controller_button_event,
                                               cen::controller_axis_event>;

class interactive_controller final
{
 public:
  interactive_controller()
      : m_window{"Controller demo"}
      , m_renderer{m_window}
      , m_rect{{0, 0}, {100, 100}}
  {
    cen::controller::load_mappings("resources/controllers.txt");

    // clang-format off
    m_dispatcher.bind<cen::quit_event>()
                .to<&interactive_controller::on_quit_event>(this);

    m_dispatcher.bind<cen::controller_button_event>()
                .to<&interactive_controller::on_controller_button_event>(this);

    m_dispatcher.bind<cen::controller_axis_event>()
                .to<&interactive_controller::on_controller_axis_event>(this);
    // clang-format on
  }

  auto run() -> int
  {
    cen::controller controller;

    m_window.show();

    while (m_running)
    {
      m_dispatcher.poll();

      controller.set_led(m_currentColor);

      m_rect.set_x(m_rect.x() + m_dx);
      m_rect.set_y(m_rect.y() + m_dy);

      render();
    }

    m_window.hide();
    return 0;
  }

 private:
  cen::window m_window;
  cen::renderer m_renderer;
  event_dispatcher m_dispatcher;

  cen::frect m_rect;
  float m_dx{};
  float m_dy{};

  std::size_t m_colorIndex{};
  cen::color m_currentColor{colors.at(0)};

  bool m_running{true};

  void render()
  {
    m_renderer.clear_with(m_currentColor);

    m_renderer.set_color(cen::colors::dark_red);
    m_renderer.fill_rect(m_rect);

    m_renderer.present();
  }

  void on_quit_event(const cen::quit_event& event)
  {
    m_running = false;
  }

  void on_controller_button_event(const cen::controller_button_event& event)
  {
    if (event.released())
    {
      ++m_colorIndex;
      m_currentColor = colors.at(m_colorIndex % colors.size());
    }
  }

  void on_controller_axis_event(const cen::controller_axis_event& event)
  {
    const auto update = [](const int value) -> float {
      if ((value < -dead_zone) || (value > dead_zone))
      {
        return static_cast<float>(value) * step_size;
      }
      else
      {
        return 0;
      }
    };

    const auto value = event.value();
    const auto axis = event.axis();

    if (axis == cen::controller_axis::left_x)
    {
      m_dx = update(value);
    }
    else if (axis == cen::controller_axis::left_y)
    {
      m_dy = update(value);
    }
  }
};

}  // namespace

auto main(int, char**) -> int
{
  const cen::library lib;
  interactive_controller demo;
  return demo.run();
}