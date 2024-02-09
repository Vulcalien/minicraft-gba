# Changelog

## [Unreleased]
### Added
- Checksum verification to validate save files.
- Options menu.
- Respawn feature, with an option to keep the inventory.

### Changed
- Optimize entity sorting, reducing the time necessary to update the
  screen.

### Fixed
- Fix the time displayed in pause, death and win menus, which was
  slightly lower than it should have been.

## [1.2.1] - 2023-05-04
Fix the linker script: add missing input sections to the output
sections. The ROM remains unchanged.

## [1.2] - 2023-05-01
### Added
- Introduce frameskip, which can lower the fps to 30.
- TPS and FPS counters in the performance overlay.

### Fixed
- Fix a visual bug that caused trailing zeros to appear after the item
  count after losing a decimal digit. (Introduced in 1.1)
- Fix a visual bug that caused some hearts to be drawn even after the
  player dies in lava.

## [1.1] - 2023-03-10
Fix a bug that caused entities to be drawn incorrectly when close to a
player holding a lantern.

## [1.0] - 2022-11-19
First release of the game.
