#ifndef CENTURION_SENSOR_HEADER
#define CENTURION_SENSOR_HEADER

#include <SDL.h>

#include <array>     // array
#include <cstddef>   // size_t
#include <optional>  // optional
#include <ostream>   // ostream
#include <string>    // string

#include "../core/czstring.hpp"
#include "../core/exception.hpp"
#include "../core/integers.hpp"
#include "../core/owner.hpp"
#include "../detail/address_of.hpp"
#include "../detail/owner_handle_api.hpp"
#include "../detail/to_string.hpp"

namespace cen {

/// \addtogroup input
/// \{

/**
 * \typedef sensor_id
 *
 * \brief Used for unique sensor instance identifiers.
 *
 * \since 5.2.0
 */
using sensor_id = SDL_SensorID;

/**
 * \enum sensor_type
 *
 * \brief Provides values that represent different sensor types.
 *
 * \see SDL_SensorType
 *
 * \since 5.2.0
 */
enum class sensor_type
{
  invalid = SDL_SENSOR_INVALID,      ///< Invalid sensor
  unknown = SDL_SENSOR_UNKNOWN,      ///< Unknown sensor
  accelerometer = SDL_SENSOR_ACCEL,  ///< Accelerometer
  gyroscope = SDL_SENSOR_GYRO        ///< Gyroscope
};

template <typename T>
class basic_sensor;

/**
 * \typedef sensor
 *
 * \brief Represents an owning sensor device.
 *
 * \since 5.2.0
 */
using sensor = basic_sensor<detail::owning_type>;

/**
 * \typedef sensor_handle
 *
 * \brief Represents a non-owning sensor device.
 *
 * \since 5.2.0
 */
using sensor_handle = basic_sensor<detail::handle_type>;

/**
 * \class basic_sensor
 *
 * \brief Represents a sensor device.
 *
 * \see `sensor`
 * \see `sensor_handle`
 *
 * \since 5.2.0
 */
template <typename T>
class basic_sensor final
{
 public:
  /// \name Construction
  /// \{

  /**
   * \brief Creates a sensor instance based on an existing pointer.
   *
   * \note The created instance will claim ownership of the supplied pointer if the class
   * has owning semantics.
   *
   * \param sensor a pointer to the SDL sensor data.
   *
   * \throws cen_error if the supplied pointer is null and the class has owning semantics.
   *
   * \since 5.2.0
   */
  explicit basic_sensor(maybe_owner<SDL_Sensor*> sensor) noexcept(!detail::is_owning<T>())
      : m_sensor{sensor}
  {
    if constexpr (detail::is_owning<T>())
    {
      if (!m_sensor)
      {
        throw cen_error{"Null sensor pointer!"};
      }
    }
  }

  /**
   * \brief Creates an owning sensor instance based on a device index.
   *
   * \param index the device index of the sensor.
   *
   * \throws sdl_error if the sensor cannot be opened.
   *
   * \since 5.2.0
   */
  template <typename TT = T, detail::is_owner<TT> = 0>
  explicit basic_sensor(const int index = 0) : m_sensor{SDL_SensorOpen(index)}
  {
    if (!m_sensor)
    {
      throw sdl_error{};
    }
  }

  /**
   * \brief Creates a sensor handle based on an owning sensor.
   *
   * \param owner the associated owning sensor.
   *
   * \since 5.2.0
   */
  template <typename TT = T, detail::is_handle<TT> = 0>
  explicit basic_sensor(const sensor& owner) noexcept : m_sensor{owner.get()}
  {}

  /// \} End of construction

  /**
   * \brief Updates the state of all open sensors.
   *
   * \note This is done automatically by the event loop if sensor events are enabled.
   *
   * \since 5.2.0
   */
  static void update() noexcept
  {
    SDL_SensorUpdate();
  }

#if SDL_VERSION_ATLEAST(2, 0, 14)

  /**
   * \brief Locks access to the sensors for multi-threading.
   *
   * \note Refer to the SDL documentation for more details regarding this.
   *
   * \see SDL_LockSensors
   *
   * \since 5.2.0
   */
  static void lock() noexcept
  {
    SDL_LockSensors();
  }

