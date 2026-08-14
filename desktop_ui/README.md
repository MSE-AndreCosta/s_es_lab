# Lab dashboard UI

A single-screen LVGL Pro dashboard written entirely in XML, 1280x720, dark theme.

```
┌───────────────────────────────────────────────────────────────────────────┐
│ Embedded Systems Lab                        ● Online │ 192.168.1.42 (…)   │
├──────────────────────┬──────────────────┬─────────────────────────────────┤
│ CLOCK                │ ENVIRONMENT      │ LEDS                            │
│  analog + digital    │  °C  °F  RH%     │  ● ● ●  red / green / blue      │
│                      │  [Request data]  │  3 switches + All off           │
├──────────────────────┴──────┬───────────┼─────────────────────────────────┤
│ GAME                        │ JOYSTICK  │ CHAT                            │
│  player marker + X/Y        │  pad, X/Y │  transcript + composer + keys   │
└─────────────────────────────┴───────────┴─────────────────────────────────┘
```

## The idea

Nothing on the screen holds state, and **the UI never decides what an output
does**. Subjects in `globals.xml` are split into two groups, and the direction of
travel is strict:

```
   application  ──── writes ────>  STATE subjects   ────>  displayed by widgets
   application  <─── reads  ────   cmd_* subjects   <────  written by widgets
```

- **STATE** (`sensor_*`, `clock_*`, `player_*`, `joy_*`, `net_*`, `led_*`,
  `chat_log`) — the application is the only writer. No widget and no code in this
  project writes them.
- **COMMAND** (`cmd_*`) — the only subjects widgets write. They carry *intent*,
  never state. Acting on them, and reporting the result back through a STATE
  subject, is the application's job.
- One exception, deliberately: `show_keyboard` is owned by the UI, because
  "is the keyboard visible" means nothing outside the screen.

So the application drives everything with plain subject writes and never touches
a widget pointer:

```c
lv_subject_set_int(&sensor_temp_c, 24);
lv_subject_copy_string(&net_ip, "10.0.0.7");
```

### What this means in practice

Flipping the **Red** switch writes `cmd_led_red` and *does not light the lamp*.
The lamp reflects `led_red`, which only the application writes, after it has
actually driven the pin. A switch left on above a dark lamp is therefore a real,
visible "requested but not confirmed" — not a UI glitch.

The same holds for the other two controls:

| You press | UI writes | UI does **not** | Application must |
| --- | --- | --- | --- |
| A LED switch / All off | `cmd_led_*` | light the lamp | drive the pin, then write `led_*` |
| Request sensor data | bumps `cmd_sensor_refresh` | change the readings | read the sensor, write `sensor_temp_c/_f/_humidity` + `sensor_status` |
| Send | `cmd_chat_message`, bumps `cmd_chat_send` | append to the transcript | transmit, then write `chat_log` |

**If nothing is connected, these controls appear to do nothing.** That is
correct behaviour, not a bug. `src/app_stub.c` stands in until your application
is wired up.

## Files

| Path | What |
| --- | --- |
| `globals.xml` | Design tokens, fonts, styles and **all subjects** (the app/UI contract) |
| `screens/main_screen.xml` | The single screen: header + two rows of cards |
| `components/ui_panel.xml` | Card chrome every section extends |
| `components/ui_row.xml`, `ui_stat.xml`, `ui_button.xml`, `ui_led.xml`, `ui_toggle.xml` | Small reusable pieces |
| `components/ui_header.xml` | Title + connected-device chip |
| `components/ui_clock.xml`, `ui_sensors.xml`, `ui_leds.xml`, `ui_game.xml`, `ui_joystick.xml`, `ui_chat.xml` | The six feature cards |
| `components/ui_chart.xml` | Two-series line chart — the compact `lv_chart` XML demo |
| `src/ui_actions.c` | The UI's only C: one event callback, and it writes commands only |
| `src/app_stub.c` | **Not UI.** Stand-in for your application; delete it when the real one is connected |

## Subject reference

Every table below is marked **S** (state, app writes) or **C** (command, UI writes).

### 1 + 7 — Sensors

| Subject | | Type | Meaning |
| --- | --- | --- | --- |
| `sensor_temp_c` | S | int | Temperature, whole °C |
| `sensor_temp_f` | S | int | Temperature, whole °F |
| `sensor_humidity` | S | int | Relative humidity, 0..100 % (also drives the bar) |
| `sensor_status` | S | string | Line under the button, e.g. `updated 12:04:31` |
| `cmd_sensor_refresh` | C | int | Bumped by *Request sensor data*. Observe the edge and take a reading. |

Values are integers on purpose: `LV_USE_FLOAT` is off in `sim/lv_conf_sdl.defaults`,
so float subjects would not compile. For one decimal, enable `LV_USE_FLOAT`, change
these to `<float …>` and set `fmt="%0.1f"` on the matching `<ui_stat>`.

### 2 — Clock

| Subject | | Type | Meaning |
| --- | --- | --- | --- |
| `clock_hour_angle` | S | int | Hour hand, **0.1° clockwise from 12 o'clock** |
| `clock_min_angle` | S | int | Minute hand, same unit |
| `clock_sec_angle` | S | int | Second hand, same unit |
| `clock_time` | S | string | Digital readout, `HH:MM:SS` |
| `clock_date` | S | string | Line under the digital readout |

LVGL cannot attach `lv_scale` needles from XML, so each hand is a rounded bar whose
`transform_rotation` style property is bound to a subject, pivoting on its bottom
edge at the dial's center. Angles:

```c
hour   = ((h % 12) * 30 + m / 2) * 10;
minute = (m * 6 + s / 10) * 10;
second = s * 60;
```

The clock is state like everything else — the UI has no time source of its own.
`app_stub.c` publishes these once a second from the system clock; replace it with
whatever owns time on your target.

### 3 — Game and joystick

| Subject | | Type | Meaning |
| --- | --- | --- | --- |
| `player_x` / `player_y` | S | int | Marker position in px from the field's top-left, 0..488 by 0..192 |
| `player_name` | S | string | Shown in the card's footer |
| `joy_x` / `joy_y` | S | int | Knob offset from the pad's center in px, -46..46 |

There is no game logic in the UI: the other application owns the simulation and
just publishes coordinates, which move the marker through bound `translate_x` /
`translate_y`. If your sender uses a different range (say -100..100 for the
joystick), scale it on the sending side or widen the pad in `ui_joystick.xml`.

The play field is 512 x 216 px at 1280x720. If you change the layout, update the
`min_value`/`max_value` of `player_x` / `player_y` to match.

### 4 — Connection

| Subject | | Type | Meaning |
| --- | --- | --- | --- |
| `net_ip` | S | string | IP address shown in the header |
| `net_peer` | S | string | Host name shown next to it |
| `net_connected` | S | int | 0 = offline (chip dims), 1 = online |

### 5 + 8 — LEDs

| Subject | | Type | Meaning |
| --- | --- | --- | --- |
| `led_red` / `led_green` / `led_blue` | S | int | The output **as actually driven**. Drives the lamps. |
| `cmd_led_red` / `cmd_led_green` / `cmd_led_blue` | C | int | What the user asked for. Drives the switches. |

Keeping these apart is the whole point: the lamp is evidence, the switch is a
request. Echo the command into the state subject only once the pin really moved.

### 6 — Chat

| Subject | | Type | Meaning |
| --- | --- | --- | --- |
| `chat_log` | S | string | The whole transcript, newline separated |
| `cmd_chat_message` | C | string | The text typed by the user, published on *Send* |
| `cmd_chat_send` | C | int | Bumped just after `cmd_chat_message` is set — observe this edge |
| `show_keyboard` | — | int | UI-local: 1 shows the on-screen keyboard over the chat card |

`lv_textarea` has no text binding, so the transcript is a bound `lv_label` inside a
scroller and the application owns the string. The Send button does **not** append
to it — a sent message appears only when the application writes `chat_log`.

String subjects share one buffer size, `UI_SUBJECT_STRING_LENGTH` (256 by default).
For a longer history build with `-DUI_SUBJECT_STRING_LENGTH=2048`; the
`chat_log_append()` helper in `app_stub.c` drops whole lines off the front when
the buffer is full.

## The C side

Two files, wired in through `user_config.cmake`, and the split between them is
the same STATE/COMMAND boundary:

**`src/ui_actions.c` — belongs to the UI.** It contains exactly one function,
because a button cannot read an `lv_textarea` through a data binding:

- `ui_chat_send(lv_event_t *)` — copies the composer's text into
  `cmd_chat_message`, bumps `cmd_chat_send`, clears the field. It writes no state.

It is referenced **by name** from `ui_chat.xml` (`<event_cb callback="…"/>`), so
renaming it means editing that file too. Everything else the UI does is a
declarative binding — there is no other UI C code.

**`src/app_stub.c` — does *not* belong to the UI.** It is a stand-in for the
application that owns the pins and the link, so the dashboard is alive before
that application exists. It subscribes to the command subjects and publishes the
state subjects:

| Observes | Publishes |
| --- | --- |
| `cmd_led_*` | `led_*` (accepts the request as-is; put your GPIO write here) |
| `cmd_sensor_refresh` | `sensor_temp_c/_f/_humidity`, `sensor_status` — **these values are fake**, a deterministic walk so you can see the bindings work |
| `cmd_chat_send` | `chat_log` |
| 1 Hz timer | `clock_hour_angle`, `clock_min_angle`, `clock_sec_angle`, `clock_time`, `clock_date` |

To connect the real thing: subscribe to the same commands from your own code,
publish the same state subjects, and drop `app_stub.c` from
`user_config.cmake` plus the `app_stub_init()` call in `sim/main.c`. The XML does
not change at all.

```c
/* the pattern app_stub.c uses — note lv_subject_add_observer() fires once
   immediately on subscribe, so filter that first call out on edge counters */
static void on_refresh(lv_observer_t * o, lv_subject_t * s)
{
    if(lv_subject_get_int(s) == lv_subject_get_previous_int(s)) return;
    lv_subject_set_int(&sensor_temp_c, read_sensor());
}
lv_subject_add_observer(&cmd_sensor_refresh, on_refresh, NULL);
```

`sim/main.c` loads the screen and calls `app_stub_init()`.

## Building

```sh
cmake -S sim -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```

Or open the folder in the LVGL Pro Editor and press the hammer / `Ctrl+B`.

## Notes

- Fonts are Montserrat (Regular / SemiBold / Bold) plus FontAwesome for the
  keyboard's control keys, converted into the firmware (`as_file="false"`).
  LVGL's built-in fonts cannot be referenced by name from the Editor, which is why
  the TTFs live in `fonts/`.
- The theme is dark only. All colors come from the `<consts>` block in
  `globals.xml`; changing `color_bg` / `color_panel` / `color_accent` there
  re-tunes the whole dashboard.
- The on-screen keyboard is `floating`, so it overlays the chat card instead of
  squeezing the transcript. It is hidden until the *Keys* button sets
  `show_keyboard` to 1 — leave it off when a real keyboard is attached.
