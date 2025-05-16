# Changelog

## [Unreleased]
### Changed
- Accept both *B* and *START* buttons to exit menus.
- Store game images as PNG files instead of hex-code, so that they can
  be viewed and modified without special tools.
- Use the *libsimplegba* library to handle many low-level and basic
  things.
- Use more accurate sine and cosine functions when the Air Wizard shoots
  sparks.

### Fixed
- Menus flickering when holding the menu's toggle buttons.

## [1.3] - 2024-02-09
### Added
- Checksum verification to validate save files.
- Options menu.
- Respawn feature, with an option to keep the inventory.

### Changed
- Use a faster algorithm to sort entities, reducing the time needed to
  update the screen.

### Fixed
- Time displayed in pause, death and win menus, which was slightly lower
  than it should be.

## [1.2.1] - 2023-05-04
### Fixed
- The linker script, which was ignoring some input sections.

## [1.2] - 2023-05-01
### Added
- Frameskip to lower the fps when necessary.
- TPS and FPS counters in the performance overlay.

### Fixed
- Visual bug that caused trailing zeros to appear after the item count
  after losing a decimal digit. (Introduced in 1.1)
- Visual bug that caused some hearts to be drawn even after the player
  dies in lava.

## [1.1] - 2023-03-10
### Fixed
- Entities being drawn on top of the darkness layer when close to a
  player holding a lantern.

## [1.0] - 2022-11-19
First release of the game.