  /**
   * \brief Unlocks access to the sensors.
   *
   * \note Refer to the SDL documentation for more details regarding this.
   *
   * \see SDL_UnlockSensors
   *
   * \since 5.2.0
   */
  static void unlock() noexcept
  {
    SDL_UnlockSensors();
  }

#endif  // SDL_VERSION_ATLEAST(2, 0, 14)

  /**
   * \brief Returns the amount of sensors currently attached to the system.
   *
   * \return the current amount of system sensors.
   *
   * \since 5.2.0
   */
  [[nodiscard]] static auto count() noexcept -> int
  {
    return SDL_NumSensors();
  }

  /// \name Instance-based queries
  /// \{

  /**
   * \brief Returns the unique identifier associated with the sensor instance.
   *
   * \return a unique instance ID.
   *
   * \since 5.2.0
   */
  [[nodiscard]] auto id() const noexcept -> sensor_id
  {
    return SDL_SensorGetInstanceID(m_sensor);
  }

  /**
   * \brief Returns the name associated with the sensor device.
   *
   * \return the name of the sensor device; a null pointer is returned if the name isn't
   * available.
   *
   * \since 5.2.0
   */
  [[nodiscard]] auto name() const noexcept -> czstring
  {
    return SDL_SensorGetName(m_sensor);
  }

  /**
   * \brief Returns the type associated with the sensor device.
   *
   * \return the type of the sensor device.
   *
   * \since 5.2.0
   */
  [[nodiscard]] auto type() const noexcept -> sensor_type
  {
    return static_cast<sensor_type>(SDL_SensorGetType(m_sensor));
  }

  /**
   * \brief Returns the non-portable type associated with the sensor.
   *
   * \return a non-portable sensor type value.
   *
   * \since 5.2.0
   */
  [[nodiscard]] auto non_portable_type() const noexcept -> int
  {
    return SDL_SensorGetNonPortableType(m_sensor);
  }

  /**
   * \brief Returns the sensor-dependent data.
   *
   * \tparam Size the number of data elements, varies from sensor to sensor.
   *
   * \return the data associated with the sensor; `std::nullopt` if something goes wrong.
   *
   * \since 5.2.0
   */
  template <std::size_t Size>
  [[nodiscard]] auto data() const noexcept -> std::optional<std::array<float, Size>>
  {
    std::array<float, Size> array{};
    if (SDL_SensorGetData(m_sensor, array.data(), isize(array)) != -1)
    {
      return array;
    }
    else
    {
      return std::nullopt;
    }
  }

  /**
   * \brief Returns a pointer to the associated SDL sensor.
   *
   * \warning Don't take ownership of the returned pointer!
   *
   * \return a pointer to the associated SDL sensor.
   *
   * \since 5.2.0
   */
  [[nodiscard]] auto get() const noexcept -> SDL_Sensor*
  {
    return m_sensor.get();
  }

  /// \} End of instance-based queries

  /// \name Index-based queries
  /// \{

  /**
   * \brief Returns the unique identifier associated with a sensor instance.
   *
   * \param index the index of the sensor device that will be queried.
   *
   * \return the unique instance ID associated with a sensor; `std::nullopt` if the index
   * is invalid.
   *
   * \since 5.2.0
   */
  [[nodiscard]] static auto id(const int index) noexcept -> std::optional<sensor_id>
  {
    const auto id = SDL_SensorGetDeviceInstanceID(index);
    if (id != -1)
    {
      return id;
    }
    else
    {
      return std::nullopt;
    }
  }

  /**
   * \brief Returns the name associated with a sensor device.
   *
   * \param index the index of the sensor device that will be queried.
   *
   * \return the name of the sensor device; a null pointer is returned if the name isn't
   * available or if the index was invalid.
   *
   * \since 5.2.0
   */
  [[nodiscard]] static auto name(const int index) noexcept -> czstring
  {
    return SDL_SensorGetDeviceName(index);
  }

  /**
   * \brief Returns the type associated with a sensor device.
   *
   * \param index the index of the sensor device that will be queried.
   *
   * \return the type of the sensor device.
   *
   * \since 5.2.0
   */
  [[nodiscard]] static auto type(const int index) noexcept -> sensor_type
  {
    return static_cast<sensor_type>(SDL_SensorGetDeviceType(index));
  }

  /**
   * \brief Returns the non-portable type associated with a sensor.
   *
   * \param index the index of the sensor device that will be queried.
   *
   * \return a non-portable sensor type value; `std::nullopt` if the index is invalid.
   *
   * \since 5.2.0
   */
  [[nodiscard]] static auto non_portable_type(const int index) noexcept
      -> std::optional<int>
  {
    const auto type = SDL_SensorGetDeviceNonPortableType(index);
    if (type != -1)
    {
      return type;
    }
    else
    {
      return std::nullopt;
    }
  }

  /// \} End of index-based queries

  /// \name Conversions
  /// \{

  /**
   * \brief Indicates whether or not the handle holds a non-null pointer.
   *
   * \return `true` if the handle holds a non-null pointer; `false` otherwise.
   *
   * \since 5.2.0
   */
  template <typename TT = T, detail::is_handle<TT> = 0>
  explicit operator bool() const noexcept
  {
    return m_sensor != nullptr;
  }

  /// \} End of conversions

 private:
  struct deleter final
  {
    void operator()(SDL_Sensor* sensor) noexcept
    {
      SDL_SensorClose(sensor);
    }
  };
  detail::pointer_manager<T, SDL_Sensor, deleter> m_sensor;
};

/**
 * \brief Returns a textual representation of a sensor instance.
 *
 * \param sensor the sensor that will be converted.
 *
 * \return a string that represents a sensor instance.
 *
 * \since 5.2.0
 */
template <typename T>
[[nodiscard]] auto to_string(const basic_sensor<T>& sensor) -> std::string
{
  return "sensor{data: " + detail::address_of(sensor.get()) +
         ", id: " + detail::to_string(sensor.id()).value() +
         ", name: " + str_or_na(sensor.name()) + "}";
}

/**
 * \brief Prints a textual representation of a sensor instance using a stream.
 *
 * \param stream the stream that will be used.
 * \param sensor the sensor that will be printed.
 *
 * \return the used stream.
 *
 * \since 5.2.0
 */
template <typename T>
auto operator<<(std::ostream& stream, const basic_sensor<T>& sensor) -> std::ostream&
{
  return stream << to_string(sensor);
}

/**
 * \brief Returns the standard gravity value.
 *
 * \return the standard gravity value.
 *
 * \since 5.2.0
 */
[[nodiscard]] constexpr auto standard_gravity() noexcept -> float
{
  return SDL_STANDARD_GRAVITY;
}

/// \name Sensor type comparison operators
/// \{

/**
 * \brief Indicates whether or not two sensor types values are equal.
 *
 * \param lhs the left-hand side sensor type.
 * \param rhs the right-hand side sensor type.
 *
 * \return `true` if the two sensor types are equal; `false` otherwise.
 *
 * \since 5.2.0
 */
[[nodiscard]] constexpr auto operator==(const sensor_type lhs,
                                        const SDL_SensorType rhs) noexcept -> bool
{
  return static_cast<SDL_SensorType>(lhs) == rhs;
}

/// \copydoc operator==(const sensor_type, const SDL_SensorType)
[[nodiscard]] constexpr auto operator==(const SDL_SensorType lhs,
                                        const sensor_type rhs) noexcept -> bool
{
  return rhs == lhs;
}

/**
 * \brief Indicates whether or not two sensor types values aren't equal.
 *
 * \param lhs the left-hand side sensor type.
 * \param rhs the right-hand side sensor type.
 *
 * \return `true` if the two sensor types aren't equal; `false` otherwise.
 *
 * \since 5.2.0
 */
[[nodiscard]] constexpr auto operator!=(const sensor_type lhs,
                                        const SDL_SensorType rhs) noexcept -> bool
{
  return !(lhs == rhs);
}

/// \copydoc operator!=(const sensor_type, const SDL_SensorType)
[[nodiscard]] constexpr auto operator!=(const SDL_SensorType lhs,
                                        const sensor_type rhs) noexcept -> bool
{
  return !(lhs == rhs);
}

/// \} End of sensor type comparison operators

/// \} End of input group

}  // namespace cen

#endif  // CENTURION_SENSOR_HEADER
